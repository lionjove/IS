#pragma once
#include <unordered_map>

namespace CryptoFunctions
{

// log(b)
constexpr long long PowMod(long long g, long long b, long long p)
{
    if (g >= p)
        throw std::runtime_error("PowMod:Bad arguments!");

    long long res = 1;
    while (b)
    {
        if (b & 1)
            res = (res * g) % p;
        g = (g * g) % p;
        b >>= 1;
    }
    return res;
}

struct GcdResult
{
    GcdResult() = default;
    constexpr GcdResult(long long a, long long b, long long c) noexcept : d(a), x(b), y(c) {}

    long long d, x, y;
};

constexpr GcdResult gcd(long long a, long long b)
{
    if (a == 0)
        return GcdResult(b, 0, 1);

    long long unPrev = 1;
    long long vnPrev = 0;
    long long unCur = 0;
    long long vnCur = 1;

    while (b != 0)
    {
        const long long bn = a / b;
        const long long newB = a % b;
        a = b;
        b = newB;

        const long long unNew = unPrev - bn * unCur;
        const long long vnNew = vnPrev - bn * vnCur;

        unPrev = unCur;
        vnPrev = vnCur;
        unCur = unNew;
        vnCur = vnNew;
    }

    return GcdResult(a, unPrev, vnPrev);
}

// log(privateKey)
constexpr long long DiffieHellman(long long publicKey, long long privateKey, long long mod)
{
    if (publicKey >= mod)
        throw std::runtime_error("DiffieHellman:Public key can't be bigger than mod");

    return PowMod(publicKey, privateKey, mod);
}

// log(sqrt(p)) + 2sqrt(p) - muls and mod
long long SmallBigStep(const long long g, const long long b, const long long p)
{
    if (gcd(g, p).d > 1)
        throw std::runtime_error("SmallBigStep:Bad input");

    const long long n = static_cast<long long>(sqrtl(static_cast<long long>(p))) + 1ll;

    // log(n)
    const long long gn = PowMod(g, n, p);

    std::unordered_map<long long, long long> vals;
    long long val1 = gn;
    for (long long i = 1ll; i < n; i++) // n muls and mod
    {
        vals[val1] = i;
        val1 = (val1 * gn) % p;
    }

    long long val2 = b % p;
    for (long long i = 0; i <= n; i++) // n muls and mod
    {
        if (vals.count(val2)) // O(1)
        {
            const long long ans = vals[val2] * n - i; // O(1)
            if (ans < p)
                return ans;
        }

        val2 = (val2 * g) % p; // O(1)
    }

    return -1ll;
}


long long CreateGenerator(long long p)
{
    if (p <= 2)
        throw std::runtime_error("CreateGenerator:p is too small");

    long long tmp = p - 1ull;
    long long q = tmp / 2ull;

    if ((tmp % 2ull != 0) || !IsPrime(q))
        throw std::runtime_error("CreateGenerator:p does not satisfy p = 2q + 1");

    long long g;
    do
    {
        g = (rand() % (p - 2)) + 1;
    } while (PowMod(g, q, p) == 1);
        

    if (g <= 1)
        throw("CreateGenerator:Unable to create generator for provided p");

    return g;
}
}
