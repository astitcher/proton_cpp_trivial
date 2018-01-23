#include <proton/connection_options.hpp>
#include <proton/container.hpp>
#include <proton/listener.hpp>
#include <proton/messaging_handler.hpp>
#include <proton/transport.hpp>
#include <proton/sasl.hpp>
#include <proton/source.hpp>
#include <proton/target.hpp>

#include <iostream>
#include <string>

namespace {
    auto c_quit = std::string("quit");
}

class ExampleHandler: public proton::messaging_handler {
    void on_container_start(proton::container& c) override {
        auto&& copts = proton::connection_options()
            .sasl_allow_insecure_mechs(true)
            .sasl_allowed_mechs("ANONYMOUS PLAIN")
        ;
        c.listen("0.0.0.0", copts);
    }

    void on_connection_open(proton::connection& c) override {
        std::cout << c << ": New incoming connection to " << c.virtual_host() << " from: "
		  << c.user() << "\n";
    }

    void on_receiver_open(proton::receiver& r) override {
        std::cout << r << ": New incoming receiver: target address: " << r.target().address() << "\n";
    }

    void on_sender_open(proton::sender& s) override {
        auto a = s.source().address();
        std::cout << s << ": New incoming sender: source address: " << a << "\n";
        if (a==c_quit) {
            s.container().stop();
        }
    }

    void on_transport_open(proton::transport& t) override {
        std::cout << t << ": Transport open\n";
    }

    void on_transport_error(proton::transport& t) override {
        std::cout << t << ": Transport error: " << t.error().what() << "\n";
    }

    void on_transport_close(proton::transport& t) override {
        std::cout << t << ": Transport close\n";
    }

};

int main() {
  try {
    auto&& h = ExampleHandler{};
    proton::container(h).run();
  } catch (std::exception& e) {
    std::cout << "Caught error: " << e.what() << "\n";
  }
}


