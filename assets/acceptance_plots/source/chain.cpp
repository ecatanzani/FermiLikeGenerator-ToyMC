#include "chain.h"

#include <algorithm>
#include <iostream>

#include "TKey.h"
#include "TFile.h"

chain::chain(const std::vector<std::string> files, const bool verbose) {
    evt_chain = std::make_shared<TChain>(get_tree_name(files[0]).c_str());
    append_files_to_chain(files, verbose);
}

const std::string chain::get_tree_name(const std::string file) {
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

void chain::append_files_to_chain(const std::vector<std::string> files, const bool verbose) {
    for (auto& file:files) {
        if (verbose)
            std::cout << "\nAdding element to chain [" << file << "]";
        evt_chain->Add(file.c_str());
    }
}

std::shared_ptr<TChain> chain::GetChain() {
    return evt_chain;
}