#include "mpicpp.hpp"

namespace mpicpp {

exception::exception(int errorcode)
{
  char c_error_string[MPI_MAX_ERROR_STRING];
  int resultlen;
  MPI_Error_string(errorcode, c_error_string, &resultlen);
  c_error_string[resultlen] = '\0';
  error_string = c_error_string;
}

exception::exception(const char* msg)
{
  error_string = msg;
}

const char* exception::what() const noexcept {
  return error_string.c_str();
}

void handle_error(int errorcode) {
  if (errorcode == MPI_SUCCESS) return;
  throw exception(errorcode);
}

status::status(MPI_Status implementation_arg)
  :implementation(implementation_arg)
{}

request::request(request const& other)
{
  if (other.implementation != MPI_REQUEST_NULL) {
    throw std::logic_error("tried to copy construct from a non-null mpicpp::request object");
  }
  implementation = other.implementation;
}

request& request::operator=(request const& other)
{
  if (other.implementation != MPI_REQUEST_NULL) {
    throw exception("tried to copy assign from a non-null mpicpp::request object");
  }
  wait();
  implementation = other.implementation;
  return *this;
}

request& request::operator=(request&& other)
{
  wait();
  implementation = other.implementation;
  other.implementation = MPI_REQUEST_NULL;
  return *this;
}

void request::wait()
{
  if (implementation != MPI_REQUEST_NULL) {
    handle_error(MPI_Wait(&implementation, MPI_STATUS_IGNORE));
  }
}

bool request::test()
{
  int flag = 1;
  if (implementation != MPI_REQUEST_NULL) {
    handle_error(MPI_Test(&implementation, &flag, MPI_STATUS_IGNORE));
  }
  return bool(flag);
}

void request::wait(status& status_arg)
{
  if (implementation != MPI_REQUEST_NULL) {
    MPI_Status status_implementation;
    handle_error(MPI_Wait(&implementation, &status_implementation));
    status_arg = status(status_implementation);
  }
}

bool request::test(status& status_arg)
{
  int flag = 1;
  if (implementation != MPI_REQUEST_NULL) {
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

void waitall(int count, request* array_of_requests)
{
  MPI_Request* array_of_implementations = &(array_of_requests->get());
  handle_error(
      MPI_Waitall(
        count,
        array_of_implementations,
        MPI_STATUSES_IGNORE));
}

op& op::operator=(op&& other)
{
  if (owned) {
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
  if (owned) {
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
    MPI_User_function* user_f,
    int commute) {
  MPI_Op implementation;
  handle_error(
      MPI_Op_create(user_f, commute, &implementation));
  return op(implementation);
}

datatype& datatype::operator=(datatype&& other)
{
  if (owned) {
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
  if (owned) {
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

datatype datatype::predefined_packed()
{
  return datatype(MPI_PACKED, false);
}

comm& comm::operator=(comm&& other) {
  if (owned) {
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
  if (owned) {
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
    void const* sendbuf,
    void* recvbuf,
    int count,
    datatype const& datatype_arg,
    op const& op_arg)
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
    void const* buf,
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
    void* buf,
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

comm comm::cart_create(
    int ndims,
    int const* dims,
    int const* periods,
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

environment::environment(int* argc, char*** argv) {
  int flag;
  handle_error(MPI_Initialized(&flag));
  if (!flag) {
    handle_error(MPI_Init(argc, argv));
  }
}

environment::~environment() {
  int flag;
  handle_error(MPI_Finalized(&flag));
  if (!flag) {
    handle_error(MPI_Finalize());
  }
}

}
