#include <proton/connection.hpp>
#include <proton/container.hpp>
#include <proton/message.hpp>
#include <proton/messaging_handler.hpp>
#include <proton/tracker.hpp>
#include <proton/types.hpp>
#include <proton/value.hpp>

#include <iostream>
#include <map>

class simple_send : public proton::messaging_handler {
  private:
    std::string url;
    proton::sender sender;
    int sent;
    int confirmed;
    int total;

  public:
    simple_send(const std::string &s, int c) : url(s), sent(0), confirmed(0), total(c) {}

    void on_container_start(proton::container &c) override {
        sender = c.open_sender(url);
    }
 
    void on_container_stop(proton::container& c) override {
        std::cout << c.id() << ": stopped\n";
    }

    void on_sendable(proton::sender &s) override {
        while (s.credit() && sent < total) {
            proton::message msg;
            std::map<std::string, int> m;
            m["sequence"] = sent + 1;

            msg.id(sent + 1);
            msg.body(m);

            s.send(msg);
            sent++;
        }
    }

    void on_tracker_accept(proton::tracker &t) override {
        confirmed++;

        if (confirmed == total) {
            std::cout << "all messages confirmed" << std::endl;
            t.container().stop();
        }
    }

    void on_transport_close(proton::transport &) override {
        sent = confirmed;
    }
};

int main(int argc, char **argv) {
    std::string address("127.0.0.1:5672/examples");
    int message_count = 100;

    try {
        simple_send send(address, message_count);
        proton::container(send).run();

        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 1;
}
