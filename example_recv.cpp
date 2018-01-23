#include <proton/connection.hpp>
#include <proton/container.hpp>
#include <proton/delivery.hpp>
#include <proton/message.hpp>
#include <proton/messaging_handler.hpp>
#include <proton/link.hpp>
#include <proton/message_id.hpp>
#include <proton/value.hpp>

#include <iostream>
#include <map>

class simple_recv : public proton::messaging_handler {
  private:
    std::string url;
    proton::receiver receiver;
    uint64_t expected;
    uint64_t received;

  public:
    simple_recv(const std::string &s, int c) : url(s), expected(c), received(0) {}

    void on_container_start(proton::container &c) override {
        receiver = c.open_receiver(url);
        std::cout << "simple_recv listening on " << url << std::endl;
    }

    void on_message(proton::delivery &d, proton::message &msg) override {
        if (proton::get<uint64_t>(msg.id()) < received) {
            return; // Ignore duplicate
        }

        if (expected == 0 || received < expected) {
            std::cout << msg.body() << std::endl;
            received++;

            if (received == expected) {
                d.receiver().close();
                d.connection().close();
            }
        }
    }
};

int main(int argc, char **argv) {
    std::string address("127.0.0.1:5672/examples");

    int message_count = 100;
    try {
        simple_recv recv(address, message_count);
        proton::container(recv).run();

        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 1;
}
