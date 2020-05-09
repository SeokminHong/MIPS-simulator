#pragma once

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
