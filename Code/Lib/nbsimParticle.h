#ifndef Particle_h
#define Particle_h
#include <iostream>
#include <Eigen/Dense>


namespace nbsim{

class Particle{

public:
    Particle();
    ~Particle();
    Particle(const Particle & p);
    Particle(Eigen::Vector3d position, Eigen::Vector3d velocity);
    Eigen::Vector3d getPosition();
    Eigen::Vector3d getVelocity();
    void integrateTimestep(Eigen::Vector3d acceleration, double timestep);
    
private:
    Eigen::Vector3d Position;
    Eigen::Vector3d Velocity;

};

}
#endif