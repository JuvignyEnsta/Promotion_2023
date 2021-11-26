# include <iostream>
# include <cstdlib>
# include <string>
# include <chrono>
# include <cmath>
#include <tbb/partitioner.h>
# include <vector>
# include <fstream>
# include <tbb/tbb.h>


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

std::vector<int>
computeMandelbrotSet( int W, int H, int maxIter )
{
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::vector<int> pixels(W*H);
    double scaleX = 3./(W-1);
    double scaleY = 2.25/(H-1.);
    constexpr const int numThreads = 6;
    // Contrôle de la "taille" des boucles
    constexpr const int grainSize  = 2;
    // Si on veut limiter le nombre de threads soi-même
    //tbb::task_scheduler_init init(numThreads);

    // Choisir parmi l'un de ces partitioneurs :
    auto partitioner = tbb::simple_partitioner();// Taille  des partitions limitée par grainSize (g/2 <= taille <= g)
    //auto partitioner = tbb::auto_partitioner();// C'est celui pas défaut si on ne précise pas le partitionner dans un blocked_range
    //auto partitioner = tbb::static_partitioner();
    start = std::chrono::system_clock::now();
    tbb::parallel_for( tbb::blocked_range2d<unsigned,unsigned>(0,W,grainSize,0,H,grainSize),
                       [&]( tbb::blocked_range2d<unsigned,unsigned> const& r)
                       {
                        // On parcourt les pixels du sous-block de l'image dans l'espace image :
                        for ( auto i = r.cols().begin(); i < r.cols().end(); ++i ) 
                        {
                            for ( auto j = r.rows().begin(); j < r.rows().end(); ++j ) 
                            {
                                Complex c{-2.+j*scaleX,-1.125+ i*scaleY};
                                pixels[j+W*i] = iterMandelbrot( maxIter, c );
                            }
                        }
                       },
                       partitioner
    );
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "Temps calcul ensemble mandelbrot : " << elapsed_seconds.count() 
              << std::endl;
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
    const int W = 800;
    const int H = 600;
    // Normalement, pour un bon rendu, il faudrait le nombre d'itérations
    // ci--dessous :
    //const int maxIter = 16777216;
    const int maxIter = 8*65536;
    auto iters = computeMandelbrotSet( W, H, maxIter );
    savePicture("mandelbrot.tga", W, H, iters, maxIter);
    return EXIT_SUCCESS;
 }
    
