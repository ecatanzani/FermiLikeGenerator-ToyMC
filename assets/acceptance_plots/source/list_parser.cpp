#include "list_parser.h"

#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iostream>

list_parser::list_parser(const char* input) {
    std::ifstream input_list(input);
    if (!input_list.is_open()) {
		std::cerr << "\nInput file list not found [" << input << "]\n\n";
		exit(100);
	}
    std::string input_string((std::istreambuf_iterator<char>(input_list)), (std::istreambuf_iterator<char>()));
	input_list.close();
	std::istringstream input_stream(input_string);
    std::string tmp_str;
    while (input_stream >> tmp_str) {
        files.push_back(tmp_str);
    }
}

const std::vector<std::string> list_parser::GetFileList() {
    return files;
}