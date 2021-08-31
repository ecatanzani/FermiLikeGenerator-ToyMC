import os
import shutil
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

        if pars['recreate']:
            self.clean_condor_dirs(pars['output'], pars['verbose'])
            self.submit_jobs()
        else:
            self.init_condor_out_dirs(pars)
            self.init_sub_files(pars['verbose'])
            self.init_bash_files(pars)

    def clean_condor_dirs(self, output: str, verbose: bool):
        if verbose:
            print(f"\nCleaning output folder [{output}]\n")
        
        self.condorDirs = [f"{output}/{filename}" for filename in os.listdir(output) if filename.startswith("job_")]
        remove_files = []
        for folder in self.condorDirs:
            remove_files += [f"{folder}/{file}" for file in os.listdir(folder) if file.startswith("out")]

        # Clean the job dir
        self.cleaner(remove_files)

    def clean_condor_dir(self, dir: str):
        remove_files = [f"{dir}/{file}" for file in os.listdir(dir) if file.startswith("out")]
        # Clean the job dir
        self.cleaner(remove_files)

    def cleaner(self, files: list):
        for elm in files:
            if os.path.isdir(elm):
                shutil.rmtree(elm)
            if os.path.isfile(elm):
                os.remove(elm)

    def init_condor_out_dirs(self, pars: dict):
        if pars['verbose']:
            print("\nCreating output folders...\n")

        for job_idx in range(pars['jobs']):
            self.condorDirs.append(f"{pars['output']}/job_{job_idx}")
            os.mkdir(self.condorDirs[-1])
            if pars['verbose']:
                print(f"HTCondor output job folder has been created [{self.condorDirs[-1]}]")

    def init_sub_files(self, verbose: bool):
        if verbose:
            print("\nCreating sub files...\n")

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
        if pars['verbose']:
            print("\nCreating bash files...\n")

        for idx, current_dir in enumerate(self.condorDirs):
            bash_script_path = f"{current_dir}/script.sh"
            tmp_output_folder = f"{current_dir}/outFiles"
            try:
                with open(bash_script_path, 'w') as out_bash:
                    out_bash.write("#!/usr/bin/env bash\n")
                    out_bash.write("source /opt/rh/devtoolset-7/enable\n")
                    out_bash.write("source /storage/gpfs_data/dampe/users/ecatanzani/deps/root-6.22/bin/thisroot.sh\n")
                    out_bash.write(f"mkdir {tmp_output_folder}\n")
                    _command = f"{pars['executable']} -w {pars['config']} -d {tmp_output_folder} -n {pars['events']} -s {idx} -v"
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

    def submit_job(self, dir: str):
        subprocess.run([f"condor_submit -name sn-01.cr.cnaf.infn.it -spool {dir}/rules.sub"], shell=True, check=True)

    def test_root_file(self, path) -> bool:
        from ROOT import TFile
        tmp_file = TFile(path)
        if tmp_file and not tmp_file.IsOpen():
            return False
        elif tmp_file and tmp_file.IsOpen() and tmp_file.IsZombie():
            tmp_file.Close()
            return False
        elif tmp_file and tmp_file.IsOpen() and tmp_file.TestBit(TFile.kRecovered):
            tmp_file.Close()
            return False
        else:
            tmp_file.Close()
            return True

    def status(self, output: str, verbose: bool):
        self.explore_output_files(output)
        if verbose:
            print(f"Found {len(self.data_dirs)} GOOD condor directories")
        if self.skipped_dirs:
            print(f"Found {len(self.skipped_dirs)} BAD condor directories...\n")
            print(f"Found {self.skipped_file_notFinalDir} directories with no output folder")
            print(f"Found {self.skipped_file_notAllOutput} directories with inconsistent number of output ROOT files")
            print(f"Found {self.skipped_file_notROOTfile} directories with no output ROOT file")
            print(f"Found {self.skipped_file_notReadable} directories with corrupted output ROOT file")
            print(f"Found {self.skipped_file_noKeys} directories where output ROOT file has no keys\n")
            print("Here the folders list...\n")

            for idx, elm in enumerate(self.skipped_dirs):
                print(f"Skipped {idx} directory: {elm}")

            print("\nResubmitting HTCondor jobs for {len(self.skipped_dirs)} directories\n".format())
            for dir in self.skipped_dirs:
                self.clean_condor_dir(dir)
                self.submit_job(dir)
        else:
            list_file = "toyMC_condor_list.txt"
            with open(list_file, "w") as final_list:
                for elm in self.data_files:
                    final_list.write(f"{elm}\n")

    def explore_output_files(self, output: str):
        from ROOT import TFile

        self.condorDirs = [f"{output}/{filename}" for filename in os.listdir(output) if filename.startswith("job_")]

        for dir in self.condorDirs:
            expected_condor_outDir = f"{dir}/outFiles"
            # Check if 'outFiles' dir exists
            if os.path.isdir(expected_condor_outDir):
                _list_dir = [f"{expected_condor_outDir}/{file}" for file in os.listdir(expected_condor_outDir) if file.endswith('.root')]
                skipped_dir = False
                for file_idx, file in enumerate(_list_dir):
                    # Check if output ROOT file exists
                    if os.path.isfile(file):
                        # Check if output ROOT file is redable
                        if self.test_root_file(file):
                            tfile = TFile.Open(file, "READ")
                            # Check if output ROOT file has keys
                            if tfile.GetNkeys():
                                if file_idx == len(_list_dir)-1 and not skipped_dir:
                                    self.data_dirs.append(dir)
                                    self.data_files.append(file)
                            else:
                                # output ROOT file has been open but has not keys
                                if not skipped_dir:
                                    self.skipped_dirs.append(dir)
                                    skipped_dir = True
                                    self.skipped_file_noKeys += 1
                        else:
                            # output ROOT file has not been opened correctly
                            if not skipped_dir:
                                self.skipped_dirs.append(dir)
                                skipped_dir = True
                                self.skipped_file_notReadable += 1
                    else:
                        # output ROOT file does not exist
                        if not skipped_dir:
                            self.skipped_dirs.append(dir)
                            skipped_dir = True
                            self.skipped_file_notROOTfile += 1
            else:
                # 'outFiles' dir does not exists
                self.skipped_dirs.append(dir)
                self.skipped_file_notFinalDir += 1