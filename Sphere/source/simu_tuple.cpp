#include "simu_tuple.h"

simu_tuple::simu_tuple(std::string name, std::string desc) {
    
    // Initialize output TTree
    simu_tree = std::make_shared<TTree>(name.c_str(), desc.c_str());

    // Branch
    branch();
}

void simu_tuple::branch() {
    simu_tree->Branch("position", &position);
    simu_tree->Branch("uplayer_position", &uplayer_position);
    simu_tree->Branch("downlayer_position", &downlayer_position);
    simu_tree->Branch("dir_cosine", &dir_cosine);
    simu_tree->Branch("R", &R);
    simu_tree->Branch("theta", &theta, "theta/D");
    simu_tree->Branch("phi", &phi, "phi/D");
    simu_tree->Branch("geometric_accepted", &geometric_accepted, "geometric_accepted/O");
}

void simu_tuple::SetPosition(const std::vector<double>& simu_position) {
    for (unsigned int idx=0; idx<simu_position.size(); ++idx)
        position[idx] = simu_position[idx];
}

void simu_tuple::SetUpLayerPosition(const std::vector<double>& up_simu_position) {
    for (unsigned int idx=0; idx<up_simu_position.size(); ++idx)
        uplayer_position[idx] = up_simu_position[idx];
}

void simu_tuple::SetDownLayerPosition(const std::vector<double>& down_simu_position) {
    for (unsigned int idx=0; idx<down_simu_position.size(); ++idx)
        downlayer_position[idx] = down_simu_position[idx];
}

void simu_tuple::SetLayerPosition(std::tuple<std::vector<double>, std::vector<double>>& layer_position) {
    for (unsigned int idx=0; idx<(std::get<0>(layer_position)).size(); ++idx) {
        uplayer_position[idx] = (std::get<0>(layer_position))[idx];
        downlayer_position[idx] = (std::get<1>(layer_position))[idx];
    }
}

void simu_tuple::SetDirCosine(const std::vector<double>& simu_dir_cosine) {
    for (unsigned int idx=0; idx<simu_dir_cosine.size(); ++idx)
        dir_cosine[idx] = simu_dir_cosine[idx];
}

void simu_tuple::SetThetaPhi(const double simu_theta, const double simu_phi) {
    theta = simu_theta;
    phi = simu_phi;
}

void simu_tuple::SetR(const std::vector<double>& simu_R) {
    for (unsigned int idx=0; idx<simu_R.size(); ++idx)
        R[idx] = simu_R[idx];
}

void simu_tuple::SetAccepted(const bool accepted) {
    geometric_accepted = accepted;
}

void simu_tuple::Write(TFile* outfile) {
    outfile->cd();
    simu_tree->Write();
}