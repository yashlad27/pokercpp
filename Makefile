CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra
SRC = main.cpp card.cpp deck.cpp player.cpp hand_evaluator.cpp advanced_hand_evaluator.cpp
TARGET = poker

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)