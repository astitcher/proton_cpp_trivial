#include <proton/default_container.hpp>
#include <proton/messaging_handler.hpp>

#include <iostream>

class ExampleHandler: public proton::messaging_handler {
    void on_container_start(proton::container& c) override {
        c.listen("0.0.0.0");
    }

    void on_connection_open(proton::connection& c) override {
        std::cout << "New incoming connection to " << c.virtual_host() << "\n";
    }

    void on_receiver_open(proton::receiver& r) override {
        std::cout << "New incoming receiver: target address: " << r.target().address() << "\n";
    }
};

int main() {
  try {
    ExampleHandler h;
    proton::default_container(h).run();
  } catch (std::exception& e) {
    std::cout << "Caught error: " << e.what() << "\n";
  }
}


