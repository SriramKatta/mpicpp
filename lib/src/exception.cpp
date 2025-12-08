#include <mpi.h>

#include "error/exception.hpp"

namespace mpicxx
{
    exception::exception(int errorcode)
    {
        char c_error_string[MPI_MAX_ERROR_STRING];
        int resultlen;
        MPI_Error_string(errorcode, c_error_string, &resultlen);
        c_error_string[resultlen] = '\0';
        error_string = c_error_string;
    }

    exception::exception(const char *msg)
    {
        error_string = msg;
    }

    const char *exception:: what() const noexcept
    {
        return error_string.c_str();
    }

    void handle_error(int errorcode)
    {
        if (errorcode == MPI_SUCCESS)
            return;
        throw exception(errorcode);
    }
}
