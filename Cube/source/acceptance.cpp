#include "acceptance.h"

#include <cmath>

const double compute_analytical_acceptance(const double telescope_lateral, const double telescope_vertical)
{
    /* 
        We are using square detectors. Thus referring to Sullivan alpha and beta are equal to telescope_lateral.
        For the same reasond gamma and delta are zero.    
    */

    double acceptance{0};
    acceptance += pow(telescope_vertical, 2)*log(((pow(telescope_vertical, 2) + pow(telescope_lateral, 2))/(pow(telescope_vertical, 2) + 2*pow(telescope_lateral, 2)))*(( pow(telescope_vertical, 2) + pow(telescope_lateral, 2))/(pow(telescope_vertical, 2))));
    acceptance += 4*telescope_lateral*sqrt(pow(telescope_vertical, 2) + pow(telescope_lateral, 2))*atan(telescope_lateral/sqrt(pow(telescope_vertical, 2) + pow(telescope_lateral, 2)));
    acceptance -= 4*telescope_lateral*telescope_vertical*atan(telescope_lateral/telescope_vertical);

    return acceptance;
}

const double compute_acceptance(std::shared_ptr<acc_counters> counters, const double cube_size) {
    double ratio {static_cast<double>(counters->accepted)/counters->generated};
    double generation_surfice {6*pow(cube_size, 2)};
    return ratio*M_PI*generation_surfice;
}