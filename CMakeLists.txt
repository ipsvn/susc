cmake_minimum_required(VERSION 3.13)
project(susc C)
#set(CMAKE_CXX_STANDARD 17)
#set(THREADS_PREFER_PTHREAD_FLAG ON)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

find_package(Threads REQUIRED)

find_package(hazelnetworking)

#find_library(HAZEL_LIBRARIES hazelnetworking PATHS ${CMAKE_INSTALL_PREFIX}/lib)
#find_path(HAZEL_INCLUDE_DIRS PATHS ${CMAKE_INSTALL_PREFIX}/include)

add_subdirectory(susc)
#add_subdirectory(test)

add_executable(tmptest
    tmptest/main.c
)
target_include_directories(tmptest
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/tmptest
    INTERFACE
        ${PROJECT_SOURCE_DIR}/susc
)
target_link_libraries(tmptest
    PRIVATE
        susc
)

#find_package(mbedTLS REQUIRED)


# Install headers
install(DIRECTORY susc/include/susc DESTINATION include/susc)

# Install the library
install(
    TARGETS susc
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)