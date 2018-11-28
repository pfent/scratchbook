#ifndef SCRATCHBOOK_ACL_H
#define SCRATCHBOOK_ACL_H

#include <sys/mman.h>
#include <sys/acl.h>
#include <fcntl.h>
#include <pwd.h>
#include <zconf.h>
#include <cstring>

void printAcl(const char* name) {
   ssize_t len;
   auto testAcl = acl_get_file(name, ACL_TYPE_ACCESS);
   std::cout << acl_to_text(testAcl, &len);
}

#define checckedCall(fun, ...) if(fun(__VA_ARGS__) < 0) {perror(#fun); throw std::runtime_error("Call to "#fun" failed");}

void* Setup_RingBuffer(size_t size) {
   int fd = shm_open(
         "/foo",
         O_CREAT | O_TRUNC | O_RDWR,
         0660 // not accessible by others
   );
   ftruncate(fd, size);

   void* result = mmap(
         nullptr,
         size,
         PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0
   );
   memset(result, 0, size);

   acl_t acList = acl_get_fd(fd);
   acl_entry_t entry;
   checckedCall(acl_create_entry, &acList, &entry);
   checckedCall(acl_set_tag_type, entry, ACL_USER);

   uid_t userId = getpwnam("test")->pw_uid;
   checckedCall(acl_set_qualifier, entry, &userId);

   acl_permset_t permset;
   checckedCall(acl_get_permset, entry, &permset);
   checckedCall(acl_add_perm, permset, ACL_READ | ACL_WRITE);
   checckedCall(acl_set_permset, entry, permset);

   checckedCall(acl_calc_mask, &acList);
   if (acl_valid(acList) == -1) {
      return nullptr;
   }
   checckedCall(acl_set_fd, fd, acList);

   checckedCall(acl_free, (void*) acList);
   close(fd);

   return result;
}

#endif //SCRATCHBOOK_ACL_H
