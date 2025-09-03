#pragma once

#include <mpi.h>

#include <vector>
#include <string>
#include <cstddef>
#include <exception>

namespace mpicpp
{

  class exception : public std::exception
  {
    std::string error_string;

  public:
    exception(int errorcode);
    exception(const char *msg);
    const char *what() const noexcept override;
  };

  void handle_error(int errorcode);

  class status
  {
    MPI_Status implementation;

  public:
    status(MPI_Status implementation_arg);
    status() = default;
    int source() const { return implementation.MPI_SOURCE; }
    int tag() const { return implementation.MPI_TAG; }
    int error() const { return implementation.MPI_ERROR; }
  };

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
    static datatype predefined_packed();
  };

  namespace details
  {

    template <class T>
    class predefined_datatype_helper;

    template <>
    class predefined_datatype_helper<std::byte>
    {
    public:
      static datatype value()
      {
        return datatype::predefined_byte();
      }
    };

    template <>
    class predefined_datatype_helper<char>
    {
    public:
      static datatype value()
      {
        return datatype::predefined_char();
      }
    };

    template <>
    class predefined_datatype_helper<unsigned>
    {
    public:
      static datatype value()
      {
        return datatype::predefined_unsigned();
      }
    };

    template <>
    class predefined_datatype_helper<unsigned long>
    {
    public:
      static datatype value()
      {
        return datatype::predefined_unsigned_long();
      }
    };

    template <>
    class predefined_datatype_helper<unsigned long long>
    {
    public:
      static datatype value()
      {
        return datatype::predefined_unsigned_long_long();
      }
    };

    template <>
    class predefined_datatype_helper<int>
    {
    public:
      static datatype value()
      {
        return datatype::predefined_int();
      }
    };

    template <>
    class predefined_datatype_helper<long long int>
    {
    public:
      static datatype value()
      {
        return datatype::predefined_long_long_int();
      }
    };

    template <>
    class predefined_datatype_helper<float>
    {
    public:
      static datatype value()
      {
        return datatype::predefined_float();
      }
    };

    template <>
    class predefined_datatype_helper<double>
    {
    public:
      static datatype value()
      {
        return datatype::predefined_double();
      }
    };

  }

  template <class T>
  datatype predefined_datatype()
  {
    return details::predefined_datatype_helper<T>::value();
  }

  class comm
  {
    MPI_Comm implementation;
    bool owned;

  public:
    constexpr comm(
        MPI_Comm implementation_arg,
        bool owned_arg)
        : implementation(implementation_arg), owned(owned_arg)
    {
    }
    comm()
        : implementation(MPI_COMM_NULL), owned(false)
    {
    }
    comm(comm const &) = delete;
    comm &operator=(comm const &) = delete;
    constexpr comm(comm &&other)
        : implementation(other.implementation), owned(other.owned)
    {
      other.implementation = MPI_COMM_NULL;
      other.owned = false;
    }
    comm &operator=(comm &&other);
    ~comm();
    int size() const;
    int rank() const;
    request iallreduce(
        void const *sendbuf,
        void *recvbuf,
        int count,
        datatype const &datatype_arg,
        op const &op_arg);
    template <class T>
    request iallreduce(
        T const *sendbuf,
        T *recvbuf,
        int count,
        op const &op_arg)
    {
      datatype datatype_arg = predefined_datatype<T>();
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
    template <class T>
    request iallreduce(
        T *buf,
        int count,
        op const &op_arg)
    {
      datatype datatype_arg = predefined_datatype<T>();
      MPI_Request request_implementation;
      handle_error(
          MPI_Iallreduce(
              MPI_IN_PLACE,
              buf,
              count,
              datatype_arg.get(),
              op_arg.get(),
              implementation,
              &request_implementation));
      return request(request_implementation);
    }
    request isend(
        void const *buf,
        int count,
        datatype datatype_arg,
        int dest,
        int tag);
    template <class T>
    request isend(
        T const *buf,
        int count,
        int dest,
        int tag)
    {
      datatype datatype_arg = predefined_datatype<T>();
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
    request irecv(
        void *buf,
        int count,
        datatype datatype_arg,
        int dest,
        int tag);
    template <class T>
    request irecv(
        T *buf,
        int count,
        int dest,
        int tag)
    {
      datatype datatype_arg = predefined_datatype<T>();
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

    template <typename VT>
    request ibcast(const std::vector<VT> &data, int root) const
    {
      int size = data.size();
      handle_error(
          MPI_Bcast(&size, 1, MPI_INT, root, implementation));
      if (rank() != root)
      {
        data.resize(size);
      }
      MPI_Request request_implementation;
      handle_error(
          MPI_Ibcast(
              data.data(),
              data.size(),
              predefined_datatype<VT>().get(),
              root,
              implementation,
              &request_implementation));
      return request(request_implementation);
    }

    static comm world();
    static comm self();
    comm dup() const;
    request ibarrier() const;
    comm split(int color, int key) const;
    comm split_type(int split_type, int key, MPI_Info info = MPI_INFO_NULL) const;
    comm cart_create(
        int ndims,
        int const *dims,
        int const *periods,
        int reorder) const;
    int cartdim_get() const;
    void cart_get(
        int maxdims,
        int dims[],
        int periods[],
        int coords[]) const;
    int cart_rank(int const coords[]) const;
    void cart_coords(int rank, int maxdims, int coords[]) const;
    constexpr MPI_Comm get() const { return implementation; }
  };

  class [[nodiscard]] environment
  {
  public:
    environment(int *argc, char ***argv);
    environment()
        : environment(nullptr, nullptr)
    {
    }
    ~environment();
    environment(environment const &) = delete;
    environment &operator=(environment const &) = delete;
    environment(environment &&) = delete;
    environment &operator=(environment &&) = delete;
  };

}
