#include <proton/container.hpp>
#include <proton/messaging_handler.hpp>

#include <iostream>

class ExampleHandler: public proton::messaging_handler {
    const std::string url_;
    
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
    auto&& h = ExampleHandler{"127.0.0.1:5672"};
    proton::container(h).run();
  } catch (std::exception& e) {
    std::cout << "Caught error: " << e.what() << "\n";
  }
}


