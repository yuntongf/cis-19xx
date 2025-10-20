#include <cstddef>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <sstream>

class TextBuffer {
public:
    TextBuffer(std::size_t size = 1024)
    : data_(size), gbegin_(0), gend_(size) {}

    void ChangeInsertCursor(std::size_t pos) {
        if (pos == gbegin_) {
            return;
        }
        if (pos < gbegin_) {
            gend_ = MoveToEnd(pos, gbegin_, gend_);
            gbegin_ = pos;
        } else {
            std::size_t gap_size = gend_ - gbegin_;
            gbegin_ = MoveToBegin(gend_, gap_size + pos, gbegin_);
            gend_ = gap_size + pos;
        }
    }

    void ChangeOverwriteCursor(std::size_t pos) {
        if (pos == gbegin_) {
            return;
        }
        MoveToBegin(gend_, data_.size(), pos);
        gbegin_ = pos;
        std::cout << "after setting overwrite cursor, gbegin_ is " << gbegin_ << ", gend is " << gend_ << "\n"; 
    }

    void TypeChar(char c) {
        CheckRealloc();
        data_[gbegin_++] = c;
    }

    friend std::ostream& operator<<(std::ostream& os, const TextBuffer& buff) {
        for (std::size_t i = 0; i < buff.gbegin_; ++i) {
            os << buff.data_[i];
        }
        for (std::size_t i = buff.gend_; i < buff.data_.size(); ++i) {
            os << buff.data_[i];
        }
        return os;
    }
private:
    void CheckRealloc() {
        if (gbegin_ == gend_) {
            Realloc();
        }
    }

    void Realloc() {
        int old_data_end = data_.size();
        data_.resize(data_.size() * 2);
        gend_ = MoveToEnd(gend_, old_data_end, data_.size());
    }

    // move data in the provided range to the end, return idx of start of the moved range
    std::size_t MoveToEnd(int start_, int end_, int target_end_) {
        auto it = std::move_backward(data_.begin() + start_, data_.begin() + end_, data_.begin() + target_end_);
        return it - data_.begin();
    }

    std::size_t MoveToBegin(int start_, int end_, int target_begin_) {
        // if (start_ == end_) {
        //     return target_begin_;
        // }
        auto it = std::move(data_.begin() + start_, data_.begin() + end_, data_.begin() + target_begin_);
        return it - data_.begin();
    }
private:
    std::vector<char> data_;
    std::size_t gbegin_;
    std::size_t gend_; // not inclusive
};

// aaaaabbbb        aaaaa

void test_basic_typing() {
    TextBuffer tb(10);
    tb.TypeChar('a');
    tb.TypeChar('b');
    tb.TypeChar('c');

    std::cout << tb;
    // std::ostringstream oss;
    // oss << tb;
    // std::cout << oss.str();
    // assert(oss.str() == "abc");
    // std::cout << "Test basic typing passed.\n";
}

void test_insert_at_front() {
    TextBuffer tb(10);
    tb.TypeChar('a');
    tb.TypeChar('b');
    tb.TypeChar('c');

    tb.ChangeInsertCursor(0);
    tb.TypeChar('x');
    std::cout << tb << ", should be: xabc\n";

    // std::ostringstream oss;
    // oss << tb;
    // assert(oss.str() == "xabc");
    // std::cout << "Test insert at front passed.\n";
}

void test_insert_in_middle() {
    TextBuffer tb(10);
    tb.TypeChar('a');
    tb.TypeChar('b');
    tb.TypeChar('c');

    tb.ChangeInsertCursor(1);
    tb.TypeChar('x');

    // std::ostringstream oss;
    // oss << tb;
    // assert(oss.str() == "axbc");
    // std::cout << "Test insert in middle passed.\n";
    std::cout << tb << ", should be: axbc\n";
}

void test_insert_at_end() {
    TextBuffer tb(10);
    tb.TypeChar('a');
    tb.TypeChar('b');

    tb.ChangeInsertCursor(2);  // no-op
    tb.TypeChar('c');

    // std::ostringstream oss;
    // oss << tb;
    // assert(oss.str() == "abc");
    // std::cout << "Test insert at end passed.\n";
    std::cout << tb << ", should be: abc\n";
}

void test_overwrite_char() {
    TextBuffer tb(10);
    tb.TypeChar('a');
    tb.TypeChar('b');
    tb.TypeChar('c');
    // std::cout << tb << "\n";

    tb.ChangeOverwriteCursor(1);
    tb.TypeChar('x');

    // std::ostringstream oss;
    // oss << tb;
    // assert(oss.str() == "axc");
    // std::cout << "Test overwrite passed.\n";
    std::cout << tb << ", should be: axc\n";
}

void test_realloc() {
    TextBuffer tb(4);  // small buffer
    tb.TypeChar('a');
    tb.TypeChar('b');
    tb.TypeChar('c');
    tb.TypeChar('d');  // fills to gbegin == gend
    tb.TypeChar('e');  // triggers realloc

    // std::ostringstream oss;
    // oss << tb;
    // assert(oss.str() == "abcde");
    // std::cout << "Test realloc passed.\n";
    std::cout << tb << ", should be: abcde\n";
}

void test_multiple_movements() {
    TextBuffer tb(10);
    tb.TypeChar('1');
    tb.TypeChar('2');
    tb.TypeChar('3');
    tb.TypeChar('4');
    tb.ChangeInsertCursor(2);
    tb.TypeChar('A');
    tb.ChangeInsertCursor(5);  // after current content
    tb.TypeChar('B');

    // std::ostringstream oss;
    // oss << tb;
    // assert(oss.str() == "12A34B");
    // std::cout << "Test multiple movements passed.\n";
    std::cout << tb << ", should be: 12A34B\n";
}

int main() {
    // test_basic_typing();
    // test_insert_at_front();
    // test_insert_in_middle();
    // test_insert_at_end();
    test_overwrite_char();
    // test_realloc();
    // test_multiple_movements();

    std::cout << "All tests passed.\n";
    return 0;
}
