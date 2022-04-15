# FermiLikeGenerator-ToyMC

This application simulates cosmic rays particles crossing the DAMPE detector in order to compute the acceptance (number of accepted events over the total).

Different models for the surfice generations have been tried, for example spheres and cubes. The particles are produced on the surfice and are propagated through the detector.

Here the parameters to use:

```
Usage: 

 -h  --help                  .......... Prints this help
 -w  --workdir               .......... <path_to_software_config_dir>                        .......... (*) Software config directory
 -o  --output                .......... <path_to_output_TFile>                               .......... Output ROOT TFile
 -d  --outputdir             .......... <path_to_output_TFile_dir>                           .......... Output ROOT TFile directory
 -v  --verbose               .......... Verbose output

 -n --number                 .......... Number of particles to be simulated
 -s --seed                   .......... Simulation seed
```

THe software automatically parses the **simu.conf** configuration file to set the detector geometry:

```
##### Simulation Config File #####

SphereRadius        1.4     // Radius of the sphere (in meters)
TelescopeSize       0.6     // Lateral dimension of the telescope (in meters)
TelescopeDistance   0.4     // Vertical displacement between the telescope plates (in meters)
```

The output file is in ROOT format.

The simulation permits to set the number of particles to generate and the initial seed.
