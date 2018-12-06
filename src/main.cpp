#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <strings.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <boost/iostreams/close.hpp>

static constexpr size_t BUFFER_SIZE = 1024;
static constexpr int RIGHTS = PROT_READ | PROT_WRITE;
static constexpr int ANON = MAP_ANONYMOUS | MAP_PRIVATE;
static constexpr int FIXED_SHARED = MAP_ANONYMOUS | MAP_PRIVATE;

int fd = -1;

void send_fd(int, int send_fd) { fd = send_fd; }

int recv_fd(int) { return fd; }

char* client_setup_shm(int server_connection) {
   int fd = memfd_create("name", MFD_CLOEXEC | MFD_ALLOW_SEALING);
   if (fd < 0) {
      perror("memfd_create");
      throw std::runtime_error("memfd_create failed");
   }

   if (ftruncate(fd, BUFFER_SIZE) < 0) {
      perror("ftruncate");
      throw std::runtime_error("ftruncate failed");
   }
   send_fd(server_connection, fd);

   char* ptr = reinterpret_cast<char*>(mmap(nullptr, BUFFER_SIZE * 2, RIGHTS, ANON, -1, 0));
   mmap(ptr, BUFFER_SIZE, RIGHTS, FIXED_SHARED, fd, 0);
   mmap(&ptr[BUFFER_SIZE], BUFFER_SIZE, RIGHTS, FIXED_SHARED, fd, 0);

   return ptr;
}

char* server_setup_shm(int client_connection) {
   int fd = recv_fd(client_connection);
   if (fcntl(fd, F_ADD_SEALS, F_SEAL_SHRINK) == -1) {
      perror("fcntl");
      throw std::runtime_error("fcntl failed");
   }
   // SHRINK_SEAL makes sure filesize is always >= BUFFER_SIZE
   if (ftruncate(fd, BUFFER_SIZE) < 0) {
      perror("ftruncate");
      throw std::runtime_error("ftruncate failed");
   }

   char* ptr = reinterpret_cast<char*>(mmap(nullptr, BUFFER_SIZE * 2, RIGHTS, ANON, -1, 0));
   mmap(ptr, BUFFER_SIZE, RIGHTS, FIXED_SHARED, fd, 0);
   mmap(&ptr[BUFFER_SIZE], BUFFER_SIZE, RIGHTS, FIXED_SHARED, fd, 0);

   return ptr;
}

int main(int, char**) {
   auto clientbuffer = client_setup_shm(0);
   auto serverbuffer = server_setup_shm(0);

   std::fill(clientbuffer, &clientbuffer[BUFFER_SIZE], 'X');
   if (not std::equal(serverbuffer, &serverbuffer[BUFFER_SIZE], serverbuffer)) {
      throw std::runtime_error("clientbuffer != serverbuffer");
   }

   // maliciously try to shrink the underlying buffer
   int res = ftruncate(fd, 0);
   if (res == 0) {
      throw std::runtime_error("ftruncate to 0 should not be allowed anymore!");
   }
   perror("ftruncate");

   return 0;
}
