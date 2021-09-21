#ifndef SIMU_TUPLE_H
#define SIMU_TUPLE_H

#include <string>
#include <memory>
#include <tuple>

#include "TFile.h"
#include "TTree.h"
#include "TVector3.h"

class simu_tuple {
    public:
        simu_tuple(std::string name="mcsphere", std::string desc="ToyMC - Spherical Geometry");
        ~simu_tuple() {};
        
        void SetPosition(const std::vector<double> &simu_position);
        void SetUpLayerPosition(const std::vector<double> &up_simu_position);
        void SetDownLayerPosition(const std::vector<double> &down_simu_position);
        void SetLayerPosition(std::tuple<std::vector<double>, std::vector<double>> &layer_position);
        void SetDirCosine(const std::vector<double>& simu_dir_cosine);
        void SetThetaPhi(const double simu_theta, const double simu_phi);
        void SetR(const std::vector<double>& simu_R);
        void SetAccepted(const bool accepted);
        void Fill() {simu_tree->Fill();};
        void Write(TFile* outfile);

    private:
        void branch();

        std::shared_ptr<TTree> simu_tree;
        TVector3 position {0, 0, 0};
        TVector3 uplayer_position {0, 0, 0};
        TVector3 downlayer_position {0, 0, 0};
        TVector3 dir_cosine {0, 0, 0};
        TVector3 R {0, 0, 0};
        double theta{0}, phi{0};
        bool geometric_accepted{false};
};

#endif