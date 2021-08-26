#include "simu_tuple.h"

simu_tuple::simu_tuple(std::string name, std::string desc) {
    
    // Initialize output TTree
    simu_tree = std::make_shared<TTree>(name.c_str(), desc.c_str());

    // Branch
    branch();
}

void simu_tuple::branch() {
    simu_tree->Branch("position", &position);
}

void simu_tuple::SetPosition(const std::vector<double> simu_position) {
    for (unsigned int idx=0; idx<simu_position.size(); ++idx)
        position[idx] = simu_position[idx];
}

void simu_tuple::Write(TFile* outfile) {
    outfile->cd();
    simu_tree->Write();
}