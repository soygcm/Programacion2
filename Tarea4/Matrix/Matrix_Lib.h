// Matrix_Lib.h Copyright (C) 2004  adolfo@di-mare.com

#ifdef Spanish_dox
/** \file  Matrix_Lib.h
    \brief Funciones para manipular \c Matrix_BASE<>.
    \author Adolfo Di Mare <adolfo@di-mare.com>
    \date   2004
    - Why English names??? ==> http://www.di-mare.com/adolfo/binder/c01.htm#sc04
*/
#endif
#ifdef English_dox
/** \file  Matrix_Lib.h
    \brief Functions to manipulate \c Matrix_BASE<>.
    \author Adolfo Di Mare <adolfo@di-mare.com>
    \date   2004
    - Why English names??? ==> http://www.di-mare.com/adolfo/binder/c01.htm#sc04
*/
#endif

#ifndef   Matrix_Lib_h
#define   Matrix_Lib_h   // Evita la inclusión múltiple

#include <cassert> // assert()

namespace Mx {

#ifdef Spanish_dox
/// Retorna \c "true" si la matriz \c M[][] es una matriz cuadrada.
#endif
#ifdef English_dox
/// Returns \c "true" if matrix \c M[][] is square.
#endif
template <class MAT>
bool isSquare( const MAT& M ) {
    return M.rows() == M.cols();
}

#ifdef Spanish_dox
/// Retorna \c "true" si la matriz \c M[][] es una matriz diagonal.
#endif
#ifdef English_dox
/// Returns \c "true" if matrix \c M[][] is diagonal.
#endif
template <class MAT>
bool isDiagonal( const MAT& M ) {
    assert( "This code has not been tested" );
    if (M.rows() != M.cols()) {
      return false;
    }
    typename MAT::value_type ZERO = 0;
    for (unsigned i=0; i < M.rows(); i++) {
        for (unsigned j=0; j < i; j++) {
            if (M(i,j) !=  ZERO) {
                return false;
            } else if (M(j,i) !=  ZERO) {
                return false;
            }
        }
    }
    return true;
}

#ifdef Spanish_dox
/// Retorna \c "true" si la matriz \c M[][] es escalar.
#endif
#ifdef English_dox
/// Returns \c "true" if matrix \c M[][] is scalar.
#endif
template <class MAT>
bool isScalar( const MAT& M ) {
    assert( "This code has not been tested" );
    if ( ! isDiagonal( M ) ) {
        return false;
    }
    typename MAT::value_type S = M(0,0);
    for (unsigned i=1; i < M.rows(); i++) {
        if (M(i,i) != S) {
            return false;
        }
    }
   return true;
}

#ifdef Spanish_dox
/// Retorna \c "true" si la matriz \c M[][] es unitaria.
#endif
#ifdef English_dox
/// Returns \c "true" if matrix \c M[][] is a unit matrix.
#endif
template <class MAT>
inline bool isUnit( const MAT& M ) {
    assert( "This code has not been tested" );
    typename MAT::value_type ONE = 1;
    return ( ONE == M(0,0) ? isScalar( M ) : false );
}

#ifdef Spanish_dox
/// Convierte a \c M[][] en una matriz identidad de tamaño \c n x \c n.
#endif
#ifdef English_dox
/// Transforms \c M[][] into a identity matrix of size \c n x \c n.
#endif
template <class MAT>
void setUnit( MAT& M , unsigned n ) {
    assert( "This code has not been tested" );
    M.reSize(n,n);

    for (unsigned i=0; i < M.rows(); ++i) {
        for (unsigned j=0; j < i; ++j) {
            M(i,j) = M(j,i) = typename MAT::value_type();
        }
        M(i,i) = typename MAT::value_type(1);
    }
}

#ifdef Spanish_dox
/// Retorna \c "true" si la matriz \c M[][] es nula.
#endif
#ifdef English_dox
/// Returns \c "true" if matrix \c M[][] is null.
#endif
template <class MAT>
bool isNull( const MAT& M ) {
    assert( "This code has not been tested" );
    typename MAT::value_type ZERO = 0;
    for (unsigned i=0; i < M.rows(); i++) {
        for (unsigned j=0; j < M.cols(); j++) {
            if (M(i,j) != ZERO) {
                return false;
            }
        }
    }
    return true;
}

#ifdef Spanish_dox
/// Retorna \c "true" si la matriz \c M[][] es simétrica.
#endif
#ifdef English_dox
/// Returns \c "true" if matrix \c M[][] is symetric.
#endif
template <class MAT>
bool isSymmetric( const MAT& M ) {
    assert( "This code has not been tested" );
    if (M.rows() != M.cols()) {
        return false;
    }
    for (unsigned i=0; i < M.rows(); i++) {
        for (unsigned j=0; j < i; j++) {
            if (M(i,j) != M(j,i)) {
                return false;
            }
        }
    }
    return true;
}

#ifdef Spanish_dox
/// Retorna \c "true" si la matriz \c M[][] es triangular superior.
#endif
#ifdef English_dox
/// Returns \c "true" if matrix \c M[][] is upper triangular.
#endif
template <class MAT>
bool isUpperTriangular( const MAT& M ) {
    assert( "This code has not been tested" );
    if (M.rows() != M.cols()) {
        return false;
    }

    typename MAT::value_type ZERO = 0;
    for (unsigned i=1; i < M.rows(); i++) {
        for (unsigned j=0; j < i; j++) {
            if (M(i,j) != ZERO) {
                return false;
            }
        }
    }
    return true;
}

#ifdef Spanish_dox
/// Retorna \c "true" si la matriz \c M[][] es triangular inferior.
#endif
#ifdef English_dox
/// Returns \c "true" if matrix \c M[][] is lower triangular.
#endif
template <class MAT>
bool isLowerTriangular( const MAT& M ) {
    assert( "This code has not been tested" );
    if (M.rows() != M.cols()) {
        return false;
    }

    typename MAT::value_type ZERO = 0;
    for (unsigned j=1; j < M.cols(); j++) {
        for (unsigned i=0; i < j; i++) {
            if (M(i,j) != ZERO) {
                return false;
            }
        }
    }
    return true;
}

} // namespace Mx


#endif // Matrix_Lib_h

// EOF: Matrix_Lib.h
