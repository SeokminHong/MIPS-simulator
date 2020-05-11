#pragma once

#include <string>
#include <vector>
#include <array>
#include "register.h"
#include "instruction.h"
#include "../units/units.h"

#define MAX_INSTRUCTION 4096

using pc_t = int32_t;

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

    bool NeedsStall() const;

    pc_t pc = 0;
    bool branchUnresolved = false;
    bool branchResolvedFlag = false;
    pc_t pendingPC = 0;

    std::array<uint32_t, REG_MAX> registers;

    UMemory memory{ EEndianess::little };

    UMultiplexer<pc_t> mux_branch;
    UMultiplexer<pc_t> mux_jump;

    struct {
        pc_t pc;
        Instruction const* curInst = nullptr;
    } if_id;

    struct {
        pc_t pc;
        int32_t rs_val;
        int32_t rt_val;
        int32_t address;
        int32_t shamt;
        
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
        pc_t offsettedPC;
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