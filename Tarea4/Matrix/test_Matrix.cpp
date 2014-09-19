// test_Matrix.cpp  (C)  2004 adolfo@di-mare.com

/** \file  test_Matrix.cpp
    \brief Este programita sirve para ver cómo funciona la matriz...

    \author Adolfo Di Mare <adolfo@di-mare.com>
    \date   2004

    - Why English names??? ==> http://www.di-mare.com/adolfo/binder/c01.htm#sc04
*/

#include <iostream>  // cout
#include <iomanip>   // setw(4)

#include <iostream>

   #undef USE_Matrix
   #undef USE_Sparse_Matrix
   #undef USE_Matrix_List
   #undef USE_Matrix_Map

/* *** ESCOJA AQUI CUAL IMPLEMENTACION DE LA MATRIZ QUIERE USAR *** */

   #define USE_Matrix        ///< Use la implmentación \c Matrix.h
// #define USE_Sparse_Matrix ///< Use la implmentación \c Sparse_Matrix.h
// #define USE_Matrix_List   ///< Use la implmentación \c Matrix_List.h
// #define USE_Matrix_Map    ///< Use la implmentación \c Matrix_Map.h

#if defined(USE_Matrix)
    #include "Matrix.h"
#elif defined(USE_Sparse_Matrix)
    #include "Sparse_Matrix.h"
    #define Matrix Sparse_Matrix
#elif defined(USE_Matrix_List)
    #include "Matrix_List.h"
    #define Matrix Matrix_List
#elif defined(USE_Matrix_Map)
    #include "Matrix_Map.h"
    #define Matrix Matrix_Map
#endif

using namespace Mx;
using namespace std;

/// Imprime por filas el valor de \c "M"
template <class E>
void print( const char* name, Matrix<E> & V ) {
    cout << endl << name << '[' << V.rows() << ',' << V.cols() << ']' << endl;
    for (unsigned i=0; i < V.rows(); ++i) {
        for (unsigned j=0; j < V.cols(); ++j) {
            cout << " " << setw(6) << V(i,j);
        }
        cout << endl;
    }
}

#include "uUnit.h"
#include "str2list.h"

