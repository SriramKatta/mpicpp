#ifndef MPICPP_HEADER_REDUCTIONOPERATION_REDUCTIONOP_HPP
#define MPICPP_HEADER_REDUCTIONOPERATION_REDUCTIONOP_HPP
#pragma once

#include <mpi.h>

namespace mpicxx
{
    class op
  {
    MPI_Op implementation;
    bool owned;

  public:
    constexpr op(MPI_Op implementation_in,
                 bool owned_in = true)
        : implementation(implementation_in), owned(owned_in)
    {
    }
    op()
        : implementation(MPI_OP_NULL), owned(false)
    {
    }
    op(op const &) = delete;
    op &operator=(op const &) = delete;
    constexpr op(op &&other)
        : implementation(other.implementation), owned(other.owned)
    {
      other.implementation = MPI_OP_NULL;
      other.owned = false;
    }
    op &operator=(op &&other);
    ~op();
    MPI_Op get() const;
    static op sum();
    static op min();
    static op max();
    static op bor();
    static op create(
        MPI_User_function *user_f,
        int commute = 1);
  };

} // namespace mpicxx


#endif
