#ifndef MPICPP_HEADER_HANDLES_STATUS_HPP
#define MPICPP_HEADER_HANDLES_STATUS_HPP

#include <mpi.h>

namespace mpicxx
{
    class status
    {
        MPI_Status implementation;

    public:
        status(MPI_Status implementation_arg) : implementation(implementation_arg)
        {
        }
        status() = default;
        int source() const { return implementation.MPI_SOURCE; }
        int tag() const { return implementation.MPI_TAG; }
        int error() const { return implementation.MPI_ERROR; }
    };
}
#endif