#include <iostream>
#include <vector>
#include <string.h>
#include <chrono>
#include <thread>
#include <Windows.h>

#include "instruction.hpp"

void wait(u_int ms) {
    std::this_thread::sleep_for(
        std::chrono::milliseconds(ms)
    );
}

void move_cursor(POINT cursor_pos) {
    SetCursorPos(
        cursor_pos.x,
        cursor_pos.y
    );
}

void click(bool left_click)
{
    INPUT input;

    ZeroMemory(&input, sizeof(INPUT));
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = left_click ?
        MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN;
    SendInput(1, &input, sizeof(INPUT));

    ZeroMemory(&input, sizeof(INPUT));
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = left_click ?
        MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP;
    SendInput(1, &input, sizeof(INPUT));
}

std::string instruction_to_str(Instruction instruction) {
    std::string op_code;
    switch (instruction.op)
    {
    case 0:
        op_code = "WAIT";
        break;
    case 1:
        op_code = "MOVE_CURSOR";
        break;
    case 2:
        op_code = "LEFT_CLICK";
        break;
    case 3:
        op_code = "RIGHT_CLICK";
        break;
    default:
        op_code = "UNKNOWN_OP_CODE";
        break;
    }

    return (
        op_code + ", {" + std::to_string(instruction.pos.x) + "px, " + 
        std::to_string(instruction.pos.y) + "px }, " + std::to_string(instruction.delay) + "ms"
    ); 
}

std::string instructionset_to_str(std::vector<Instruction> instructionset) {
    std::string instructionset_str = "";
    for (Instruction instruction : instructionset) {
        instructionset_str += instruction_to_str(instruction) + '\n';
    }
    return instructionset_str;
}

std::vector<Instruction> generate_default_instructionset() {
    std::vector<Instruction> instructionset;

    Instruction instruction1;
    instruction1.op = LEFT_CLICK;
    instructionset.push_back(instruction1);

    Instruction instruction2;
    instruction2.op = WAIT;
    instruction2.delay = 50;
    instructionset.push_back(instruction2);

    return instructionset;
}

void execute_instruction(Instruction instruction) {
    switch (instruction.op)
    {
    case WAIT:
        wait(instruction.delay);
        break;
    case MOVE_CURSOR:
        move_cursor(instruction.pos);
        break;
    case LEFT_CLICK:
        click();
        break;
    case RIGHT_CLICK:
        click(false);
        break;
    default:
        std::cerr << "could not run instruction with"
        << "unrecognized opcode: " << instruction.op << '\n';
    }
}