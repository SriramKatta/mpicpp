#pragma once

#include <mpi.h>

#include <vector>
#include <string>
#include <cstddef>
#include <exception>

namespace mpicpp
{

  
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

    template <>
    class predefined_datatype_helper<bool>
    {
    public:
      static datatype value()
      {
        return datatype::predefined_bool();
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
    request ibcast(VT &buffer, int root) const
    {
      MPI_Request request_implementation;
      handle_error(
          MPI_Ibcast(
              &buffer,
              1,
              predefined_datatype<VT>().get(),
              root,
              implementation,
              &request_implementation));
      return request(request_implementation);
    }

    request ibcast(std::string &buffer, int root) const
    {
      int size;
      if (rank() == root)
      {
        size = buffer.size();
      }
      ibcast(size, root);
      if (rank() != root)
      {
        buffer.resize(size);
      }
      MPI_Request request_implementation;
      handle_error(MPI_Ibcast(buffer.data(), size, MPI_CHAR, root,
                              implementation, &request_implementation));
      return request(request_implementation);
    }

    template <typename VT>
    request ibcast(std::vector<VT> &buffer, int root) const
    {
      MPI_Request request_implementation;
      handle_error(
          MPI_Ibcast(
              buffer.data(),
              buffer.size(),
              predefined_datatype<VT>().get(),
              root,
              implementation,
              &request_implementation));
      return request(request_implementation);
    }

    template <typename VT, size_t N>
    request ibcast(std::array<VT, N> &buffer, int root) const
    {
      MPI_Request request_implementation;
      handle_error(
          MPI_Ibcast(
              buffer.data(),
              buffer.size(),
              predefined_datatype<VT>().get(),
              root,
              implementation,
              &request_implementation));
      return request(request_implementation);
    }

    template <typename VT>
    request iscatterv(std::vector<VT> &send_buffer, std::vector<int> &send_counts, std::vector<int> &displacements, std::vector<VT> &receive_buffer, int root) const
    {
      MPI_Request request_implementation;
      handle_error(
          MPI_Iscatterv(
              send_buffer.data(),
              send_counts.data(),
              displacements.data(),
              predefined_datatype<VT>().get(),
              receive_buffer.data(),
              receive_buffer.size(),
              predefined_datatype<VT>().get(),
              root,
              implementation,
              &request_implementation));
      return request(request_implementation);
    }

    template <typename VT>
    request igather(VT &send_buffer, std::vector<VT> &receive_buffer, int root) const
    {
      MPI_Request request_implementation;
      handle_error(
          MPI_Igather(
              &send_buffer,
              1,
              predefined_datatype<VT>().get(),
              receive_buffer.data(),
              1,
              predefined_datatype<VT>().get(),
              root,
              implementation,
              &request_implementation));
      return request(request_implementation);
    }

    template <typename VT>
    request igatherv(std::vector<VT> &send_buffer, std::vector<VT> &receive_buffer, std::vector<int> &recv_counts, std::vector<int> &recv_disp, int root) const
    {
      MPI_Request request_implementation;
      handle_error(
          MPI_Igatherv(
              send_buffer.data(),
              send_buffer.size(),
              predefined_datatype<VT>().get(),
              receive_buffer.data(),
              recv_counts.data(),
              recv_disp.data(),
              predefined_datatype<VT>().get(),
              root,
              implementation,
              &request_implementation));
      return request(request_implementation);
    }

    template <typename VT>
    void exscan(const VT &sendbuf, std::vector<VT> &recvbuf, op const &op_arg) const
    {
      handle_error(
          MPI_Exscan(
              &sendbuf,
              recvbuf.data(),
              1,
              predefined_datatype<VT>().get(),
              op_arg.get(),
              implementation));
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
