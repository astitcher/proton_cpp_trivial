#include <proton/container.hpp>
#include <proton/messaging_handler.hpp>

#include <string>

class ExampleHandler: public proton::messaging_handler {
    std::string url_;
    
    // The container has started
    void on_container_start(proton::container& c) override {
    }

    // A message can be sent
    void on_sendable(proton::sender& s) override {
    }

    // A message is received
    void on_message(proton::delivery& d, proton::message& m) override {
    }

  public:
    ExampleHandler(const std::string& url) : url_(url) {}
};

int main() {
    auto h = ExampleHandler{"localhost"};
    proton::container(h).run();
}

