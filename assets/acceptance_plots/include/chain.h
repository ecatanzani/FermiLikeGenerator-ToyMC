#ifndef CHAIN_H
#define CHAIN_H

#include <string>
#include <vector>
#include <memory>

#include "TChain.h" 

class chain {
    public:
        chain(const std::vector<std::string> files, const bool verbose);
        ~chain() {};
        std::shared_ptr<TChain> GetChain();
    private:
        const std::string get_tree_name(const std::string file);
        void append_files_to_chain(const std::vector<std::string> files, const bool verbose);
        std::shared_ptr<TChain> evt_chain;
};

#endif