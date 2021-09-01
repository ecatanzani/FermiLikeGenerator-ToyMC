#include "main.h"

#include <string>

int main(int argc, char** argv) {
    AnyOption opt;

	opt.addUsage("Usage: ");
	opt.addUsage("");

	opt.addUsage(" -h  --help                  .......... Prints this help");
	opt.addUsage(" -i  --input                 .......... <path_to_root_file_list>                             .......... (*) Input ROOT file list");
	opt.addUsage(" -w  --workdir               .......... <path_to_software_config_dir>                        .......... (*) Software config directory");
	opt.addUsage(" -o  --output                .......... <path_to_output_TFile>                               .......... Output ROOT TFile");
	opt.addUsage(" -v  --verbose               .......... Verbose output");

	opt.setFlag("help", 'h');
	opt.setOption("workdir", 'w');
	opt.setOption("output", 'o');
	opt.setFlag("verbose", 'v');

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

    return 0;
}