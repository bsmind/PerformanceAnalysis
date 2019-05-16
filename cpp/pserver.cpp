#ifdef _USE_MPINET
#include "chimbuko/net/mpi_net.hpp"
#include <mpi.h>
#else
#include "chimbuko/net/zmq_net.hpp"
#endif

#include "chimbuko/param/sstd_param.hpp"

int main (int argc, char ** argv)
{
    chimbuko::SstdParam param;
    int nt = 1;
#ifdef _USE_MPINET
    int provided;
    chimbuko::MPINet net;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
#else
    chimbuko::ZMQNet net;
#endif

    nt = std::thread::hardware_concurrency();
    std::cout << "Run parameter server with " << nt << " threads" << std::endl;

    // Note: for some reasons, internal MPI initialization cause segmentation error!! 
    net.init(nullptr, nullptr, nt);
    net.set_parameter(&param);
    net.run();

#ifdef _USE_MPINET
    MPI_Finalize();
#endif
    return 0;
}
