cmake_minimum_required (VERSION 2.8.11)
	project (fobrob)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

find_library(USB_LIB rtlsdr)
if(NOT USB_LIB)
	  message(FATAL_ERROR "rtlsdr library not found")
endif()

add_executable (rpitxify rpitxify.c hex.c manchester.c)
add_executable (fobrob fobrob.c demodulator.c filter.c runningavg.c hex.c protocol.c manchester.c)
add_executable (rollthecode rollthecode.c hex.c protocol.c manchester.c)

TARGET_LINK_LIBRARIES(fobrob pthread rtlsdr m)
