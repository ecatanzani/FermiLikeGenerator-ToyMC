#include "simu.h"
#include "config.h"

#include <memory>
#include <vector>

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
    




	outfile->Close();
}