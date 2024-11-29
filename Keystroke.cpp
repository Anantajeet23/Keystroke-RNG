#include <iostream>
#include <chrono>
#include <map>
#include <vector>
#include <SFML/Graphics.hpp>

// Maps to store vectors of press and release durations for each key
std::map<sf::Keyboard::Key, std::vector<long long>> keyPressDurations;  // Press duration (time key held down)
std::map<sf::Keyboard::Key, std::vector<long long>> keyReleaseDurations;  // Release duration (time between release and next press)
std::map<sf::Keyboard::Key, std::vector<std::chrono::steady_clock::time_point>> keyPressTimes;  // To track press time
auto lastReleaseTime = std::chrono::steady_clock::now();  // Used to track release times

// Vectors to store all press and release durations
std::vector<long long> combinedDurations;

// Function to handle key press events
void keyPress(sf::Keyboard::Key key) {
    auto pressTime = std::chrono::steady_clock::now();
    keyPressTimes[key].push_back(pressTime);  // Store the press time in a vector
}

// Function to handle key release events
void keyRelease(sf::Keyboard::Key key) {
    auto releaseTime = std::chrono::steady_clock::now();
    
    if (keyPressTimes[key].empty()) {
        return;
    }

    auto pressTime = keyPressTimes[key].back();  // Get the last press time for this key
    keyPressTimes[key].pop_back();  // Remove the last press time since it's been handled

    // Calculate press duration (in milliseconds)
    long long pressDuration = std::chrono::duration_cast<std::chrono::milliseconds>(releaseTime - pressTime).count();
    keyPressDurations[key].push_back(pressDuration);  // Store the press duration

    // Calculate release duration (in milliseconds)
    long long releaseDuration = std::chrono::duration_cast<std::chrono::milliseconds>(releaseTime - lastReleaseTime).count();
    keyReleaseDurations[key].push_back(releaseDuration);  // Store the release duration

    // Update the last release time
    lastReleaseTime = releaseTime;
}

// Function to print out the values stored in pressDurations and releaseDurations vectors
void printVectorDurations() {
    std::cout << "\n Durations:" << std::endl;
    for (const auto& duration : combinedDurations) {
        std::cout << duration << " ms ";
    }
    std::cout << std::endl;
}

std::vector<long long> combineAndreturn(){
    
        for (const auto& entry : keyPressDurations) {
        combinedDurations.insert(combinedDurations.end(), entry.second.begin(), entry.second.end());
    }

    // Extract values from keyReleaseDurations map and store in allReleaseDurations
    for (const auto& entry : keyReleaseDurations) {
        combinedDurations.insert(combinedDurations.end(), entry.second.begin(), entry.second.end());
    }

    printVectorDurations();

    return combinedDurations;


}



