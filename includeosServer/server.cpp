#include <sstream>

#include <os>
#include <net/inet>
#include <timers>

using namespace std::chrono;

void Service::start()
{
    // Get the first IP stack
    // It should have configuration from config.json
    auto& inet = net::Super_stack::get(0);

    // Print some useful netstats every 30 secs
    Timers::periodic(5s, 30s,
                     [&inet] (uint32_t) {
                         printf("<Service> TCP STATUS:\n%s\n", inet.tcp().status().c_str());
                     });

    auto& server = inet.tcp().listen(1234);
    // Add a TCP connection handler - here a hardcoded HTTP-service
    server.on_connect(
            [] (net::tcp::Connection_ptr conn) {
                printf("<Service> @on_connect: Connection %s successfully established.\n",
                       conn->remote().to_string().c_str());
                // read async with a buffer size of 1024 bytes
                // define what to do when data is read
                conn->on_read(1024,
                              [conn] (auto buf)
                              {
                                  printf("<Service> @on_read: %lu bytes received.\n", buf->size());
                                  conn->write(buf);
                              });
            });

    printf("*** Basic demo service started ***\n");
}
