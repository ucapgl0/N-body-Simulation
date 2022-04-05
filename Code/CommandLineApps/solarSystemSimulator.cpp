#include <iostream>
#include "../Lib/nbsimParticle.h"
#include "../Lib/nbsimMassiveParticle.h"
#include "../Lib/nbsimSimulation.h"
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


int main(int argc, char* argv[]) {
	
	CLI::App app{"Solar-system simulator"};
    double step_size, time_length;
    app.add_option("-s,--TimeStep", step_size,  "input the step size(the time of each step)");
    app.add_option("-t,--LengthOfTime", time_length,  "input the total time of simulation");
    CLI11_PARSE(app, argc, argv);


    nbsim::SolarSystem solar_system;
    solar_system.generator();

    // Calculate the initial kinetic energy and potential energy
    solar_system.calculateEnergy();

    // Output the original position and energy
    std::cout << "\nOriginal Position: \n"<< std::endl;               
    solar_system.printPosition();
    std::cout << "---------------------------------------------------------------\n"
                << std::endl;
    std::cout << "\nOriginal Energy: \n"
                << std::endl;          
    solar_system.printEnergy();
    std::cout << "-----------------------------------------------------------------" << std::endl;

    std::clock_t c1_start = std::clock();
    auto t1_start = std::chrono::high_resolution_clock::now();
    // Add all other MassiveParticle bodies to their list of attractors
    solar_system.attraction();

    // Implement the evolution of the solar system with time
    solar_system.evolution(time_length, step_size);

    solar_system.calculateEnergy();

    std::clock_t c1_end = std::clock();
    auto t1_end = std::chrono::high_resolution_clock::now();

    // Output the final position and energy
    std::cout << "\nCurrent Position: \n"
                << std::endl;
    solar_system.printPosition();
    std::cout << "---------------------------------------------------------------\n"
                << std::endl;
    std::cout << "\nFinal Energy: \n"
                << std::endl;          
    solar_system.printEnergy();
    std::cout << "-----------------------------------------------------------------" << std::endl;

    // Output the runtime of code
    std::cout << std::fixed << std::setprecision(2) << "CPU time used: "
              << 1000.0 * (c1_end - c1_start) / CLOCKS_PER_SEC << " ms\n"
              << "Wall clock time passed: "
              << std::chrono::duration<double, std::milli>(t1_end-t1_start).count()
              << " ms\n";

}