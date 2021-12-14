// Produit matrice-vecteur
# include <cassert>
# include <vector>
# include <iostream>
# include <mpi.h>

// ---------------------------------------------------------------------
class Matrix : public std::vector<double>
{
public:
    Matrix (int dim, int rank, int nbp);
    Matrix( int nrows, int ncols, int rank, int nbp );
    Matrix( const Matrix& A ) = delete;
    Matrix( Matrix&& A ) = default;
    ~Matrix() = default;

    Matrix& operator = ( const Matrix& A ) = delete;
    Matrix& operator = ( Matrix&& A ) = default;
    
    double& operator () ( int i, int j ) {
        return m_arr_coefs[i + j*m_nrows];
    }
    double  operator () ( int i, int j ) const {
        return m_arr_coefs[i + j*m_nrows];
    }
    
    std::vector<double> operator * ( const std::vector<double>& u ) const;
    
    std::ostream& print( std::ostream& out ) const
    {
        const Matrix& A = *this;
        out << "[\n";
        for ( int i = 0; i < m_nrows; ++i ) {
            out << " [ ";
            for ( int j = 0; j < m_ncols; ++j ) {
                out << A(i,j) << " ";
            }
            out << " ]\n";
        }
        out << "]";
        return out;
    }
private:
    int m_nrows, m_ncols, m_rank, m_nbp;
    std::vector<double> m_arr_coefs;
};
// ---------------------------------------------------------------------
inline std::ostream& 
operator << ( std::ostream& out, const Matrix& A )
{
    return A.print(out);
}
// ---------------------------------------------------------------------
inline std::ostream&
operator << ( std::ostream& out, const std::vector<double>& u )
{
    out << "[ ";
    for ( const auto& x : u )
        out << x << " ";
    out << " ]";
    return out;
}
// ---------------------------------------------------------------------
std::vector<double> 
Matrix::operator * ( const std::vector<double>& u ) const
{
    const Matrix& A = *this;
    assert( u.size() == unsigned(m_ncols) );
    std::vector<double> v_loc(m_nrows, 0.);
    std::vector<double> v(m_nrows * m_nbp);
    for ( int i = 0; i < m_nrows; ++i ) {
        for ( int j = 0; j < m_ncols; ++j ) {
            v_loc[i] += A(i,j)*u[j];
        }            
    }
    MPI_Allgather(v_loc.data(), v_loc.size(), MPI_DOUBLE, v.data(), v_loc.size(), MPI_DOUBLE, MPI_COMM_WORLD);
    return v;
}

// =====================================================================
Matrix::Matrix (int dim, int rank, int nbp) 
    :   m_nrows(dim/nbp), 
        m_ncols(dim),
        m_rank(rank),
        m_nbp(nbp),
        m_arr_coefs(dim*dim/nbp)
{
    int starti = m_nrows * m_rank;
    for ( int i = 0; i < m_nrows; ++ i ) {
        for ( int j = 0; j < dim; ++j ) {
            int iglob = i + starti;
            (*this)(i,j) = (iglob+j)%dim;
        }
    }
}
// ---------------------------------------------------------------------
Matrix::Matrix( int nrows, int ncols, int rank, int nbp ) 
    :   m_nrows(nrows/nbp), 
        m_ncols(ncols),
        m_rank(rank),
        m_nbp(nbp),
        m_arr_coefs(nrows*ncols/nbp)
{
    int starti = m_nrows * m_rank;
    int dim = (nrows > ncols ? nrows : ncols );
    for ( int i = 0; i < m_nrows; ++ i ) {
        int iglob = i + starti;
        for ( int j = 0; j < ncols; ++j ) {
            {
                (*this)(i,j) = (iglob+j)%dim;
            }
        }
    }    
}
// =====================================================================
int main( int nargs, char* argv[] )
{
    const int N = 12;
    int rank, nbp;
    MPI_Init(&nargs, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nbp);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    Matrix A(N, rank, nbp);
    std::cout  << "A[" << rank << "] : " << A << std::endl;
    std::vector<double> u( N );
    for ( int i = 0; i < N; ++i ) u[i] = i+1;
    std::cout << " u[" << rank << "] : " << u << std::endl;
    std::vector<double> v = A*u;
    std::cout << "A.u[" << rank << "] = " << v << std::endl;
    MPI_Finalize();
    return EXIT_SUCCESS;
}
