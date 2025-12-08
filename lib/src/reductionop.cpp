#include "reductionoperation/reductionop.hpp"
#include "error/exception.hpp"

namespace mpicpp
{
    op &op::operator=(op &&other)
    {
        if (owned)
        {
            handle_error(MPI_Op_free(&implementation));
        }
        implementation = other.implementation;
        owned = other.owned;
        other.implementation = MPI_OP_NULL;
        other.owned = false;
        return *this;
    }

    op::~op()
    {
        if (owned)
        {
            handle_error(MPI_Op_free(&implementation));
        }
    }

    MPI_Op op::get() const { return implementation; }

    op op::sum()
    {
        return op(MPI_SUM, false);
    }

    op op::min()
    {
        return op(MPI_MIN, false);
    }

    op op::max()
    {
        return op(MPI_MAX, false);
    }

    op op::bor()
    {
        return op(MPI_BOR, false);
    }

    op op::create(
        MPI_User_function *user_f,
        int commute)
    {
        MPI_Op implementation;
        handle_error(
            MPI_Op_create(user_f, commute, &implementation));
        return op(implementation);
    }

} // namespace mpicpp
