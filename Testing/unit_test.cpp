#include "catch.hpp"
#include "../Code/Lib/nbsimParticle.h"
#include "../Code/Lib/nbsimMassiveParticle.h"
#include <iostream>
#include <Eigen/Dense>
#include <cmath>

TEST_CASE("Particle status","[Position Velocity]"){
  // Units tests for the position and velocity of particle after applying an acceleration and time step.
    // Acceleration = (0,0,0)
    nbsim::Particle particle1(Eigen::Vector3d (1, 1, 1), Eigen::Vector3d (0, 0, 1));
    Eigen::Vector3d a1(0, 0, 0);
    double timestep = 0.001;
    for(double t=0; t<1; t += timestep){
      particle1.integrateTimestep(a1,timestep);
    }
    Eigen::Vector3d p1 = particle1.getPosition();
    Eigen::Vector3d v1 = particle1.getVelocity();
    REQUIRE(p1.isApprox(Eigen::Vector3d (1,1,2)));
    REQUIRE(v1.isApprox(Eigen::Vector3d (0,0,1)));

    // Acceleration = const
    nbsim::Particle particle2(Eigen::Vector3d (2, 1, 1), Eigen::Vector3d (1, 1, 2));
    Eigen::Vector3d a2(1, 1, 1);
    for(double t=0; t<1; t += timestep){
      particle2.integrateTimestep(a2,timestep);
    }
    Eigen::Vector3d p2 = particle2.getPosition();
    Eigen::Vector3d v2 = particle2.getVelocity();
    REQUIRE(p2.isApprox(Eigen::Vector3d (3.5,2.5,3.5), 0.01));
    REQUIRE(v2.isApprox(Eigen::Vector3d (2,2,3), 0.01));

    // Acceleration = -x(t)
    nbsim::Particle particle3(Eigen::Vector3d (1, 0, 0), Eigen::Vector3d (0, 1, 0));
    for(double t=0; t<2*M_PI; t += timestep){
      Eigen::Vector3d a3 = -particle3.getPosition();
      particle1.integrateTimestep(a3,timestep);
    }
    Eigen::Vector3d p3 = particle3.getPosition();
    Eigen::Vector3d v3 = particle3.getVelocity();
    REQUIRE(p3.isApprox(Eigen::Vector3d (1,0,0), 0.01));
    REQUIRE(v3.isApprox(Eigen::Vector3d (0,1,0), 0.01));
}

