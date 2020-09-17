#pragma once
#include <fstream>
#include "common.hpp"

namespace CryptoFunctions
{

struct CDResult
{
    long long C;
    long long D;
};

CDResult GetCD(long long p)
{
    CDResult result;

    long long p1 = p - 1;

    result.C = rand() % p1;

    while (gcd(p1, result.C).d != 1) result.C--;

    result.D = gcd(p1, result.C).y;
    if (result.D < 0) result.D += p1;

    long long res = (result.C * result.D) % p1;

    if (res != 1)
        throw std::runtime_error("GetCD: Bad result!");

    return result;
}

void Shamir()
{
    // stage 1 Random Prime Number
    long long P = PRIME_NUMBERS[rand() % PRIME_NUMBERS_ARRAY_SIZE];

    CDResult aCD = GetCD(P);

    CDResult bCD = GetCD(P);

    // m^Ca mod P
    {
        std::ifstream inputData("Capture.PNG", std::ios::binary);
        if (!inputData.is_open())
            throw std::runtime_error("can't find file: ShamirInputData.txt!");

        std::ofstream pass("ShamirPass1", std::ios::binary);

        std::vector<char> buffer(std::istreambuf_iterator<char>(inputData.rdbuf()), {});
        for (const auto& it : buffer)
        {
            long long res = PowMod(static_cast<long long>(static_cast<unsigned char>(it)), aCD.C, P);
            pass.write(reinterpret_cast<char*>(&res), 8);
        }
    }

    // x1^Cb mod P
    {
        std::ifstream inputData("ShamirPass1", std::ios::binary);
        std::ofstream pass("ShamirPass2", std::ios::binary);

        std::vector<char> buffer(std::istreambuf_iterator<char>(inputData), {});
        for (char* begin = buffer.data(); begin != buffer.data() + buffer.size(); begin += 8)
        {
            const long long it = *(reinterpret_cast<long long*>(begin));
            long long res = PowMod(static_cast<long long>(it), aCD.D, P);
            pass.write(reinterpret_cast<char*>(&res), 8);
        }
    }

    // x2^Da mod P
    {
        std::ifstream inputData("ShamirPass2", std::ios::binary);
        std::ofstream pass("ShamirPass3", std::ios::binary);

        std::vector<char> buffer(std::istreambuf_iterator<char>(inputData), {});
        for (char* begin = buffer.data(); begin != buffer.data() + buffer.size(); begin += 8)
        {
            const long long it = *(reinterpret_cast<long long*>(begin));
            long long res = PowMod(static_cast<long long>(it), bCD.C, P);
            pass.write(reinterpret_cast<char*>(&res), 8);
        }
    }

    // x3^Db mod P
    {
        std::ifstream inputData("ShamirPass3", std::ios::binary);
        std::ofstream pass("ShamirResult", std::ios::binary);

        std::vector<char> buffer(std::istreambuf_iterator<char>(inputData), {});
        for (char* begin = buffer.data(); begin != buffer.data() + buffer.size(); begin += 8)
        {
            const long long it = *(reinterpret_cast<long long*>(begin));
            unsigned char res = PowMod(static_cast<long long>(it), bCD.D, P);
            pass.write(reinterpret_cast<char*>(&res), 1);
        }
    }
}

}