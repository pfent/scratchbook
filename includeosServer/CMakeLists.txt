cmake_minimum_required(VERSION 2.8.9)

# IncludeOS install location
if (NOT DEFINED ENV{INCLUDEOS_PREFIX})
    set(ENV{INCLUDEOS_PREFIX} /home/philipp/bin/includeos)
endif()
include($ENV{INCLUDEOS_PREFIX}/includeos/pre.service.cmake)
project (demo_client)

set(SERVICE_NAME "IncludeOS minimal example")
set(BINARY       "server_example")
set(SOURCES
        server.cpp # ...add more here
        )

# To add your own include paths:
# set(LOCAL_INCLUDES ".")

if ("$ENV{PLATFORM}" STREQUAL "x86_solo5")
    set(DRIVERS
            solo5net
            )
else()
    set(DRIVERS
            virtionet     # Virtio networking
            vmxnet3
            boot_logger   # Display boot information

            # Use "boot --drivers ." to see other drivers
            # virtioblk # Virtio block device
            # ... Others from src/drivers
            )
endif()

set(PLUGINS
        # Use "boot --plugins ." to see other plugins
        )

include($ENV{INCLUDEOS_PREFIX}/includeos/post.service.cmake)
