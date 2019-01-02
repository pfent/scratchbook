#include <sstream>

#include <os>
#include <net/inet>
#include <timers>

using namespace std::chrono;

void Service::start() {
    // Get the first IP stack
    // It should have configuration from config.json
    auto &inet = net::Super_stack::get(0);

    // Print some useful netstats every 30 secs
    Timers::periodic(5s, 30s,
                     [&inet](uint32_t) {
                         printf("<Service> TCP STATUS:\n%s\n", inet.tcp().status().c_str());
                     });

    printf("connecting");
    auto dest = net::Socket(net::tcp::Address(10, 0, 0, 42), 1024);
    inet.tcp().connect(dest, [](net::tcp::Connection_ptr conn) {
        conn->write("ping");
        conn->on_read(1024, [conn](auto buf) {
            printf("received");
        });
    });

    printf("*** Basic demo service started ***\n");
}
