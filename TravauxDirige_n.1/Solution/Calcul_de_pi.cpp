# include <chrono>
# include <random>
# include <cstdlib>
# include <sstream>
# include <string>
# include <fstream>
# include <iostream>
# include <iomanip>
# include <mpi.h>

// Attention , ne marche qu'en C++ 11 ou supérieur :
double approximate_pi( unsigned long nbSamples ) 
{
    typedef std::chrono::high_resolution_clock myclock;
    myclock::time_point beginning = myclock::now();
    myclock::duration d = beginning.time_since_epoch();
    unsigned seed = d.count();
    std::default_random_engine generator(seed);
    std::uniform_real_distribution <double> distribution ( -1.0 ,1.0);
    unsigned long nbDarts = 0;
    // Throw nbSamples darts in the unit square [-1 :1] x [-1 :1]
    for ( unsigned sample = 0 ; sample < nbSamples ; ++ sample ) {
        double x = distribution(generator);
        double y = distribution(generator);
        // Test if the dart is in the unit disk
        if ( x*x+y*y<=1 ) nbDarts ++;
    }
    // Number of nbDarts throwed in the unit disk
    double ratio = double(nbDarts)/double(nbSamples);
    return 4*ratio;
}

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

    unsigned long nbSamples = 100000;
    if (nargs > 1 ) nbSamples = std::atoi(argv[1]);

    MPI_Request* requests = nullptr;
    double* other_results = nullptr;
    const int tag = 101;
    if (0 == rank)
    {
        // On prépare déjà les réceptions pour le rang 0 :
        requests = new MPI_Request[nbp-1];
        other_results = new double[nbp-1];
        for ( int p = 1; p < nbp; ++p )
            MPI_Irecv(other_results+p - 1, 1, MPI_DOUBLE, p, tag, globComm, requests + p - 1);
    }

    unsigned long nbSamples_loc = nbSamples/nbp;
    // Pour traiter le cas où ce n'est pas divisible, on réparti les samplings non encore affectés
    // sur les premiers processus (nombre de samplings non affectés = reste de la division entière)
    if (nbSamples_loc%nbp < (unsigned long)rank) nbSamples_loc += 1;
    double partial_result =  approximate_pi(nbSamples_loc);
    if (rank > 0)
        MPI_Send(&partial_result, 1, MPI_DOUBLE, 0, tag, globComm);
    else
    {
        MPI_Status* array_of_statuses;
        array_of_statuses = new MPI_Status[nbp-1];
        // Si rang zéro, on attend les résultats des autres processus :
        MPI_Waitall(nbp-1, requests, array_of_statuses);
        // Puis on somme les résultats :
        for ( int i = 0; i < nbp - 1; ++i )
            partial_result = partial_result + other_results[i];
        partial_result = partial_result / nbp;
        delete [] array_of_statuses;
        delete [] other_results;
        delete [] requests;
        std::cout << "Valeur approchee de pi trouvee : " << partial_result << std::flush << std::endl;
    }



	output.close();
	// A la fin du programme, on doit synchroniser une dernière fois tous les processus
	// afin qu'aucun processus ne se termine pendant que d'autres processus continue à
	// tourner. Si on oublie cet instruction, on aura une plantage assuré des processus
	// qui ne seront pas encore terminés.
	MPI_Finalize();
	return EXIT_SUCCESS;
}
