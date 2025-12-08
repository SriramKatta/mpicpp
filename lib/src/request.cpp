#include <stdexcept>

#include "error/exception.hpp"
#include "handles/request.hpp"

namespace mpicxx
{
    request::request(request const &other)
    {
        if (other.implementation != MPI_REQUEST_NULL)
        {
            throw std::logic_error("tried to copy construct from a non-null mpicxx::request object");
        }
        implementation = other.implementation;
    }

    request &request::operator=(request const &other)
    {
        if (other.implementation != MPI_REQUEST_NULL)
        {
            throw exception("tried to copy assign from a non-null mpicxx::request object");
        }
        wait();
        implementation = other.implementation;
        return *this;
    }

    request &request::operator=(request &&other)
    {
        wait();
        implementation = other.implementation;
        other.implementation = MPI_REQUEST_NULL;
        return *this;
    }

    void request::wait()
    {
        if (implementation != MPI_REQUEST_NULL)
        {
            handle_error(MPI_Wait(&implementation, MPI_STATUS_IGNORE));
        }
    }

    bool request::test()
    {
        int flag = 1;
        if (implementation != MPI_REQUEST_NULL)
        {
            handle_error(MPI_Test(&implementation, &flag, MPI_STATUS_IGNORE));
        }
        return bool(flag);
    }

    void request::wait(status &status_arg)
    {
        if (implementation != MPI_REQUEST_NULL)
        {
            MPI_Status status_implementation;
            handle_error(MPI_Wait(&implementation, &status_implementation));
            status_arg = status(status_implementation);
        }
    }

    bool request::test(status &status_arg)
    {
        int flag = 1;
        if (implementation != MPI_REQUEST_NULL)
        {
            MPI_Status status_implementation;
            handle_error(MPI_Test(&implementation, &flag, &status_implementation));
            status_arg = status(status_implementation);
        }
        return bool(flag);
    }

    request::~request()
    {
        wait();
    }

    void waitall(int count, request *array_of_requests)
    {
        MPI_Request *array_of_implementations = &(array_of_requests->get());
        handle_error(
            MPI_Waitall(
                count,
                array_of_implementations,
                MPI_STATUSES_IGNORE));
    }

}