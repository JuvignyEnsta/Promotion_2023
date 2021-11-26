#include <algorithm>
#include <cassert>
#include <iostream>
#include <tbb/blocked_range2d.h>
#include <tbb/parallel_for.h>
#include <thread>
#include <tbb/tbb.h>
#include "ProdMatMat.hpp"

namespace
{
    void prodSubBlocks( int iRowBlkA, int iColBlkB, int iColBlkA, int szBlock,
                        const Matrix &A, const Matrix &B, Matrix &C )
    {
        for ( int j = iColBlkB; j < std::min( B.nbCols, iColBlkB + szBlock ); j++ )
            for ( int k = iColBlkA; k < std::min( A.nbCols, iColBlkA + szBlock ); k++ )
                for ( int i = iRowBlkA; i < std::min( A.nbRows, iRowBlkA + szBlock ); ++i )
                    C( i, j ) += A( i, k ) * B( k, j );
    }
    const int szBlock = 128;
}  // namespace

Matrix operator*( const Matrix &A, const Matrix &B )
{
    Matrix C( A.nbRows, B.nbCols, 0.0 );
    tbb::parallel_for( tbb::blocked_range2d<int,int>(0,B.nbCols, szBlock, 0, A.nbRows, szBlock),
    [&](const tbb::blocked_range2d<int,int>& r) {
        for ( int jBlk = r.rows().begin(); jBlk < r.rows().end(); jBlk += szBlock )
        {
            for (int iBlk = r.cols().begin(); iBlk < r.cols().end(); iBlk += szBlock )
            {
                for ( int kBlk = 0; kBlk < A.nbCols; kBlk += szBlock)
                    prodSubBlocks(iBlk, jBlk, kBlk, szBlock, A, B, C);
            }
        }
    });
    /*tbb::parallel_for(0, B.nbCols, szBlock, [&](int jBlk){
        for ( int kBlk = 0; kBlk < A.nbCols; kBlk += szBlock)
            for ( int iBlk = 0; iBlk < A.nbRows; iBlk += szBlock )
                prodSubBlocks(iBlk, jBlk, kBlk, szBlock, A, B, C);
    
    });*/
    return C;
}
