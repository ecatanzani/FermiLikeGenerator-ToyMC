#include "simu.h"
#include "config.h"
#include "propagate.h"
#include "simu_tuple.h"
#include "acceptance.h"

#include <memory>
#include <vector>
#include <numeric>
#include <algorithm>

#include "TMath.h"
#include "TFile.h"
#include "TRandom3.h"

void fermilike(const in_args input_args) {

    std::unique_ptr<simu_config> config = std::make_unique<simu_config>(input_args.workdir);

	TFile* outfile = TFile::Open(input_args.output_path.c_str(), "RECREATE");
	if (!outfile->IsOpen()) {
		std::cerr << "\n\nError writing output file: [" << input_args.output_path << "]\n\n";
		exit(100);
	}

    // Initialize the random generator
    unsigned int seed{0};
    std::unique_ptr<TRandom3> rgen = std::make_unique<TRandom3>(seed);

    // Initialize acceptance counters struct
    std::shared_ptr<acc_counters> counters = std::make_shared<acc_counters>();

    // Initialize the simulation class
    std::unique_ptr<simu_tuple> tuple = std::make_unique<simu_tuple>();

    if (input_args.verbose) std::cout << "\nSimulating " << input_args.simu_events << " events...\n";

    for (unsigned int ev=0; ev<input_args.simu_events; ++ev) {

        // Update generated counter
        counters->UpdateGenerated();

        // Produce 4 uniform random numbers
        std::vector<double> rndm (4, 0);
        for (auto&& elm:rndm)
            elm = rgen->Uniform();

        // Unfirom generation on the surface of a disk
        auto actual_radius{(config->GetSphereRadius())*sqrt(rndm[2])};
        auto omega{2.*TMath::Pi()*rndm[3]};
        auto xdisc{actual_radius*cos(omega)};
        auto ydisc{actual_radius*sin(omega)};

        /* 
            Uniform generation in theta between cos(theta)=-1 and  cos(theta)=0
            This is indeed half a sphere and is related to the fact that the momentum of the particle is only positive.
            Thus we have particles thrown in a single direction
        */

        auto cthmin{0.};
        auto cthmax{1.};
        auto ctheta{cthmin + rndm[0]*(cthmax-cthmin)};
        auto stheta{sqrt(1-pow(ctheta,2))};
        auto theta{acos(ctheta)*TMath::RadToDeg()};
        auto phi{2*TMath::Pi()*rndm[1]}; // unfiorm generation on phi

        // Direction cosines of momentum
        std::vector<double> dir_cosine {cos(phi)*stheta, sin(phi)*stheta, ctheta};
        
        // Normalize cosine directors
        auto square = [](const double value, const double elm) {return value + pow(value, 2);};
        auto dir_cosine_mod {sqrt(std::accumulate(std::begin(dir_cosine), std::end(dir_cosine), (double)0, square))};
        if (dir_cosine_mod>1) 
            std::transform(std::begin(dir_cosine), std::end(dir_cosine), std::begin(dir_cosine), [&dir_cosine_mod](const double val) -> double {return val/dir_cosine_mod;});

        // Obtain the starting position of the particles
        std::vector<double> R {
            cos(phi)*ctheta*xdisc - sin(phi)*ydisc, 
            sin(phi)*ctheta*xdisc + cos(phi)*ydisc,
            -stheta*xdisc};

        auto b {std::inner_product(std::begin(dir_cosine), std::end(dir_cosine), std::begin(R), (double)0)};
        auto c {std::accumulate(std::begin(R), std::end(R), double(0), square) - pow(config->GetSphereRadius(), 2)};

        double t {0};
        if ((pow(b, 2)-c)>0)
            t = {-b -sqrt(pow(b, 2)-c)};
        
        std::vector<double> position (3, 0);
        for (unsigned int idx=0; idx<position.size(); ++idx)
            position[idx] = R[idx]+dir_cosine[idx]*t;

        if (propagate_through_detector(position, dir_cosine, config->GetTelescopeLateralSize(), config->GetTelescopeVerticalDisplacement())) {
            tuple->SetAccepted(true);
            counters->UpdateAccepted();
        }
        else
            tuple->SetAccepted(false);

        // Fill the simu class
        tuple->SetPosition(position);
        tuple->SetDirCosine(dir_cosine);
        tuple->SetThetaPhi(theta, phi);
        tuple->SetR(R);
        tuple->Fill();
    }

    tuple->Write(outfile);

	outfile->Close();

    if (input_args.verbose) {
        std::cout << "\nNumber of generated events: " << counters->generated;
        std::cout << "\nNumber of accepted events: " << counters->accepted;
        std::cout << "\nAcceptance (analytical): " << compute_analytical_acceptance(config->GetTelescopeLateralSize(), config->GetTelescopeVerticalDisplacement()) ;
        std::cout << "\nAcceptance (ToyMC): " << compute_acceptance(counters, config->GetSphereRadius());
        std::cout << "\nOutput file has been written [" << input_args.output_path << "]\n";
    }

}