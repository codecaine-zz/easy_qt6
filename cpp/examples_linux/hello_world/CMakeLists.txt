add_executable(example_linux_hello_world main.cpp)
target_link_libraries(example_linux_hello_world PRIVATE simplegui)
set_target_properties(example_linux_hello_world PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
