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

add_compile_options("-std=c99")

add_library(encoding STATIC hex.c manchester.c)
add_library(runningavg STATIC runningavg.c)
add_library(demodulator STATIC demodulator.c)
add_library(filter STATIC filter.c)
add_library(protocol STATIC protocol.c)
add_library(modem STATIC demodulator.c filter.c runningavg.c)

add_executable (rpitxify rpitxify.c)
add_executable (fobrob fobrob.c)
add_executable (rollthecode rollthecode.c)

TARGET_LINK_LIBRARIES(rpitxify encoding)
TARGET_LINK_LIBRARIES(rollthecode encoding protocol)
TARGET_LINK_LIBRARIES(fobrob encoding modem protocol pthread rtlsdr m)
