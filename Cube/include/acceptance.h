#ifndef ACCEPTANCE_H
#define ACCEPTANCE_H

#include <memory>

struct acc_counters {
    unsigned int generated {0};
    unsigned int accepted {0};

    void UpdateGenerated() { ++generated; }
    void UpdateAccepted() { ++accepted; }
};

extern const double compute_analytical_acceptance(const double telescope_lateral, const double telescope_vertical);
extern const double compute_acceptance(std::shared_ptr<acc_counters> counters, const double cube_size);

#endif