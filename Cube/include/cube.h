#ifndef CUBE_H
#define CUBE_H

#include <math.h>
#include <vector>
#include <tuple>
#include <memory>

#include "config.h"

#include "TRandom3.h"

class mccube {
    public:
        mccube(
            const double size,
            std::shared_ptr<cube_faces> faces,
            const unsigned int seed) :              position (3, 0), 
                                                    direction (3, 0) {
                                                        rgen = std::make_unique<TRandom3>(seed);
                                                        lateral_size = size;
                                                        mycube_details = faces;
                                                    };
        ~mccube() {};
        
        const std::tuple<std::vector<double>, std::vector<double>> GetEvent();
        const double GetTheta();
        const double GetPhi();
    private:
        void reset();
        void get_position();
        void get_direction();
        const unsigned int get_random_face();
        void rotate(const unsigned int face);
        void invert_direction();
        void update_evts_counter();

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

        unsigned int face {0};
        unsigned int evts {0};
        std::unique_ptr<TRandom3> rgen;
        std::shared_ptr<cube_faces> mycube_details;
};

#endif