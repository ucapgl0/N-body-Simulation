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

double Gravitational_constant = 6.67e-11;

std::vector<nbsim::MassiveParticle> generator(int n){
    std::vector<nbsim::MassiveParticle> system;
    // Central plant
    double large_mass = 100 / Gravitational_constant;
    nbsim::MassiveParticle central_plant(Eigen::Vector3d (0,0,0),Eigen::Vector3d (0,0,0),large_mass); 
    system.push_back(central_plant);

    // Generate remaining particles
    for (int i=1;i<n;i++){
        srand(time(0));
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
	
	CLI::App app{"Solar-system simulator"};
    double step_size, time_length;
    app.add_option("-s,--TimeStep", step_size,  "input the step size(the time of each step)");
    app.add_option("-t,--LengthOfTime", time_length,  "input the total time of simulation");
    std::string planet[9];
    Eigen::Vector3d position, velocity;
    double mass;
    std::string name[9];
    std::shared_ptr<nbsim::MassiveParticle> planet_ptr[9];
    CLI11_PARSE(app, argc, argv);


    std::clock_t c1_start = std::clock();
    auto t1_start = std::chrono::high_resolution_clock::now();

    // create a set of MassiveParticle‘s corresponding to the major bodies
    for (int ibody=0; ibody <9; ibody++) {		
		name[ibody] = nbsim::solarSystemData[ibody].name;
		mass = nbsim::solarSystemData[ibody].mu / Gravitational_constant;
		position = nbsim::solarSystemData[ibody].position;
		velocity = nbsim::solarSystemData[ibody].velocity;
		planet_ptr[ibody] = std::make_shared<nbsim::MassiveParticle>(position, velocity, mass);
	}

    // For each MassiveParticle, add all other MassiveParticle bodies to their list of attractors
	for (int i=0; i <9; i++) {
		for (int j=0; j<9; j++) {

			if (i != j) {

                planet_ptr[i]->addAttractor(planet_ptr[j]);
            } 
		}
	}
    
    //  Implement the evolution of the solar system with time
    for (double time=0; time<time_length; time+=step_size){
            
		for (int i=0;i<9;i++){
			planet_ptr[i]->calculateAcceleration();
		}
        
		for (int i=0;i<9;i++){
			planet_ptr[i]->integrateTimestep(step_size);
		}	
	}

    // Calculate the kinetic, potential and total energy at the beginning and end of the simulation for solar system
    double k_begin=0, k_end=0, p_begin=0, p_end=0, total_begin, total_end;
    for (int i=0; i < 9; i++){
        Eigen::Vector3d ini_velocity = nbsim::solarSystemData[i].velocity;
        Eigen::Vector3d final_velocity = planet_ptr[i]->getVelocity();
        k_begin += 0.5 * nbsim::solarSystemData[i].mu * ini_velocity.dot(ini_velocity);
        k_end += 0.5 * nbsim::solarSystemData[i].mu * final_velocity.dot(final_velocity);
        Eigen::Vector3d ini_position = nbsim::solarSystemData[i].position;
        Eigen::Vector3d final_position = planet_ptr[i]->getPosition();
        for (int j=0; j<9;j++){
            if(i!=j){
                Eigen::Vector3d ini_position_attractor = nbsim::solarSystemData[j].position;
                Eigen::Vector3d final_position_attractor = planet_ptr[j]->getPosition();
                p_begin += -0.5 * nbsim::solarSystemData[i].mu * nbsim::solarSystemData[j].mu / (ini_position - ini_position_attractor).norm();
                p_end += -0.5 * nbsim::solarSystemData[i].mu * nbsim::solarSystemData[j].mu / (final_position - final_position_attractor).norm();
            }
        }
    }
    
    std::clock_t c1_end = std::clock();
    auto t1_end = std::chrono::high_resolution_clock::now();

/*
    //benchmark 2000 plants system simulation
    std::clock_t c2_start = std::clock();
    auto t2_start = std::chrono::high_resolution_clock::now();

    int n = 2000;
    std::shared_ptr<nbsim::MassiveParticle> system_ptr[n];
    std::vector<nbsim::MassiveParticle> PlanetSystem = generator(n);

    for (int i=0; i<n; i++){
        system_ptr[i] = std::make_shared<nbsim::MassiveParticle>(PlanetSystem[i].getPosition(), PlanetSystem[i].getVelocity(), PlanetSystem[i].getMu()/Gravitational_constant);
    }

    // For each MassiveParticle, add all other MassiveParticle bodies to their list of attractors
	for (int i=0; i < n; i++) {

		for (int j=0; j<n; j++) {

			if (i != j) {

                system_ptr[i]->addAttractor(system_ptr[j]);
            } 
		}
	}
    
    // Implement the evolution of the 2000 plants-system with time
    for (double time=0; time<time_length; time+=step_size){
            
		for (int i=0;i<n;i++){
			system_ptr[i]->calculateAcceleration();
		}
        
		for (int i=0;i<n;i++){
			system_ptr[i]->integrateTimestep(step_size);
		}	
	}

    // Calculate the kinetic, potential and total energy at the beginning and end of the simulation for 2000 plants-system
    double k2_begin=0, k2_end=0, p2_begin=0, p2_end=0, total2_begin, total2_end;
    for (int i=0; i < n; i++){
        Eigen::Vector3d ini_velocity = PlanetSystem[i].getVelocity();
        Eigen::Vector3d final_velocity = system_ptr[i]->getVelocity();
        k2_begin += 0.5 * PlanetSystem[i].getMu() * ini_velocity.dot(ini_velocity);
        k2_end += 0.5 * PlanetSystem[i].getMu() * final_velocity.dot(final_velocity);
        Eigen::Vector3d ini_position = PlanetSystem[i].getPosition();
        Eigen::Vector3d final_position = system_ptr[i]->getPosition();
        for (int j=0; j<n;j++){
            if(i!=j){
                Eigen::Vector3d ini_position_attractor = PlanetSystem[j].getPosition();
                Eigen::Vector3d final_position_attractor = system_ptr[j]->getPosition();
                p2_begin += -0.5 * PlanetSystem[i].getMu() * PlanetSystem[j].getMu()  / (ini_position - ini_position_attractor).norm();
                p2_end += -0.5 * PlanetSystem[i].getMu() * PlanetSystem[j].getMu()  / (final_position - final_position_attractor).norm();
            }
        }
    }

    std::clock_t c2_end = std::clock();
    auto t2_end = std::chrono::high_resolution_clock::now();

*/
    // Summarising the position of the solar system bodies at the start and end of the simulation
    std::cout<<"Summarising the position of the solar system bodies at the start and end of the simulation: "<<std::endl;
    for (int i=0;i<9;i++){
		std::cout<<name[i]<<"\n original position:\n"<<nbsim::solarSystemData[i].position<<"\n current position:\n"<<planet_ptr[i]->getPosition()<<std::endl;
        std::cout<<"\n";
    }

    // Output the energy information at the beginning and end of the simulation
    total_begin = k_begin + p_begin;
    total_end = k_end + p_end;
    std::cout<<"The energy at the beginning of simulation: "<<std::endl;
    std::cout<<"The Kinetic energy:"<<k_begin<<std::endl;
    std::cout<<"The Potential energy:"<<p_begin<<std::endl;
    std::cout<<"The Total energy:"<<total_begin<<std::endl;
    std::cout<<"The total energy at the end of simulation: "<<std::endl;
    std::cout<<"The Kinetic energy:"<<k_end<<std::endl;
    std::cout<<"The Potential energy:"<<p_end<<std::endl;
    std::cout<<"The Total energy:"<<total_end<<std::endl;

    // Output the runtime of code
    std::cout << std::fixed << std::setprecision(2) << "CPU time used: "
              << 1000.0 * (c1_end - c1_start) / CLOCKS_PER_SEC << " ms\n"
              << "Wall clock time passed: "
              << std::chrono::duration<double, std::milli>(t1_end-t1_start).count()
              << " ms\n";

}