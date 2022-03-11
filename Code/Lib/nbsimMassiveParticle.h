#ifndef MassiveParticle_h
#define MassiveParticle_h
#include <iostream>
#include <Eigen/Dense>

namespace nbsim{

class MassiveParticle{

public:
    MassiveParticle(const MassiveParticle & m);
    void getMu();
    void addAttractor();
    void removeAttractor();
    void calculateAcceleration();
    void integrateTimestep(double timestep);

private:
    Eigen::Vector3d acceleration;
    Eigen::Vector3d position;
    Eigen::Vector3d velocity;
};
}
#endif