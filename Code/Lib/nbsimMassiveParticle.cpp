#include "nbsimMassiveParticle.h"
#include <iostream>
#include <Eigen/Dense>

namespace nbsim{

    MassiveParticle::MassiveParticle(Eigen::Vector3d pos, Eigen::Vector3d vel, double m):Particle(pos,vel){
        mass = m;    
    }

    double MassiveParticle::getMu(){
        double Gravitational_constant = 6.67e-11;
        return mass*Gravitational_constant;
    }

     void MassiveParticle::addAttractor(std::shared_ptr<MassiveParticle> attractor){
        attractors.insert(attractor);
    }

    void MassiveParticle::removeAttractor(std::shared_ptr<MassiveParticle> attractor){
        attractors.erase(attractor);
    }

    void MassiveParticle::calculateAcceleration(){
        acceleration = Eigen::Vector3d (0,0,0);
        for(auto attractor:attractors){
            Eigen::Vector3d r_i = getPosition() - attractor->getPosition();
            acceleration += (-attractor->getMu()/r_i.dot(r_i)) * r_i.normalized();
        }
    }

    void MassiveParticle::integrateTimestep(double time){
        Position = Position + Velocity*time;
        Velocity = Velocity + acceleration*time;
    }
}