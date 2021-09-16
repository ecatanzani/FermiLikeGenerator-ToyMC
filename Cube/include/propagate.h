#ifndef PROPAGATE_H
#define PROPAGATE_H

#include <vector>

extern const bool propagate(
    const std::vector<double> dir_cosine,
    const std::vector<double> position,
    std::vector<double> &detector_layer_position,
    const double detector_lateral);

extern const bool propagate_through_detector(
    const std::vector<double> position, 
    const std::vector<double> dir_cosine,
    const double detector_lateral,
    const double detector_vertical,
    const bool bottom_active);

#endif