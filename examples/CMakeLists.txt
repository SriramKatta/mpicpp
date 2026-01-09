add_executable(testprog main.cpp)
target_link_libraries(testprog PRIVATE mpicxx::mpicxx)