from argparse import ArgumentParser
from helper_class import helper

def main(args=None):
    
    parser = ArgumentParser(description='HTCondor Job Status Controller - FermiLike ToyMC')
   
    parser.add_argument("-o", "--output", type=str, dest='output', help='HTC output directory')
    parser.add_argument("-e", "--evts", type=int, dest='evts', const=10000000, nargs='?', help='events to process in job')
    parser.add_argument("-j", "--jobs", type=int, dest='jobs', const=1000, nargs='?', help='number of jobs')
    parser.add_argument("-x", "--executable", type=str, dest='executable', help='ToyMC executable')     
    parser.add_argument("-c", "--config", type=str, dest='config', help='Software Config Directory')                  
    parser.add_argument("-v", "--verbose", dest='verbose', default=False, action='store_true', help='run in high verbosity mode')
    parser.add_argument("-r", "--recreate", dest='recreate', default=False, action='store_true', help='recreate output dirs if present')
    parser.add_argument("-s", "--status", dest='status', default=False, action='store_true', help='check output file status')

    opts = parser.parse_args(args)

    pars = {
        "output": opts.output,
        "jobs": opts.jobs,
        "executable": opts.executable,
        "config": opts.config,
        "events": opts.evts,
        "verbose": opts.verbose,
        "recreate": opts.recreate
    }

    toymc_helper = helper()
    if opts.status:
        toymc_helper.status(pars['output'], opts.verbose)
    else:
        toymc_helper.init(pars)
        toymc_helper.submit_jobs()

if __name__ == '__main__':
    main()