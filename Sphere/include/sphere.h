#ifndef SPHERE_H
#define SPHERE_H

#include <math.h>
#include <vector>
#include <tuple>
#include <memory>

#include "TRandom3.h"

class mcsphere {
    public:
        mcsphere(const double radius, const unsigned int seed) :    rndm (4, 0),
                                                                    position (3, 0), 
                                                                    direction (3, 0),
                                                                    R (3, 0) {
                                                                        rgen = std::make_unique<TRandom3>(seed);
                                                                        sphere_radius = radius;
                                                                    };
        ~mcsphere() {};
        
        const std::tuple<std::vector<double>, std::vector<double>> GetEvent();
        const double GetTheta();
        const double GetPhi();
        const std::vector<double> GetImpactParameter();
    private:
        void reset();
        void get_random();
        void generate_on_disc();
        void get_position();
        void get_direction();
        

        double sphere_radius {0};
        
        const double cthmin {0};
        const double cthmax {1};

        double actual_radius {0};
        double omega {0};
        double xdisc {0};
        double ydisc {0};
        double ctheta {0};
        double stheta {0};

        double theta {0};
        double phi {0};

        double b {0};
        double c {0};
        double t {0};

        std::vector<double> rndm;

        std::vector<double> position;
        std::vector<double> direction;
        std::vector<double> R;

        std::unique_ptr<TRandom3> rgen;
};

#endif