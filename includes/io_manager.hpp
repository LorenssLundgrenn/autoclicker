#ifndef IO_MANAGER
#define IO_MANAGER

#include <fstream>
#include <vector>
#include <string.h>

#include "instruction.hpp"

bool write_instructionset_to_file(const std::string &filename, const std::vector<Instruction> &instructionset);
bool read_instructionset_from_file(const std::string &filename, std::vector<Instruction> &instructionset);
void select_file_dialog(std::string &path);

#endif