#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <memory>
#include <vector>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>

struct cube_faces {
    bool top;
    bool bottom;
    bool sides;
    unsigned int number {0};
};

class simu_config {
    public:
        simu_config(const std::string wd);
        ~simu_config(){};

        const double GetCubeDimension();
        const double GetTelescopeLateralSize();
        const double GetTelescopeVerticalDisplacement();
        std::shared_ptr<cube_faces> GetCubeFacesInfo();
        const bool IsBottomActive();
        const unsigned int GetNumberOfCubeFaces();

    private:
        std::string parse_config_file(const std::string wd, const std::string config_file);
        void get_config_info(const std::string parsed_config);
        const bool check_cube_details();
        
        double cube_dimension{0};
        double telescope_lateral{0};
        double telescope_vertical{0};
        std::shared_ptr<cube_faces> mycube = std::make_shared<cube_faces>();
};

#endif