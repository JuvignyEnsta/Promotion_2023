# include <iostream>
# include <cstdlib>
# include <string>
# include <chrono>
# include <cmath>
# include <vector>
# include <fstream>
#include <mpi.h>

/** Une structure complexe est définie pour la bonne raison que la classe
 * complex proposée par g++ est très lente ! Le calcul est bien plus rapide
 * avec la petite structure donnée ci--dessous
 **/
struct Complex
{
    Complex() : real(0.), imag(0.)
    {}
    Complex(double r, double i) : real(r), imag(i)
    {}
    Complex operator + ( const Complex& z )
    {
        return Complex(real + z.real, imag + z.imag );
    }
    Complex operator * ( const Complex& z )
    {
        return Complex(real*z.real-imag*z.imag, real*z.imag+imag*z.real);
    }
    double sqNorm() { return real*real + imag*imag; }
    double real,imag;
};

std::ostream& operator << ( std::ostream& out, const Complex& c )
{
  out << "(" << c.real << "," << c.imag << ")" << std::endl;
  return out;
}

/** Pour un c complexe donné, calcul le nombre d'itérations de mandelbrot
 * nécessaires pour détecter une éventuelle divergence. Si la suite
 * converge, la fonction retourne la valeur maxIter
 **/
int iterMandelbrot( int maxIter, const Complex& c)
{
    Complex z{0.,0.};
    // On vérifie dans un premier temps si le complexe
    // n'appartient pas à une zone de convergence connue :
    // Appartenance aux disques  C0{(0,0),1/4} et C1{(-1,0),1/4}
    if ( c.real*c.real+c.imag*c.imag < 0.0625 )
        return maxIter;
    if ( (c.real+1)*(c.real+1)+c.imag*c.imag < 0.0625 )
        return maxIter;
    // Appartenance à la cardioïde {(1/4,0),1/2(1-cos(theta))}    
    if ((c.real > -0.75) && (c.real < 0.5) ) {
        Complex ct{c.real-0.25,c.imag};
        double ctnrm2 = sqrt(ct.sqNorm());
        if (ctnrm2 < 0.5*(1-ct.real/ctnrm2)) return maxIter;
    }
    int niter = 0;
    while ((z.sqNorm() < 4.) && (niter < maxIter))
    {
        z = z*z + c;
        ++niter;
    }
    return niter;
}

/**
 * On parcourt chaque pixel de l'espace image et on fait correspondre par
 * translation et homothétie une valeur complexe c qui servira pour
 * itérer sur la suite de Mandelbrot. Le nombre d'itérations renvoyé
 * servira pour construire l'image finale.
 
 Sortie : un vecteur de taille W*H avec pour chaque case un nombre d'étape de convergence de 0 à maxIter
 MODIFICATION DE LA FONCTION :
 j'ai supprimé le paramètre W étant donné que maintenant, cette fonction ne prendra plus que des lignes de taille W en argument.
 **/
void 
computeMandelbrotSetRow( int W, int H, int maxIter, int num_ligne, int* pixels)
{
    // Calcul le facteur d'échelle pour rester dans le disque de rayon 2
    // centré en (0,0)
    double scaleX = 3./(W-1);
    double scaleY = 2.25/(H-1.);
    //
    // On parcourt les pixels de l'espace image :
    for ( int j = 0; j < W; ++j ) {
       Complex c{-2.+j*scaleX,-1.125+ num_ligne*scaleY};
       pixels[j] = iterMandelbrot( maxIter, c );
    }
}

