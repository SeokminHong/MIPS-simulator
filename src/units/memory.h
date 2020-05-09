#pragma once

#include <cstdint>
#include <cstring>

#define MAX_MEMORY 65536

enum class EEndianess
{
    big,
    little
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
    return (rw < EMemoryRW::half) ? 1 : 
            ((rw < EMemoryRW::word) ? 2 : 
              (rw == EMemoryRW::word ? 4 : 0));
}

class UMemory
{
public:
    UMemory()
    {}

    UMemory(EEndianess endianess) :
        endian(endianess)
    {}

    template<typename T>
    T Get(size_t address) const;

    template<typename T>
    void Set(size_t address, const T& value);

private:
    uint8_t raw[MAX_MEMORY]{};

    EEndianess endian = EEndianess::little;
};

template<>
inline int8_t UMemory::Get<int8_t>(size_t address) const
{
    int8_t ret;
    memset(&ret, raw[address], 1);
    return ret;
}

template<>
inline uint8_t UMemory::Get<uint8_t>(size_t address) const
{
    return raw[address];
}

template<>
inline uint16_t UMemory::Get<uint16_t>(size_t address) const
{
    uint16_t v0 = raw[address];
    uint16_t v1 = raw[address + 1];
    if (endian == EEndianess::little)
    {
        return (v1 << 8) | v0;
    }
    else
    {
        return (v0 << 8) | v1;
    }
}

template<>
inline int16_t UMemory::Get<int16_t>(size_t address) const
{
    uint16_t ret = Get<uint16_t>(address);
    return *(int16_t*)&ret;
}

template<>
inline int32_t UMemory::Get<int32_t>(size_t address) const
{
    uint32_t ret = 0;
    if (endian == EEndianess::little)
    {
        for (int i = 0; i < 4; ++i)
        {
            ret = (ret << 8) | raw[address + 3 - i];
        }
    }
    else
    {
        for (int i = 0; i < 4; ++i)
        {
            ret = (ret << 8) | raw[address + i];
        }
    }
    return *(int32_t*)&ret;
}

template<>
inline void UMemory::Set<int8_t>(size_t address, const int8_t& value)
{
    memset(raw + address, value, 1);
}

template<>
inline void UMemory::Set<uint8_t>(size_t address, const uint8_t& value)
{
    memset(raw + address, value, 1);
}

template<>
inline void UMemory::Set<int16_t>(size_t address, const int16_t& value)
{
    if (endian == EEndianess::little)
    {
        memset(raw + address, value & 255, 1);
        memset(raw + address + 1, (value >> 8) & 255, 1);
    }
    else
    {
        memset(raw + address + 1, value & 255, 1);
        memset(raw + address, (value >> 8) & 255, 1);
    }
}

template<>
inline void UMemory::Set<uint16_t>(size_t address, const uint16_t& value)
{
    if (endian == EEndianess::little)
    {
        memset(raw + address, value & 255, 1);
        memset(raw + address + 1, (value >> 8) & 255, 1);
    }
    else
    {
        memset(raw + address + 1, value & 255, 1);
        memset(raw + address, (value >> 8) & 255, 1);
    }
}

template<>
inline void UMemory::Set<int32_t>(size_t address, const int32_t& value)
{
    if (endian == EEndianess::little)
    {
        memset(raw + address, value & 255, 1);
        memset(raw + address + 1, (value >> 8) & 255, 1);
        memset(raw + address + 2, (value >> 16) & 255, 1);
        memset(raw + address + 3, (value >> 24) & 255, 1);
    }
    else
    {
        memset(raw + address + 3, value & 255, 1);
        memset(raw + address + 2, (value >> 8) & 255, 1);
        memset(raw + address + 1, (value >> 16) & 255, 1);
        memset(raw + address, (value >> 24) & 255, 1);
    }
}
