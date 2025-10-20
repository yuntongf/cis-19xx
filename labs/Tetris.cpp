#include <vector>
#include <cstddef>
#include <unordered_map>
#include <array>

template <int N>
class Tetris {
public:
    struct Coord {
        int row;
        int col;
    };

    struct Displacement {
        int delta_row;
        int delta_col;
    };

    static_assert(N >= 5);

    struct Block {
        Coord start;
        std::vector<Displacement> displacements; 
    };

    enum class Direction {Rotate, Down, Left, Right};

    Tetris() {
        SpawnBlock();
    }

    void Start() {
        while (!CheckLose()) {
            // make one move either from user or automatic move down
        }
    }

    void Move(Direction dir) {
        if (!CanMove(dir)) {
            if (IsAtBottom()) {
                LockBlock();
                SpawnBlock();
            }
            return;
        }
        if (dir == Direction::Rotate) {
            if (CanMove(dir)) {
                for (auto& disp : curr_blk_.displacements) {
                    int delta_row = disp.delta_row;
                    int delta_col = disp.delta_col;
                    disp.delta_row = ROTATION_MATRIX[0][0] * delta_row + ROTATION_MATRIX[0][1] * delta_col;
                    disp.delta_col = ROTATION_MATRIX[1][0] * delta_row + ROTATION_MATRIX[1][1] * delta_col;
                }
            }
        } else {
            const auto [row_move, col_move] = DIR_MAP.at(dir);
            curr_blk_.start.row += row_move;
            curr_blk_.start.col += col_move;
        }
    }

private:
    void SpawnBlock() {
        std::vector<Displacement> dis = {
            {0, 0},
            {0, 1},
            {1, 1}
        };
        curr_blk_ = Block{SPAWN_COORD_, dis};
    }

    bool CanMove(Direction dir) const {
        for (const auto [delta_row, delta_col] : curr_blk_.displacements) {
            int start_row = curr_blk_.start.row;
            int start_col = curr_blk_.start.col;
            int new_row, new_col;
            if (dir == Direction::Rotate) {
                new_row = start_row + ROTATION_MATRIX[0][0] * delta_row + ROTATION_MATRIX[0][1] * delta_col;
                new_col = start_col + ROTATION_MATRIX[1][0] * delta_row + ROTATION_MATRIX[1][1] * delta_col;
            } else {
                const auto [row_move, col_move] = DIR_MAP.at(dir);
                new_row = start_row + delta_row + row_move;
                new_col = start_col + delta_col + col_move;
            }
            // Out of bound
            if (new_row < 0 || new_row >= N || new_col < 0 || new_col >= N) {
                return false;
            }
            if (board_[new_row][new_col]) {
                return false;
            }
        }
        return true;
    }

    bool IsAtBottom() const {
        return !CanMove(Direction::Down);
    }

    void LockBlock() {
        for (const auto [delta_row, delta_col] : curr_blk_.displacements) {
            int row = curr_blk_.start.row + delta_row;
            ++counts_[row];      
        }
        for (int i = 0; i < N; ++i) {
            if (counts_[i] == N) {
                CancelRow(i);
            }      
        }
    }

    void CancelRow(int row) {
        for (int i = row; i >= 1; ++i) {
            board_[i] = std::move(board_[i - 1]);
            counts_[i] = counts_[i-1];
        }
        board_[0].fill(0);
        counts_[0] = 0;
    }

    bool CheckLose() const {
        return !CanMove(Direction::Left) &&
               !CanMove(Direction::Right) &&
               !CanMove(Direction::Down);
    }
private:
    using Row = std::array<bool, N>;
    using Board = std::array<Row, N>;
    Board board_{};
    Block curr_blk_;
    std::array<int, N> counts_{};
    static const Coord SPAWN_COORD_;
    static const std::vector<std::vector<int>> ROTATION_MATRIX;
    static const std::unordered_map<Direction, Displacement> DIR_MAP;
};

template <int N>
const std::vector<std::vector<int>> Tetris<N>::ROTATION_MATRIX = {
    {0, -1},
    {1, 0}
};

template <int N>
const typename Tetris<N>::Coord Tetris<N>::SPAWN_COORD_ = {0, 0};

template <int N>
const std::unordered_map<typename Tetris<N>::Direction, typename Tetris<N>::Displacement> Tetris<N>::DIR_MAP = {
    {Direction::Down, {1, 0}},
    {Direction::Left, {0, -1}},
    {Direction::Right, {0, 1}}
};

int main() {
    Tetris<10> t;
    t.Move(Tetris<10>::Direction::Down);
    return 0;
}

// struct MyClass {
//     MyClass(int a, int b) : a_(a), b_(b) {}
// private:
//     int a_;
//     int b_;
// };

// int main() {
//     MyClass obj{2, 3};
// };