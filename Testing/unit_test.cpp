#include "catch.hpp"
#include "../Code/Lib/nbsimParticle.h"
#include "../Code/Lib/nbsimMassiveParticle.h"
#include <iostream>
#include <Eigen/Dense>

TEST_CASE("Particle status","[Position Velocity]"){
  // Units tests for the position and velocity of particle after applying an acceleration and time step.
    // Acceleration = (0,0,0)
    nbsim::Particle particle1(Eigen::Vector3d (1, 0, 0), Eigen::Vector3d (0, 0, 1));
    particle1.integrateTimestep(Eigen::Vector3d (0, 0, 0),0.5);
    Eigen::Vector3d p1 = particle1.getPosition();
    Eigen::Vector3d v1 = particle1.getVelocity();
    REQUIRE(p1.isApprox(Eigen::Vector3d (1,0,0.5)));
    REQUIRE(v1.isApprox(Eigen::Vector3d (0,0,1)));

    // Acceleration = const
    nbsim::Particle particle2(Eigen::Vector3d (2, 1, 1), Eigen::Vector3d (1, 1, 1));
    particle2.integrateTimestep(Eigen::Vector3d (1, 1, 1),0.5);
    Eigen::Vector3d p2 = particle2.getPosition();
    Eigen::Vector3d v2 = particle2.getVelocity();
    REQUIRE(p2.isApprox(Eigen::Vector3d (2.5,1.5,1.5)));
    REQUIRE(v2.isApprox(Eigen::Vector3d (1.5,1.5,1.5)));

    // Acceleration = -x(t)
    nbsim::Particle particle3(Eigen::Vector3d (1, 0, 0), Eigen::Vector3d (0, 1, 0));
    particle3.integrateTimestep(Eigen::Vector3d (1, 1, 1),0.5);
    Eigen::Vector3d p3 = particle3.getPosition();
    Eigen::Vector3d v3 = particle3.getVelocity();
    REQUIRE(p3.isApprox(Eigen::Vector3d (1,0.5,0)));
    REQUIRE(v3.isApprox(Eigen::Vector3d (0.5,1.5,0.5)));
}