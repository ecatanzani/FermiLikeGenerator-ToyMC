#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>

class simu_config {
    public:
        simu_config(const std::string wd);
        ~simu_config(){};

        const double GetSphereRadius();
        const double GetTelescopeLateralSize();
        const double GetTelescopeVerticalDisplacement();

    private:
        std::string simu_config::parse_config_file(const std::string wd, const std::string config_file);
        void simu_config::get_config_info(const std::string parsed_config);
        
        double sphere_radius{0};
        double telescope_lateral{0};
        double telescope_vertical{0};
};

#endif