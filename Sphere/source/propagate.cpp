#include "propagate.h"

#include <cmath>

#include <iostream>

const bool propagate_through_detector(
    const std::vector<double> position, 
    const std::vector<double> dir_cosine,
    const double detector_lateral,
    const double detector_vertical)
{
    std::vector<double> uplayer_position {0, 0, detector_vertical/2};
    std::vector<double> downlayer_position {0, 0, -detector_vertical/2};
    bool status {false};

    if (position[2]<downlayer_position[2]) {
    
        const bool up_status = propagate(dir_cosine, position, uplayer_position, detector_lateral);
        const bool down_status = up_status ? propagate(dir_cosine, position, downlayer_position, detector_lateral) : false;

        if (up_status && down_status) status = true;
    }

    return status;
}

const bool propagate(
    const std::vector<double> dir_cosine,
    const std::vector<double> position,
    std::vector<double> &detector_layer_position,
    const double detector_lateral)
{
    bool status = true;
    
    double dz {fabs(position[2] - detector_layer_position[2])};
    double radius {dz/dir_cosine[2]};

    for (unsigned int idx{0}; idx<detector_layer_position.size()-1; ++idx) {
        detector_layer_position[idx] = position[idx] + radius*dir_cosine[idx];
        if (fabs(detector_layer_position[idx])>detector_lateral/2) {
            status = false;
            break;
        }
    }

    return status;
}