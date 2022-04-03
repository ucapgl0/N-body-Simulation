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
        void attraction();
        void evolution(double time_length, double time_step);
        void calculateEnergy();        
        std::vector<std::shared_ptr<MassiveParticle>> getParticle_list() const;
        double getEk() const;
        double getEp() const;
        double getEtol() const;
        void printEnergy() const;
        void validationTime(double time_length, double time_step);
    };


    // SolarSystemsimulation subclass, the initial condition of particles comes from real Solar System generatred by reading a file.
    class SolarSystem : public Simulation
    {
    public:
        SolarSystem();
        ~SolarSystem();
        virtual void generator();
        void printPosition() const;
    };

    // RandomSystemsimulation subclass, the particles would be produced randomly
    class RandomSystem : public Simulation
    {
    private:
    int particle_number;
    public:
        RandomSystem();
        RandomSystem(int particle_number);
        ~RandomSystem();
        virtual void generator();
        void calculateEnergy_Openmp(int thread_number);
        void evolution_Openmp(double time_length, double time_step, int thread_number);
    };
}
#endif