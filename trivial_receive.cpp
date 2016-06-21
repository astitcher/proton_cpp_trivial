#include <proton/default_container.hpp>
#include <proton/delivery.hpp>
#include <proton/messaging_handler.hpp>

#include <functional>
#include <iostream>
#include <random>
#include <string>

class MessageReceiver: public proton::messaging_handler {
    std::function<bool(proton::value&)> test_;

    // A message is received
    void on_message(proton::delivery& d, proton::message& m) override {
        std::cout << "Received message body: " << m.body();
        if (test_(m.body())) {
            d.accept();
            std::cout << " - Accepting\n";
        } else {
            d.reject();
            std::cout << " - Rejecting\n";
        }
    }

  public:
    MessageReceiver(std::function<bool(proton::value&)> test) : test_(test) {}
};

auto coin = std::bind(std::uniform_int_distribution<int>(0, 1), std::default_random_engine{});

bool validate(proton::value&) {
    return coin()==1;
}

int main() {
  try {
    MessageReceiver receiver{validate};
    auto container = make_default_container(receiver);
    container->open_receiver("127.0.0.1/examples");
    container->run();
  } catch (std::exception& e) {
    std::cout << "Caught error: " << e.what() << "\n";
  }
}

