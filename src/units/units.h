#pragma once

#include <tuple>

#include "multiplexer.h"
#include "alu.h"
#include "memory.h"

class Machine;

struct ctrl_EX
{
    uint8_t regDst : 1;
    uint8_t aluSrc : 1;
    uint8_t op;
    uint8_t funct;
};
struct ctrl_M
{
    uint8_t branch : 1;
    uint8_t beq : 1;
    uint8_t jump : 1;
    uint8_t memRead : 1;
    uint8_t memWrite : 1;
    EMemoryRW numBytes = EMemoryRW::word;
};
struct ctrl_WB
{
    uint8_t regWrite : 1;
    uint8_t memtoReg : 1;
};

std::tuple<ctrl_EX, ctrl_M, ctrl_WB> Control(const union inst_t& inst);

class UForward
{
public:
    UForward(const Machine& machine) :
        owner(machine)
    {}

    int GetA() const;
    int GetB() const;

    // Forward the read value from memory (eg. lw, lh, ...).
    // It forwards when the store instruction locates just
    // right after the load instructions.
    int GetC() const;

    uint8_t ex_rs = 0;
    uint8_t ex_rt = 0;
    uint8_t mem_rd = 0;
    uint8_t wb_rd = 0;

    const Machine& owner;
};

class UHazardDetector
{
public:
    UHazardDetector(const Machine& machine) :
        owner(machine)
    {};

    bool IsHazardDetected() const;

    bool id_memWrite = false;
    bool ex_memRead = false;
    uint8_t ex_rt = 0;
    uint8_t id_rs = 0;
    uint8_t id_rt = 0;

    const Machine& owner;
};
