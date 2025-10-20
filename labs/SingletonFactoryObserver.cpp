#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <algorithm>

// Singleton
class Singleton {
public:
    static int* GetInstance() {
        static int* instance = new int{5};
        return instance;
    }
private:
    Singleton();
    ~Singleton();
};

// int main() {
//     int* instance = Singleton::GetInstance();
//     int* redux = Singleton::GetInstance();
//     std::cout << *instance << ", " << *redux << "\n";
// }

// Factory
class Base {
public:
    virtual ~Base() {}
};

class DerivedA : public Base {

};

class DerivedB : public Base {

};

class Factory {
public:
    std::unique_ptr<Base> GetInstance(const std::string& type) const {
        if (type == "DerivedA") {
            return std::make_unique<DerivedA>();
        }
        if (type == "DerivedB") {
            return std::make_unique<DerivedB>();
        }
        return nullptr;
    }
};

// Observer
class Subject;

class Observer : public std::enable_shared_from_this<Observer> {
public:
    Observer(int id) : id_{id} {}

    ~Observer();

    void callback() const {
        std::cout << "[" << id_ << "]" << ": Call back!\n";
    }

    void subscribe(std::shared_ptr<Subject> sub, const std::string& topic);
private:
    int id_;
    std::vector<std::shared_ptr<Subject>> subs_;
};

class Subject {
public:
    void subscribe(const std::string& topic, std::shared_ptr<Observer> observer) {
        topics_[topic].emplace_back(observer);
    }

    void unsubscribe(Observer* observer) {
        for (auto& [topic, wps] : topics_) {
            wps.erase(std::remove_if(wps.begin(), wps.end(), 
                        [&observer](const auto& wp){
                            return wp.lock().get() == observer;
                        }), 
                     wps.end());
        } 
    }

    void notify(const std::string& topic) {
        if (topics_.find(topic) == topics_.end()) {
            return;
        }
        auto& wps = topics_[topic];
        wps.erase(std::remove_if(
                wps.begin(), 
                wps.end(),
                [](const auto& wp){
                    return wp.expired();
                }
            ),
            wps.end()
        );
        for (auto& wp : wps) {
            auto sp = wp.lock();
            sp->callback();
        }
    }
private:
    std::unordered_map<std::string, std::vector<std::weak_ptr<Observer>>> topics_;
};

void Observer::subscribe(std::shared_ptr<Subject> sub, const std::string& topic) {
    subs_.emplace_back(sub);
    sub->subscribe(topic, shared_from_this());
}

Observer::~Observer() {
    for (auto& sp : subs_) {
        sp->unsubscribe(this);
    }
}

int main() {
    auto sub = std::make_shared<Subject>();
    auto ob1 = std::make_shared<Observer>(1);
    // Observer ob2{2};
    ob1->subscribe(sub, "health");
    // ob2.subscribe(sub, "health");
    sub->notify("health");

    return 0;
}