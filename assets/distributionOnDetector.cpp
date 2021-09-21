#include "TKey.h"
#include "TFile.h"
#include <ROOT/RDataFrame.hxx>

#include <iostream>
#include <memory>

void getDistributionOnDetectorLayers(
    const char* in_path,
    const char* out_path,
    const double Xstart,
    const double Xend, 
    const int Xbins, 
    const double Ystart, 
    const double Yend, 
    const int Ybins,
    unsigned int threads = 1) {
    
    TFile* input_file = TFile::Open(in_path, "READ");
    if (!input_file->IsOpen()) {
        std::cerr << "\n\nError reading input file [" << in_path << "]\n\n";
        exit(100);
    }

    std::unique_ptr<TTree> evt;
    for (TObject* keyAsObject : *input_file->GetListOfKeys()) {
        auto key = dynamic_cast<TKey*>(keyAsObject);
        if (!strcmp(key->GetClassName(), "TTree"))
            evt = std::unique_ptr<TTree>(static_cast<TTree*>(input_file->Get(key->GetName())));
    }

    ROOT::EnableImplicitMT(threads);
    ROOT::RDataFrame _data_fr(*evt);

    auto h_up_X_distribution = _data_fr.Define("X", "uplayer_position.X()").Histo1D({"h_up_X_distribution", "X distribution - UP detector; X (m)", Xbins, Xstart, Xend}, "X");
    auto h_up_Y_distribution = _data_fr.Define("Y", "uplayer_position.Y()").Histo1D({"h_up_Y_distribution", "Y distribution - UP detector; Y (m)", Ybins, Ystart, Yend}, "Y");
    auto h_up_distribution = _data_fr.Define("X", "uplayer_position.X()")
                                    .Define("Y", "uplayer_position.Y()")
                                    .Histo2D({"h_up_distribution", "UP detector distribution; X (m); Y (m)", Xbins, Xstart, Xend, Ybins, Ystart, Yend}, "X", "Y");

    auto h_down_X_distribution = _data_fr.Define("X", "downlayer_position.X()").Histo1D({"h_down_X_distribution", "X distribution - BOTTOM detector; X (m)", Xbins, Xstart, Xend}, "X");
    auto h_down_Y_distribution = _data_fr.Define("Y", "downlayer_position.Y()").Histo1D({"h_down_Y_distribution", "Y distribution - BOTTOM detector; Y (m)", Ybins, Ystart, Yend}, "Y");
    auto h_down_distribution = _data_fr.Define("X", "downlayer_position.X()")
                                    .Define("Y", "downlayer_position.Y()")
                                    .Histo2D({"h_down_distribution", "BOTTOM detector distribution; X (m); Y (m)", Xbins, Xstart, Xend, Ybins, Ystart, Yend}, "X", "Y");



    TFile* output_file = TFile::Open(out_path, "RECREATE");
    if (!output_file->IsOpen()) {
        std::cerr << "\n\nError writing output file [" << out_path << "]\n\n";
        exit(100);
    }

    h_up_X_distribution->Write();
    h_up_Y_distribution->Write();
    h_up_distribution->Write();
    h_down_X_distribution->Write();
    h_down_Y_distribution->Write();
    h_down_distribution->Write();

    output_file->Close();
}