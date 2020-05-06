#pragma once

#include <string>
#include <vector>
#include <array>
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

    inline int GetMode() const
    {
        return mode;
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

    pc_t pc;

    std::array<uint32_t, REG_MAX> registers;
    char memory[MAX_MEMORY];

    UMultiplexer<pc_t> mux_pc;

    struct {
        pc_t pc;
        Instruction const* curInst = nullptr;
    } if_id;

    struct {
        pc_t pc;
        int32_t rs_val;
        int32_t rt_val;
        int32_t address;
        
        reg_t rd0;
        reg_t rd1;
        
        // Forwarding unit inputs.
        reg_t rs;
        reg_t rt;

        // Control signals.
        ctrl_EX ex;
        ctrl_M m;
        ctrl_WB wb;
    } id_ex;

    struct {
        pc_t pc;
        int32_t aluResult;
        int32_t rt_val;

        bool zero = 0;
        // Forwarding unit inputs.
        reg_t rd;
        
        // Control signals.
        ctrl_M m;
        ctrl_WB wb;
    } ex_mem;

    struct {
        uint32_t readData;
        uint32_t aluResult;
        
        // Forwarding unit inputs.
        reg_t rd;
        
        // Control signals.
        ctrl_WB wb;
    } mem_wb;

    std::vector<Instruction> instructions;

    int maxCycle = 0;
    int mode = 0;

    // Forwarding multiplexer
    UMultiplexer<int32_t, 3> mux_fwd0;
    UMultiplexer<int32_t, 3> mux_fwd1;

    UForward forwarding{ *this };

    UHazardDetector hazardDetector{ *this };

    char outputBuffer[100]{};
};