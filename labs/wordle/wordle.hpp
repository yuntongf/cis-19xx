#pragma once

#include <string>
#include <cstring>
#include <iostream>
#include <cctype>

class Wordle {
public:
    enum class Color { Grey, Yellow, Green };

    static constexpr int MAX_ATTEMPTS = 6;
    static constexpr int WORD_LENGTH = 5;

    explicit Wordle(const std::string& answer, int remaining_attempts = MAX_ATTEMPTS);

    bool IsValidGuess(const std::string& guess) const;
    int GetRemainingAttempts() const;
    void ReduceRemainingAttempts();
    void EvaluateGuess(const std::string& guess);
    bool Won() const;
private:
    bool isEnglishWord(const std::string& guess) const;
    void printTile(char c, Color tile) const;

private:
    std::string answer_;
    int remaining_attempts_;
    int answer_freq_map_[26];
    bool won_ = false;
};