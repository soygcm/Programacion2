// Matrix.h Copyright (C) 2004  adolfo@di-mare.com

/** \file  Matrix.h
    \brief Declaraciones y definiciones para la clase \c Matrix.

    \author Adolfo Di Mare <adolfo@di-mare.com>
    \date   2004

    - Why English names??? ==> http://www.di-mare.com/adolfo/binder/c01.htm#sc04
*/

#ifndef   Matrix_h
#define   Matrix_h

/// Macro definida para evitar que \c assert() cancele la ejecución.
/// Tira \c std::out_of_range si \c assert(X) falla.
#ifdef ASSERT_NO_ABORT
    #include <stdexcept>  // std::out_of_range
    #include <string>
    #include <iostream>   // cout

    #ifndef NDEBUG
        #ifdef assert
            #undef assert
        #endif
        #define assert(X) do { \
            std::string line, msg = "Assertion failed: " #X ", file " __FILE__ ;  \
            unsigned long n = __LINE__; \
            if ( n==0 ) { line = "0"; } \
            else do {                   \
                char ch = (n%10) + '0'; \
                line = ch + line;       \
                n = n/10;               \
            } while ( n!=0 );           \
            msg += ", line " + line + "\n"; \
            if ( !(X) ) { throw std::out_of_range( msg ); }   \
        } while (false)
    #endif
#else
    #include <cassert> // assert()
#endif

/// Definido por la biblioteca C++ estándar
namespace std {} // Está acá para que Doxygen lo documente

/// Matriz chirrisquitica de adolfo@di-mare.com
namespace Mx {

/** Esta es una clase matriz muy chirrisquitica que puede cambiar dinámicamente de tamaño.
    - La matriz tiene tamaño \c rows() x \c cols()
    - Se le puede cambiar el tamaño dinámicamente con el método \c reSize().
    - La clase \c E debe incluir un neutro para la adición, cuyo valor debe poderse
      obtener invocando el convertidor \c Sparse_Matrix<E>::value_type().
    - Las operaciones aritméticas "+" "-" "*" deben estar definidas para
      \c Matrix<E>::value_type y debe existir el valor \c Matrix<E>::value_type() y también
      \c Matrix<E>::value_type(1) (para matrices unitarias)
    - Los valores de la matriz pueden estar almacenados por filas o por columnas,
      según quede implementado el método \c Matrix<E>::operator(unsigned, unsigned)

    \see http://www.oonumerics.org/oon/
*/
template <class E>
class Matrix {
public:
    /// Tipo del objeto almacenado, similar al nombre usado en STL
    typedef E value_type;
    /// Tipo del objeto almacenado, similar al nombre usado en STL
    typedef value_type& reference;
    /// Tipo del objeto almacenado, similar al nombre usado en STL
    typedef const value_type& const_reference;
    /// Tipo del tamaño de un objeto, similar al nombre usado en STL
    typedef unsigned size_type;
public:
    Matrix(unsigned m = 1, unsigned n = 1);
    Matrix(const Matrix& o); ///< Constructor de copia
    /// Matriz escalar de valor \c V.
    Matrix(const value_type V) : m_rows(0), m_cols(0), m_val(0) { reSize(1,1); (*this)(0,0) = V; }
    ~Matrix();
public:
    unsigned rows() const { return m_rows; } ///< Cantidad de filas de la matriz
    unsigned cols() const { return m_cols; } ///< Cantidad de columnas de la Matriz
    /** Cantidad de valores almacenados en la matriz

    \dontinclude test_Matrix.cpp
    \skipline    test::size()
    \until       }}

    */
    unsigned size()  const { return m_rows * m_cols; }
    unsigned count() const { return size(); } ///< Cantidad de valores almacenados en la matriz
    /// Cantidad máxima posible de valores diferentes que pueden ser almacenados en la matriz
    size_type capacity() const { return size(); }
public:
    Matrix& operator= (const Matrix &o) { return copy(o); } ///< Sinónimo de \c this->copy(o)
    Matrix& copy( const Matrix &o );
    Matrix& move( Matrix &o );
    Matrix& swap( Matrix &o );
public:
    /// ¿¿¿ (p == q) ???
    friend bool operator == (const Matrix &p, const Matrix &q) { return p.equals(q); }
    /// ¿¿¿ (p != q) ???
    friend bool operator != (const Matrix &p, const Matrix &q) { return !(p==q); }
    bool equals( const Matrix & o ) const; ///< ¿¿¿ (*this==o) ???
    /// Retorna \c true si \c "o" comparte sus valores con \c "*this"
    bool same( const Matrix & o ) const { return this == &o; }
protected: // disponibles para clases derivadas
    void add( const Matrix & );
    void substract( const Matrix & );
    void multiply( const Matrix &, const Matrix & );
public:
    friend Matrix operator + (const Matrix& A, const Matrix& B)
    { Matrix Res = A; Res.add(B); return Res; } ///< Retorna A.add(B)
    friend Matrix operator - (const Matrix& A, const Matrix& B)
    { Matrix Res = A; Res.substract(B); return Res; } ///< Retorna \c A-B
    friend Matrix operator * (const Matrix& A, const Matrix& B)
    { Matrix Res;  Res.multiply(A, B); return Res; } ///< Retorna \c A*B
public:
    reference operator () (unsigned, unsigned);
    const_reference operator () (unsigned, unsigned) const;
    reference at (unsigned m, unsigned n)
    { return operator() (m,n); } ///< Retorna \c operator()(m,n).
    const_reference at (unsigned m, unsigned n) const
    { return operator() (m,n); } ///< Retorna \c operator()(m,n) \c "const".
public:
    void reSize( unsigned, unsigned);
    void reShape(unsigned, unsigned);
    void transpose() {
        unsigned tmp = m_rows;
        m_rows = m_cols;
        m_cols = tmp;
    } ///< Transpone la matriz.

