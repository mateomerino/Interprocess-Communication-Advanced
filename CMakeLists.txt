# Operating Systems II - UNC FCEFyN 2023


# First, we need to specify the CMake version
cmake_minimum_required(VERSION 3.15.0)

# Then, we setup our project
project(
    Client-Server
    VERSION 1.0.0
    DESCRIPTION "A Basic Server-Client Arquitechture using IPC mechanisms"
    LANGUAGES C
)

# Set the C version
set(CMAKE_C_STANDARD 11)

add_compile_options(-Wall -Werror -pedantic -Wextra -Wconversion)

add_library(servers STATIC server_unix.c server_inet4.c server_inet6.c functs.c cJSON.c)

add_executable(server server.c)
target_link_libraries(server servers)
target_link_libraries(server "-lz")

add_executable(client_unix client_unix.c)
target_link_libraries(client_unix servers)
target_link_libraries(client_unix "-lz")

add_executable(client_inet4 client_inet4.c)
target_link_libraries(client_inet4 servers)
target_link_libraries(client_inet4 "-lz")

add_executable(client_inet6 client_inet6.c)
target_link_libraries(client_inet6 servers)
target_link_libraries(client_inet6 "-lz")

target_include_directories(server PRIVATE "include/")
target_include_directories(client_unix PRIVATE "include/")
target_include_directories(client_inet4 PRIVATE "include/")
target_include_directories(client_inet4 PRIVATE "include/")





# The we add the headers of our project
# target_include_directories(server PRIVATE "include/")
# target_include_directories(client1 PRIVATE "include/")
# target_include_directories(client2 PRIVATE "include/")
# target_include_directories(client3 PRIVATE "include/")