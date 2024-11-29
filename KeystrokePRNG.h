#include <vector>

class KeystrokePRNG {
public:
    // Constructor to initialize with keystroke data
    KeystrokePRNG(const std::vector<long long>& keystrokeData);

    // Function to generate a random 32-bit integer
    unsigned int generate();

    // Function to generate a random number in [0, 1]
    double generateRandomNumber();

private:

    // Function to generate a seed from keystroke data using SHA-256
    unsigned long generateSeedFromKeystrokeData(const std::vector<long long>& keystrokeData);

    // Function to initialize the state vector with keystroke data
    void initPRNG(const std::vector<long long>& keystrokeData);

};

