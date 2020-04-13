#pragma once

#include <string>
#include <memory>
#include "register.h"

using inst_ptr = std::shared_ptr<class IInstructionBase>;

class IInstructionBase
{
public:
    virtual const std::string& GetName() const = 0;

    static inst_ptr MakeInstance(uint32_t rawInst);

    inline uint32_t GetRawInst() const
    {
        return rawInst;
    }

    virtual void UpdatePc(int& pc) const = 0;

protected:
    IInstructionBase(uint32_t raw);

    uint32_t rawInst;
};

template <typename inst_t>
class IInstruction : public IInstructionBase
{
public:
    inline inst_t GetInst() const
    {
        inst_t parsedInst;
        parsedInst.raw = rawInst;
        return parsedInst;
    }

    virtual void UpdatePc(int& pc) const override;

protected:
    IInstruction(uint32_t raw) : IInstructionBase(raw) {}
};

union inst_imm
{
    uint32_t raw;
    struct {
        uint32_t immediate : 16;
        uint32_t rt : 5;
        uint32_t rs : 5;
        uint32_t op : 6;
    } field;
};

union inst_reg
{
    uint32_t raw;
    struct {
        uint32_t funct : 6;
        uint32_t shamt : 5;
        uint32_t rd : 5;
        uint32_t rt : 5;
        uint32_t rs : 5;
        uint32_t op : 6;
    } field;
};

union inst_base
{
    uint32_t raw;
    struct {
        int32_t address : 16;
        int32_t rt : 5;
        int32_t rs : 5;
        int32_t op : 6;
    } field;
};

union inst_direct
{
    uint32_t raw;
    struct {
        uint32_t address : 26;
        uint32_t op : 6;
    } field;
};
