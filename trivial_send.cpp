#include <proton/connection_options.hpp>
#include <proton/container.hpp>
#include <proton/message.hpp>
#include <proton/messaging_handler.hpp>
#include <proton/value.hpp>

#include <iostream>
#include <string>

class MessageSender: public proton::messaging_handler {
    const proton::value message_;

    // A message can be sent
    void on_sendable(proton::sender& s) override {
        auto msg = proton::message(message_);
        s.send(proton::message(msg));
        s.close();
        std::cout << "Message sent\n";
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
    auto&& sender =  MessageSender{"Hello World"};
    auto&& container = proton::container(sender);
    auto&& copts = proton::connection_options()
      .sasl_allow_insecure_mechs(true)
      .user("user@proton")
      .password("password")
    ;
    container.open_sender("127.0.0.1/examples", copts);
    container.run();
  } catch (std::exception& e) {
    std::cout << "Caught error: " << e.what() << "\n";
  }
}