    template<class T> friend bool  check_ok( const Matrix<T>& M );
    template<class T> friend class test_Matrix; ///< Datos de prueba para la clase
private:
    value_type * m_val; ///< Vector de valores de la matriz
    unsigned     m_rows; ///< Cantidad de filas de la matriz
    unsigned     m_cols; ///< Cantidad de columnas de la matris
}; // Matrix


/** Verifica la invariante de la clase.
    - Es posible que la matriz tenga dimensiones nulas, lo que implica que todos los
      punteros a los vectors paralelos deben ser nulos. Este hecho se marca dándolo
      el valor \c 0 (cero) al campo \c m_val.
    - Las matrices quedan almacenadas en un vector de tamaño [M x N].
    - En todos los algoritmos, "m" o "m_rows" es la cantidad de filas == \c rows()
    - En todos los algoritmos, "n" o "m_cols" es la cantidad de columnas == \c cols()

    \par <em>Rep</em> Modelo de la clase
\code
+---+                                         /         \
| 2 |  M(i,j) ==> m_val[ (i * m_cols) + j ]   | 0 1 2 3 |   m_rows == 2
+---+  (almacenamiento por filas)             | 4 5 6 7 |   m_cols == 4
| 4 |                                         \         /
+---+   +---+---+---+---+---+---+---+---+
| *-|-->| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
+---+   +---+---+---+---+---+---+---+---+
\endcode

    \par <em>Rep</em> Modelo de la clase
\code
+---+
| 4 |  M(i,j) ==> m_val[ i + (j * m_rows) ]   / a e \
+---+  (almacenamiento por columnas)          | b f |   m_rows == 4
| 2 |                                         | c g |   m_cols == 2
+---+   +---+---+---+---+---+---+---+---+     \ d h /
| *-|-->| a | b | c | d | e | f | g | h |
+---+   +---+---+---+---+---+---+---+---+
\endcode
   - http://www.di-mare.com/adolfo/binder/c03.htm#k1-Rep
   \remark
    Libera al programador de implementar el método \c Ok()
    - http://www.di-mare.com/adolfo/binder/c04.htm#sc11
*/
template<class T>
bool check_ok( const Matrix<T>& M ) {
    if ( M.m_rows == 0 || M.m_cols == 0 ) {    // si alguno es cero...
       if ( M.m_rows != 0 || M.m_cols != 0 ) { // ... los 2 deben serlo
            return false; /// - Invariante: <code>(M.m_rows == 0) <==> (M.m_cols == 0)</code>
       }
       else {
            if (M.m_val != 0) {
                return false; /// - Invariante: <code>(M.m_rows == 0) <==> (M.m_val == 0)</code>
            }
            return true; // La implementación sí permite (m_val == 0)
       }
    }

    unsigned MxN = M.m_rows * M.m_cols;
    for (unsigned k=0; k<MxN; ++k) {
        if ( ! check_ok( M.m_val[k] ) ) {
            return false; /// - Invariante: <code>check_ok( m_val[k] )</code>
        }
    }
    return true;
}


/** \fn template <class E>inline Matrix<E>::Matrix(const value_type V);
    \brief Constructor a partir de \c Matrix<E>::value_type(V).

    - La matriz resultante es una matriz escalar, de dimensiones 1x1,
      y su valor es \c "V".
*/

/** Constructor de vector.
    - Obtiene suficiente memoria dinámica para almacenas los
      <code> n * m </code> valores de la matriz
    - Si \c "value_type" tiene un constructor de vector, lo
      usar para inicializar cada uno de los valores de la matriz;
      de lo contrario, los deja tal cual están en la memoria
    - Si \c "value_type" es uno de los tipos escalares básicos,
      como lo son \c int o \c float, los valores almacenados
      en la matriz quedan tal cual están y no son inicializados.

    \pre
     -  <code> m * n > 0  </code>
     -  <code> (m > 0) && (n > 0) </code>

    \dontinclude test_Matrix.cpp
    \skipline    test::constructor()
    \until       }}
*/
template <class E>
inline Matrix<E>::Matrix(unsigned m, unsigned n) {
    if (m == 0 || n == 0) {
        m_rows = m_cols = 0;
        m_val = 0;
    } else {
        m_rows = m;
        m_cols = n;
        m_val = new value_type [n*m];
    }
}

/** \fn     unsigned Matrix::cols() const

    \dontinclude test_Matrix.cpp
    \skipline    test::rows_cols()
    \until       }}
*/

template <class E>
Matrix<E>::Matrix(const Matrix<E>& o) {
    if (o.m_val == 0) {        // OJO: una matriz "vacía" produce errores en
        m_rows = m_cols = 0;   // Matrix<E>::operator(unsigned, unsigned)
        m_val = 0;
        return;
    }
    m_rows = o.m_rows;
    m_cols = o.m_cols;
    const unsigned MxN = o.m_rows * o.m_cols;
    m_val = new value_type [MxN];

    // como las matrices son del mismo tamaño,
    // basta copiarlas entrada por entrada.

    value_type *pSelf = this->m_val;
    value_type *pO    = o.m_val;
    value_type *pEND  = &m_val[MxN];
    for (; pSelf != pEND; ++pSelf, ++pO) {
        *pSelf = *pO;
    }
}

/// Destructor
template <class E>
inline Matrix<E>::~Matrix() {
    if ( m_val != 0 ) {
        delete [] m_val;
    }
}

/**

    \dontinclude test_Matrix.cpp
    \skipline    test::operator==
    \until       }}
*/

template <class E>
bool Matrix<E>::equals( const Matrix & o ) const {
    if (m_rows != o.m_rows || m_cols != o.m_cols) {
        return false;
    } else if ( m_val == 0 ) {
        return true; // las 2 matrices están vacías ==> son iguales
    }
    const unsigned MxN = o.m_rows * o.m_cols;
    value_type *pSelf = this->m_val;
    value_type *pO    = o.m_val;
    value_type *pEND  = &m_val[MxN];
    for (; pSelf != pEND; ++pSelf, ++pO) {
        if (*pSelf != *pO) {
            return false;
        }
    }
    return true;
}

/** Copia desde \c "o".
    - Copia todo el valor de \c "o" sobre \c "*this", de forma que el nuevo valor de
      \c "*this" sea un duplicado exacto del valor de \c "o"
    - El valor anterior de \c "*this" se pierde
    - El objeto \c "o" mantiene su valor anterior
    - Luego de la copia, cuando el valor de \c "o" cambia, el de \c "*this" no cambiará,
      y viceversa, pues la copia es una copia profunda; no es superficial
    - Si \c "*this" es \c "o" entonces su valor no cambia
    - Después de esta operación siempre ocurre que <code> *this == o </code>

    \par Complejidad:
         O( <code>  rows() * cols() </code> )

    \dontinclude test_Matrix.cpp
    \skipline    test::copy()
    \until       }}

    \returns *this

    \see http://www.di-mare.com/adolfo/binder/c04.htm#sc05
*/
template <class E>
Matrix<E>& Matrix<E>::copy( const Matrix<E> &o ) {
    if (this == &o) { // evita auto-borrado
        return *this;
    } else if (o.m_val == 0) {
        if (m_val != 0) {
            delete [] m_val;
            m_rows = m_cols = 0;
            m_val = 0;
        }
        return *this;
    }
    // se asegura de que las dos matrices son del mismo tamaño
     const unsigned MxN = o.m_rows * o.m_cols;
     if (MxN != m_rows * m_cols) { // truco para evitar borrar la memoria dinámica
		if (m_val!=0) { delete [] m_val; }
        m_val = new value_type [MxN];
    }
    m_rows = o.m_rows;
    m_cols = o.m_cols;


    // como las matrices son del mismo tamaño,
    // basta copiarlas entrada por entrada.

    value_type *pSelf = this->m_val;
    value_type *pO    = o.m_val;
    value_type *pEND  = &m_val[MxN];
    for (; pSelf != pEND; ++pSelf, ++pO) {
        *pSelf = *pO;
    }
    return *this;
}

/** Traslada el valor de \c "o" a \c "*this".
  - El valor anterior de \c "*this" se pierde
  - El nuevo valor de \c "*this" es el que \c "o" tuvo
  - \c "o" queda en el estado en que lo dejaría \c Erase()
  - Si \c "*this" es \c "o" entonces su valor no cambia
  - En general, después de esta operación casi
    nunca ocurre que <code> (*this == o) </code>

    \par Complejidad:
         O( <code>  rows() * cols() </code> )

    \dontinclude test_Matrix.cpp
    \skipline    test::swap_move()
    \until       }}

    \returns \c *this

    \see http://www.di-mare.com/adolfo/binder/c04.htm#sc07
*/
template <class E>
Matrix<E>& Matrix<E>::move( Matrix<E> &o ) {
    if (this == &o) { // evita auto-borrado
        return *this;
    } else if (o.m_val == 0) {
        if (m_val != 0) {
            delete [] m_val;
            m_rows = m_cols = 0;
            m_val = 0;
        }
        return *this;
    } else if ( m_val != 0 ) {
        delete [] m_val;
    }
    m_val = o.m_val;   // me robo los valores de "o"
    m_rows = o.m_rows;
    m_cols = o.m_cols;

    o.m_rows = o.m_cols = 0;
    o.m_val = 0;
    return *this;
}

/** Intercambia los valores de \c "*this" y \c "o".
    - Debido a que algunos métodos retornan copias del valor de \c "*this" en
      lugar de una referencia, como ocurre con \c Matrix::Child(), a veces
      \c swap() no tiene el resultado esperado por el programador.
    - Por ejemplo, si se invoca <code> T.Child(i). swap( T.Child(j) ) </code>
      el resultado no es intercambiar los hijos, sino más bien intercambiar
      los valores de los sub-árboles temporales \c T.Child(i) y \c T.Child(j).
      La forma correcta de intercambiar hijos es usar \c Graft().

      \par Complejidad:
         O( \c 1 )

    \dontinclude test_Matrix.cpp
    \skipline    test::swap_move()
    \until       }}

    \returns *this

    \see http://www.di-mare.com/adolfo/binder/c04.htm#sc08
*/
template <class E>
inline Matrix<E>& Matrix<E>::swap( Matrix<E> &o ) {
    std::swap( this->m_val  , o.m_val  );
    std::swap( this->m_rows , o.m_rows );
    std::swap( this->m_cols , o.m_cols );
    return *this;
}

/** Le cambia las dimensiones a la matriz.
    - En algunos casos los valores almacenados en la matriz no quedan
      todos iguales a \c Matrix<E>::value_type().
    - Si <code> (m * n == 0) </code> deja la matriz vacía.

    \dontinclude test_Matrix.cpp
    \skipline    test::resize()
    \until       }}

*/
template <class E>
void Matrix<E>::reSize(unsigned m, unsigned n) {
     const unsigned MxN = m * n;
    if (MxN == 0) {
        if (m_val != 0) { // sólo borra si hay algo que borrar
            delete [] m_val;
            m_rows = m_cols = 0;
            m_val = 0;
        }
    } else if ( MxN == m_rows * m_cols ) { // truco para evitar borrar la memoria dinámica
        m_rows = m;
        m_cols = n;
     } else {
        if (m_val != 0) {
            delete [] m_val;
        }
        m_val = new value_type [MxN] ;
        m_rows = m;
        m_cols = n;
    }
    return;

/*  NOTA
    Esta es la antigua especificación de reSize(). Es incorrecta
    porque presume que el Rep de la matriz es un vector denso en
    donde están almacenados todos los valores de la matriz:

    +----------------------------------------------------------+
    | reSize(): Le cambia las dimensiones a la matriz.         |
    | ========                                                 |
    | - Si ocurre que (m*n) == rows() * cols() los valores de  |
    |   la matriz se mantienen, aunque cambian sus dimensiones.|
    | - Si (m*n) != rows() * cols() los valores de la matriz   |
    |   quedarán inicializados de la misma forma en que los    |
    |   inicializaría CERO == Sparse_Matrix<E>::value_type().  |
    |                                                          |
    | \pre (m > 0) && (n > 0)                                  |
    +----------------------------------------------------------+

    En la actual especificación, que ya está corregida, no queda
    implícita la presunción sobre cómo está organizada internamente
    la matriz. Por eso, esta nueva especificación sí sirve para una
    matriz implementada con un vector denso de valores, o para la
    matriz implementada como una matriz rala.

    Estos pequeños detalles en algunas ocasiones surgen cuando el
    programador de una clase introduce mejoras o modificaciones, pues
    muchas veces es muy difícil o prácticamente imposible predecir
    todos los pormenores y detalles de una especificación o de una
    implementación.
*/

}

/** Le ajusta las dimensiones a la matriz.
    - Si ocurre que <code> (m*n) == rows()*cols() </code> hace
      lo mismo que haría \c reSize(m,n).
    - En caso contrario, no hace nada.

    \dontinclude test_Matrix.cpp
    \skipline    test::reShape()
    \until       }}

*/
template <class E>
inline void Matrix<E>::reShape(unsigned m, unsigned n) {
     const unsigned MxN = m * n;
     if (MxN == m_rows * m_cols) {
        m_rows = m;
        m_cols = n;
    }
}

/** Retorna una referencia al elemento [i,j] de la matriz ( \c const ).
 - \c M(i,j) significa lo que en arreglos se denota con \c M[i][j].
 - <code>val = M(i,j); // M(i,j) es un "rvalue" (const)</code>

    \dontinclude test_Matrix.cpp
    \skipline    test::operator()
    \until       }}

 */

template <class E>
inline const E& Matrix<E>::operator() (unsigned i, unsigned j) const {
    assert( "Matrix<E>::operator()()" && (i < rows()) );
    assert( "Matrix<E>::operator()()" && (j < cols()) );

    return m_val[ (i * m_cols) + j ] ; // almacena los valores por filas
//  return m_val[ i + (j * m_rows) ] ; // almacena los valores por columnas
}

/// Retorna una referencia al elemento [i,j] de la matriz.
/// - \c M(i,j) significa lo que en arreglos se denota con \c M[i][j].
/// - <code>M(i,j) = val; // M(i,j) es un "lvalue" (modificable)</code>
template <class E>
inline E& Matrix<E>::operator() (unsigned i, unsigned j) {
    assert( "Matrix<E>::operator()()" && (i < rows()) );
    assert( "Matrix<E>::operator()()" && (j < cols()) );

    return m_val[ (i * m_cols) + j ] ; // almacena los valores por filas
//  return m_val[ i + (j * m_rows) ] ; // almacena los valores por columnas
}

/** Le suma a \c "*this" la matriz \c "O".
    \pre
    - \c "*this" y \c "O" deben tener las mismas dimensiones
    - <code> rows() == O.rows() && cols() == O.cols() </code>

    \par Complejidad:
         O( <code> rows() * cols() </code> )

    \remarks
    - Esta es la implementación de Matrix<E> operator+( Matrix<E>&, Matrix<E> )
    - El compilador tiene problemas en compilar un función amiga (<em>"friend"</em>)
      definida con plantillas si esa función amiga no está definida (implementada)
      dentro de la declaración de la clase. Para solventar esta deficiencia existe
      este método que realiza el trabajo, aunque es poco cómodo de usar.

    \dontinclude test_Matrix.cpp
    \skipline    test::operator+
    \until       }}

*/
template <class E>
void Matrix<E>::add( const Matrix<E> & O ) {
    // verifica que las dos matrices sean del mismo tamaño
    assert( (rows() == O.rows()) && (cols() == O.cols()) );

    // Como las matrices son del mismo tamaño basta sumarlas entrada por entrada.

    value_type *pThis = m_val;
    value_type *pO    = & O.m_val[0];
    value_type *pEND  = &m_val[m_cols * m_rows];
    for ( ; pThis != pEND; ++pThis, ++pO) {
        *pThis += *pO;
    }
    return;
}

/** Le resta a \c "*this" la matriz \c "O".
    \pre
    - \c "*this" y \c "O" deben tener las mismas dimensiones
    - <code> rows() == O.rows() && cols() == O.cols() </code>

    \par Complejidad:
         O( <code> rows() * cols() </code> )

    \remarks
    - Esta es la implementación de Matrix<E> operator-( Matrix<E>&, Matrix<E> )
    - El compilador tiene problemas en compilar un función amiga (<em>"friend"</em>)
      definida con plantillas si esa función amiga no está definida (implementada)
      dentro de la declaración de la clase. Para solventar esta deficiencia existe
      este método que realiza el trabajo, aunque es poco cómodo de usar.

    \dontinclude test_Matrix.cpp
    \skipline    test::operator-
    \until       }}

*/
template <class E>
void Matrix<E>::substract( const Matrix<E> & O ) {
    // verifica que las dos matrices sean del mismo tamaño
    assert( (rows() == O.rows()) && (cols() == O.cols()) );

    // Como las matrices son del mismo tamaño basta sumarlas entrada por entrada.

    value_type *pThis = m_val;
    value_type *pO    = & O.m_val[0];
    value_type *pEND  = &m_val[m_cols * m_rows];
    for ( ; pThis != pEND; ++pThis, ++pO) {
        *pThis -= *pO;
    }
    return;
}

/** Calcula la multiplicación <code> A * B </code> y la almacena en \c "*this".
    - Las dimensiones de \c "*this" se ajustan de manera que:
      - <code> rows() == A.rows() && cols() == B.cols()</code>

    \pre
    - \c "A" y \c "B" deben tener dimensiones compatibles
    - <code> A.cols() == B.rows() </code>
    - La multiplicación se hace [Fila x Columna], lo que implica que la cantidad
      de valores en la filas de \c "A" debe ser igual a la cantidad de columnas de \c "B"

    \par Complejidad:
         O( <code> A.cols() * B.cols() * A.cols() </code> )

    \remarks
    - Esta es la implementación de Matrix<E> operator*( Matrix<E>&, Matrix<E> )
    - El compilador tiene problemas en compilar un función amiga (<em>"friend"</em>)
      definida con plantillas si esa función amiga no está definida (implementada)
      dentro de la declaración de la clase. Para solventar esta deficiencia existe
      este método que realiza el trabajo, aunque es poco cómodo de usar.

    \dontinclude test_Matrix.cpp
    \skipline    test::operator*
    \until       }}

*/
template <class E>
void Matrix<E>::multiply( const Matrix<E> & A, const Matrix<E> & B ) {
    // Verifica que las matrices se puedan multiplicar
    assert( (A.cols() == B.rows()) && " => Matrix<E>::multiply()" );

    reSize( A.rows(), B.cols() );
    value_type sum;
    for (unsigned i=0; i<rows(); i++) {
        for (unsigned j=0; j<cols(); j++) {
            sum = value_type(); // sum = E(0);
            for (unsigned k=0; k<A.cols(); k++) {
                sum = sum + A(i,k) * B(k,j);
            }
            // this->(i,j) = sum; // produce un error de compilación
            // this->operator()(i,j) = sum; // también funciona
            (*this)(i,j) = sum; // también funciona
        }
    }
    return;
}

/// Graba en el flujo \c COUT el valor de \c M[][].
template <class E>
std::ostream& operator<<(std::ostream& COUT, const Matrix<E>& M) {
    COUT << '[' << M.rows() << 'x' << M.cols() << ']' << std::endl;
    for (unsigned i=0; i < M.rows(); ++i) {
        for (unsigned j=0; j < M.cols(); ++j) {
            COUT << "  " << M(i,j);
        }
        COUT << std::endl;
    }
    return COUT;
}

/// Obtiene del flujo \c CIN el valor para \c M[][].
template <class E>
std::istream& operator>>(std::istream& CIN, Matrix<E>& M) {
    assert( "This code has not been tested" );
    unsigned rows,cols;
    CIN >> rows >> cols;
    M.reSize(rows,cols);
    for (unsigned i=0; i<rows; i++) {
        for (unsigned j=0; j<cols; j++) {
            CIN >> M(i,j);
        }
    }
    return CIN;
}

} // namespace Mx

#include "Matrix_Lib.h"

#endif // Matrix_h
// EOF: Matrix.h
