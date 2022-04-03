#include "nbsimSolarSystemData.ipp"
#include "nbsimSimulation.h"
#include "nbsimParticle.h"
#include "nbsimMassiveParticle.h"
#include <Eigen/Dense>
#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <omp.h>
#include <math.h>

double Gravitational_constant = 6.67e-11;

namespace nbsim{

    void Simulation::validationTime(double time_length, double step_size){
        if (step_size <= 0)
        {
            throw std::runtime_error(" User should input positive time step size ");
        }
        else if (time_length < 0)
        {
            throw std::runtime_error(" User should input length of simulation time  which is greater or equal to 0 ");
        }
        else if (step_size > time_length)
        {
            throw std::runtime_error(" The time step size should be smaller or equal to the length of time");
        }
    }    
    Simulation::Simulation(){};

    Simulation::Simulation(std::vector<std::shared_ptr<MassiveParticle>> particlelist) : particle_list(particlelist){};

    Simulation::~Simulation(){};

    void Simulation::attraction(){
        // For each MassiveParticle, add all other MassiveParticle bodies to their list of attractors
	    for (int i=0; i <9; i++) {
		    for (int j=0; j<9; j++) {
                // Avoid adding attractor to self
			    if (i != j) {

                    particle_list[i]->addAttractor(particle_list[j]);
                } 
		    }
	    }
    }

    //  Implement the evolution of the system with time
    void Simulation::evolution(double time_length, double step_size){
        validationTime(time_length, step_size);
        //  Implement the evolution of the solar system with time
        for (double time=0; time<time_length; time+=step_size){
            
		    for (int i=0;i<particle_list.size();i++){
			    particle_list[i]->calculateAcceleration();
		    }
        
		    for (int i=0;i<particle_list.size();i++){
			    particle_list[i]->integrateTimestep(step_size);
		    }	
	    }
    }

    // Calculate the kinetic, potential and total energy at the beginning and end of the simulation for system
    void Simulation::calculateEnergy(){
        E_k = 0;
        E_p = 0;
        for (int i = 0; i < particle_list.size(); i++)
        {
            Eigen::Vector3d v = particle_list[i]->getVelocity();
            E_k += 0.5 * particle_list[i]->getMu() * v.dot(v);

            for (int j = 0; j < particle_list.size(); j++)
            {
                if (i != j)
                {
                    Eigen::Vector3d r = particle_list[i]->getPosition() - particle_list[j]->getPosition();
                    E_p += -0.5 * particle_list[i]->getMu() * particle_list[j]->getMu() / r.norm();
                }
            }
        }
        E_tol = E_k + E_p;
    }

    std::vector<std::shared_ptr<MassiveParticle>> Simulation::getParticle_list() const
   {
        return particle_list;
    }

    double Simulation::getEk() const
    {
        return E_k;
    };

    double Simulation::getEp() const
    {
        return E_p;
    };

    double Simulation::getEtol() const
    {
        return E_tol;
    }

    void Simulation::printEnergy() const
    {
        std::cout << "\nkinetic energy: " << E_k << "\n"
                  << "potential energy: " << E_p << "\n"
                  << "total energy: " << E_tol << std::endl;
    }

    SolarSystem::SolarSystem(){};

    void SolarSystem::generator()
    {
        particle_list.clear();
        // Initialise all the instances of Solar System bodies from the file
        
        // double mu;
        // Eigen::Vector3d pos, vel;
        // for (auto body : solarSystemData)
        // {
        //     mu = body.mu;
        //     pos = body.position;
        //     vel = body.velocity;

        //     std::shared_ptr<nbsim::MassiveParticle> ptr_body(new nbsim::MassiveParticle(pos, vel, mu));
        //     particle_list.push_back(ptr_body);
        // }

        // create a set of MassiveParticle‘s corresponding to the major bodies
        Eigen::Vector3d position, velocity;
        double mass;
        for (int ibody=0; ibody <9; ibody++) {			    
		    mass = nbsim::solarSystemData[ibody].mu / Gravitational_constant;
		    position = nbsim::solarSystemData[ibody].position;
		    velocity = nbsim::solarSystemData[ibody].velocity;
            std::shared_ptr<nbsim::MassiveParticle> body(new nbsim::MassiveParticle(position, velocity, mass));
		    particle_list.push_back(body);
	    }

    }

