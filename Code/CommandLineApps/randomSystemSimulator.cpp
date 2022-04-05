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
	
	CLI::App app{"Random-system simulator"};
    double step_size, time_length;
    int threads;
    app.add_option("-s,--TimeStep", step_size,  "input the step size(the time of each step)");
    app.add_option("-t,--LengthOfTime", time_length,  "input the total time of simulation");
    app.add_option("-n,--SetNumThreads", threads, "input the number of threads");
    CLI11_PARSE(app, argc, argv);
    
    //benchmark 2000 plants system simulation  
    int n = 2000;
    nbsim::RandomSystem random_system(n);
    random_system.generator();

    // Calculate the initial kinetic energy and potential energy
    random_system.calculateEnergy_Openmp(threads);


    std::clock_t c_start = std::clock();
    auto t_start = std::chrono::high_resolution_clock::now();

    random_system.attraction();

    // Implement the evolution of the random system with time
    random_system.evolution_Openmp(time_length, step_size, threads);

    random_system.calculateEnergy_Openmp(threads);

    std::clock_t c_end = std::clock();
    auto t_end = std::chrono::high_resolution_clock::now();

    // Output the runtime of code
    std::cout << std::fixed << std::setprecision(2) << "CPU time used: "
              << 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC << " ms\n"
              << "Wall clock time passed: "
              << std::chrono::duration<double, std::milli>(t_end-t_start).count()
              << " ms\n";
}  
    
