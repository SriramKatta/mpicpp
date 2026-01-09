#include <iostream>
#include "mpicpp.hpp"

int main(int argc, char** argv) {
  auto mpi_env = mpicxx::environment(argc, argv);
  auto comm = mpicxx::comm::world();
  double value = 1.0;
  // MPI_Datatype is deduced because double is a fundamental type
  // ignoring the return value makes this blocking
  comm.iallreduce(&value, 1, mpicxx::op::sum());
  std::cout << "sum of ones is " << value << '\n';
  // MPI_Finalize is called from environment destructor
}
