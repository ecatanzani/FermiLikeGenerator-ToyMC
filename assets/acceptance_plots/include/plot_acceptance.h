#ifndef PLOT_ACCEPTANCE_H
#define PLOT_ACCEPTANCE_H

#include <string>
#include <vector>
#include <cmath>
#include <memory>

struct acc_counters {
    unsigned int generated {0};
    unsigned int accepted {0};

    void UpdateGenerated() { ++generated; }
    void UpdateAccepted() { ++accepted; }
    void UpdateGeneratedTo(const unsigned int value) { generated = value; }
    void UpdateAcceptedTo(const unsigned int value) { accepted = value; }
    void Reset() { generated=0; accepted=0; }
};

extern void build_acceptance_plots(
    const std::vector<std::string> files, 
    const double telescope_lateral,
    const double telescope_vertical,
    const double sphere_radius,
    const unsigned int threads,
    const std::string output_path,
    const bool verbose);

extern const double compute_analytical_acceptance(const double telescope_lateral, const double telescope_vertical);
extern const double compute_acceptance(std::shared_ptr<acc_counters> counters, const double sphere_radius);

#endif