#include <iostream>
#include "../Lib/nbsimParticle.h"
#include "../Lib/nbsimMassiveParticle.h"
#include "../Lib/nbsimSolarSystemData.ipp"
#include "../3rdParty/CLI11.hpp"
#include <memory>
#include <chrono>
#include <omp.h>
#include <Eigen/Dense>
#include <string>
#include <ctime>
#include <vector>
#include <math.h>
#include <random>

double Gravitational_constant = 6.67e-11;

std::vector<nbsim::MassiveParticle> generator(int n){
    std::vector<nbsim::MassiveParticle> system;
    // Central plant
    double large_mass = 100 / Gravitational_constant;
    nbsim::MassiveParticle central_plant(Eigen::Vector3d (0,0,0),Eigen::Vector3d (0,0,0),large_mass); 
    system.push_back(central_plant);

    // Generate remaining particles
    srand(time(0));
    for (int i=1;i<n;i++){        
        // Control 0.4 < radius < 40 and 0 < theta < 2*Pi and mu < 0.03
        double radius = 0.4 + 39.6*(rand() / double(RAND_MAX));
        double theta = 2*M_PI*(rand() / double(RAND_MAX));
        double mass = 0.03*(rand() / double(RAND_MAX)) / Gravitational_constant;
        double r_x = radius * sin(theta), r_y = radius * cos(theta);
        double v_x = -cos(theta)/sqrt(radius), v_y = sin(theta)/sqrt(radius);
        nbsim::MassiveParticle plant(Eigen::Vector3d (r_x,r_y,0),Eigen::Vector3d (v_x,v_y,0),mass);
        system.push_back(plant);
    }
    return system;
}

int main(int argc, char* argv[]) {
	
	CLI::App app{"Random-system simulator"};
    double step_size, time_length;
    int threads;
    app.add_option("-s,--TimeStep", step_size,  "input the step size(the time of each step)");
    app.add_option("-t,--LengthOfTime", time_length,  "input the total time of simulation");
    app.add_option("-n,--SetNumThreads", threads, "input the number of threads");
    //benchmark 2000 plants system simulation
    std::clock_t c_start = std::clock();
    auto t_start = std::chrono::high_resolution_clock::now();

    int n = 2000;

    std::shared_ptr<nbsim::MassiveParticle> system_ptr[n];
    std::vector<nbsim::MassiveParticle> PlanetSystem = generator(n);

    omp_set_num_threads(threads);

    
    for (int i=0; i < n; i++){
        system_ptr[i] = std::make_shared<nbsim::MassiveParticle>(PlanetSystem[i].getPosition(), PlanetSystem[i].getVelocity(), PlanetSystem[i].getMu()/Gravitational_constant);
    }

    // For each MassiveParticle, add all other MassiveParticle bodies to their list of attractors
    //#pragma omp parallel
	for (int i=0; i < n; i++) {

		for (int j=0; j < n; j++) {
            // Avoid adding attractor to self
			if (i != j) {

                system_ptr[i]->addAttractor(system_ptr[j]);
            } 
		}
	}
    
    // Implement the evolution of the 2000 plants-system with time
    #pragma omp parallel
    for (double time=0; time < time_length; time+=step_size){
            
        #pragma omp for     
		for (int i=0;i<n;i++){
			system_ptr[i]->calculateAcceleration();
		}
        
        #pragma omp for 
		for (int i=0;i<n;i++){
			system_ptr[i]->integrateTimestep(step_size);
		}	
	}

    // Calculate the kinetic, potential and total energy at the beginning and end of the simulation for 2000 plants-system  
    double k2_begin=0, k2_end=0, p2_begin=0, p2_end=0, total2_begin, total2_end;
    #pragma omp parallel for
    for (int i=0; i < n; i++){
        Eigen::Vector3d ini_velocity = PlanetSystem[i].getVelocity();
        Eigen::Vector3d final_velocity = system_ptr[i]->getVelocity();
        k2_begin += 0.5 * PlanetSystem[i].getMu() * ini_velocity.dot(ini_velocity);
        k2_end += 0.5 * PlanetSystem[i].getMu() * final_velocity.dot(final_velocity);
        Eigen::Vector3d ini_position = PlanetSystem[i].getPosition();
        Eigen::Vector3d final_position = system_ptr[i]->getPosition();
        #pragma omp parallel for
        for (int j=0; j < n;j++){
            if(i!=j){
                Eigen::Vector3d ini_position_attractor = PlanetSystem[j].getPosition();
                Eigen::Vector3d final_position_attractor = system_ptr[j]->getPosition();
                p2_begin += -0.5 * PlanetSystem[i].getMu() * PlanetSystem[j].getMu()  / (ini_position - ini_position_attractor).norm();
                p2_end += -0.5 * PlanetSystem[i].getMu() * PlanetSystem[j].getMu()  / (final_position - final_position_attractor).norm();
            }
        }
    }

    total2_begin = k2_begin + p2_begin;
    total2_end = k2_end + p2_end;

    std::clock_t c_end = std::clock();
    auto t_end = std::chrono::high_resolution_clock::now();

    // Output the runtime of code
    std::cout << std::fixed << std::setprecision(2) << "CPU time used: "
              << 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC << " ms\n"
              << "Wall clock time passed: "
              << std::chrono::duration<double, std::milli>(t_end-t_start).count()
              << " ms\n";
}  
    
