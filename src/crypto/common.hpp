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
    if (num <= 3ull)
        return num > 1ull;
    else if (num % 2ull == 0 || num % 3ull == 0)
        return false;

    long long i = 5;

    while (i * i <= num)
    {
        if (num % i == 0 || num % (i + 2) == 0)
            return false;
        i = i + 6;
    }
        
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


constexpr const size_t PRIME_NUMBERS_ARRAY_SIZE = 150ull;
constexpr const long long PRIME_NUMBERS_START_POSITION = 255;

const auto PRIME_NUMBERS = CryptoFunctions::GetPrimeNumbersStartingFrom<PRIME_NUMBERS_ARRAY_SIZE>(PRIME_NUMBERS_START_POSITION);

}