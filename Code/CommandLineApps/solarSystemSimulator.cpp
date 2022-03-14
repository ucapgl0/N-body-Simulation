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

static void show_usage(std::string name)
{
	std::cerr << "Usage: " << name << "\n"
		<< "Users should input the step size and the length of time to simulate the model.\n"
		<< "Option:\n"
        << "\t-s\t\tstep size(the time of each step)\n"
        << "\t-t\t\tthe total time of simulation\n"
		<< "\t-h,--help\t\tShow this help message\n" 
		<< std::endl;
}


int main(int argc, char* argv[]) {
	
    // if (argc == 1) {
	// 	show_usage(argv[0]);
	// 	return 0;
	// }

	CLI::App app{"Solar-system simulator"};
    double step_size, time_length;
    app.add_option("-s,--TimeStep", step_size,  "step size(the time of each step)");
    app.add_option("-t,--LengthOfTime", time_length,  "the total time of simulation");
    // app.add_option("-h","Users should input the step size and the length of time to simulate the model.");
    // app.add_option("-help","Users should input the step size and the length of time to simulate the model.");
    std::string planet[9];
    Eigen::Vector3d position, velocity;
    double mass;
    std::string name[9];
    std::shared_ptr<nbsim::MassiveParticle> planet_ptr[9];
    CLI11_PARSE(app, argc, argv);

    double Gravitational_constant = 6.67e-11;

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

    // Summarising the position of the solar system bodies at the start and end of the simulation
    std::cout<<"Summarising the position of the solar system bodies at the start and end of the simulation: "<<std::endl;
    for (int i=0;i<9;i++){
		std::cout<<name[i]<<"\n original position:\n"<<nbsim::solarSystemData[i].position<<"\n current position:\n"<<planet_ptr[i]->getPosition()<<std::endl;
        std::cout<<"\n";
    }
}