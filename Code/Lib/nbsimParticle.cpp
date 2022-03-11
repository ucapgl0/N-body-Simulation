#include "nbsimParticle.h"
#include <iostream>
#include <Eigen/Dense>

namespace nbsim{

    Particle::Particle(const Particle &p){
        Velocity = p.Velocity;
        Position = p.Position;
    }

    Eigen::Vector3d Particle::getPosition(){
        return Position;
    }

    Eigen::Vector3d Particle::getVelocity(){
        return Velocity;
    }

    Particle::Particle(Eigen::Vector3d position, Eigen::Vector3d velocity){
        Position = position;
        Velocity = velocity;
    }
    
    void Particle::integrateTimestep(Eigen::Vector3d acceleration, double time){
        Position = Position + Velocity*time;
        Velocity = Velocity + acceleration*time;
    }

}