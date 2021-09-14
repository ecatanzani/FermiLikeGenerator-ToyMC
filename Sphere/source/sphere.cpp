#include "sphere.h"

#include <math.h>
#include <stdlib.h>
#include <numeric>
#include <algorithm>

#include "TMath.h"

const std::tuple<std::vector<double>, std::vector<double>> mcsphere::GetEvent() {
    if (evts) reset();
    get_random();
    generate_on_disc();
    get_direction();
    get_position();
    update_evts_counter();
    return std::tuple<std::vector<double>, std::vector<double>> (position, direction);
}

void mcsphere::get_random() {
    for (auto&& elm:rndm)
        elm = rgen->Uniform();
}

void mcsphere::generate_on_disc() {
    // Unfirom generation on the surface of a disk
    actual_radius = sphere_radius*sqrt(rndm[2]);
    omega = 2.*M_PI*rndm[3];
    xdisc = actual_radius*cos(omega);
    ydisc = actual_radius*sin(omega);
}

void mcsphere::get_direction() {
    /* 
        Uniform generation in theta between cos(theta)=1 and  cos(theta)=0
        This is indeed half a sphere and is related to the fact that the momentum of the particle is only positive.
        Thus we have particles thrown in a single direction
    */
    
    ctheta = cthmin + rndm[0]*(cthmax-cthmin);
    stheta = sqrt(1-pow(ctheta,2));
    theta = acos(ctheta)*TMath::RadToDeg();
    phi = 2*M_PI*rndm[1]; // unfiorm generation on phi

    // Direction cosines of momentum
    direction = std::vector<double> {cos(phi)*stheta, sin(phi)*stheta, ctheta};

    // Normalize cosine directors
    auto dir_cosine_mod {sqrt(std::accumulate(
        std::begin(direction), 
        std::end(direction), 
        (double)0,
        [](const double value, const double elm) {return value + pow(elm, 2);}))};

    if (dir_cosine_mod>1) 
        std::transform(
            std::begin(direction), 
            std::end(direction), 
            std::begin(direction), 
            [&dir_cosine_mod](const double val) -> double {return val/dir_cosine_mod;});
}

void mcsphere::get_position() {
    // Obtain the starting position of the particles
    R = std::vector<double> {
        cos(phi)*ctheta*xdisc - sin(phi)*ydisc, 
        sin(phi)*ctheta*xdisc + cos(phi)*ydisc,
        -stheta*xdisc};

    b = std::inner_product(std::begin(direction), std::end(direction), std::begin(R), (double)0);
    c = std::accumulate(std::begin(R), std::end(R), double(0), [](const double value, const double elm) {return value + pow(elm, 2);}) - pow(sphere_radius, 2);

    if ((pow(b, 2)-c)>=0)
        t = {-b -sqrt(pow(b, 2)-c)};
    
    for (unsigned int idx=0; idx<position.size(); ++idx)
            position[idx] = R[idx]+direction[idx]*t;
}

void mcsphere::reset() {
    actual_radius = 0;
    omega = 0;
    xdisc = 0;
    ydisc = 0;
    ctheta = 0;
    stheta = 0;
    theta = 0;
    phi = 0;
    b = 0;
    c = 0;
    t = 0;
    position = std::vector<double> (3, 0);
    direction = std::vector<double> (3, 0);
    R = std::vector<double> (3, 0);
    rndm = std::vector<double> (4, 0);
}

void mcsphere::update_evts_counter() {
    ++evts;
}

const double mcsphere::GetTheta() {
    return theta;   
}

const double mcsphere::GetPhi() {
    return phi*TMath::RadToDeg();
}

const std::vector<double> mcsphere::GetImpactParameter() {
    return R;
}