#ifndef MassiveParticle_h
#define MassiveParticle_h
#include "nbsimParticle.h"
#include <iostream>
#include <Eigen/Dense>
#include <set>
#include <memory>

namespace nbsim{

class MassiveParticle : public Particle{

public:
    MassiveParticle();
    //MassiveParticle(const MassiveParticle & m);
    MassiveParticle(Eigen::Vector3d pos, Eigen::Vector3d vel, double m);
    double getMu();
    void addAttractor(std::shared_ptr<MassiveParticle> attractor);
    void removeAttractor(std::shared_ptr<MassiveParticle> attractor);
    void calculateAcceleration();
    void integrateTimestep(double timestep);

private:
    double mass;
    Eigen::Vector3d acceleration;
    std::set<std::shared_ptr<MassiveParticle>> attractors;
};
}
#endif