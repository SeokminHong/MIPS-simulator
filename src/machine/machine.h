#pragma once

#include <string>
#include <vector>
#include <array>
#include <stack>
#include "register.h"
#include "instruction.h"
#include "units.h"

#define MAX_INSTRUCTION 4096
#define MAX_MEMORY 65536

using pc_t = uint32_t;

class Machine
{
public:
    Machine();

    void AddInstruction(uint32_t instruction);

    inline void SetMaxCycle(int newMaxCycle)
    {
        maxCycle = newMaxCycle;
    }

    inline void SetMode(int newMode)
    {
        mode = newMode;
    }

    void SetRegister(ERegister reg, uint32_t regVal);

    void Run();

private:
    void Cycle();

    void IF();
    void ID();
    void EX();
    void MEM();
    void WB();

    pc_t pc = 0;

    std::array<uint32_t, REG_MAX> registers;
    char memory[MAX_MEMORY];

    Multiplexer<pc_t> mux_pc;

    struct {
        pc_t pc;
        Instruction const* curInst = nullptr;
    } if_id;

    struct {
        pc_t pc;
        int32_t rs_val;
        int32_t rt_val;
        int32_t address;
        
        uint32_t rd0 : 5;
        uint32_t rd1 : 5;
        
        // Forwarding unit inputs.
        uint32_t rs : 5;
        uint32_t rt : 5;

        // Control signals.
        ctrl_EX ex;
        ctrl_M m;
        ctrl_WB wb;
    } id_ex;

    struct {
        pc_t pc;
        int32_t aluResult;
        int32_t rt_val;

        uint32_t zero : 1;
        // Forwarding unit inputs.
        uint32_t rd : 5;
        
        // Control signals.
        ctrl_M m;
        ctrl_WB wb;
    } ex_mem;

    struct {
        uint32_t readData;
        uint32_t aluResult;
        
        // Forwarding unit inputs.
        uint32_t rd : 5;
        
        // Control signals.
        ctrl_WB wb;
    } mem_wb;

    std::vector<Instruction> instructions;

    // Only used for the extra challenge of Phase1.
    // jal: push PC+4 into $ra.
    // jr: pop PC from $ra.
    std::stack<uint32_t> ra;

    int maxCycle = 0;
    int mode = 0;

    // Forwarding multiplexer
    Multiplexer<int32_t, 3> mux_fwd0;
    Multiplexer<int32_t, 3> mux_fwd1;

    Forward forwarding;

    char outputBuffer[100] = {};
};