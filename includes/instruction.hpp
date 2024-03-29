#ifndef INSTRUCTIONS
#define INSTRUCTIONS

#include <iostream>
#include <vector>
#include <string.h>
#include <chrono>
#include <thread>
#include <Windows.h>

enum Command {
    WAIT,
    MOVE_CURSOR,
    LEFT_CLICK,
    RIGHT_CLICK
};

struct Instruction {
    Command op{};
    POINT pos{};
    u_int delay{};
};

void wait(u_int ms);
void move_cursor(POINT cursor_pos);
void click(bool left_click = true);

void print_instructionset(std::vector<Instruction> instructionset);
std::vector<Instruction> generate_default_instructionset();
void execute_instruction(Instruction instruction);

#endif