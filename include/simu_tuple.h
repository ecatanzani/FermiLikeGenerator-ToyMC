#ifndef SIMU_TUPLE_H
#define SIMU_TUPLE_H

#include <string>
#include <memory>

#include "TFile.h"
#include "TTree.h"
#include "TVector3.h"

class simu_tuple {
    public:
        simu_tuple(std::string name="fermilike_dampe_simu", std::string desc="Fermi-Like Simulation Tree");
        ~simu_tuple() {};
        void SetPosition(const std::vector<double> simu_position);
        void Fill() {simu_tree->Fill();};
        void Write(TFile* outfile);

    private:
        void branch();

        std::shared_ptr<TTree> simu_tree;
        TVector3 position {0, 0, 0};
};

#endif