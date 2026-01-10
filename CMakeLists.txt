cmake_minimum_required(VERSION 3.15.0...3.21.0)

project(mpicpp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(TOP_LEVELPROJ)
include(cmake/CPM.cmake)
include(cmake/Gtest.cmake)
endif()

find_package(MPI REQUIRED COMPONENTS CXX)

add_subdirectory(lib)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()



# include(GNUInstallDirs)
# include(CMakePackageConfigHelpers)

# target_include_directories(mpicpp
#   PUBLIC
#   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
#   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

# target_link_libraries(mpicpp PUBLIC MPI::MPI_CXX)

# install(TARGETS mpicpp
#   EXPORT mpicpp-targets
#   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#   ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#   RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#   INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#   PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#   )

# configure_package_config_file(
#   "${CMAKE_CURRENT_SOURCE_DIR}/config.cmake.in"
#   "${CMAKE_CURRENT_BINARY_DIR}/mpicpp-config.cmake"
#   INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mpicpp)
# install(FILES
#   "${CMAKE_CURRENT_BINARY_DIR}/mpicpp-config.cmake"
#   DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mpicpp)
# install(
#   EXPORT mpicpp-targets
#   NAMESPACE mpicxx::
#   DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mpicpp)