    SolarSystem::~SolarSystem(){};

    // Print the position of the solar system bodies at the start and end of the simulation
    void SolarSystem::printPosition() const
    {
        for (int i = 0; i < 9; ++i)
        {
            std::cout << nbsim::solarSystemData[i].name << ": "
                      << '(' << particle_list[i]->getPosition()[0] << ','
                      << particle_list[i]->getPosition()[1] << ','
                      << particle_list[i]->getPosition()[2] << ')'
                      << std::endl;             
        }
    }

    RandomSystem::RandomSystem(){};

    RandomSystem::RandomSystem(int particle_num) : particle_number(particle_num){};

    void RandomSystem::generator(){
        
        particle_list.clear();
        // Central plant
        double large_mass = 100 / Gravitational_constant;
        std::shared_ptr<nbsim::MassiveParticle> central_plant(new nbsim::MassiveParticle(Eigen::Vector3d (0,0,0),Eigen::Vector3d (0,0,0),large_mass));
        particle_list.push_back(central_plant);

        // Generate remaining particles
        srand(time(0));
        for (int i=1;i<particle_number;i++){        
            // Control 0.4 < radius < 40 and 0 < theta < 2*Pi and mu < 0.03
            double radius = 0.4 + 39.6*(rand() / double(RAND_MAX));
            double theta = 2*M_PI*(rand() / double(RAND_MAX));
            double mass = 0.03*(rand() / double(RAND_MAX)) / Gravitational_constant;
            double r_x = radius * sin(theta), r_y = radius * cos(theta);
            double v_x = -cos(theta)/sqrt(radius), v_y = sin(theta)/sqrt(radius);
            std::shared_ptr<nbsim::MassiveParticle> plant(new nbsim::MassiveParticle(Eigen::Vector3d (r_x,r_y,0),Eigen::Vector3d (v_x,v_y,0),mass));           
            particle_list.push_back(plant);
        }
        
    }

    //  Implement the evolution of the system with time
    void RandomSystem::evolution_Openmp(double time_length, double step_size, int thread_number){
        validationTime(time_length, step_size);
        for (double time=0; time<time_length; time+=step_size){
            // Parallel the for loop using OpenMP
            omp_set_num_threads(thread_number);
            #pragma omp parallel for
		    for (int i=0;i<particle_list.size();i++){
			    particle_list[i]->calculateAcceleration();
		    }
            // Parallel the for loop using OpenMP
            omp_set_num_threads(thread_number);
            #pragma omp parallel for
		    for (int i=0;i<particle_list.size();i++){
			    particle_list[i]->integrateTimestep(step_size);
		    }	
	    }	
    }

    // Calculate the kinetic, potential and total energy at the beginning and end of the simulation for system
    void RandomSystem::calculateEnergy_Openmp(int thread_number){
        E_k = 0;
        E_p = 0;

        omp_set_num_threads(thread_number);
        // Parallelise by using an OpenMP reduction
        #pragma omp parallel for reduction(+ : E_k)
        for (int i = 0; i < particle_list.size(); i++)
        {
            Eigen::Vector3d v = particle_list[i]->getVelocity();
            E_k += 0.5 * particle_list[i]->getMu() * v.dot(v);

            
        }
        omp_set_num_threads(thread_number);
        // Parallelise by using an OpenMP reduction
        #pragma omp parallel for reduction(+ : E_p)
        for (int i = 0; i < particle_list.size(); i++){
            for (int j = 0; j < particle_list.size(); j++)
            {
                if (i != j)
                {
                    Eigen::Vector3d r = particle_list[i]->getPosition() - particle_list[j]->getPosition();
                    E_p += -0.5 * particle_list[i]->getMu() * particle_list[j]->getMu() / r.norm();
                }
            }
        }
        E_tol = E_k + E_p;
    }

}
    