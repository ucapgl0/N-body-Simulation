#ifndef nbsimSimulation_h
#define nbsimSimulation_h

#include "nbsimParticle.h"
#include "nbsimMassiveParticle.h"
#include <Eigen/Dense>
#include <iostream>
#include <vector>

namespace nbsim
{
    
    class Simulation
    {
    protected:
        double E_k; // The kinetic energy
        double E_p; // The potential energy
        double E_tol; // The total energy
        std::vector<std::shared_ptr<MassiveParticle>> particle_list;

    public:
        Simulation();
        Simulation(std::vector<std::shared_ptr<MassiveParticle>> _particlelist);
        ~Simulation();
        virtual void generator()=0;
        void attract();
        void calculateEnergy();
        void evolution(double time_length, double time_step);
        std::vector<std::shared_ptr<MassiveParticle>> getParticlelist() const;
        double getEk() const;
        double getEp() const;
        double getEtol() const;
        void printEnergy() const;
        void validationTime(double time_length, double time_step);
    };

    // By subclassing the class Simulation, the new class can implement different generators to initialise the particlas.

    // In this SolarSystemsimulation subclass, the initial condition of particles comes from real Solar System
    // The list of particles would be generatred by reading a file.
    class SolarSystemsimulation : public Simulation
    {
    public:
        SolarSystemsimulation();
        ~SolarSystemsimulation();
        virtual void generator();
        void printPosition() const;
    };

    // In this RandomSystemsimulation subclass, the particles would be produced randomly
    // Given the amount of the particles, the generator would produce particles in random initial conditions
    class RandomSystemsimulation : public Simulation
    {
    private:
    int particle_number;
    public:
        RandomSystemsimulation();
        RandomSystemsimulation(int particle_number);
        ~RandomSystemsimulation();
        virtual void generator();
        void calculateEnergy_Openmp(int thread_number);
        void evolution_Openmp(double time_length, double time_step, int thread_number);
    };
}
#endif