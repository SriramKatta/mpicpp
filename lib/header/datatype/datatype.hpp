#ifndef MPICPP_HEADER_DATATYPE_DATATYPE_HPP
#define MPICPP_HEADER_DATATYPE_DATATYPE_HPP
#pragma once


#include <mpi.h>

namespace mpicpp
{

    class datatype
    {
        MPI_Datatype implementation;
        bool owned;

    public:
        constexpr datatype(
            MPI_Datatype implementation_in,
            bool owned_in)
            : implementation(implementation_in), owned(owned_in)
        {
        }
        datatype()
            : implementation(MPI_DATATYPE_NULL), owned(false)
        {
        }
        datatype(datatype const &) = delete;
        datatype &operator=(datatype const &) = delete;
        constexpr datatype(datatype &&other)
            : implementation(other.implementation), owned(other.owned)
        {
            other.implementation = MPI_DATATYPE_NULL;
            other.owned = false;
        }
        datatype &operator=(datatype &&other);
        ~datatype();
        constexpr MPI_Datatype get() const { return implementation; }
        static datatype predefined_byte();
        static datatype predefined_char();
        static datatype predefined_unsigned();
        static datatype predefined_unsigned_long();
        static datatype predefined_unsigned_long_long();
        static datatype predefined_int();
        static datatype predefined_long_long_int();
        static datatype predefined_float();
        static datatype predefined_double();
        static datatype predefined_bool();
        static datatype predefined_packed();
    };

} // namespace mpicpp

#endif