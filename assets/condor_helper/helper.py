import os
import argparse
import subprocess

class helper():

    def __init__(self):
        self.condorDirs = []
        self.skipped_dirs = []
        self.data_dirs = []
        self.data_files = []

        self.skipped_file_notFinalDir = 0
        self.skipped_file_notAllOutput = 0
        self.skipped_file_noSingleROOTfile = 0
        self.skipped_file_notROOTfile = 0
        self.skipped_file_notReadable = 0
        self.skipped_file_noKeys = 0

    def init(self, pars: dict):

        self.init_condor_out_dirs(pars)
        self.init_sub_files(pars['verbose'])
        self.init_bash_files(pars)

    def init_condor_out_dirs(self, pars: dict):
        for job_idx in range(pars['jobs']):
            self.condorDirs.append(f"{pars['output']}/job_{job_idx}")
            os.mkdir(self.condorDirs[-1])
            if pars['verbose']:
                print(f"HTCondor output job folder has been created [{self.condorDirs[-1]}]")

    def init_sub_files(self, verbose: bool):
        for current_dir in self.condorDirs:
        
            # Find out paths
            output_path = f"{current_dir}/output.log"
            log_path = f"{current_dir}/output.clog"
            err_path = f"{current_dir}/output.err"
            bash_script_path = f"{current_dir}/script.sh"
        
            # Writing sub file
            sub_file_path = f"{current_dir}/rules.sub"
            
            try:
                with open(sub_file_path, 'w') as out_sub:
                    out_sub.write("universe = vanilla\n")
                    out_sub.write(f"executable = {bash_script_path}\n")
                    out_sub.write(f"output = {output_path}\n")
                    out_sub.write(f"error = {err_path}\n")
                    out_sub.write(f"log = {log_path}\n")
                    out_sub.write("ShouldTransferFiles = YES\n")
                    out_sub.write("WhenToTransferOutput = ON_EXIT\n")
                    out_sub.write("queue 1")
            except OSError:
                print(f"ERROR creating HTCondor sub file [{current_dir}]")
                raise
            else:
                if verbose:
                    print(f"HTCondor sub file created [{current_dir}]")

    def init_bash_files(self, pars: dict):
        for current_dir in self.condorDirs:
            bash_script_path = f"{current_dir}/script.sh"
            tmp_output_folder = f"{current_dir}/outFiles"
            try:
                with open(bash_script_path, 'w') as out_bash:
                    out_bash.write("#!/usr/bin/env bash\n")
                    out_bash.write("source /opt/rh/devtoolset-7/enable\n")
                    out_bash.write("source /storage/gpfs_data/dampe/users/ecatanzani/deps/root-6.22/bin/thisroot.sh\n")
                    out_bash.write(f"mkdir {tmp_output_folder}\n")
                    _command = f"{pars['executable']} -w {pars['config']} -d {tmp_output_folder} -n {pars['events']} -v"
                    out_bash.write(_command)

            except OSError:
                    print(f"ERROR creating HTCondor bash script file [{current_dir}]")
                    raise
            else:
                if pars['verbose']:
                    print(f"HTCondor bash script file created [{current_dir}]")

    def submit_jobs(self):
        for folder in self.condorDirs:
            subprocess.run([f"condor_submit -name sn-01.cr.cnaf.infn.it -spool {folder}/rules.sub"], shell=True, check=True)

def main(args=None):
    
    parser = argparse.ArgumentParser(description='HTCondor Job Status Controller - FermiLike ToyMC')
   
    parser.add_argument("-o", "--output", type=str,
                            dest='output', help='HTC output directory')
    parser.add_argument("-e", "--evts", type=int, dest='evts',
                            const=10000000, nargs='?', help='events to process in job')
    parser.add_argument("-j", "--jobs", type=int, dest='jobs',
                            const=1000, nargs='?', help='number of jobs') 
    parser.add_argument("-x", "--executable", type=str,
                            dest='executable', help='ToyMC executable')     
    parser.add_argument("-c", "--config", type=str,
                            dest='config', help='Software Config Directory')                  
    parser.add_argument("-v", "--verbose", dest='verbose', default=False,
                        action='store_true', help='run in high verbosity mode')

    opts = parser.parse_args(args)

    pars = {
        "output": opts.output,
        "jobs": opts.jobs,
        "executable": opts.executable,
        "config": opts.config,
        "events": opts.evts,
        "verbose": opts.verbose
    }

    toymc_helper = helper()
    toymc_helper.init(pars)
    toymc_helper.submit_jobs()


if __name__ == '__main__':
    main()