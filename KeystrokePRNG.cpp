#include "KeystrokePRNG.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <openssl/evp.h>
#include <stdexcept>

#define MT_SIZE 624
#define MATRIX_A 0x9908b0df  // constant vector for twisting
#define UMASK 0x80000000     // most significant bit mask
#define LMASK 0x7fffffff     // least significant bits mask

// State vector and index to track the current state
static std::vector<unsigned long> state(MT_SIZE);
static int stateIndex = MT_SIZE; 

// Constructor to initialize with keystroke data
KeystrokePRNG::KeystrokePRNG(const std::vector<long long>& keystrokeData) {
    // Initialize the state using keystroke data
    initPRNG(keystrokeData);
}

// Initialize the state vector with a seed value
void KeystrokePRNG::initPRNG(const std::vector<long long>& keystrokeData) {
    unsigned long seed = generateSeedFromKeystrokeData(keystrokeData);
    state[0] = seed;
    for (int i = 1; i < MT_SIZE; ++i) {
        state[i] = (1812433253 * (state[i - 1] ^ (state[i - 1] >> 30)) + i);
    }
    stateIndex = MT_SIZE;  // Reset the state index for number generation
}

// Generate the next random number
unsigned int KeystrokePRNG::generate() {
    if (stateIndex >= MT_SIZE) {
        // Apply the twist when the state vector is exhausted
        for (int i = 0; i < MT_SIZE; i++) {
            unsigned long y = (state[i] & UMASK) | (state[(i + 1) % MT_SIZE] & LMASK);
            state[i] = state[(i + 397) % MT_SIZE] ^ (y >> 1);
            if (y % 2 != 0) {
                state[i] ^= MATRIX_A;
            }
        }
        stateIndex = 0;  // Reset index
    }

    unsigned long y = state[stateIndex++];  // Get the next number from the state
    y ^= (y >> 11);  // Apply tempering
    y ^= (y << 7) & 0x9d2c5680;
    y ^= (y << 15) & 0xefc60000;
    y ^= (y >> 18);

    return static_cast<unsigned int>(y);  // Return the 32-bit random number
}


unsigned long KeystrokePRNG::generateSeedFromKeystrokeData(const std::vector<long long>& keystrokeData) {
    // Create a stringstream to accumulate the keystroke data
    std::stringstream ss;

    // Combine all keystroke data into the stringstream (as a single string or byte stream)
    for (long long data : keystrokeData) {
        ss << data;
    }

    // Get the combined data as a string
    std::string combinedData = ss.str();

    // Initialize the EVP context for SHA-256
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Failed to create EVP_MD_CTX");
    }

    // Initialize the SHA-256 context
    if (EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("Failed to initialize SHA-256 context");
    }

    // Update the digest with the combined keystroke data
    if (EVP_DigestUpdate(ctx, combinedData.c_str(), combinedData.length()) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("Failed to update SHA-256 context");
    }

    // Finalize the digest and retrieve the hash value
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLength = 0;
    if (EVP_DigestFinal_ex(ctx, hash, &hashLength) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("Failed to finalize SHA-256 context");
    }

    // Clean up the EVP context
    EVP_MD_CTX_free(ctx);

    // Convert the first 8 bytes of the hash to a single unsigned long value (seed)
    unsigned long seed = 0;
    for (int i = 0; i < 8; ++i) {
        seed = (seed << 8) | hash[i];  // Combine the bytes into a 64-bit unsigned long
    }

    return seed;  
}


double KeystrokePRNG::generateRandomNumber() {
    unsigned int random_number = KeystrokePRNG::generate();  // Get next random number
    return static_cast<double>(random_number) / static_cast<double>(0xFFFFFFFF);  // Normalize b/w [0, 1]
}
