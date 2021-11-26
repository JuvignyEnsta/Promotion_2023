/**
 * @file compute_pi.cpp
 * @author Xavier Juvigny (you@domain.com)
 * @brief Calcule pi à l'aide d'une intégration numérique
 * @version 0.1
 * @date 2021-11-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */
 #include <cmath>
 #include <chrono>
 #include <iostream>
#include <tbb/blocked_range.h>
#include <tbb/parallel_reduce.h>


 double seq_integration(int num_intervals)
 {
     double dx = 1.0 / num_intervals;
     double sum= 0.0;
     for ( int i = 0; i < num_intervals; ++i )
     {
         double x = (i+0.5)*dx;
         double h = std::sqrt(1.-x*x);
         sum += h*dx;
     }
     double pi = 4. * sum;
     return pi;
 }

double par_integration(int num_intervals)
{
    double dx = 1.0 / num_intervals;
    double sum = tbb::parallel_reduce(
        /* Range */ tbb::blocked_range<int>(0,num_intervals),
        /* Identity */ 0.0,
        /*func */
        [=]( const tbb::blocked_range<int>& r, double init) -> double {
            for ( int i = r.begin(); i != r.end(); ++i )
            {
                double x = (i+0.5)*dx;
                double h = std::sqrt(1.-x*x);
                init += h*dx;
            }
            return init;
        },
        /*Reduction operator*/
        [](double x, double y) -> double 
        {
            return x + y;
        }
    );
    double pi = 4 * sum;
    return pi;
}

 int main()
 {
    const int num_intervals = 10'000'000;
    auto start_seq = std::chrono::system_clock::now();
    double pi_seq  = seq_integration(num_intervals);
    auto end_seq   = std::chrono::system_clock::now();

    auto start_par = std::chrono::system_clock::now();
    double pi_par  = par_integration(num_intervals);
    auto end_par   = std::chrono::system_clock::now();

    std::chrono::duration<double> delta_seq = end_seq - start_seq;
    std::chrono::duration<double> delta_par = end_par - start_par;
    std::cout << "Calcul de pi en sequentiel : " << pi_seq << " calcule en " << delta_seq.count() << "s" << std::endl;
    std::cout << "Calcul de pi en parallele  : " << pi_par << " calcule en " << delta_par.count() << "s" << std::endl;
    return EXIT_SUCCESS;
 }