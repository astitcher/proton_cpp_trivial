#include <proton/container.hpp>
#include <proton/messaging_handler.hpp>

#include <iostream>

class ExampleHandler: public proton::messaging_handler {
    // The container has started
    void on_container_start(proton::container& c) override {
        std::cout << "Started container " << c.id() << "\n";
    }

    // A message can be sent
    void on_sendable(proton::sender&) override {
    }

    // A message is received
    void on_message(proton::delivery&, proton::message&) override {
    }
};

int main() {
    auto&& handler = ExampleHandler{};
    auto&& container = proton::container(handler, "job-processor-3");
    container.run();
}

