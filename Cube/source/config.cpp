#include "config.h"

simu_config::simu_config(const std::string wd) {
    std::string config_file_name{"config/simu.conf"};
    get_config_info(parse_config_file(wd, config_file_name));
	if (!check_cube_details()) {
		std::cerr << "\n\nPlease switc on at least one cube face\n\n";
		exit(200);
	}
}

std::string simu_config::parse_config_file(const std::string wd, const std::string config_file) {
	std::string configPath{wd + "/" + config_file};
	std::ifstream input_file(configPath.c_str());
	if (!input_file.is_open()) {
		std::cerr << "\nInput config file not found [" << configPath << "]\n\n";
		exit(100);
	}
	std::string input_string((std::istreambuf_iterator<char>(input_file)), (std::istreambuf_iterator<char>()));
	input_file.close();
	return input_string;
}

void simu_config::get_config_info(const std::string parsed_config) {
	std::string tmp_str;
	std::istringstream input_stream(parsed_config);
	std::string::size_type sz;

	while (input_stream >> tmp_str) {
		if (!strcmp(tmp_str.c_str(), "CubeSide")) {
			input_stream >> tmp_str;
			cube_dimension = stod(tmp_str, &sz);
		}
        if (!strcmp(tmp_str.c_str(), "TelescopeSize")) {
			input_stream >> tmp_str;
			telescope_lateral = stod(tmp_str, &sz);
		}
        if (!strcmp(tmp_str.c_str(), "TelescopeDistance")) {
			input_stream >> tmp_str;
			telescope_vertical = stod(tmp_str, &sz);
		}

		if (!strcmp(tmp_str.c_str(), "TOP")) {
			input_stream >> tmp_str;
			if (!strcmp(tmp_str.c_str(), "ON") || !strcmp(tmp_str.c_str(), "on")) {
				mycube->top = true;
				++mycube->number;
			}
			else if (!strcmp(tmp_str.c_str(), "OFF") || !strcmp(tmp_str.c_str(), "off"))
				mycube->top = false;
			else {
				std::cout << "\n\nError parsing config file. Please check\n\n";
				exit(200);
			}
		}

		if (!strcmp(tmp_str.c_str(), "BOTTOM")) {
			input_stream >> tmp_str;
			if (!strcmp(tmp_str.c_str(), "ON") || !strcmp(tmp_str.c_str(), "on")) {
				mycube->bottom = true;
				++mycube->number;
			}
			else if (!strcmp(tmp_str.c_str(), "OFF") || !strcmp(tmp_str.c_str(), "off"))
				mycube->bottom = false;
			else {
				std::cout << "\n\nError parsing config file. Please check\n\n";
				exit(200);
			}
		}

		if (!strcmp(tmp_str.c_str(), "SIDES")) {
			input_stream >> tmp_str;
			if (!strcmp(tmp_str.c_str(), "ON") || !strcmp(tmp_str.c_str(), "on")) {
				mycube->sides = true;
				mycube->number += 4;
			}
			else if (!strcmp(tmp_str.c_str(), "OFF") || !strcmp(tmp_str.c_str(), "off"))
				mycube->sides = false;
			else {
				std::cout << "\n\nError parsing config file. Please check\n\n";
				exit(200);
			}
		}
    }
}

const bool simu_config::check_cube_details() {
	return mycube->number>0 ? true : false;
}

const double simu_config::GetCubeDimension() {
	return cube_dimension;
}

const double simu_config::GetTelescopeLateralSize() {
	return telescope_lateral;
}

const double simu_config::GetTelescopeVerticalDisplacement() {
	return telescope_vertical;
}

std::shared_ptr<cube_faces> simu_config::GetCubeFacesInfo() {
	return mycube;
}

const bool simu_config::IsBottomActive() {
	return mycube->bottom ? true : false;
}

const unsigned int simu_config::GetNumberOfCubeFaces() {
	return mycube->number;
}
