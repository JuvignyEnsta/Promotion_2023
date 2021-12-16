# include <iostream>
# include <cstdlib>
# include <string>
# include <chrono>
# include <cmath>
# include <vector>
# include <fstream>
#include <cuda_runtime.h>

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

__global__ void iterMandelbrot( int maxIter, float scaleX, float scaleY, int W, int H, int* image )
{
    unsigned int indx = threadIdx.x + blockIdx.x * blockDim.x;
    unsigned int indy = threadIdx.y + blockIdx.y * blockDim.y;

    if ( (indx < W) && (indy < H) )
    {
        unsigned int glob_idx = indx + indy*W;
        float cr = -2.f+indx*scaleX;
        float ci = -1.125f+ indy*scaleY;
        float zr = 0.f;
        float zi = 0.f;

        int niter = 0;
        while ((zr*zr+zi*zi < 4.) && (niter < maxIter))
        {
            float zt = zr*zr-zi*zi + cr;
            zi = 2*zr*zi + ci;
            zr = zt;
            ++niter;
        }
        image[glob_idx] = niter;
    }
}

std::vector<int>
computeMandelbrotSet( int W, int H, int maxIter )
{
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::time_point<std::chrono::system_clock> start2, end2;
    std::vector<int> pixels(W*H);
    float scaleX = 3.f/(W-1.f);
    float scaleY = 2.25f/(H-1.f);
    int* d_pixel;
    start = std::chrono::system_clock::now();
    start2 = std::chrono::system_clock::now();
    cudaMalloc(&d_pixel, W*H*sizeof(int));
    dim3 block{16,16,1};
    dim3 grid {unsigned((W+15)/16),unsigned((H+15)/16),1};
    iterMandelbrot<<<grid,block>>>(maxIter, scaleX, scaleY, W, H, d_pixel);

    cudaMemcpy(pixels.data(), d_pixel, W*H*sizeof(int), cudaMemcpyDeviceToHost);
    end2 = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds2 = end2-start2;
    std::cout << "Temps calcul noyau cuda ensemble mandelbrot : " << elapsed_seconds2.count() 
              << std::endl;

    cudaFree(d_pixel);
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
    int* d_tmp;
    cudaMalloc(&d_tmp, 10*sizeof(int));
 
    // Normalement, pour un bon rendu, il faudrait le nombre d'itérations
    // ci--dessous :
    //const int maxIter = 16777216;
    const int maxIter = 8*65536;
    auto iters = computeMandelbrotSet( W, H, maxIter );
    savePicture("mandelbrot.tga", W, H, iters, maxIter);
    cudaFree(d_tmp);
    return EXIT_SUCCESS;
 }
    
