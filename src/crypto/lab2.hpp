#pragma once
#include <fstream>
#include <filesystem>
#include <algorithm>
#include "common.hpp"
#include "lab1.hpp"

namespace
{
const char* FILE_TO_ENCODE = "Capture.PNG";
}

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
        std::ifstream inputData(FILE_TO_ENCODE, std::ios::binary);
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
        std::ofstream pass("ShamirResult.PNG", std::ios::binary);

        std::vector<char> buffer(std::istreambuf_iterator<char>(inputData), {});
        for (char* begin = buffer.data(); begin != buffer.data() + buffer.size(); begin += 8)
        {
            const long long it = *(reinterpret_cast<long long*>(begin));
            unsigned char res = PowMod(static_cast<long long>(it), bCD.D, P);
            pass.write(reinterpret_cast<char*>(&res), 1);
        }
    }
}

void ELGamal()
{
    const auto p = 257;
    const auto g = 5;

    const long long bobPrivateKey = 13;
    const long long bobPublicKey = PowMod(g, bobPrivateKey, p);

    const long long k = 3;

    {
        std::ifstream inputData(FILE_TO_ENCODE, std::ios::binary);
        if (!inputData.is_open())
            throw std::runtime_error("can't find file: Capture.PNG!");

        std::ofstream pass("ELGamalCoded", std::ios::binary);

        long long r = PowMod(g, k, p);
        pass.write(reinterpret_cast<char*>(&r), 8);

        std::vector<char> buffer(std::istreambuf_iterator<char>(inputData.rdbuf()), {});
        for (const auto& it : buffer)
        {
            long long e = (it * PowMod(bobPublicKey, k, p)) % p;
            pass.write(reinterpret_cast<char*>(&e), 8);
        }
    }

    {
        std::ifstream inputData("ELGamalCoded", std::ios::binary);
        std::ofstream pass("ELGamalEncoded.PNG", std::ios::binary);

        long long r;
        inputData.read(reinterpret_cast<char*>(&r), 8);

        std::vector<char> buffer(std::istreambuf_iterator<char>(inputData), {});
        for (char* begin = buffer.data(); begin != buffer.data() + buffer.size(); begin += 8)
        {
            const long long it = *(reinterpret_cast<long long*>(begin));
            unsigned char res = (it * PowMod(r, p - 1 - bobPrivateKey, p)) % p;
            pass.write(reinterpret_cast<char*>(&res), 1);
        }
    }
}

void Vernam()
{
    std::ifstream inputFile(FILE_TO_ENCODE, std::ios::binary);
    std::vector<char> buffer(std::istreambuf_iterator<char>(inputFile.rdbuf()), {});

    std::vector<unsigned char> keys(buffer.size());
    std::generate_n(keys.begin(), buffer.size(), [] { return static_cast<unsigned char>(rand() % 255); });

    for (size_t i = 0; i < buffer.size(); i++)
        buffer[i] ^= keys[i];

    std::ofstream("VernamEncrypted.png", std::ios::binary).write(buffer.data(), buffer.size());

    inputFile = std::ifstream("VernamEncrypted.png", std::ios::binary);
    buffer = std::vector<char>(std::istreambuf_iterator<char>(inputFile.rdbuf()), {});

    for (size_t i = 0; i < buffer.size(); i++)
        buffer[i] ^= keys[i];

    std::ofstream("VernamResult.png", std::ios::binary).write(buffer.data(), buffer.size());
}

void RSA()
{
    long long PA = PRIME_NUMBERS[rand() % PRIME_NUMBERS.size()];
    long long QA = PRIME_NUMBERS[rand() % PRIME_NUMBERS.size()];
    long long NA = PA * QA;

    long long FA = (PA - 1) * (QA - 1);

    long long DA;
    do
    {
        DA = rand() % FA;
    } while (gcd(DA, FA).d != 1);

    long long CA = gcd(FA, DA).y;
    if (CA < 0) CA += FA;

    std::ifstream inputFile(FILE_TO_ENCODE, std::ios::binary);
    std::vector<char> buffer(std::istreambuf_iterator<char>(inputFile.rdbuf()), {});

    std::ofstream outFile("EncodedRSA.png", std::ios::binary);
    for (auto& it : buffer)
    {
        long long e = PowMod(it, DA, NA);
        outFile.write(reinterpret_cast<char*>(&e), 8);
    }

    inputFile = std::ifstream("EncodedRSA.png", std::ios::binary);
    buffer = std::vector<char>(std::istreambuf_iterator<char>(inputFile.rdbuf()), {});
    outFile = std::ofstream("RSAResult.png", std::ios::binary);

    for (size_t i = 0; i < buffer.size() / 8; i++)
    {
        long long it = *reinterpret_cast<long long*>(buffer.data() + i * 8);

        char res = PowMod(it, CA, NA);

        outFile.write(&res, 1);
    }
}

}