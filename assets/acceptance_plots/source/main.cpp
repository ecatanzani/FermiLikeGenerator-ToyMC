#include "main.h"
#include "chain.h"
#include "config.h"
#include "list_parser.h"
#include "plot_acceptance.h"

#include <string>
#include <memory>

int main(int argc, char** argv) {
    AnyOption opt;

	opt.addUsage("Usage: ");
	opt.addUsage("");

	opt.addUsage(" -h  --help                  .......... Prints this help");
	opt.addUsage(" -i  --input                 .......... <path_to_root_file_list>                             .......... (*) Input ROOT file list");
	opt.addUsage(" -w  --workdir               .......... <path_to_software_config_dir>                        .......... (*) Software config directory");
	opt.addUsage(" -o  --output                .......... <path_to_output_TFile>                               .......... Output ROOT TFile");
	opt.addUsage(" -v  --verbose               .......... Verbose output");

	opt.addUsage("");
	opt.addUsage(" -p  --parallel              .......... Number of threads");


	opt.setFlag("help", 'h');
	opt.setOption("input", 'i');
	opt.setOption("workdir", 'w');
	opt.setOption("output", 'o');
	opt.setFlag("verbose", 'v');
	opt.setOption("parallel", 'p');

    opt.processCommandArgs(argc, argv);

    in_args input_args;

    if (!opt.hasOptions()) {
		opt.printUsage();
		return 0;
	}
    if (opt.getFlag("help") || opt.getFlag('h')) {
		opt.printUsage();
		return 0;
	}
	if (opt.getValue("input") || opt.getValue('i'))
		input_args.input_path = opt.getValue('i');
    if (opt.getValue("workdir") || opt.getValue('w'))
		input_args.workdir = opt.getValue('w');
	if (opt.getValue("output") || opt.getValue('o'))
		input_args.output_path = opt.getValue('o');
	if (opt.getFlag("verbose") || opt.getFlag('v'))
		input_args.verbose = opt.getFlag('v');
	if (opt.getValue("parallel") || opt.getValue('p'))
		input_args.threads = stoul(opt.getValue('p'));

	// Parse config file
	std::unique_ptr<simu_config> config = std::make_unique<simu_config>(input_args.workdir);
	// Parse input file list
	std::unique_ptr<list_parser> parser = std::make_unique<list_parser>((input_args.input_path).c_str());

	// Build plots
	build_acceptance_plots(
		parser->GetFileList(), 
		config->GetTelescopeLateralSize(), 
		config->GetTelescopeVerticalDisplacement(),
		config->GetSphereRadius(),
		input_args.threads,
		input_args.output_path,
		input_args.verbose);

    return 0;
}