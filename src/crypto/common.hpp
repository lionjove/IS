#pragma once
#include <array>

namespace CryptoFunctions
{

constexpr long long FastPowll(long long a, long long b)
{
    long long result = 1;
    while (b != 0)
    {
        if (b & 1) result *= a;
        b >>= 1;
        a *= a;
    }

    return result;
}

constexpr bool IsPrime(long long num)
{
    for (long long i = 2; i <= num / 2; i++)
        if (num % i == 0)
            return false;
    return true;
}

template<size_t size>
constexpr std::array<long long, size> GetPrimeNumbersStartingFrom(long long startNumber)
{
    std::array<long long, size> result{};

    for (auto& it : result)
    {
        while (!IsPrime(startNumber))
        {
            if (startNumber == LLONG_MAX) throw("GetPrimeNumbersStartingFrom:Overflow!");

            startNumber++;
        }

        it = startNumber;
        startNumber++;
    }

    return result;
}

}