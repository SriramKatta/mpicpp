#ifndef MPICPP_HEADER_REQUEST_REQUEST_HPP
#define MPICPP_HEADER_REQUEST_REQUEST_HPP

#include <mpi.h>
#include "status.hpp"

namespace mpicxx
{
    class request
    {
        MPI_Request implementation;

    public:
        request()
            : implementation(MPI_REQUEST_NULL)
        {
        }
        explicit constexpr request(MPI_Request implementation_arg)
            : implementation(implementation_arg)
        {
        }
        request(request const &other);
        request &operator=(request const &other);
        constexpr request(request &&other) noexcept
            : implementation(other.implementation)
        {
            other.implementation = MPI_REQUEST_NULL;
        }
        request &operator=(request &&other);
        void wait();
        bool test();
        void wait(status &status_arg);
        bool test(status &status_arg);
        ~request();
        MPI_Request &get() { return implementation; }
    };


    void waitall(int count, request *array_of_requests);
}
#endif