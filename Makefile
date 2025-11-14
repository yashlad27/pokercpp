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
      montecarlo/MonteCarloSimulator.cpp \
      utils/performance_monitor.cpp \
      utils/game_logger.cpp

# Core model/library files (no main.cpp)
LIB_SRC = model/card.cpp model/deck.cpp model/player.cpp \
          model/advanced_hand_evaluator.cpp \
          model/bot_player.cpp \
          montecarlo/MonteCarloSimulator.cpp \
          view/bot_thinking_visualizer.cpp \
          view/bot_thinking_config.cpp

TARGET = poker
TEST_MC = tests/test_monte_carlo
TEST_HAND = tests/test_hand_evaluator

# Main game target
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

# Test targets
test_monte_carlo: tests/test_monte_carlo.cpp $(LIB_SRC)
	$(CXX) $(CXXFLAGS) tests/test_monte_carlo.cpp $(LIB_SRC) -o $(TEST_MC)
	./$(TEST_MC)

test_hand_evaluator: tests/test_hand_evaluator.cpp model/advanced_hand_evaluator.cpp model/card.cpp
	$(CXX) $(CXXFLAGS) tests/test_hand_evaluator.cpp model/advanced_hand_evaluator.cpp model/card.cpp -o $(TEST_HAND)
	./$(TEST_HAND)

# Run all tests
test: test_hand_evaluator test_monte_carlo
	@echo "\n=== All Tests Passed ==="

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(TEST_MC) $(TEST_HAND)