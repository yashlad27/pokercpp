# CLI Texas Hold'em Poker Game in C++

This project is a **command-line poker game** built in C++

# Poker rules - Texas Hold'em

- **Poker Strategy**
- Hand ranking, betting logic, odds, bluffing, pot management
- **C++ Concepts**
- Classes, structs, STL (vector, maps, sets), algorithms
- **DSA concepts**
- Sorting, frequency maps, hand evaluation logic, shuffling, simulations

# Poker Rules - Texas Hold'em

1. **Two hole cards** are dealt to each player
2. **Five community** cards are dealt in stages:
	- The Flop (3 cards)
	- The Turn (1 card)
	- The River (1 card)
3. Each player makes the **best 5-card hand** using their 2 hole cards + 5 community cards.
4. Players take actions: **fold, call, raise, check**
5. The game ends with:
	- Everyone folding except one -> winner takes pot
	- A **showdown** if more than one player remains -> best hand wins 

# Hand Rankings (Highest to Lowest)
1. Royal Flush (A, K, Q, J, 10)
2. Straight Flush
3. Four of a kind
4. Full house 
5. Flush
6. Straight
7. Three of a kind
8. Two Pair
9. One Pair
10. High card


# Features (Planned)
1. Card and deck generation
2. CLI game loop with user input
3. Hand evaluator
4. Betting round logic
5. Bot opponent
6. Showdown with hand comparison
7. Monte Carlo Simulation for bot strategy 

# How code works:
1. Card: Struct with suit and rank
2. Deck: class that creates 52 cards, shuffles and deals
3. Player: tracks chip count, hole cards, player actions
4. Game: Runs each round: deal->betting->showdown
5. HandEvaluator: Uses frequency maps to evaluate hands

# Poker Player's Thinking Process
- Do I have a strong hand pre-flop?
- What could my opponent be holding
- What are the odds of improving my hand?
- Should i bluff, raise or fold?
- What's the pot odds and expected value?

--- 
## 🧰 How to Run the Game

### 📦 Requirements
- macOS (Apple Clang / g++ pre-installed)
- Sublime Text or any C++ IDE
- Terminal

### ▶️ Run the Game
```bash
clang++ main.cpp -o poker && ./poker
