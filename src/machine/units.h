#pragma once

#include <tuple>

template <typename T>
class Multiplexer
{
public:
    Multiplexer()
    {}

    Multiplexer(const T& _trueValue, const T& _falseValue) :
        trueValue(_trueValue), falseValue(_falseValue)
    {}

    inline T GetValue(bool isTrue)
    {
        return isTrue ? trueValue : falseValue;
    }

    inline void SetTrueValue(const T& value)
    {
        trueValue = value;
    }

    inline void SetFalseValue(const T& value)
    {
        falseValue = value;
    }

private:
    T trueValue = 0;
    T falseValue = 0;
};

struct ctrl_EX
{
    uint8_t regDst : 1;
    uint8_t aluOP1 : 1;
    uint8_t aluOP0 : 1;
    uint8_t aluSrc : 1;
};
struct ctrl_M
{
    uint8_t branch : 1;
    uint8_t memRead : 1;
    uint8_t memWrite : 1;
};
struct ctrl_WB
{
    uint8_t regWrite : 1;
    uint8_t memtoReg : 1;
};

std::tuple<ctrl_EX, ctrl_M, ctrl_WB> Control(union inst_t inst);

enum EALU
{
    EALU_and = 0,
    EALU_or = 1,
    EALU_add = 2,
    EALU_subtract = 6,
    EALU_slt = 7,
    EALU_nor = 10
};

inline EALU GetALUControl(uint8_t aluOp1, uint8_t aluOp0, uint32_t funct)
{
    if (aluOp1 == 0)
    {
        return EALU_add;
    }
    if (aluOp0 == 1)
    {
        return EALU_subtract;
    }
    switch (funct & 15)
    {
        case 0:
        {
            return EALU_add;
        }
        case 2:
        {
            return EALU_subtract;
        }
        case 4:
        {
            return EALU_and;
        }
        case 5:
        {
            return EALU_or;
        }
        case 10:
        {
            return EALU_slt;
        }
    }
    return EALU_add;
}

template<typename T, typename U>
static bool ALU(EALU control, const T& arg1, const U& arg2, std::common_type_t<T, U>& ret)
{
    switch (control)
    {
        case EALU_and:
        {
            ret = arg1 & arg2;
            break;
        }
        case EALU_or:
        {
            ret = arg1 | arg2;
            break;
        }
        case EALU_add:
        {
            ret = arg1 + arg2;
            break;
        }
        case EALU_subtract:
        {
            ret = arg1 - arg2;
            break;
        }
        case EALU_slt:
        {
            ret = arg1 < arg2;
            break;
        }
        case EALU_nor:
        {
            ret = !(arg1 | arg2);
            break;
        }
    }
    return false;
}