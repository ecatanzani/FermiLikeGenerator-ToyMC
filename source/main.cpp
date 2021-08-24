#include "main.h"
#include "utils.h"
#include "config.h"

#include <memory>

int main(int argc, char** argv)
{
    AnyOption opt;

	opt.addUsage("Usage: ");
	opt.addUsage("");

	opt.addUsage(" -h  --help                  .......... Prints this help");
	opt.addUsage(" -w  --workdir               .......... <path_to_software_config_dir>                        .......... (*) Collector config directory");
	opt.addUsage(" -o  --output                .......... <path_to_output_TFile>                               .......... Output ROOT TFile");
	opt.addUsage(" -d  --outputdir             .......... <path_to_output_TFile_dir>                           .......... Output ROOT TFile directory");
	opt.addUsage(" -v  --verbose               .......... Verbose output");

    opt.addUsage("");

	opt.setFlag("help", 'h');
	opt.setOption("workdir", 'w');
	opt.setOption("output", 'o');
	opt.setOption("outputdir", 'd');
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
    if (opt.getValue("workdir") || opt.getValue('w'))
		input_args.wd = opt.getValue('w');
	if (opt.getValue("output") || opt.getValue('o'))
		input_args.output_path = expand_output_path(opt, opt.getValue('o'));
	if (opt.getValue("outputdir") || opt.getValue('d'))
		input_args.output_path = expand_output_path(opt, opt.getValue('d'));
	if (opt.getFlag("verbose") || opt.getFlag('v'))
		input_args.verbose = opt.getFlag('v');







    std::unique_ptr<simu_config> config = std::make_unique<simu_config>()

    

    return 0;
}