#include <iostream>
#include <vector>
extern "C" {
    #include "unif01.h"  // For RNGs
    #include "bbattery.h"
}
#include "Keystroke.h"
#include "KeystrokePRNG.h"


KeystrokePRNG* prng = nullptr;

// MyRandomFunction: Wrapper for KeystrokePRNG's generateRandomNumber
double MyRandomFunction() {
    return prng->generateRandomNumber();  // Call generateFloat on the global PRNG instance
}


int main() {
    // Create a window using SFML
    sf::RenderWindow window(sf::VideoMode(1800, 800), "Keyboard Input Detector");

    sf::Font font;
    if (!font.loadFromFile("assets/Typewriter.ttf")) {
        std::cerr << "Error loading font" << std::endl;
        return -1;
    }

    // Initial static text to display
    sf::Text introText("Type few words below and then close the window to start generating random numbers: ", font, 30);
    introText.setPosition(50, 50); // Position the intro text on the window

    // Dynamic text (the text the user will type)
    sf::Text typedText("", font, 30);
    typedText.setPosition(100, 150); // Position the typed text below the intro text

    // String to store the user typed text
    std::string typedString = "";

    // Game loop to keep the window open
    while (window.isOpen()) {
        sf::Event event;
        
        // Handle all events in the event queue
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            /// Check for text input (captures characters entered by the user)
            if (event.type == sf::Event::TextEntered) {
                // Ignore non-ASCII and special keys
                if (event.text.unicode < 128) {
                    char enteredChar = static_cast<char>(event.text.unicode);

                    // Handle backspace: remove last character
                    if (enteredChar == '\b' && typedString.size() > 0) {
                        typedString.pop_back();
                    } else if (enteredChar != '\b') {
                        // Add the typed character to the string
                        typedString += enteredChar;
                    }

                    // Update the dynamic text to display what the user typed
                    typedText.setString(typedString);
                }
            }

            // Handle key press and release for storing keystroke data
            if (event.type == sf::Event::KeyPressed) {
                keyPress(event.key.code);
            }

            if (event.type == sf::Event::KeyReleased) {
                keyRelease(event.key.code);
            }
        
        }
        // Draw the intro text and the dynamic text
        window.draw(introText);
        window.draw(typedText);

        
        // Display the contents of the window
        window.display();


    }

    
    std::vector<long long> keystrokeData = combineAndreturn();

    prng = new KeystrokePRNG(keystrokeData);


    //Create an external generator instance for your RNG
    unif01_Gen *gen = unif01_CreateExternGen01("Keystroke PRNG", MyRandomFunction);
    
    //Run the SmallCrush test suite
    bbattery_SmallCrush(gen);  // Run the basic tests (you can adjust if you need more comprehensive tests)

    // Clean ups
    unif01_DeleteExternGen01(gen);
    delete prng;
    prng = nullptr;

    


    return 0;
}