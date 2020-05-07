#pragma once

#include <tuple>
#include <type_traits>

class Machine;

template <typename T, int N = 2>
class UMultiplexer
{
private:
public:
    UMultiplexer()
    {}

    template<typename ...TArgs>
    UMultiplexer(TArgs&&... ts) :
        values{ std::forward<TArgs>(ts)... }
    {
        static_assert(sizeof...(ts) == N, "Number of arguments is inappropriate.");
    }

    inline const T& GetValue(int index) const
    {
        return values[index];
    }

    inline void SetValue(int index, const T& value)
    {
        values[index] = value;
    }
    inline void SetValue(int index, T&& value)
    {
        values[index] = std::move(value);
    }

private:
    T values[N]{};
};

class UForward
{
public:
    UForward(const Machine& machine) :
        owner(machine)
    {}

    int GetA() const;
    int GetB() const;

    uint32_t id_ex_rs : 5;
    uint32_t id_ex_rt : 5;
    uint32_t ex_mem_rd : 5;
    uint32_t mem_wb_rd : 5;

    const Machine& owner;
};

class UHazardDetector
{
public:
    UHazardDetector(const Machine& machine) :
        owner(machine)
    {};

    bool IsHazardDetected() const;

    bool id_ex_memRead = false;
    uint8_t ex_rt = 0;
    uint8_t id_rs = 0;
    uint8_t id_rt = 0;

    const Machine& owner;
};

enum class EMemoryRW : uint8_t
{
    byte, ubyte,
    half, uhalf,
    word
};

// Returns the numbers to RW bytes.
inline static constexpr uint8_t operator*(EMemoryRW rw)
{
    switch (rw)
    {
        case EMemoryRW::byte:
        case EMemoryRW::ubyte:
        {
            return 1;
        }
        case EMemoryRW::half:
        case EMemoryRW::uhalf:
        {
            return 2;
        }
        case EMemoryRW::word:
        {
            return 4;
        }
        default:
        {
            return 0;
        }
    }
}

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
    EMemoryRW numBytes;
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

inline static EALU GetALUControl(uint8_t aluOp1, uint8_t aluOp0, uint32_t funct)
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
            ret = ~(arg1 | arg2);
            break;
        }
    }
    return false;
}
