#include "simu_tuple.h"

simu_tuple::simu_tuple(std::string name, std::string desc) {
    
    // Initialize output TTree
    simu_tree = std::make_shared<TTree>(name.c_str(), desc.c_str());

    // Branch
    branch();
}

void simu_tuple::branch() {
    simu_tree->Branch("position", &position);
    simu_tree->Branch("dir_cosine", &dir_cosine);
    simu_tree->Branch("R", &R);
    simu_tree->Branch("theta", &theta, "theta/D");
    simu_tree->Branch("phi", &phi, "phi/D");
}

void simu_tuple::SetPosition(const std::vector<double> simu_position) {
    for (unsigned int idx=0; idx<simu_position.size(); ++idx)
        position[idx] = simu_position[idx];
}

void simu_tuple::SetDirCosine(const std::vector<double> simu_dir_cosine) {
    for (unsigned int idx=0; idx<simu_dir_cosine.size(); ++idx)
        dir_cosine[idx] = simu_dir_cosine[idx];
}

void simu_tuple::SetThetaPhi(const double simu_theta, const double simu_phi) {
    theta = simu_theta;
    phi = simu_phi;
}

void simu_tuple::SetR(const std::vector<double> simu_R) {
    for (unsigned int idx=0; idx<simu_R.size(); ++idx)
        R[idx] = simu_R[idx];
}

void simu_tuple::Write(TFile* outfile) {
    outfile->cd();
    simu_tree->Write();
}