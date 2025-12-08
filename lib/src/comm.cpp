#include "error/exception.hpp"
#include "handles/request.hpp"
#include "datatype/datatype.hpp"
#include "communicators/comm.hpp"

namespace mpicxx
{

    comm &comm::operator=(comm &&other)
    {
        if (owned)
        {
            handle_error(MPI_Comm_free(&implementation));
        }
        implementation = other.implementation;
        owned = other.owned;
        other.implementation = MPI_COMM_NULL;
        other.owned = false;
        return *this;
    }

    comm::~comm()
    {
        if (owned)
        {
            handle_error(MPI_Comm_free(&implementation));
        }
    }

    int comm::size() const
    {
        int result_size;
        handle_error(
            MPI_Comm_size(
                implementation,
                &result_size));
        return result_size;
    }

    int comm::rank() const
    {
        int result_rank;
        handle_error(
            MPI_Comm_rank(
                implementation,
                &result_rank));
        return result_rank;
    }

    request comm::iallreduce(
        void const *sendbuf,
        void *recvbuf,
        int count,
        datatype const &datatype_arg,
        op const &op_arg)
    {
        MPI_Request request_implementation;
        handle_error(
            MPI_Iallreduce(
                sendbuf,
                recvbuf,
                count,
                datatype_arg.get(),
                op_arg.get(),
                implementation,
                &request_implementation));
        return request(request_implementation);
    }

    request comm::isend(
        void const *buf,
        int count,
        datatype datatype_arg,
        int dest,
        int tag)
    {
        MPI_Request request_implementation;
        handle_error(
            MPI_Isend(
                buf,
                count,
                datatype_arg.get(),
                dest,
                tag,
                implementation,
                &request_implementation));
        return request(request_implementation);
    }

    request comm::irecv(
        void *buf,
        int count,
        datatype datatype_arg,
        int dest,
        int tag)
    {
        MPI_Request request_implementation;
        handle_error(
            MPI_Irecv(
                buf,
                count,
                datatype_arg.get(),
                dest,
                tag,
                implementation,
                &request_implementation));
        return request(request_implementation);
    }

    comm comm::world()
    {
        return comm(MPI_COMM_WORLD, false);
    }

    comm comm::self()
    {
        return comm(MPI_COMM_SELF, false);
    }

    comm comm::dup() const
    {
        MPI_Comm new_implementation;
        handle_error(
            MPI_Comm_dup(
                implementation,
                &new_implementation));
        return comm(new_implementation, true);
    }

    request comm::ibarrier() const
    {
        MPI_Request request_implementation;
        handle_error(
            MPI_Ibarrier(
                implementation,
                &request_implementation));
        return request(request_implementation);
    }

    comm comm::split(int color, int key) const
    {
        MPI_Comm new_implementation;
        handle_error(
            MPI_Comm_split(
                implementation,
                color,
                key,
                &new_implementation));
        return comm(new_implementation, true);
    }

    comm comm::split_type(int split_type, int key, MPI_Info info) const
    {
        MPI_Comm new_implementation;
        handle_error(
            MPI_Comm_split_type(
                implementation,
                split_type,
                key,
                info,
                &new_implementation));
        return comm(new_implementation, true);
    }

    comm comm::cart_create(
        int ndims,
        int const *dims,
        int const *periods,
        int reorder) const
    {
        MPI_Comm cart_implementation;
        handle_error(
            MPI_Cart_create(
                implementation,
                ndims,
                dims,
                periods,
                reorder,
                &cart_implementation));
        return comm(cart_implementation, true);
    }

    int comm::cartdim_get() const
    {
        int ndims;
        handle_error(
            MPI_Cartdim_get(
                implementation,
                &ndims));
        return ndims;
    }

    void comm::cart_get(
        int maxdims,
        int dims[],
        int periods[],
        int coords[]) const
    {
        handle_error(
            MPI_Cart_get(
                implementation,
                maxdims,
                dims,
                periods,
                coords));
    }

    int comm::cart_rank(int const coords[]) const
    {
        int result_rank;
        handle_error(
            MPI_Cart_rank(
                implementation,
                coords,
                &result_rank));
        return result_rank;
    }

    void comm::cart_coords(int rank, int maxdims, int coords[]) const
    {
        handle_error(
            MPI_Cart_coords(
                implementation,
                rank,
                maxdims,
                coords));
    }
} // namespace mpicxx
