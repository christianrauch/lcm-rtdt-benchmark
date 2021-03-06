#include <lcm/lcm-cpp.hpp>
#include <ByteVectorStamped.hpp>

#include <iostream>

class PingResponse {
public:
    void onPing(const lcm::ReceiveBuffer* /*rbuf*/, const std::string& /*channel*/,  const ByteVectorStamped* ping) {
        lcm.publish("pong", ping);
    }

    bool setup() {
        if(!lcm.good())
            return false;

        lcm.subscribe("ping", &PingResponse::onPing, this);

        return true;
    }

    [[noreturn]] void run() {
        while(true) {
            lcm.handle();
        }
    }

private:
    lcm::LCM lcm;
};

int main(/*int argc, char *argv[]*/) {
    PingResponse ping_response;

    if(ping_response.setup()) {
        ping_response.run();
    }
    else {
        std::cerr << "error in LCM setup, exit" << std::endl;
        exit(EXIT_FAILURE);
    }
}
