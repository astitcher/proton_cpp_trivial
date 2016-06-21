#include <proton/default_container.hpp>
#include <proton/messaging_handler.hpp>

#include <iostream>

class ExampleHandler: public proton::messaging_handler {
    std::string url_;
    
    void on_container_start(proton::container& c) override {
        c.connect(url_);
    }

    void on_connection_open(proton::connection& c) override {
        std::cout << "Connection to " << url_ << " is open\n";
    }

  public:
    ExampleHandler(const std::string& url) : url_(url) {}
};

int main() {
  try {
    ExampleHandler h{"127.0.0.1:5672"};
    proton::default_container(h).run();
  } catch (std::exception& e) {
    std::cout << "Caught error: " << e.what() << "\n";
  }
}


