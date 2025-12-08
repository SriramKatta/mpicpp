#include "datatype/datatype.hpp"

#include "error/exception.hpp"

namespace mpicxx
{
    datatype &datatype::operator=(datatype &&other)
    {
        if (owned)
        {
            handle_error(MPI_Type_free(&implementation));
        }
        implementation = other.implementation;
        owned = other.owned;
        other.implementation = MPI_DATATYPE_NULL;
        other.owned = false;
        return *this;
    }

    datatype::~datatype()
    {
        if (owned)
        {
            handle_error(MPI_Type_free(&implementation));
        }
    }

    datatype datatype::predefined_byte()
    {
        return datatype(MPI_BYTE, false);
    }

    datatype datatype::predefined_char()
    {
        return datatype(MPI_CHAR, false);
    }

    datatype datatype::predefined_unsigned()
    {
        return datatype(MPI_UNSIGNED, false);
    }

    datatype datatype::predefined_unsigned_long()
    {
        return datatype(MPI_UNSIGNED_LONG, false);
    }

    datatype datatype::predefined_unsigned_long_long()
    {
        return datatype(MPI_UNSIGNED_LONG_LONG, false);
    }

    datatype datatype::predefined_int()
    {
        return datatype(MPI_INT, false);
    }

    datatype datatype::predefined_long_long_int()
    {
        return datatype(MPI_LONG_LONG_INT, false);
    }

    datatype datatype::predefined_float()
    {
        return datatype(MPI_FLOAT, false);
    }

    datatype datatype::predefined_double()
    {
        return datatype(MPI_DOUBLE, false);
    }

    datatype datatype::predefined_bool()
    {
        return datatype(MPI_C_BOOL, false);
    }

    datatype datatype::predefined_packed()
    {
        return datatype(MPI_PACKED, false);
    }
}