std::vector<int>
computeMandelbrotSet( int W, int H, int maxIter )
{
    int rank, size, irow;
    MPI_Status status;
    std::vector<int> pixels(1);
    std::vector<int> row(W);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    
    if (rank == 0)// Si je suis le maître
    {
      // Je realloue W*H pixels pour le maître
      std::vector<int>(W*H).swap(pixels);
      /* Je distribue les size-1 premières lignes sur les autres processus */
      irow = 0;
      for ( int p = 1; p < size; ++p )
      {
        MPI_Send(&irow, 1, MPI_INT, p, 101, MPI_COMM_WORLD);
        irow ++;
      }
      do// Boucle sur les lignes restantes à distribuer
      {
        /* Puis j'attends le résultat d'un esclave avant de lui envoyer une nouvelle ligne à calculer
        */
        MPI_Recv(row.data(), W, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        int jrow, from_who;
        from_who = status.MPI_SOURCE;
        jrow     = status.MPI_TAG;
        MPI_Send(&irow, 1, MPI_INT, from_who, 101, MPI_COMM_WORLD);
        std::copy(row.data(), row.data()+W, pixels.data() + W*(H-jrow-1));
        irow++;
      } while (irow < H);
      // On n'a plus de lignes à distribuer. On reçoit les dernières lignes et on signales aux esclaves
      // qu'ils n'ont plus de travail à effectuer
      irow = -1; // -1 => signal de terminaison
      for ( int p = 1; p < size; ++p )
      {
        MPI_Recv(row.data(), W, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        int jrow, from_who;
        from_who = status.MPI_SOURCE;
        jrow     = status.MPI_TAG;
        MPI_Send(&irow, 1, MPI_INT, from_who, 101, MPI_COMM_WORLD);
        std::copy(row.data(), row.data()+W, pixels.data() + W*(H-jrow-1));
      }
    }
    else
    { // Les esclaves
        do
        {
          MPI_Recv(&irow, 1, MPI_INT, 0, 101, MPI_COMM_WORLD, &status);
          if (irow != -1)
          {
            computeMandelbrotSetRow(W, H, maxIter, irow, row.data());
            // On envoie la ligne résultat avec un tag = la ligne calculée pour que le proc zéro
            // sache à quelle ligne de l'image correspond les données reçues.
            MPI_Send(row.data(), W, MPI_INT, 0, irow, MPI_COMM_WORLD);
          }
        } while (irow != -1);
    }
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "["<<rank<<"] "
              <<"Temps calcul ensemble mandelbrot : " << elapsed_seconds.count()
              << std::flush << std::endl;
    return pixels;
}

/** Construit et sauvegarde l'image finale **/
void savePicture( const std::string& filename, int W, int H, const std::vector<int>& nbIters, int maxIter )
{
    double scaleCol = 1./maxIter;//16777216
    std::ofstream ofs( filename.c_str(), std::ios::out | std::ios::binary );
    ofs << "P6\n"
        << W << " " << H << "\n255\n";
    for ( int i = 0; i < W * H; ++i ) {
        double iter = scaleCol*nbIters[i];
        unsigned char r = (unsigned char)(256 - (unsigned (iter*256.) & 0xFF));
        unsigned char b = (unsigned char)(256 - (unsigned (iter*65536) & 0xFF));
        unsigned char g = (unsigned char)(256 - (unsigned( iter*16777216) & 0xFF));
        ofs << r << g << b;
    }
    ofs.close();
}

int main(int argc, char *argv[] ) 
 { 
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const int W = 800;
    const int H = 600;

    std::chrono::time_point<std::chrono::system_clock> start, end, end2;
    start = std::chrono::system_clock::now();

    // Normalement, pour un bon rendu, il faudrait le nombre d'itérations
    // ci--dessous :
    //const int maxIter = 16777216;
    const int maxIter = 8*65536;
    auto pixels = computeMandelbrotSet( W, H, maxIter );
    // ici iters est l'image locale
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "["<<rank<<"] "
              <<"Temps total : " << elapsed_seconds.count()
              << std::endl;

    if (rank == 0) {
      savePicture("mandelbrot.tga", W, H, pixels, maxIter);
      end2 = std::chrono::system_clock::now();
      elapsed_seconds = end2-end;
      std::cout << "["<<rank<<"] "
                <<"Temps save : " << elapsed_seconds.count()
                << std::endl;
    }    
    MPI_Finalize();
    return EXIT_SUCCESS;
 }
    
