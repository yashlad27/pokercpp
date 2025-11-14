CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra
SRC = main.cpp \
      controller/poker_controller.cpp \
      view/cli_view.cpp \
      view/ascii_art.cpp \
      view/bot_thinking_visualizer.cpp \
      view/bot_thinking_config.cpp \
      model/card.cpp model/deck.cpp model/player.cpp \
      model/advanced_hand_evaluator.cpp \
      model/bot_player.cpp \
      animation/spinner.cpp \
      animation/card_animation.cpp \
      montecarlo/MonteCarloSimulator.cpp
TARGET = poker

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)