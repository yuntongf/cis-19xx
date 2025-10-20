#include "wordle_sol.hpp"

Wordle::Wordle(const std::string& answer, int remaining_attempts)
    : answer_{answer}, remaining_attempts_{remaining_attempts}, answer_freq_map_{} 
{
    for (char c : answer) {
        ++answer_freq_map_[getLetterIdx(c)];
    }
}

bool Wordle::IsValidGuess(const std::string& guess) const {
    return guess.length() == WORD_LENGTH &&
           isAlpha(guess) &&
           isEnglishWord(guess);
}

int Wordle::GetRemainingAttempts() const {
    return remaining_attempts_;
}

void Wordle::ReduceRemainingAttempts() {
    --remaining_attempts_;
}

void Wordle::EvaluateGuess(const std::string& guess) {
    Color colors[WORD_LENGTH];
    int guess_freq_map[26];
    std::memcpy(guess_freq_map, answer_freq_map_, sizeof(answer_freq_map_));

    // check for green tiles
    won_ = true;
    for (int i = 0; i < WORD_LENGTH; ++i) {
        if (guess[i] == answer_[i]) {
            colors[i] = Color::Green;
            --guess_freq_map[getLetterIdx(guess[i])];
        } else {
            won_ = false;
        }
    }

    if (won_) {
        printTiles(colors, guess);
        return;
    }

    // check for yellow and grey
    for (int i = 0; i < WORD_LENGTH; ++i) {
        if (colors[i] == Color::Green) continue;

        int letter_idx = getLetterIdx(guess[i]);
        if (guess_freq_map[letter_idx]) {
            colors[i] = Color::Yellow;
            --guess_freq_map[letter_idx];
        } else {
            colors[i] = Color::Grey;
        }
    }

    printTiles(colors, guess);
}

bool Wordle::Won() const {
    return won_;
}

bool Wordle::isAlpha(const std::string& guess) const {
    for (char c : guess) {
        if (!std::isalpha(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}

bool Wordle::isEnglishWord([[maybe_unused]] const std::string& guess) const {
    return true;
}

void Wordle::printTiles(Color* colors, const std::string& guess) const {
    for (int i = 0; i < WORD_LENGTH; ++i) {
        printTile(guess[i], colors[i]);
        std::cout << " ";
    }
    std::cout << "\n\n";
}

void Wordle::printTile(char c, Color tile) const {
    const char* color_code;

    switch (tile) {
        case Color::Green:
            color_code = "\033[42m"; // green
            break;
        case Color::Yellow:
            color_code = "\033[43m"; // yellow
            break;
        case Color::Grey:
        default:
            color_code = "\033[100m"; // dark grey
            break;
    }

    std::cout << color_code
              << "\033[30m "
              << static_cast<char>(std::toupper(c))
              << " \033[0m";
}

int Wordle::getLetterIdx(char c) const {
    return std::tolower(static_cast<unsigned char>(c)) - 'a';
}
