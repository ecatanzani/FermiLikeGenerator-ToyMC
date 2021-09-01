#ifndef LIST_PARSER_H
#define LIST_PARSER_H

#include <string>
#include <vector>

class list_parser {
    public:
        list_parser(const char* input);
        ~list_parser() {};
        const std::vector<std::string> GetFileList();
    private:
        std::vector<std::string> files;
};

#endif