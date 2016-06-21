#include <proton/default_container.hpp>
#include <proton/messaging_handler.hpp>

#include <string>

class ExampleHandler: public proton::messaging_handler {
    std::string url_;
    
    // The container has started
    void on_container_start(proton::container&) override {
    }

    // A message can be sent
    void on_sendable(proton::sender&) override {
    }

    // A message is received
    void on_message(proton::delivery&, proton::message&) override {
    }

  public:
    ExampleHandler(const std::string& url) : url_(url) {}
};

int main() {
    ExampleHandler h("localhost");
    proton::default_container(h).run();
}

