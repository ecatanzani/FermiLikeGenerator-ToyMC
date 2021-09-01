#include "plot_acceptance.h"

#include <iostream>

#include "TKey.h"
#include "TH1D.h"
#include "TFile.h"
#include <ROOT/RDataFrame.hxx>

inline const std::string get_tree_name(const std::string file) {
    TFile* input_file = TFile::Open(file.c_str(), "READ");
    if (!input_file->IsOpen()) {
        std::cerr << "\n\nError reading input file [" << file << "]\n\n";
        exit(100);
    }
    std::string tree_name{""};
    for (TObject* keyAsObject : *input_file->GetListOfKeys()) {
        auto key = dynamic_cast<TKey*>(keyAsObject);
        if (!strcmp(key->GetClassName(), "TTree"))
            tree_name = static_cast<std::string>(key->GetName());
    }
    return tree_name;
}

inline const double progress (unsigned int done, unsigned int total) {
    return (static_cast<double>(done+1)/total)*100;
}

void build_acceptance_plots(
    const std::vector<std::string> files, 
    const double telescope_lateral,
    const double telescope_vertical,
    const double sphere_radius,
    const unsigned int threads,
    const std::string output_path,
    const bool verbose) {

    if (verbose) std::cout << "\nAnalysis running...\n";

    std::unique_ptr<TH1D> h_acceptance = std::make_unique<TH1D>("h_acceptance", "acceptance histo", 100, 0, 4);
    std::unique_ptr<TH1D> h_acceptance_diff = std::make_unique<TH1D>("h_acceptance_diff", "acceptance histo difference", 100, -1, 1);
    
    double analytical_acceptance {compute_analytical_acceptance(telescope_lateral, telescope_vertical)};
    auto tree_name {get_tree_name(files[0])};
    std::shared_ptr<acc_counters> counters = std::make_shared<acc_counters>();
    unsigned int total_files {static_cast<unsigned int>(files.size())};
    unsigned int file_counter {0};

    ROOT::EnableImplicitMT(threads);
    
    for (auto& file:files) {
        if (verbose) std::cout << "\nProcessing file (" << progress(file_counter, total_files) << "%) - [" << file << "]";
        TFile* input_file = TFile::Open(file.c_str(), "READ");
        if (!input_file->IsOpen()) {
            std::cerr << "\n\nError reading input file [" << file << "]\n\n";
            exit(100);
        }
        ROOT::RDataFrame _data_fr(*(dynamic_cast<TTree*>(input_file->Get(tree_name.c_str()))));

        counters->UpdateGeneratedTo(*(_data_fr.Count()));
        counters->UpdateAcceptedTo(*(_data_fr.Filter("geometric_accepted==true").Count()));

        double acceptance {compute_acceptance(counters, sphere_radius)};
        h_acceptance->Fill(acceptance);
        h_acceptance_diff->Fill(acceptance-analytical_acceptance);
        input_file->Close();
        ++file_counter;
    }

    TFile *outfile = TFile::Open(output_path.c_str(), "RECREATE");
    if (!outfile->IsOpen()) {
        std::cerr << "\n\nError writing output file [" << output_path << "]\n\n";
        exit(100);
    }

    h_acceptance->Write();
    h_acceptance_diff->Write();

    outfile->Close();

    if (verbose) std::cout << "\n\nOutput file has been written [" << output_path << "]\n\n";

}

const double compute_analytical_acceptance(const double telescope_lateral, const double telescope_vertical)
{
    /* 
        We are using square detectors. Thus referring to Sullivan alpha and beta are equal to telescope_lateral.
        For the same reasond gamma and delta are zero.    
    */

    double acceptance{0};
    acceptance += pow(telescope_vertical, 2)*log(((pow(telescope_vertical, 2) + pow(telescope_lateral, 2))/(pow(telescope_vertical, 2) + 2*pow(telescope_lateral, 2)))*(( pow(telescope_vertical, 2) + pow(telescope_lateral, 2))/(pow(telescope_vertical, 2))));
    acceptance += 4*telescope_lateral*sqrt(pow(telescope_vertical, 2) + pow(telescope_lateral, 2))*atan(telescope_lateral/sqrt(pow(telescope_vertical, 2) + pow(telescope_lateral, 2)));
    acceptance -= 4*telescope_lateral*telescope_vertical*atan(telescope_lateral/telescope_vertical);

    return acceptance;
}

const double compute_acceptance(std::shared_ptr<acc_counters> counters, const double sphere_radius) {
    double ratio {static_cast<double>(counters->accepted)/counters->generated};
    double generation_surfice {4*M_PI*pow(sphere_radius, 2)/2};
    return ratio*M_PI*generation_surfice;
}