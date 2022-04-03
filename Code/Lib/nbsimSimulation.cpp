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
        for (int i=0;i<9;i++){
			particle_list[i]->calculateAcceleration();
		}
        
		for (int i=0;i<9;i++){
			particle_list[i]->integrateTimestep(step_size);
		}	
    }

    // Calculate the kinetic, potential and total energy at the beginning and end of the simulation for system
    void Simulation::calculateEnergy(){
        E_k = 0;
        E_p = 0;
        for (int i = 0; i < particle_list.size(); ++i)
        {
            Eigen::Vector3d v = particle_list[i]->getVelocity();
            E_k += 0.5 * particle_list[i]->getMu() * v.dot(v);

            for (int j = 0; j < particle_list.size(); ++j)
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
    