set(LIB_INTERNAL_NAME mpicxx)

add_library(${LIB_INTERNAL_NAME})

target_link_libraries(${LIB_INTERNAL_NAME} PUBLIC MPI::MPI_CXX)

set(SRCS
    src/exception.cpp
    src/request.cpp
    src/reductionop.cpp
    src/datatype.cpp
    src/environment.cpp
)

target_sources(${LIB_INTERNAL_NAME} PRIVATE ${SRCS})

target_include_directories(${LIB_INTERNAL_NAME} PUBLIC header)
