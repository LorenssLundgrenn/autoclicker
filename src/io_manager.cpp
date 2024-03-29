#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>

#include "io_manager.hpp"
#include "instruction.hpp"

bool write_instructionset_to_file(const std::string &filename, const std::vector<Instruction> &instructionset) {
    std::ofstream file(filename, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing.\n";
        return 0;
    }

    for (const Instruction& instruction : instructionset) {
        file.write(reinterpret_cast<const char*>(&instruction), sizeof(Instruction));
    }

    file.close();
    return 1;
}

bool read_instructionset_from_file(const std::string &filename, std::vector<Instruction> &instructionset) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading.\n";
        return 0;
    }

    Instruction instruction_buffer;
    while (file.read(reinterpret_cast<char*>(&instruction_buffer), sizeof(Instruction))) {
        instructionset.push_back(instruction_buffer);
    }

    file.close();
    return 1;
}

void select_file_dialog(std::string &path) {
    OPENFILENAME ofn;
    TCHAR szFile[260] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = TEXT("All Files (*.*)\0*.*\0");
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileName(&ofn) == TRUE) {
        std::basic_string<TCHAR> file_path(ofn.lpstrFile);
        std::string str_path(file_path.begin(), file_path.end());
        path = str_path;
    }
}