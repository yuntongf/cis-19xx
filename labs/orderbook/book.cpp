#include <cstdint>
#include <map>
#include <unordered_map>
#include <list>
#include <vector>
#include <utility>

using Amount = uint64_t;
using Price = double;

class Order {
public:
    using Id = uint64_t;
    enum class Side {Buy, Sell};

    Order(const Id id, const Side& side, const Amount amount, const Price price)
    : id_{id}, side_{side}, amount_{amount}, price_{price} {}

    Id GetId() const noexcept {
        return id_;
    }

    Side GetSide() const noexcept {
        return side_;
    }

    Amount GetAmount() const noexcept {
        return amount_;
    }

    Price GetPrice() const noexcept {
        return price_;
    }

    Amount Fill(Amount amount) {
        amount_ -= amount;
        return amount_;
    }

    [[nodiscard]] bool CanFill(Amount amount) {
        return amount_ >= amount;
    }
private:
    Id id_;
    Side side_;
    Amount amount_;
    Price price_;
};

struct Trade {
    Order::Id order1;
    Order::Id order2;
    Amount amount;
    Price price;
};

class OrderBook {
    inline static Order::Id id = 0;
    using Iter = std::list<Order>::iterator;
public:
    void MatchOrders() {
        while (!buy_.empty() && !sell_.empty()) {
            auto& [buy_price, buy_orders] = *buy_.begin();
            auto& [sell_price, sell_orders] = *sell_.begin();
            if (buy_price < sell_price) {
                return;
            }
            while (!buy_orders.empty() && !sell_orders.empty()) {
                auto& buy_order = buy_orders.front();
                auto& sell_order = sell_orders.front();
                
                Amount fill_amount = std::min(buy_order.GetAmount(), sell_order.GetAmount());

                buy_order.Fill(fill_amount);
                sell_order.Fill(fill_amount);
                trades_.push_back({
                    buy_order.GetId(), 
                    sell_order.GetId(), 
                    fill_amount, 
                    buy_price
                });

                if (buy_order.GetAmount() == 0) {
                    buy_orders.pop_front();
                    orders_.erase(buy_order.GetId());
                }
                if (sell_order.GetAmount() == 0) {
                    sell_orders.pop_front();
                    orders_.erase(sell_order.GetId());
                }
            }
            if (buy_orders.empty()) {
                buy_.erase(buy_price);
            }
            if (sell_orders.empty()) {
                sell_.erase(sell_price);
            }
        }
    }

    void AddBuy(const Price price, const Amount amount) {
        auto it = buy_.find(price);
        Order order {
            id++,
            Order::Side::Buy,
            amount,
            price
        };
        if (it == buy_.end()) {
            auto [iter, _] = buy_.insert({price, {std::move(order)}});
            orders_[order.GetId()] = iter->second.begin();
        } else {
            it->second.emplace_back(std::move(order));
            orders_[order.GetId()] = std::prev(it->second.end());
        }
    }

    void AddSell(const Price price, const Amount amount) {
        auto it = sell_.find(price);
        Order order {
            id++,
            Order::Side::Sell,
            amount,
            price
        };
        if (it == sell_.end()) {
            sell_.insert({price, {std::move(order)}});
            orders_[order.GetId()] = sell_[price].begin();
        } else {
            it->second.emplace_back(std::move(order));
            orders_[order.GetId()] = std::prev(it->second.end());
        }
    }

    void CancelOrder(Order::Id id) {
        auto liter = orders_[id];
        if (liter->GetSide() == Order::Side::Buy) {
            auto& orders = buy_[liter->GetPrice()];
            orders.erase(liter);
        } else {
            auto& orders = sell_[liter->GetPrice()];
            orders.erase(liter);
        }
        orders_.erase(id);
    }
private:
    std::map<Price, std::list<Order>, std::greater<Price>> buy_;
    std::map<Price, std::list<Order>> sell_;
    std::unordered_map<Order::Id, Iter> orders_;
    std::vector<Trade> trades_;
};

int main() {
    return 0;
}
