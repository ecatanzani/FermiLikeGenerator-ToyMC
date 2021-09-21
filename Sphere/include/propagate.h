#ifndef PROPAGATE_H
#define PROPAGATE_H

#include <vector>
#include <tuple>

extern std::tuple<bool, std::tuple<std::vector<double>, std::vector<double>>> propagate_through_detector(
    const std::vector<double> position, 
    const std::vector<double> dir_cosine,
    const double detector_lateral,
    const double detector_vertical);
    
extern const bool propagate(
    const std::vector<double> dir_cosine,
    const std::vector<double> position,
    std::vector<double> &detector_layer_position,
    const double detector_lateral);

#endif