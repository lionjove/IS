#pragma once
#include "common.hpp"
#include "lab1.hpp"

namespace CryptoFunctions
{
constexpr long long CreateGenerator(long long p)
{
    if (p <= 2)
        throw std::runtime_error("CreateGenerator:p is too small");

    long long tmp = p - 1ull;
    long long q = tmp / 2ull;

    if ((tmp % 2ull != 0) || !IsPrime(q))
        throw std::runtime_error("CreateGenerator:p does not satisfy p = 2q + 1");

    long long g = p - 2;
    while (PowMod(g, q, p) == 1)
        g--;

    if (g <= 1)
        throw("CreateGenerator:Unable to create generator for provided p");

    return g;
}
}