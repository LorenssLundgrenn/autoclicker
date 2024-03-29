#include <iostream>
#include <vector>
#include <string.h>
#include <chrono>
#include <thread>
#include <future>
#include <Windows.h>

#include "io_manager.hpp"
#include "instruction.hpp"

// Define a custom message ID
#define WM_END_LOOP (WM_USER + 1)

std::vector<Instruction> instructionset {};
std::vector<Instruction> instructionset_buffer {};
std::future<void> input_future;
std::thread execution_thread;
// active_execution_thread toggles the execution
// of the execute_instructionset function on a
// parallell thread, and is governed by the 
// kb_listener function
bool recording_to_instructionset = false;
bool first_recorded_click = true;
bool active_execution_thread = false;
bool macros_enabled = true;

void execute_instructionset();
LRESULT CALLBACK keyboard_listener(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK mouse_listener(int nCode, WPARAM wParam, LPARAM lParam);
void record_click(bool left_click);

int main(int argc, char* argv[]) {
    if (argc >= 2) {
        read_instructionset_from_file(argv[1], instructionset);
    } else {
        instructionset = generate_default_instructionset();
    }

    HHOOK keyboard_hook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboard_listener, NULL, 0);
    if (keyboard_hook == NULL) {
        std::cerr << "Failed to set keyboard hook" << std::endl;
        return 1;
    }

    HHOOK mouse_hook = SetWindowsHookEx(WH_MOUSE_LL, mouse_listener, NULL, 0);
    if (mouse_hook == NULL) {
        std::cerr << "Failed to set mouse hook" << std::endl;
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if (msg.message == WM_END_LOOP) break;
    }

    UnhookWindowsHookEx(keyboard_hook);
    UnhookWindowsHookEx(mouse_hook);
    return 0;
}

void activate_instruction_execution() {
    active_execution_thread = true;
    execution_thread = std::thread(execute_instructionset);
}

void deactivate_instruction_execution() {
    active_execution_thread = false;
    execution_thread.join();
}

// continuously execute the currently loaded instructionset
void execute_instructionset() {
    while (active_execution_thread) {
        for (const auto& instruction : instructionset) {
            if (!active_execution_thread) break; // abort mid-execution
            execute_instruction(instruction);
        }
    }
}

void record_click(bool left_click) {
    static std::chrono::steady_clock::time_point last_click;
    if (first_recorded_click) {
        last_click = std::chrono::steady_clock::now();
        first_recorded_click = false;
    }

    Instruction wait_instruction;
    wait_instruction.op = WAIT;
    auto current_time = std::chrono::steady_clock::now();
    auto delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_click);
    wait_instruction.delay = (u_int)delta_time.count();;
    instructionset_buffer.push_back(wait_instruction);

    Instruction move_instruction;
    move_instruction.op = MOVE_CURSOR;
    POINT cursor_pos;
    GetCursorPos(&cursor_pos);
    move_instruction.pos.x = cursor_pos.x;
    move_instruction.pos.y = cursor_pos.y;
    instructionset_buffer.push_back(move_instruction);

    Instruction click_instruction;
    click_instruction.op = left_click ? LEFT_CLICK : RIGHT_CLICK;
    instructionset_buffer.push_back(click_instruction);

    last_click = current_time;
}


// change the keybinds, they are shit
LRESULT CALLBACK keyboard_listener(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && wParam == WM_KEYDOWN && macros_enabled) {
        KBDLLHOOKSTRUCT* pKeyboard = (KBDLLHOOKSTRUCT*)lParam;
        bool ctrl_pressed = GetAsyncKeyState(VK_CONTROL) & 0x8000;
        bool shift_pressed = GetAsyncKeyState(VK_SHIFT) & 0x8000;

        if (pKeyboard->vkCode == VK_ESCAPE && shift_pressed) {
            if (active_execution_thread) deactivate_instruction_execution();
            PostMessage(NULL, WM_END_LOOP, 0, 0);
        }
        else if (pKeyboard->vkCode == '1' && ctrl_pressed && !recording_to_instructionset) {
            if (!active_execution_thread) {
                activate_instruction_execution();
            } else {
                deactivate_instruction_execution();
            }
        }
        else if (pKeyboard->vkCode == '2' && ctrl_pressed) {
            recording_to_instructionset = !recording_to_instructionset;
            if (recording_to_instructionset) {
                instructionset_buffer = {};
                first_recorded_click = true;
                if ( active_execution_thread) {
                    deactivate_instruction_execution();
                }
            }
            else if (!recording_to_instructionset) {
                instructionset = instructionset_buffer;
            }
        }
        else if (pKeyboard->vkCode == 'S' && ctrl_pressed) {
            macros_enabled = false;
            input_future = std::async(std::launch::async, [](){
                std::string path;
                select_file_dialog(path);
                if (!path.empty()) {
                    write_instructionset_to_file(path, instructionset);
                }
                macros_enabled = true;
            });
        }
        else if (pKeyboard->vkCode == 'L' && ctrl_pressed) {
            macros_enabled = false;
            input_future = std::async(std::launch::async, [](){
                std::string path;
                select_file_dialog(path);
                if (!path.empty()) {
                    instructionset = {};
                    read_instructionset_from_file(path, instructionset);
                }
                macros_enabled = true;
            });
        }
        else if (pKeyboard->vkCode == 'P' && ctrl_pressed) {
            print_instructionset(instructionset);
        }
        else if (pKeyboard->vkCode == 'R' && ctrl_pressed) {
            instructionset = generate_default_instructionset();
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK mouse_listener(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && recording_to_instructionset) {
        if (wParam == WM_LBUTTONDOWN) {
            std::async(std::launch::async, []() {
                record_click(true);
            });
        }
        else if (wParam == WM_RBUTTONDOWN) {
            std::async(std::launch::async, []() {
                record_click(false);
            });
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}