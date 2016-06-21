#include <proton/default_container.hpp>
#include <proton/messaging_handler.hpp>

#include <iostream>
#include <string>

class MessageSender: public proton::messaging_handler {
    const proton::value message_;
    proton::tracker tracker_;

    // A message can be sent
    void on_sendable(proton::sender& s) override {
        if (!tracker_) {
            auto msg = proton::message(message_);
            tracker_ = s.send(proton::message(msg));
            std::cout << "Message sent\n";
        }
    }

    void on_tracker_accept(proton::tracker& t) override {
        std::cout << "Message accepted\n";
        t.connection().close();
    }

    void on_tracker_reject(proton::tracker& t) override {
        std::cout << "Message rejected\n";
        t.connection().close();
    }

  public:
    MessageSender(const proton::value message) : message_(message) {}
};

int main() {
  try {
    MessageSender sender{"Hello World"};
    auto container = make_default_container(sender);
    container->open_sender("127.0.0.1/examples");
    container->run();
  } catch (std::exception& e) {
    std::cout << "Caught error: " << e.what() << "\n";
  }
}

