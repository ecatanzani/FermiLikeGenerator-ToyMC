#include "cube.h"

#include <stdlib.h>

#include "TMath.h"

const std::tuple<std::vector<double>, std::vector<double>> mccube::GetEvent() {
    if (evts) reset();
    get_position();
    get_direction();
    rotate(static_cast<unsigned int>(rand() % 6 + 1));
    update_evts_counter();
    return std::tuple<std::vector<double>, std::vector<double>> (position, direction);
}

void mccube::get_position() {
    // Generate particle coordinates from the up face of the cube
    position[0] = rgen->Uniform(-lateral_size/2, lateral_size/2);
    position[1] = rgen->Uniform(-lateral_size/2, lateral_size/2);
    position[2] = lateral_size/2;
}

void mccube::get_direction() {
    phi = rgen->Uniform(phimin, phimax);
    theta = acos(sqrt(rgen->Uniform(cos2min, cos2max)));
    direction = std::vector<double> {sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta)};
}

void mccube::rotate(const unsigned int face) {
    double tmp {0};
    switch(face) {
        case 1:
            tmp = position[2];
            position[2] = -position[1];
            position[1] = tmp;
            tmp = direction[2];
            direction[2] = -direction[1];
            direction[1] = tmp;
            break;
        case 3:
            tmp = position[2];
            position[2] = position[1];
            position[1] = -tmp;
            tmp = direction[2];
            direction[2] = direction[1];
            direction[1] = -tmp;
            break;
        case 2:
            tmp = position[0];
            position[0] = position[2];
            position[2] = -tmp;
            tmp = direction[0];
            direction[0] = direction[2];
            direction[2] = -tmp;
            break;
        case 4:
            tmp = position[0];
            position[0] = -position[2];
            position[2] = tmp;
            tmp = direction[0];
            direction[0] = -direction[2];
            direction[2] = tmp;
            break;
        case 5:
            position[1] = -position[1];
            position[2] = -position[2];
            direction[1] = -direction[1];
            direction[2] = -direction[2];
    }
}

void mccube::reset() {
    face_idx = 0;
    theta = 0;
    phi = 0;
    position = std::vector<double> (3, 0);
    direction = std::vector<double> (3, 0);
}

void mccube::update_evts_counter() {
    ++evts;
}

const double mccube::GetTheta() {
    return theta*TMath::RadToDeg();   
}

const double mccube::GetPhi() {
    return phi*TMath::RadToDeg();
}