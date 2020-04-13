#pragma once

#include <string>
#include <memory>
#include "register.h"

union inst_t
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

class IInstruction
{
    using inst_ptr = std::shared_ptr<class IInstruction>;

public:
    virtual void Execute() = 0;

    virtual const std::string& GetName() const = 0;

    static inst_ptr MakeInstance(uint32_t rawInst);

protected:
    IInstruction(uint32_t raw);

    inst_t inst;
};
