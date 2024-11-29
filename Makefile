# Compiler settings
CXX = g++
CXXFLAGS = -std=c++11 -Wall -I/Users/anantajeetdevaraj/Developer/KeystrokeRNG/TestU01-1.2.3/include -I/opt/homebrew/include

# Source and output files
SRC = main.cpp Keystroke.cpp KeystrokePRNG.cpp # Added Keystroke.cpp
OBJ = main.o Keystroke.o KeystrokePRNG.o # Added Keystroke.o
OUT = main  # Output executable name

# Targets
all: compile link

# Compile step
compile:
	$(CXX) $(CXXFLAGS) -c $(SRC)  # Compile main.cpp and Keystroke.cpp into object files

# Link step (Add the correct library paths using -L for SFML)
link:
	$(CXX) $(OBJ) -o $(OUT) \
		-L/Users/anantajeetdevaraj/Developer/KeystrokeRNG/TestU01-1.2.3/testu01/.libs \
		-L/Users/anantajeetdevaraj/Developer/KeystrokeRNG/TestU01-1.2.3/probdist/.libs \
		-L/Users/anantajeetdevaraj/Developer/KeystrokeRNG/TestU01-1.2.3/mylib/.libs \
		-L/opt/homebrew/lib \
		-ltestu01 -lprobdist -lm -lsfml-graphics -lsfml-window -lsfml-system \
		-lssl -lcrypto

# Clean up
clean:
	rm -f $(OBJ) $(OUT)

# Run the program
run: $(OUT)
	export DYLD_LIBRARY_PATH=/Users/anantajeetdevaraj/Developer/KeystrokeRNG/TestU01-1.2.3/testu01/.libs:/Users/anantajeetdevaraj/Developer/KeystrokeRNG/TestU01-1.2.3/probdist/.libs:/Users/anantajeetdevaraj/Developer/KeystrokeRNG/TestU01-1.2.3/mylib/.libs:/opt/homebrew/lib:$DYLD_LIBRARY_PATH && ./$(OUT)