/// Programa que ejercita las principales funciones de las matrices
int main() {
{{ // test::constructor()
    int M[4][5];       //   [0][1][2][3][4]
    intmatrix<4,5>( M , " |  1  2  3  4  5 | "   // [0]
                        " |  6  7  8  9 10 | "   // [1]
                        " | 11 12 13 14 15 | "   // [2]
                        " | 16 17 18 19 20 | " );// [3]

    assertTrue( M[0][0] == 1 );  assertTrue( M[2][1] == 12 );

    Matrix<int> Mchirri(4,5); // la chirrisquitica
    int k=1;
    for ( size_t i=0; i<4; ++i ) {
        for ( size_t j=0; j<5; ++j ) {
            Mchirri(i,j) = M[i][j];
            assertTrue( Mchirri(i,j)==k ); k++;
        }
    }
}}
{{ // test::rows_cols()
    int M[4][5];       //   [0][1][2][3][4]
    intmatrix<4,5>( M , " |  1  2  3  4  5 | "   // [0]
                        " |  6  7  8  9 10 | "   // [1]
                        " | 11 12 13 14 15 | "   // [2]
                        " | 16 17 18 19 20 | " );// [3]

    Matrix<int> Mchirri(4,5); // la chirrisquitica
    int k=1;
    for ( size_t i=0; i<4; ++i ) {
        for ( size_t j=0; j<5; ++j ) {
            Mchirri(i,j) = M[i][j];
            assertTrue( Mchirri(i,j)==k ); k++;
        }
    }
    assertTrue( Mchirri.rows() == 4 );
    assertTrue( Mchirri.cols() == 5 );
}}
{{ // test::isUpperTriangular()
    int M[4][4];       //   [0][1][2][3][4]
    intmatrix<4,4>( M , " | 1  2  3  4 | "   // [0]
                        " | 0  5  6  7 | "   // [1]
                        " | 0  0  8  9 | "   // [2]
                        " | 0  0  0 10 | " );// [3]

    Matrix<int> Mchirri(4,4); // la chirrisquitica
    for ( size_t i=0; i<4; ++i ) {
        for ( size_t j=0; j<4; ++j ) {
            Mchirri(i,j) = M[i][j];
        }
    }
    assertTrue( isUpperTriangular(Mchirri) );
    Mchirri(3,0) = 12;
    assertTrue( !isUpperTriangular(Mchirri) );
}}
{{ // test::isLowerTriangular()
    int M[4][4];       //   [0][1][2][3][4]
    intmatrix<4,4>( M , " | 1  0  0  0 | "   // [0]
                        " | 2  3  0  0 | "   // [1]
                        " | 4  5  6  0 | "   // [2]
                        " | 7  8  9 10 | " );// [3]

    Matrix<int> Mchirri(4,4); // la chirrisquitica

    for ( size_t i=0; i<4; ++i ) {
        for ( size_t j=0; j<4; ++j ) {
            Mchirri(i,j) = M[i][j];
        }
    }
    assertTrue( isLowerTriangular(Mchirri) );
    Mchirri(0,3) = 12;
    assertTrue( !isLowerTriangular(Mchirri) );
}}

{{ // test::size()
    int M[4][5];       //   [0][1][2][3][4]
    intmatrix<4,5>( M , " |  1  2  3  4  5 | "   // [0]
                        " |  6  7  8  9 10 | "   // [1]
                        " | 11 12 13 14 15 | "   // [2]
                        " | 16 17 18 19 20 | " );// [3]

    Matrix<int> Mchirri(4,5); // la chirrisquitica
    for ( size_t i=0; i<4; ++i ) {
        for ( size_t j=0; j<5; ++j ) {
            Mchirri(i,j) = M[i][j];
        }
    }

    assertTrue( Mchirri.size() == 20 );
    assertTrue( Mchirri.count() == 20 );
    assertTrue( Mchirri.capacity() == 20 );
}}
{{ // test::copy()
    int M[4][5];       //   [0][1][2][3][4]
    intmatrix<4,5>( M , " |  1  2  3  4  5 | "   // [0]
                        " |  6  7  8  9 10 | "   // [1]
                        " | 11 12 13 14 15 | "   // [2]
                        " | 16 17 18 19 20 | " );// [3]

    Matrix<int> Mchirri(4,5); // la chirrisquitica
    for ( size_t i=0; i<4; ++i ) {
        for ( size_t j=0; j<5; ++j ) {
            Mchirri(i,j) = M[i][j];
        }
    }

    int N[4][5];       //   [0][1][2][3][4]
    intmatrix<4,5>( N , " | 13  2  3  4  5 | "   // [0]
                        " |  6  7  8  9 10 | "   // [1]
                        " | 11 12 13 14 15 | "   // [2]
                        " | 16 17 18 19 25 | " );// [3]

    Matrix<int> Nchirri(4,5); // la chirrisquitica
    for ( size_t i=0; i<4; ++i ) {
        for ( size_t j=0; j<5; ++j ) {
            Nchirri(i,j) = N[i][j];
        }
    }

    Matrix<int> Achi(4,4);
    for ( size_t i=0; i<4; ++i ) {
        for ( size_t j=0; j<4; ++j ) {
            Achi(i,j) = i*j;
        }
    }

    Matrix<int> Bchi;

    assertTrue( Nchirri(0,0) == 13 );
    Bchi.copy(Nchirri);
    Nchirri = Mchirri;
    assertTrue( Nchirri(0,0) == 1 );
    assertTrue( Nchirri.size() == 20 );
    Nchirri = Achi;
    assertTrue( Nchirri.size() == 16 );
    assertTrue( Nchirri(3,2) == 6 );
    assertTrue( Bchi.size() == 20 );
}}
{{ //test::swap_move()
    int N[4][5];       //   [0][1][2][3][4]
    intmatrix<4,5>( N , " | 13  2  3  4  5 | "   // [0]
                        " |  6  7  8  9 10 | "   // [1]
                        " | 11 12 13 14 15 | "   // [2]
                        " | 16 17 18 19 25 | " );// [3]

    Matrix<int> Nchirri(4,5); // la chirrisquitica
    for ( size_t i=0; i<4; ++i ) {
        for ( size_t j=0; j<5; ++j ) {
            Nchirri(i,j) = N[i][j];
        }
    }

    Matrix<int> Achi(4,4);
    for ( size_t i=0; i<4; ++i ) {
        for ( size_t j=0; j<4; ++j ) {
            Achi(i,j) = i*j;
        }
    }

    Matrix<int> Bchi;

    assertTrue( Nchirri(0,0) == 13 );
    Bchi.move(Nchirri);
    assertTrue( Bchi(0,0) == 13 );
    assertTrue( Bchi.size() == 20 );
    assertTrue( Nchirri.size() == 0 );
    assertTrue( Achi.size() == 16 );
    Bchi.swap(Achi);
    assertTrue( Achi.size() == 20 );
    assertTrue( Bchi.size() == 16 );
}}

{{     //test::operator==
    Matrix<int> E(4,5);
    Matrix<int> F(4,5);
    assertTrue(E==F);
}}

{{    //test::operator!=
    Matrix<int> G(4,5);
    Matrix<int> H(4,5);
    assertTrue(G!=H);

}}
    std::cout << "==============\n";

    const unsigned M = 5;
    const unsigned N = 8;
    unsigned i,j,k;
    char * above = new char [10];
    typedef Matrix<unsigned> Matrix_unsigned;
    Matrix<unsigned> V(M,N);
    char * below = new char [10];

    k = 0;
    for (i=0; i < V.rows(); ++i) {
        for (j=0; j < V.cols(); ++j) {
            V(i,j) = k++;        // V.operator() (i,j) = k++;
        }
    }

    Matrix<unsigned> A = V;
    assertTrue( A == V );
    assertTrue( A.same(A) );
    print("A", A);

    V.reSize(N,M);
    print("V", V);

    // k = 0;
    assertTrue( k == A.rows()*A.cols() );
    for (i=0; i < V.rows(); ++i) {
        for (j=0; j < V.cols(); ++j) {
            V(i,j) = k++;
        }
    }

    Matrix<unsigned> B;
    B = V;
    assertTrue( B == V );
    print("B", B);
    Matrix_unsigned C = V;
{{    //test::operator+
    Matrix_unsigned C = V;
    C = C + C;
    print("C", C);
}}

{{    //test::operator-
    C = A - A;
    print("C", C);
    assertTrue( isNull( C ) && ! isScalar( C ) );
    assertTrue( ! isLowerTriangular( C ) && ! isUpperTriangular( C ) );
    assertTrue( ! isSquare( C ) &&  ! isUnit( C ) );
    assertTrue( ! isSymmetric( C ) );
}}

{{    //test::operator*
    print("A", A);
//  B.transpose();
    print("B", B);
    C = B * A;
    print("C", C);
    assertTrue( ! isNull( C ) && isSquare( C ) );
    assertTrue( ! isLowerTriangular( C ) && ! isUpperTriangular( C ) );
    assertTrue( ! isUnit( C ) && ! isScalar( C ) );
    assertTrue( ! isSymmetric( C ) );

    print("A", A);
    print("B", B);
    C = A * B;
    print("C", C);
    assertTrue( ! isNull( C ) && isSquare( C ) );
    assertTrue( ! isLowerTriangular( C ) && ! isUpperTriangular( C ) );
    assertTrue( ! isUnit( C ) && ! isScalar( C ) );
    assertTrue( ! isSymmetric( C ) );
}}

{{ //test::operator()
    cout<<"A (0,2)"<<A(0,2)<<endl;
    Matrix<unsigned> D = A+A;
    assertTrue( ! isNull( D ) && !isSquare( D ) );
    assertTrue( ! isLowerTriangular( D ) && ! isUpperTriangular( D ) );
    assertTrue( ! isUnit( D ) && ! isScalar( D ) );
    assertTrue( ! isSymmetric( D ) );
    cout<<"D(0,2)"<<D(0,2)<<endl;
    D(0,2)=23;
    cout<<"D(0,2)=23 =>"<<D(0,2)<<endl;
    cout<<endl;
}}

{{     //test::at(m,n)
    assertTrue(A.at(3,2)==A(3,2));

}}

{{     //test::resize()
    cout<<"Matriz A (7x8)";
    print("A",A);


    A.reSize(2,3);

    assertTrue( ! isNull( A ) && !isSquare( A ) );
    assertTrue( ! isLowerTriangular( A ) && ! isUpperTriangular( A ) );
    assertTrue( ! isSymmetric( A ) );
    cout<<"Matriz A (2x3)";
    print("A",A);
}}

{{     //test::reShape()
    A.reShape(2,3);
    if (2*3==A.rows()*A.cols())
        assertTrue(2*3==A.rows()*A.cols());
    else
        assertTrue(!2*3==A.rows()*A.cols());
    print("A",A);
}}

{{     //test::transpose()

    assertTrue( A.cols() == 3 );
    A.transpose();
    assertTrue( A.cols() == 2 );

}}


    return 0;
}

// EOF: test_Matrix.cpp
