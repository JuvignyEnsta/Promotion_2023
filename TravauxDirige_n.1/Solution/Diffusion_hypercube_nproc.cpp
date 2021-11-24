# include <cstdlib>
# include <sstream>
# include <string>
# include <fstream>
# include <iostream>
# include <iomanip>
# include <cmath>
# include <mpi.h>

int main( int nargs, char* argv[] )
{
	// On initialise le contexte MPI qui va s'occuper :
	//    1. Créer un communicateur global, COMM_WORLD qui permet de gérer
	//       et assurer la cohésion de l'ensemble des processus créés par MPI;
	//    2. d'attribuer à chaque processus un identifiant ( entier ) unique pour
	//       le communicateur COMM_WORLD
	//    3. etc...
	MPI_Init( &nargs, &argv );
	// Pour des raisons de portabilité qui débordent largement du cadre
	// de ce cours, on préfère toujours cloner le communicateur global
	// MPI_COMM_WORLD qui gère l'ensemble des processus lancés par MPI.
	MPI_Comm globComm;
	MPI_Comm_dup(MPI_COMM_WORLD, &globComm);
	// On interroge le communicateur global pour connaître le nombre de processus
	// qui ont été lancés par l'utilisateur :
	int nbp;
	MPI_Comm_size(globComm, &nbp);
	// On interroge le communicateur global pour connaître l'identifiant qui
	// m'a été attribué ( en tant que processus ). Cet identifiant est compris
	// entre 0 et nbp-1 ( nbp étant le nombre de processus qui ont été lancés par
	// l'utilisateur )
	int rank;
	MPI_Comm_rank(globComm, &rank);
	// Création d'un fichier pour ma propre sortie en écriture :
	std::stringstream fileName;
	fileName << "Output" << std::setfill('0') << std::setw(5) << rank << ".txt";
	std::ofstream output( fileName.str().c_str() );

	int jeton;
	if (0 == rank) jeton = 31415; // Seul le proc zéro initialise le jeton pour le diffuser

	const int tag = 101;
	MPI_Status status;
	// Calcul dimension du cube :
	int ndim = 0;
	int nbnodes = nbp;
	while (nbnodes > 1)
	{
		nbnodes /= 2;
		++ ndim;
	}
	// Bon à savoir : (1<<d) correspond à calculer 2^d (2 à la puissance d)
	// On vérifie qu'on avait bien une puissance de deux comme nombre de processus :
	if (nbp - (1<<ndim) > 0)
	{ // Si la différence est non nulle, c'est qu'on n'avait pas une puissance de deux !
		std::cerr << "Attention : le nombre de processus doit etre une puissance de deux !" << std::flush << std::endl;
		MPI_Abort(globComm, -1);
	}

	for ( int d = 0; d < ndim; ++d )
	{
		int nb_nodes_in_hcube_d = (1<<d);
		if (rank < nb_nodes_in_hcube_d)// Les noeuds appartenant à l'hypercube de dimension d diffuse
		{                              // le jeton.
			MPI_Send(&jeton, 1, MPI_INT, rank + nb_nodes_in_hcube_d, tag, globComm);
		}
		else if (rank < 2*nb_nodes_in_hcube_d)// Les noeuds qui se trouvent sur l'hypercube de dimension d+1
		{                                     // mais pas de dimension d recoivent le jeton.
			MPI_Recv(&jeton, 1, MPI_INT, rank - nb_nodes_in_hcube_d, tag, globComm, &status);
		}
	}
	// Remarque: on a bien fait un nombre d'échange égal à la dimension du cube !

    output << "Apres echange, je possede le jeton avec la valeur : " << jeton << std::flush
           << std::endl;
	
	output.close();
	// A la fin du programme, on doit synchroniser une dernière fois tous les processus
	// afin qu'aucun processus ne se termine pendant que d'autres processus continue à
	// tourner. Si on oublie cet instruction, on aura une plantage assuré des processus
	// qui ne seront pas encore terminés.
	MPI_Finalize();
	return EXIT_SUCCESS;
}
