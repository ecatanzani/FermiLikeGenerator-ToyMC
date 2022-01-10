#include "main.h"
#include "simu.h"
#include "utils.h"

#include <string>

int main(int argc, char** argv) {
    AnyOption opt;

	opt.addUsage("Usage: ");
	opt.addUsage("");

	opt.addUsage(" -h  --help                  .......... Prints this help");
	opt.addUsage(" -w  --workdir               .......... <path_to_software_config_dir>                        .......... (*) Software config directory");
	opt.addUsage(" -o  --output                .......... <path_to_output_TFile>                               .......... Output ROOT TFile");
	opt.addUsage(" -d  --outputdir             .......... <path_to_output_TFile_dir>                           .......... Output ROOT TFile directory");
	opt.addUsage(" -v  --verbose               .......... Verbose output");

    opt.addUsage("");

	opt.addUsage(" -n --number                 .......... Number of particles to be simulated");
	opt.addUsage(" -s --seed                   .......... Simulation seed");

	opt.addUsage("");

	opt.setFlag("help", 'h');
	opt.setOption("workdir", 'w');
	opt.setOption("output", 'o');
	opt.setOption("outputdir", 'd');
	opt.setFlag("verbose", 'v');
	opt.setOption("number", 'n');
	opt.setOption("seed", 's');

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
    if (opt.getValue("workdir") || opt.getValue('w'))
		input_args.workdir = opt.getValue('w');
	if (opt.getValue("output") || opt.getValue('o'))
		input_args.output_path = expand_output_path(opt, opt.getValue('o'));
	if (opt.getValue("outputdir") || opt.getValue('d'))
		input_args.output_path = expand_output_path(opt, opt.getValue('d'));
	if (opt.getFlag("verbose") || opt.getFlag('v'))
		input_args.verbose = opt.getFlag('v');
	if (opt.getValue("number") || opt.getValue('n'))
		input_args.simu_events = stoi(std::string(opt.getValue('n')));
	if (opt.getValue("seed") || opt.getValue('s'))
		input_args.simu_seed = stoi(std::string(opt.getValue('s')));	

	flsphere(input_args);

    return 0;
}