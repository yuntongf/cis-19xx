#include <unordered_set>
#include <vector>
#include <cstdlib>
#include <iostream>

const std::vector<std::vector<int>> DIRS = {
    {-1, -1},
    {-1, 0},
    {-1, 1},
    {0, -1},
    {0, 1},
    {1, -1},
    {1, 0},
    {1, 1}
};

class Board {
public:
    struct Cell {
        bool isMine = false;
        bool isFlagged = false;
        bool isRevealed = false;
        int number = 0;
    };

    enum class ClickResult {
        Continue,
        Win,
        Lose
    };

    Board(std::size_t dim) : dim_{dim}, rows_{Rows(dim, Row(dim, Cell()))}, count_{0} {}

    ClickResult Click(int row, int col) {
        auto& cell = rows_[row][col];
        if (cell.isRevealed || cell.isFlagged) {
            return ClickResult::Continue;
        }
        if (cell.isMine) {
            return ClickResult::Lose;
        }
        // Reveal the cell
        cell.isRevealed = true;
        ++count_;
        for (auto dir : DIRS) {
            int new_row = row + dir[0];
            int new_col = col + dir[1];
            if (new_row < 0 || new_row >= dim_ || new_col < 0 || new_col >= dim_) {
                continue;
            }
            Click(new_row, new_col);
        }
        if (CheckWinning()) {
            return ClickResult::Win;
        }
        return ClickResult::Continue;
    }

    ClickResult Flag(int row, int col) {
        auto& cell = rows_[row][col];
        if (cell.isFlagged || cell.isRevealed) {
            return ClickResult::Continue;
        }
        cell.isFlagged = true;
        if (!cell.isMine) {
            ++count_;
        }
        if (CheckWinning()) {
            return ClickResult::Win;
        }
        return ClickResult::Continue;
    }

private:
    using Mine = std::pair<int, int>;

    struct PairHash {
        std::size_t operator()(const Mine& m) const {
            auto hasher = std::hash<int>();
            return hasher(m.first) ^ hasher(m.second << 1);
        }
    };
    void InitBoard() {
        std::unordered_set<Mine, PairHash> mines;
        // Every row has one mine
        for (int i = 0; i < dim_; ++i) {
            while (true) {
                int col = rand() % dim_;
                if (!mines.count({i, col})) {
                    mines.insert({i, col});
                    break;
                }
            }
        }
        for (auto [row, col] : mines) {
            PrepareMine(row, col, mines);
        }
    }

    void PrepareMine(int row, int col, const std::unordered_set<Mine, PairHash>& mines) {
        rows_[row][col].isMine = true;

        for (auto dir : DIRS) {
            int new_row = row + dir[0];
            int new_col = col + dir[1];
            if (new_row < 0 || new_row >= dim_ || new_col < 0 || new_col >= dim_) {
                continue;
            }
            auto& cell = rows_[new_row][new_col];
            if (mines.count({new_row, new_col})) {
                continue;
            }
            ++cell.number;
        }
    }

    [[nodiscard]] bool CheckWinning() const noexcept {
        return dim_ * dim_ - count_ == dim_;
    }
private:
    using Row = std::vector<Cell>;
    using Rows = std::vector<Row>;
    Rows rows_;
    std::size_t dim_;
    int count_;
};

int main() {
    Board board(10);
}