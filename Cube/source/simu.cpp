#include "simu.h"
#include "config.h"
#include "cube.h"
#include "propagate.h"
#include "simu_tuple.h"
#include "acceptance.h"

#include <memory>
#include <vector>
#include <numeric>
#include <algorithm>
#include <tuple>


#include "TMath.h"
#include "TFile.h"
#include "TRandom3.h"

void cube(const in_args input_args) {

    std::unique_ptr<simu_config> config = std::make_unique<simu_config>(input_args.workdir);

	TFile* outfile = TFile::Open(input_args.output_path.c_str(), "RECREATE");
	if (!outfile->IsOpen()) {
		std::cerr << "\n\nError writing output file: [" << input_args.output_path << "]\n\n";
		exit(100);
	}

    // Initialize acceptance counters struct
    std::shared_ptr<acc_counters> counters = std::make_shared<acc_counters>();

    // Initialize the simulation class
    std::unique_ptr<simu_tuple> tuple = std::make_unique<simu_tuple>();

    // Initiaize the cube class
    std::unique_ptr<mccube> simu_cube = std::make_unique<mccube>(config->GetCubeDimension(), input_args.simu_seed);

    if (input_args.verbose) std::cout << "\nSimulating " << input_args.simu_events << " events...\n";

    std::vector<double> position, direction;

    for (unsigned int ev=0; ev<input_args.simu_events; ++ev) {

        // Update generated counter
        counters->UpdateGenerated();

        // Generate coordinates
        std::tie(position, direction) = simu_cube->GetEvent();
        
    }

    //tuple->Write(outfile);

	outfile->Close();

    if (input_args.verbose) {
        std::cout << "\nNumber of generated events: " << counters->generated;
        std::cout << "\nNumber of accepted events: " << counters->accepted;
        std::cout << "\nAcceptance (analytical): " << compute_analytical_acceptance(config->GetTelescopeLateralSize(), config->GetTelescopeVerticalDisplacement()) ;
        //std::cout << "\nAcceptance (ToyMC): " << compute_acceptance(counters, config->GetSphereRadius());
        std::cout << "\nOutput file has been written [" << input_args.output_path << "]\n";
    }

}