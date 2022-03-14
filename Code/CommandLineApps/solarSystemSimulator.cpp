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

    double Gravitational_constant = 6.67e-11;

    std::clock_t c_start = std::clock();
    auto t_start = std::chrono::high_resolution_clock::now();

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

    // Calculate the kinetic, potential and total energy at the beginning and end of the simulation
    double k_begin=0, k_end=0, p_begin=0, p_end=0, total_begin, total_end;
    for (int i=0; i < 9; i++){
        Eigen::Vector3d ini_velocity = nbsim::solarSystemData[i].velocity;
        Eigen::Vector3d final_velocity = planet_ptr[i]->getVelocity();
        k_begin += 0.5 * nbsim::solarSystemData[i].mu * ini_velocity.dot(ini_velocity);
        k_end += 0.5 * nbsim::solarSystemData[i].mu * final_velocity.dot(final_velocity);
        Eigen::Vector3d ini_position = nbsim::solarSystemData[i].position;
        Eigen::Vector3d final_position = planet_ptr[i]->getVelocity();
        for (int j=0; j<9;j++){
            if(i!=j){
                Eigen::Vector3d ini_position_attractor = nbsim::solarSystemData[j].position;
                Eigen::Vector3d final_position_attractor = planet_ptr[j]->getVelocity();
                p_begin += -0.5 * nbsim::solarSystemData[i].mu * nbsim::solarSystemData[j].mu / (ini_position - ini_position_attractor).norm();
                p_end += -0.5 * nbsim::solarSystemData[i].mu * nbsim::solarSystemData[j].mu / (final_position - final_position_attractor).norm();
            }
        }
    }

    std::clock_t c_end = std::clock();
    auto t_end = std::chrono::high_resolution_clock::now();

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
              << 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC << " ms\n"
              << "Wall clock time passed: "
              << std::chrono::duration<double, std::milli>(t_end-t_start).count()
              << " ms\n";

}