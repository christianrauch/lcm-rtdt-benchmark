#include <lcm/lcm-cpp.hpp>
#include <ByteVectorStamped.hpp>
#include <Duration.hpp>

#include <thread>
#include <chrono>
#include <iostream>

class DelayPub {
public:
    void onPong(const lcm::ReceiveBuffer* /*rbuf*/, const std::string& /*channel*/,  const ByteVectorStamped* pong) {
        float rtt = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count()-pong->utime) / 1.0e6f;
        std::cout << "rtt: " << rtt << " s" << std::endl;
        Duration duration;
        duration.duration = rtt;
        lcm.publish("roundtrip_time", &duration);
    }

    bool setup() {
        if(!lcm.good())
            return false;

        lcm.subscribe("pong", &DelayPub::onPong, this);

        // periodically publish pings
        ping_thread = std::thread([this](){
            while(true) {
                ByteVectorStamped ping;
                ping.utime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                ping.ndata = 0;
                lcm.publish("ping", &ping);
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        });

        return true;
    }

    [[noreturn]] void run() {
        while(true) {
            lcm.handle();
        }
    }

private:
    lcm::LCM lcm;
    std::thread ping_thread;
};

int main(/*int argc, char *argv[]*/) {
    DelayPub delay_pub;

    if(delay_pub.setup()) {
        delay_pub.run();
    }
    else {
        std::cerr << "error in LCM setup, exit" << std::endl;
        exit(EXIT_FAILURE);
    }
}
