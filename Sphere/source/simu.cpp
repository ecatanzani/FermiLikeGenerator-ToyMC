#include "simu.h"
#include "config.h"
#include "sphere.h"
#include "propagate.h"
#include "simu_tuple.h"
#include "acceptance.h"

#include "progressbar.hpp"

#include <tuple>
#include <memory>
#include <vector>
#include <numeric>
#include <algorithm>

#include "TMath.h"
#include "TFile.h"
#include "TRandom3.h"

void flsphere(const in_args input_args) {

    std::unique_ptr<simu_config> config = std::make_unique<simu_config>(input_args.workdir);
    if (input_args.verbose) config->PrintConfigPars();

	TFile* outfile = TFile::Open(input_args.output_path.c_str(), "RECREATE");
	if (!outfile->IsOpen()) {
		std::cerr << "\n\nError writing output file: [" << input_args.output_path << "]\n\n";
		exit(100);
	}
   
    // Initialize acceptance counters struct
    std::shared_ptr<acc_counters> counters = std::make_shared<acc_counters>();

    // Initialize the simulation class
    std::unique_ptr<simu_tuple> tuple = std::make_unique<simu_tuple>();

    // Initiaize the sphere class
    std::unique_ptr<mcsphere> simu_sphere = std::make_unique<mcsphere>(config->GetSphereRadius(), input_args.simu_seed);

    if (input_args.verbose) std::cout << "\nSimulating " << input_args.simu_events << " events...\n\n";

    std::vector<double> position, direction;
    std::tuple<std::vector<double>, std::vector<double>> detector_position;
    bool status;
    progressbar bar(input_args.simu_events);
    bar.set_todo_char(" ");
    bar.set_done_char("█");

    for (unsigned int ev=0; ev<input_args.simu_events; ++ev) {
        bar.update();

        // Update generated counter
        counters->UpdateGenerated();

        // Generate coordinates and directions
        std::tie(position, direction) = simu_sphere->GetEvent();
        std::tie(status, detector_position) = propagate_through_detector(position, direction, config->GetTelescopeLateralSize(), config->GetTelescopeVerticalDisplacement());
        if (status) {
            tuple->SetAccepted(true);
            counters->UpdateAccepted();
        }
        else
            tuple->SetAccepted(false);

        // Fill the simu class
        tuple->SetPosition(position);
        tuple->SetLayerPosition(detector_position);
        tuple->SetDirCosine(direction);
        tuple->SetThetaPhi(simu_sphere->GetTheta(), simu_sphere->GetPhi());
        tuple->SetR(simu_sphere->GetImpactParameter());
        tuple->Fill();
    }

	outfile->Close();

    if (input_args.verbose) {
        auto theo_acc {compute_analytical_acceptance(config->GetTelescopeLateralSize(), config->GetTelescopeVerticalDisplacement())};
        auto mc_acc {compute_acceptance(counters, config->GetSphereRadius())};

        std::cout << "\n\n*** Stats ***\n";
        std::cout << "\nNumber of generated events: " << counters->generated;
        std::cout << "\nNumber of accepted events: " << counters->accepted;
        std::cout << "\nAcceptance (analytical): " << theo_acc;
        std::cout << "\nAcceptance (ToyMC): " << mc_acc;
        std::cout << "\nDiscrepancy theoretical/MC: [" << (fabs(theo_acc-mc_acc)/theo_acc)*100 << " %]\n";
        std::cout << "\nOutput file has been written [" << input_args.output_path << "]\n";
    }

}