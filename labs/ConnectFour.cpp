#include <deque>
#include <cstddef>
#include <unordered_map>

class Board {
    struct Cell {
        bool occupied = false;
        int player = -1;
    };
public:
    Board() {}

    bool PlayCell(int row_idx, int col_idx, int player) {
        // check if cell has been played
        if (IsOccupied(row_idx, col_idx)) {
            return false;
        }
        Col& col = cols[col_idx];
        if (col.size() <= col_idx) {
            col.resize(col_idx + 1);
        }
        col[row_idx].occupied = true;
        col[row_idx].player = player;
        return true;
    }

    bool CheckWinning(int row_idx, int col_idx, int player) {
        return CheckPositiveDiagonal(row_idx, col_idx, player) ||
                CheckNegativeDiagonal(row_idx, col_idx, player);
    }

    bool IsOccupied(int row_idx, int col_idx) {
        if (!cols.count(col_idx)) {
            return false;
        }
        const Col& col = cols[col_idx];
        return (col.size() > row_idx &&
            col[row_idx].occupied);
    }
private:
    bool CheckPositiveDiagonal(int row_idx, int col_idx, int player) {
        int count = 0;
        int curr_row = row_idx;
        int curr_col = col_idx;
        while (IsOccupied(curr_row, curr_col) &&
            cols[curr_row][curr_col].player == player) {
            ++count;
            ++curr_row;
            ++curr_col;
            if (count == 5) {
                return true;
            }
        }
        curr_row = row_idx;
        curr_col = col_idx;
        while (IsOccupied(curr_row, curr_col) &&
            cols[curr_row][curr_col].player == player) {
            ++count;
            --curr_row;
            --curr_col;
            if (count == 5) {
                return true;
            }
        }
        return false;
    }

    bool CheckNegativeDiagonal(int row_idx, int col_idx, int player) {
        int count = 0;
        int curr_row = row_idx;
        int curr_col = col_idx;
        while (IsOccupied(curr_row, curr_col) &&
            cols[curr_row][curr_col].player == player) {
            ++count;
            ++curr_row;
            --curr_col;
            if (count == 5) {
                return true;
            }
        }
        curr_row = row_idx;
        curr_col = col_idx;
        while (IsOccupied(curr_row, curr_col) &&
            cols[curr_row][curr_col].player == player) {
            ++count;
            --curr_row;
            ++curr_col;
            if (count == 5) {
                return true;
            }
        }
        return false;
    }
private:
    using Col = std::deque<Cell>;
    using ColNum = int;
    using ColMap = std::unordered_map<ColNum, Col>;
    ColMap cols;
};

int main() {
    constexpr std::size_t num_rows = 10, num_cols = 10;

}