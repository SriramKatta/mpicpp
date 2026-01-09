#include "mpienv/environment.hpp"

#include "error/exception.hpp"

namespace mpicxx
{
    environment::environment( int &argc, char **&argv)
    {
        int flag;
        handle_error(MPI_Initialized(&flag));
        if (!flag)
        {
            handle_error(MPI_Init(&argc, &argv));
        }
    }
    environment::environment()
    {
        int flag;
        handle_error(MPI_Initialized(&flag));
        if (!flag)
        {
            handle_error(MPI_Init(nullptr, nullptr));
        }
    }

    environment::~environment()
    {
        int flag;
        handle_error(MPI_Finalized(&flag));
        if (!flag)
        {
            handle_error(MPI_Finalize());
        }
    }
} // namespace mpicxx
