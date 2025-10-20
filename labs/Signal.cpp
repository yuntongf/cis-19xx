#include <iostream>
#include <functional>
#include <string>
#include <vector>

class Slot {
public:
    Slot(int id) : id_{id} {}

    void operator()(const std::string& message) const {
        std::cout << "[" << id_ << "] " << "Signal received: " << message << "\n";
    }
private:
    int id_;
};

template <typename... Args>
class Signal {
public:
    using CallableSlot = std::function<void(Args...)>;

    void connect(const CallableSlot& slot) {
        slots_.emplace_back(slot);
    }

    void operator()(Args&&... args) {
        for (auto& slot : slots_) {
            slot(args...);
        }
    }
private:
    std::vector<CallableSlot> slots_;
};

int main() {
    Slot slot1{1};
    Slot slot2{2};

    Signal<std::string> signal;

    signal.connect(slot1);
    signal.connect(slot2);

    signal("Hii!");
    // should print signal received with the message
}