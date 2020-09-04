#include <iostream>
#include "crypto/export.hpp"

namespace
{
constexpr const size_t PRIME_NUMBERS_ARRAY_SIZE = 150ull;
constexpr const long long PRIME_NUMBERS_START_POSITION = 75ull;

constexpr const long long PRIME_NUMBER_INDEX = 5;

constexpr const auto PRIME_NUMBERS = CryptoFunctions::GetPrimeNumbersStartingFrom<PRIME_NUMBERS_ARRAY_SIZE>(PRIME_NUMBERS_START_POSITION);

constexpr const auto P = PRIME_NUMBERS[PRIME_NUMBER_INDEX] * 2ull + 1ull;
constexpr const auto G = CryptoFunctions::CreateGenerator(P);

constexpr const long long MY_PRIVATE_KEY = 7;
constexpr const long long MY_PUBLIC_KEY = CryptoFunctions::PowMod(G, MY_PRIVATE_KEY, P);

constexpr const long long BOB_PUBLIC_KEY = 5;
}

int main()
{
    try
    {
        constexpr const auto powMod = CryptoFunctions::PowMod(G, MY_PRIVATE_KEY, P);
        std::cout << "PowMod = " << powMod << "\n\n";

        constexpr const auto gcdResult = CryptoFunctions::gcd(192, 248);
        std::cout << "gcd = " << gcdResult.d << '\n'
            << "X = " << gcdResult.x << " Y = " << gcdResult.y << "\n\n";

        constexpr const auto diffieHellmanResult = CryptoFunctions::DiffieHellman(BOB_PUBLIC_KEY, MY_PRIVATE_KEY, P);
        std::cout << "DiffieHellman = " << diffieHellmanResult << "\n\n";

        const auto smallBigStep = CryptoFunctions::SmallBigStep(BOB_PUBLIC_KEY, diffieHellmanResult, P);
        std::cout << "SmallBigStep = " << smallBigStep << '\n';
    }
    catch (const std::runtime_error& err)
    {
        std::cout << "Error: " << err.what() << '\n';
    }
}
