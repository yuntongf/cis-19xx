#include "wordle.hpp"

int main() {
    Wordle game("favor");
    std::cout << "Welcome to Wordle!\n";

    while (game.GetRemainingAttempts() > 0) {
        std::cout << "Enter your guess: ";
        std::string guess;
        std::cin >> guess;

        if (!game.IsValidGuess(guess)) {
            std::cout << "Your guess must be a valid 5-letter English word :(\n";
            continue;
        }

        game.ReduceRemainingAttempts();
        game.EvaluateGuess(guess);

        if (game.Won()) {
            std::cout << "Congratulations! You've guessed the word with "
                      << Wordle::MAX_ATTEMPTS - game.GetRemainingAttempts()
                      << " attempts.\n";
            return 0;
        }
    }

    std::cout << "You lost. Be better next time.\n";
    return 0;
}
