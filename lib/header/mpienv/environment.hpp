#ifndef MPICPP_HEADER_MPIENVIRONMENT_ENVIRONMENT_HPP
#define MPICPP_HEADER_MPIENVIRONMENT_ENVIRONMENT_HPP
#pragma once

#include <mpi.h>

namespace mpicxx
{
    class [[nodiscard]] environment
    {
    public:
        environment(int& argc,char**& argv);
        environment();
        ~environment();
        environment(environment const &) = delete;
        environment &operator=(environment const &) = delete;
        environment(environment &&) = delete;
        environment &operator=(environment &&) = delete;
    };
} // namespace mpicxx

#endif
