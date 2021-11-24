# include <iostream>
# include <cstdlib>
# include <mpi.h>
# include <sstream>
# include <fstream>

int main( int nargs, char* argv[] )
{
    MPI_Init(&nargs, &argv);
    int numero_du_processus, nombre_de_processus;

    MPI_Comm_rank(MPI_COMM_WORLD,
                  &numero_du_processus);
    MPI_Comm_size(MPI_COMM_WORLD, 
                  &nombre_de_processus);
    std::cout << "Hello world from " 
              << numero_du_processus << " in "
              << nombre_de_processus << " executed" 
              << std::endl;
    MPI_Finalize();
    return EXIT_SUCCESS;
}
