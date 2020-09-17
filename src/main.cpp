#include <iostream>
#include "crypto/export.hpp"

namespace
{

constexpr const long long PRIME_NUMBER_INDEX = 5;

const auto P = CryptoFunctions::PRIME_NUMBERS[PRIME_NUMBER_INDEX] * 2ull + 1ull;
const auto G = CryptoFunctions::CreateGenerator(P);

constexpr const long long MY_PRIVATE_KEY = 7;

constexpr const long long BOB_PUBLIC_KEY = 5;
}

int main()
{
    try
    {
        const auto powMod = CryptoFunctions::PowMod(G, MY_PRIVATE_KEY, P);
        std::cout << "PowMod = " << powMod << "\n\n";

        const auto gcdResult = CryptoFunctions::gcd(192, 248);
        std::cout << "gcd = " << gcdResult.d << '\n'
            << "X = " << gcdResult.x << " Y = " << gcdResult.y << "\n\n";

        const auto diffieHellmanResult = CryptoFunctions::DiffieHellman(BOB_PUBLIC_KEY, MY_PRIVATE_KEY, P);
        std::cout << "DiffieHellman = " << diffieHellmanResult << "\n\n";

        const auto smallBigStep = CryptoFunctions::SmallBigStep(BOB_PUBLIC_KEY, diffieHellmanResult, P);
        std::cout << "SmallBigStep = " << smallBigStep << '\n';

        CryptoFunctions::Shamir();
    }
    catch (const std::runtime_error& err)
    {
        std::cout << "Error: " << err.what() << '\n';
    }
}
