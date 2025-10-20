#include <cstddef>
#include <list>
#include <iostream>
#include <vector>

class Snake {
public:
    enum class Dir {Up, Down, Left, Right};

    Snake(int dim)
    : apple_{dim / 2, dim / 2, Dir::Left},
      dim_{dim},
      snake_{},
      dir_{Dir::Left},
      score_{0}
    {
        for (int i = 0; i < DEFAULT_SPAWN_LENGTH; ++i) {
            snake_.push_back({dim / 2 - 1, dim / 2 - 1 + i, Dir::Left});
        }
    }

    void Start() {
        while (true) {
            // change direction based on user input
            ChangeDirection();

            if (!CanMove()) {
                std::cout << "You Lost!\n";
                return;
            }

            bool eaten = Move();
            if (eaten) {
                ++score_;
                AddTail();
            }
        }
    }

    void ChangeDirection() {}

    int GetScore() const noexcept {
        return score_;
    }
private:
    [[nodiscard]] bool CanMove() {
        const auto head = snake_.front();
        const auto [drow, dcol] = DIR_MAP[static_cast<std::size_t>(dir_)];
        int nrow = head.row + drow;
        int ncol = head.col + dcol;
        if (nrow < 0 || nrow >= dim_ || ncol < 0 || ncol >= dim_) {
            return false;
        }
        Coord new_head = {nrow, ncol, Dir::Left};
        for (auto it = snake_.begin(); it != std::prev(snake_.end()); ++it) {
            if (*it == new_head) {
                return false;
            }
        }
        return true;
    }

    bool Move() {
        snake_.pop_back();
        return AddHead();
    }

    bool AddHead() {
        const auto head = snake_.front();
        const auto [drow, dcol] = DIR_MAP[static_cast<std::size_t>(dir_)];
        Coord new_head = {head.row + drow, head.col + dcol, dir_};
        snake_.push_front(new_head);
        return new_head == apple_;
    }

    void AddTail() {
        const auto tail = snake_.back();
        auto reverse_dir = DIR_REVERSE[static_cast<std::size_t>(tail.dir)];
        const auto [drow, dcol] = DIR_MAP[static_cast<std::size_t>(reverse_dir)];
        int nrow = tail.row + drow;
        int ncol = tail.col + dcol;
        snake_.push_back({nrow, ncol, reverse_dir});
    }
private:
    struct Coord {
        int row;
        int col;
        Dir dir;

        bool operator==(const Coord& other) const noexcept {
            return row == other.row && col == other.col;
        }
    };
    struct Movement {
        int drow;
        int dcol;
    };
    static const int DEFAULT_SPAWN_LENGTH = 5;
    static const std::vector<Movement> DIR_MAP;
    static const std::vector<Dir> DIR_REVERSE;
    Coord apple_;
    int dim_;
    std::list<Coord> snake_;
    Dir dir_;
    int score_ = 0;
};

const std::vector<Snake::Dir> Snake::DIR_REVERSE = {
    Dir::Down,
    Dir::Up,
    Dir::Right,
    Dir::Left
};

const std::vector<Snake::Movement> Snake::DIR_MAP = {
    {-1, 0},
    {1, 0},
    {0, -1},
    {0, 1}
};

int main() {

}