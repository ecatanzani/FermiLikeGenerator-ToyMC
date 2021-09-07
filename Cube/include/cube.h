#ifndef CUBE_H
#define CUBE_H

#include <math.h>
#include <vector>
#include <tuple>
#include <memory>

#include "TRandom3.h"

class mccube {
    public:
        mccube(const double size, const unsigned int seed) :    position (3, 0), 
                                                                direction (3, 0) {
                                                                    rgen = std::make_unique<TRandom3>(seed);
                                                                    lateral_size = size;
                                                            };
        ~mccube() {};
        
        const std::tuple<std::vector<double>, std::vector<double>> GetEvent();
    private:
        void reset();
        void get_position(const unsigned int face);
        void get_direction();

        double lateral_size {0};
        unsigned int face_idx {0};
        
        const double cos2max {1};
        const double cos2min {0};
        const double phimax {2*M_PI};
        const double phimin {0};

        double theta {0};
        double phi {0};

        std::vector<double> position;
        std::vector<double> direction;

        std::unique_ptr<TRandom3> rgen;
};

#endif