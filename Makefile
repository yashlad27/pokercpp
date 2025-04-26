CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra
SRC = main.cpp \
      controller/poker_controller.cpp \
      view/cli_view.cpp \
      model/card.cpp model/deck.cpp model/player.cpp \
      model/advanced_hand_evaluator.cpp \
      model/bot_player.cpp \
      animation/spinner.cpp
TARGET = poker

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)