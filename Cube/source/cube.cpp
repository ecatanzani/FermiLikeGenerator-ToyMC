#include "cube.h"

#include <stdlib.h>

const std::tuple<std::vector<double>, std::vector<double>> mccube::GetEvent() {
    reset();
    get_position(static_cast<unsigned int>(rand() % 6 + 1));
    get_direction();

    return std::tuple<std::vector<double>, std::vector<double>> (position, direction);
}

void mccube::get_position(const unsigned int face) {
    switch(face) {
        case 1:
            position[0] = lateral_size/2;
        case 3:
            if (!position[0])
                position[0] = -lateral_size/2;
            position[1] = rgen->Uniform(-lateral_size/2, lateral_size/2);
            position[2] = rgen->Uniform(-lateral_size/2, lateral_size/2);
            break;
        case 2:
            position[2] = lateral_size/2;
        case 4:
            if (!position[2])
                position[2] = -lateral_size/2;
            position[0] = rgen->Uniform(-lateral_size/2, lateral_size/2);
            position[2] = rgen->Uniform(-lateral_size/2, lateral_size/2);
            break;
        case 5:
            position[2] = lateral_size/2;
        default:
            if (!position[2])
                position[2] = -lateral_size/2;
            position[0] = rgen->Uniform(-lateral_size/2, lateral_size/2);
            position[1] = rgen->Uniform(-lateral_size/2, lateral_size/2);
    }
}

 void mccube::get_direction() {
    phi = rgen->Uniform(phimin, phimax);
    theta = acos(sqrt(rgen->Uniform(cos2min, cos2max)));
    direction = std::vector<double> {sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta)};
 }

void mccube::reset() {
    face_idx = 0;
    theta = 0;
    phi = 0;
    position = std::vector<double> (3, 0);
    direction = std::vector<double> (3, 0);
}