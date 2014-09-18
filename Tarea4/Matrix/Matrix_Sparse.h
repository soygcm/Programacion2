// Matrix_Sparse.h Copyright (C) 2004  adolfo@di-mare.com

/** \file  Matrix_Sparse.h
    \brief Declaraciones y definiciones para la clase \c Matrix_Sparse.

    \author Adolfo Di Mare <adolfo@di-mare.com>
    \date   2004

    - Why English names??? ==> http://www.di-mare.com/adolfo/binder/c01.htm#sc04
*/

#ifndef   Matrix_Sparse_h
#define   Matrix_Sparse_h

#include <cassert> // assert()

/// Definido por la biblioteca C++ est�ndar
namespace std {} // Est� ac� para que Doxygen lo documente

/// Matriz chirrisquitica de adolfo@di-mare.com
namespace Mx {

/** Esta es una clase matriz muy chirrisquitica almacenada como una matriz rala.
    - La matriz tiene tama�o \c rows() x \c cols().
    - Se le puede cambiar el tama�o din�micamente con el m�todo \c reSize().
    - La clase \c E debe incluir un neutro para la adici�n, cuyo valor debe poderse
      obtener invocando el convertidor \c Matrix_Sparse<E>::value_type().
    - Las operaciones aritm�ticas "+" "-" "*" deben estar definidas para
      \c Matrix_Sparse<E>::value_type y debe existir el valor \c Matrix_Sparse<E>::value_type().
    \see http://www.oonumerics.org/oon/
*/
template <class E>
class Matrix_Sparse {
public:
    /// Tipo del objeto almacenado, similar al nombre usado en STL
    typedef E value_type;
    /// Tipo del objeto almacenado, similar al nombre usado en STL
    typedef value_type& reference;
    /// Tipo del objeto almacenado, similar al nombre usado en STL
    typedef const value_type& const_reference;
    /// Tipo del tama�o de un objeto, similar al nombre usado en STL
    typedef unsigned size_type;
public:
    Matrix_Sparse(unsigned m = 1, unsigned n = 1);
    Matrix_Sparse(const Matrix_Sparse& o); ///< Constructor de copia
    /// Matriz escalar de valor \c V.
    Matrix_Sparse(const value_type V) : m_capacity(0) { reSize(1,1); (*this)(0,0) = V; }
    ~Matrix_Sparse();
public:
    unsigned rows() const { return m_rows; } ///< Cantidad de filas de la matriz
    unsigned cols() const { return m_cols; } ///< Cantidad de columnas de la Matriz
    unsigned size()  const { return m_rows * m_cols; } ///< Cantidad de valores almacenados en la matriz
    unsigned count() const { return size(); } ///< Cantidad de valores almacenados en la matriz
    /// Cantidad m�xima posible de valores diferentes que pueden ser almacenados en la matriz
    size_type capacity() const { return m_capacity; }
public:
    Matrix_Sparse& operator= (const Matrix_Sparse &o) { return copy(o); } ///< Sin�nimo de \c this->copy(o)
    Matrix_Sparse& copy( const Matrix_Sparse &o );
    Matrix_Sparse& move( Matrix_Sparse &o );
    Matrix_Sparse& swap( Matrix_Sparse &o );
public:
    /// ��� (p == q) ???
    friend bool operator == (const Matrix_Sparse &p, const Matrix_Sparse &q) { return p.equals(q); }
    /// ��� (p != q) ???
    friend bool operator != (const Matrix_Sparse &p, const Matrix_Sparse &q) { return !(p==q); }
    bool equals( const Matrix_Sparse & o ) const; ///< ��� (*this==o) ???
    /// Retorna \c true si \c "o" comparte sus valores con \c "*this"
    bool same( const Matrix_Sparse & o ) const { return this == &o; }
private:
    void add( const Matrix_Sparse & );
    void substract( const Matrix_Sparse & );
    void multiply( const Matrix_Sparse &, const Matrix_Sparse & );
public:
    friend Matrix_Sparse operator + (const Matrix_Sparse& A, const Matrix_Sparse& B)
    { Matrix_Sparse Res = A; Res.add(B); return Res; } ///< Retorna \c A+B
    friend Matrix_Sparse operator - (const Matrix_Sparse& A, const Matrix_Sparse& B)
    { Matrix_Sparse Res = A; Res.substract(B); return Res; } ///< Retorna \c A-B
    friend Matrix_Sparse operator * (const Matrix_Sparse& A, const Matrix_Sparse& B)
    { Matrix_Sparse Res;  Res.multiply(A, B); return Res; } ///< Retorna \c A*B
public:
    reference operator () (unsigned, unsigned);
    const_reference operator () (unsigned, unsigned) const;
public:
    void reSize( unsigned, unsigned);
    void reShape(unsigned, unsigned);
public:
    void setDefault(const E& same);
    /// Valor almacenado en la mayor parte de la \c Matrix_Sparse
    const E& getDefault() { return m_same; }
    /// Ajusta la matriz para que pueda almacenar \c n valores diferentes a \c getDefault().
    void reserve(size_type _Count);
    void reSize(unsigned newsize);
    void clear(); ///< Borra todos los valores de la \c Matrix_Sparse

    template<class T> friend bool  check_ok( const Matrix_Sparse<T>& M );
    template<class T> friend class test_Matrix_Sparse; ///< Datos de prueba para la clase
private:
    unsigned* m_I;   ///< Indice "i" de \c M(i,j) <code>0 <= i < m_capacity</code>
    unsigned* m_J;   ///< Indice "j" de \c M(i,j) <code>0 <= i < m_capacity</code>
    E       * m_val; ///< Valor para \c M(i,j) <code>0 <= i < m_capacity</code>
    unsigned  m_size;   ///< Cantidad de valores insertados en los 3 vectores paralelos
    unsigned  m_capacity; ///< Tama�o de los 3 vectores paralelos
    unsigned  m_rows; ///< Cantidad de filas de la matriz
    unsigned  m_cols; ///< Cantidad de columnas de la matris
    E         m_same; ///< Valor almacenado en la mayor parte de la \c Matrix_Sparse
}; // Matrix_Sparse

/** Verifica la invariante de la clase.
    - El campo \c m_same indica cu�l es el valor que se repite m�s en toda la matriz.
    - Usualmente \c same es el neutro aditivo \c value_type().
    - No existe un constructor expl�cito para darle a \c m_same su valor inicial, que
      es siempre inicializado en \c value_type(). Para cambiarlo es necesario invocar
      el m�todo \c setgetDefault().
    - Los vectores \c m_I[], \c m_J[] y \c m_val[] son vectores paralelos, todos de
      longitud \c Matrix_Sparse::m_capacity.
    - La cantidad m�xima de valores diferente que pueden ser almacenados en la matriz
      es \c Matrix_Sparse::m_capacity.
    - El largo de estos vectores aumenta cuando se hace referencia a un valor M(i,j)
      mediante la versi�n que no es \c const del \c operator()(i,j). O sea, que es
      ese m�todo el encargado de agregar valores en \c m_val[], pues el operador
      hom�nimo \c const operator()(i,j) nunca agrega nada y, como es \c const, en
      general retorna una referencia constante a \c m_same.
    - Es posible que la matriz tenga dimensiones nulas, lo que implica que todos los
      punteros a los vectors paralelos deben ser nulos. Este hecho se marca d�ndolo
      el valor \c 0 (cero) al campo \c m_capacity.
    - En todos los algoritmos, "m" o "m_rows" es la cantidad de filas == \c rows()
    - En todos los algoritmos, "n" o "m_cols" es la cantidad de columnas == \c cols()

    \par <em>Rep</em> Modelo de la clase
\code
         ____________________________________
        /          m_capacity                \
        +---+---+---+---+---+---+-..-+---+---+       0 1 2 3 4 5 6
 m_I--->| 1 | 3 | 3 |'-'| ...       ...  |'-'|   0 / - - - - - - - \
        +---+---+---+---+ ...       ...  +---+   1 | - a - - - - - |
 m_J--->| 1 | 2 | 1 |'-'| ...       ...  |'-'|   2 | - - - - - - - |
        +---+---+---+---+ ...       ...  +---+   3 | - c b - - - - |
m_val-->|'a'|'b'|'c'|'-'| ...       ...  |'-'|   4 \ - - - - - - - /
        +---+---+---+---+---+---+-..-+---+---+
          0   1   2   |
        m_size--------+ == 3    m_same == '-'   m_rows == 5  m_cols == 7
\endcode
    - http://www.di-mare.com/adolfo/binder/c03.htm#k1-Rep
    \remark
    Libera al programador de implementar el m�todo \c Ok()
    - http://www.di-mare.com/adolfo/binder/c04.htm#sc11
*/
template<class T>
bool check_ok( const Matrix_Sparse<T>& M ) {
    if ( M.m_capacity == 0 ) { // m_capacity es el "marcador" que determina el valor de los dem�s
        if ( M.m_I != 0 ) {
            return false; /// - Invariante: <code>(m_capacity == 0) <==> (m_I == 0)</code>
        }
        if ( M.m_J != 0 ) {
            return false; /// - Invariante: <code>(m_capacity == 0) <==> (m_J == 0)</code>
        }
        if ( M.m_val != 0 ) {
            return false; /// - Invariante: <code>(m_capacity == 0) <==> (m_val == 0)</code>
        }
    }
    else {
        if ( M.m_rows == 0 ) {
            return false; /// - Invariante: <code>(m_rows == 0) ==> (m_capacity == 0)</code>
        }
        if ( M.m_cols == 0 ) {
            return false; /// - Invariante: <code>(m_cols == 0) ==> (m_capacity == 0)</code>
        }
        if ( M.m_I == 0 ) {
            return false; /// - Invariante: <code>(m_capacity != 0) <==> (m_I != 0)</code>
        }
        if ( M.m_J == 0 ) {
            return false; /// - Invariante: <code>(m_capacity != 0) <==> (m_J != 0)</code>
        }
        if ( M.m_val == 0 ) {
            return false; /// - Invariante: <code>(m_capacity != 0) <==> (m_val != 0)</code>
        }
    }

    if (M.m_rows == 0 || M.m_cols == 0) {
        if (M.m_rows == 0 && M.m_cols == 0) {
            // OK ==> los 2 son nulos
        }
        else {
            return false;  /// - Invariante: <code>(m_rows == 0) <==> (m_cols == 0)</code>
        }
    }

    if ( M.m_size > M.m_capacity ) {
        return false; /// - Invariante: <code>( m_size <= m_capacity )</code>
    }
    if ( ! check_ok (M.m_same) ) {
        return false; /// - Invariante: <code>check_ok (m_same)</code>
    }
    for (unsigned k=0; k<M.m_size; ++k) {
        if ( ! ( (0<=M.m_I[k]) && (M.m_I[k]<M.m_rows) ) ) {
            return false; /// - Invariante: <code>( (0<=m_I[k]) && (m_I[k] < m_rows) ) k = [0..m_size-1]</code>
        }
        if ( ! ( (0<=M.m_J[k]) && (M.m_J[k]<M.m_cols) ) ) {
            return false; /// - Invariante: <code>( (0<=m_J[k]) && (m_J[k] < m_cols) ) k = [0..m_size-1]</code>
        }
        if ( ! check_ok( M.m_val[k] ) ) {
            return false; /// - Invariante: <code>check_ok( m_val[k] )</code>
        }
    }
    return true;
}

/// Define el escalar que por defecto est� en todas las entradas de la \c Matrix_Sparse.
/// - Si <code>same != getDefault()</code> la matriz queda vac�a.
/// - De lo contrario, nada ocurre.
template<class E>
inline void Matrix_Sparse<E>::setDefault(const E& same) {
    if ( m_same != same ) {
        m_same = same;
        m_size = 0;
    }
}
/** \fn template <class E>inline Matrix_Sparse<E>::Matrix_Sparse(const value_type V);

    \brief Constructor a partir de \c Matrix_Sparse<E>::value_type(V).

    - La matriz resultante es una matriz escalar, de dimensiones 1x1,
      y su valor es \c "V"
*/

/** Constructor de vector.
    - Obtiene suficiente memoria din�mica para almacenas los
      <code> n * m </code> valores de la matriz.
    - Si \c "value_type" tiene un constructor de vector, lo
      usar para inicializar cada uno de los valores de la matriz;
      de lo contrario, los deja tal cual est�n en la memoria.
    - Si \c "value_type" es uno de los tipos escalares b�sicos,
      como lo son \c int o \c float, los valores almacenados
      en la matriz quedan tal cual est�n y no son inicializados.
    \pre
     -  <code> m * n > 0  </code>
     -  <code> (m > 0) && (n > 0) </code>
*/
template <class E>
inline Matrix_Sparse<E>::Matrix_Sparse(unsigned m, unsigned n) : m_same() {
    if (m == 0 || n == 0) {
        m_rows = m_cols = 0;
        m_val = 0;
        m_capacity = 0;
    } else {
        m_rows = m;
        m_cols = n;

        m_capacity = 16; // pues 16 == 2*2*2*2 ...
        m_I   = new size_type [m_capacity];
        m_J   = new size_type [m_capacity];
        m_val = new value_type[m_capacity];
    }

    m_size = 0;
//  m_same = E(); // Usa el n�mero "cero" como neutro tipo "E"
}

template <class E>
inline Matrix_Sparse<E>::Matrix_Sparse(const Matrix_Sparse<E>& o) {
    m_capacity = 0; // m_capacity==0 indica que NO se est� usando memoria din�mica
    copy( o );     // assert( this != &o );
    return;       // ... pues ya "o" existe y se est� usando para incializar a "*this"
/*  NOTA
    Cuando un objeto usa memoria din�mica, copiarlo es un proceso diferente a
    inicializarlo a partir de otro valor de su misma clase. En otras palabras,
    el trabajo que hace el constructor CLS::CLS( const CLS& ) es muy diferente
    al que hace el copiador CLS& operator=( const CLS & ).

    El truco de programaci�n usado en en esta implementaci�n consiste en "marcar"
    el valor "m_capacity" para saber si se ha obtenido o no memoria din�mica
    para los vectores paralelos. La implementaci�n de copy() est� hecha de manera
    que si "m_capacity" es un puntero nulo, en copy() se inicializan correctamente
    todos los del Rep de Matrix_Sparse. Eso explica por qu� la implementaci�n de
    copy() es un tanto m�s elaborada que lo que a primera vista pareciera que es
    necesario.
*/
}

/// Destructor
template <class E>
inline Matrix_Sparse<E>::~Matrix_Sparse() {
    if (m_capacity != 0) { // s�lo devuelve la memoria din�mica que s� ha adquirido
        delete [] m_I;
        delete [] m_J; // Retorna la memoria din�mica en uso
        delete [] m_val;
    }
}

template <class E>
bool Matrix_Sparse<E>::equals( const Matrix_Sparse & o ) const {
    if ( this == & o ) {
        return true;
    }
    if (rows() != o.rows() || cols() != o.cols()) {
        return false;
    }

    for (unsigned i=0; i<rows(); i++) {
        for (unsigned j=0; j<cols(); j++) {
            if ( (*this)(i,j) != o(i,j) ) {
                return false;
            }
        }
    }
    return true;
}

/** Copia desde \c "o".
    - Copia todo el valor de \c "o" sobre \c "*this", de forma que el nuevo valor de
      \c "*this" sea un duplicado exacto del valor de \c "o".
    - El valor anterior de \c "*this" se pierde.
    - El objeto \c "o" mantiene su valor anterior.
    - Luego de la copia, cuando el valor de \c "o" cambia, el de \c "*this" no cambiar�,
      y viceversa, pues la copia es una copia profunda; no es superficial.
    - Si \c "*this" es \c "o" entonces su valor no cambia.
    - Despu�s de esta operaci�n siempre ocurre que <code> *this == o </code>.

    \par Complejidad:
         O( <code>  rows() * cols() </code> )

    \returns *this
    \see http://www.di-mare.com/adolfo/binder/c04.htm#sc05
*/
template <class E>
Matrix_Sparse<E>& Matrix_Sparse<E>::copy( const Matrix_Sparse<E> &o ) {
    if (this == &o) { // evita auto-borrado
        return *this;
    }
    if (o.m_capacity == 0) {
        if (m_capacity != 0) { // Como copy() puede ser invocado cuando el Rep
            delete [] m_val;   // NO ha sido inicializado, en esta implementaci�n
            delete [] m_J;     // hay que tener cuidado de no usar los otros campos
            delete [] m_I;     // del Rep antes de darles valor.
        }                      // - "m_capacity" SI debe tener su valor correcto.
        m_rows = m_cols = 0;
        m_val = 0;
        m_same = o.m_same;
        m_size = 0;       //  assert( o.m_size == 0 );
        m_capacity = 0;   //  assert( o.m_capacity == 0 );
        return *this;
    }

    // se asegura de que las dos matrices sean del mismo tama�o
    if (m_capacity != o.m_capacity) { // truco para evitar borrar la memoria din�mica
        if (m_capacity != 0) { // s�lo devuelve la memoria din�mica que s� ha adquirido
            delete [] m_val;
            delete [] m_J;
            delete [] m_I;
        }

        m_capacity = o.m_capacity;
        m_I   = new size_type [m_capacity];
        m_J   = new size_type [m_capacity];
        m_val = new value_type[m_capacity];
    }
    m_same = o.m_same;
    m_size = o.m_size;

    // como las matrices son del mismo tama�o,
    // basta copiarlas entrada por entrada.
    m_rows = o.m_rows;
    m_cols = o.m_cols;
    for (unsigned k=0; k<m_size; ++k) {
        m_I[k]   = o.m_I[k];
        m_J[k]   = o.m_J[k];
        m_val[k] = o.m_val[k];
    }
    return *this;
}

/** Traslada el valor de \c "o" a \c "*this".
  - El valor anterior de \c "*this" se pierde.
  - El nuevo valor de \c "*this" es el que \c "o" tuvo.
  - \c "o" queda en el estado en que lo dejar�a \c Erase().
  - Si \c "*this" es \c "o" entonces su valor no cambia.
  - En general, despu�s de esta operaci�n casi
    nunca ocurre que <code> (*this == o) </code>

    \par Complejidad:
         O( <code>  rows() * cols() </code> )

    \returns \c *this

    \see http://www.di-mare.com/adolfo/binder/c04.htm#sc07
*/
template <class E>
Matrix_Sparse<E>& Matrix_Sparse<E>::move( Matrix_Sparse<E> &o ) {
    if (this == &o) { // evita auto-borrado
        return *this;
    } else if (o.m_val == 0) {
        if (m_val != 0) {
            delete [] m_val;
        }
        m_rows = m_cols = 0;
        m_val = 0;
        return *this;
    } else if ( m_val != 0 ) {
        delete [] m_val;
    }
    m_val = o.m_val;
    m_rows = o.m_rows;
    m_cols = o.m_cols;

    o.m_val = 0;
    o.m_rows = o.m_cols = 0;
    return *this;
}

/** Intercambia los valores de \c "*this" y \c "o".
    - Debido a que algunos m�todos retornan copias del valor de \c "*this" en
      lugar de una referencia, como ocurre con \c Matrix_Sparse::Child(), a veces
      \c swap() no tiene el resultado esperado por el programador.
    - Por ejemplo, si se invoca <code> T.Child(i). swap( T.Child(j) ) </code>
      el resultado no es intercambiar los hijos, sino m�s bien intercambiar
      los valores de los sub-�rboles temporales \c T.Child(i) y \c T.Child(j).
      La forma correcta de intercambiar hijos es usar \c Graft().

      \par Complejidad:
         O( \c 1 )

    \returns *this

    \see http://www.di-mare.com/adolfo/binder/c04.htm#sc08
*/
template <class E>
inline Matrix_Sparse<E>& Matrix_Sparse<E>::swap( Matrix_Sparse<E> &o ) {
    std::swap( this->m_I        , o.m_I        );
    std::swap( this->m_J        , o.m_J        );
    std::swap( this->m_val      , o.m_val      );
    std::swap( this->m_size     , o.m_size     );
    std::swap( this->m_capacity , o.m_capacity );
    std::swap( this->m_rows     , o.m_rows     );
    std::swap( this->m_cols     , o.m_cols     );
    std::swap( this->m_same     , o.m_same     );
    return *this;
}

/** Le cambia las dimensiones a la matriz.
    - En algunos casos los valores almacenados en la matriz no quedan
      todos iguales a \c Matrix_Sparse<E>::value_type().
    \pre <code> (m > 0) && (n > 0) </code>
*/
template <class E>
void Matrix_Sparse<E>::reSize(unsigned m, unsigned n) {
    // NO hace falta cambiar m_capacity porque lo �nico que est� cambiando
    // el la dimensi�n de la matriz
    if (m != m_rows || n != m_cols) {
        m_size = 0; // desecha todos los valores anteriores
        m_rows = m; // cambia las dimensiones de la matriz
        m_cols = n;
    }

    if (m==0 || n==0) {
        m_rows = 0;
        m_cols = 0;
        m_size = 0;
        if (m_capacity != 0) {
            m_capacity = 0; // todo esto mantiene la invariante de la clase
            delete [] m_I;     m_I = 0;
            delete [] m_J;     m_J = 0;
            delete [] m_val;   m_val = 0;
        }
    }
    return;

#if 0
/*  NOTA
    Esta es la antigua especificaci�n de reSize(). Es incorrecta
    porque presume que el Rep de la matriz es un vector denso en
    donde est�n almacenados todos los valores de la matriz:

    +----------------------------------------------------------+
    | reSize(): Le cambia las dimensiones a la matriz.         |
    | ========                                                 |
    | - Si ocurre que (m*n) == rows() * cols() los valores de  |
    |   la matriz se mantienen, aunque cambian sus dimensiones.|
    | - Si (m*n) != rows() * cols() los valores de la matriz   |
    |   quedar�n inicializados de la misma forma en que los    |
    |   inicializar�a CERO == Matrix_Sparse<E>::value_type().  |
    |                                                          |
    | \pre (m > 0) && (n > 0)                                  |
    +----------------------------------------------------------+

    En la actual especificaci�n, que ya est� corregida, no queda
    impl�cita la presunci�n sobre c�mo est� organizada internamente
    la matriz. Por eso, esta nueva especificaci�n s� sirve para una
    matriz implementada con un vector denso de valores, o para la
    matriz implementada como una matriz rala.

    Estos peque�os detalles en algunas ocasiones surgen cuando el
    programador de una clase introduce mejoras o modificaciones, pues
    muchas veces es muy dif�cil o pr�cticamente imposible predecir
    todos los pormenores y detalles de una especificaci�n o de una
    implementaci�n.
*/

/*  NOTA
    Esta es la imnplementaci�n anterior, que debe reacomodar los �nices
    (i,j) de aquellos valores almacenados en los vectores paralalos para
    simular que la matriz rala en realidad est� almacenada como una
    matriz densa, dentro de un vector.
    - Al modificar la especificaci�n de para reSize() ya no hace falta
    hacer todo este trabajo.

    NO hace falta cambiar m_capacity porque lo �nico que est� cambiando
    el la dimensi�n de la matriz
*/

/*  Nota para esta implementaci�n:
    Este m�todo funciona de una forma similar a reSize() para la matriz implementada con
    un vector. Para lograr que las clases Matrix<E> y Matrix_Sparse<E> tengan la misma
    funcionalidad, esta implementaci�n reacomoda los �ndices de la matriz de manera que
    las m_rows*m_cols entradas queden reacomodadas como si la implementaci�n usara un
    vector de dimension m*n. Sin embargo, lo l�gico ser�a eliminar todos los valores
    de la matriz trocando el ciclo "for(;k;){}" por un eficiente "m_size=0;"
    - Esto muestra que una especificaci�n que aparenta ser "amplia", como lo es la de
      Matrix<E>::reSize(n,m), en realidad se queda "corta" si se toma en cuenta que la
      misma especificaci�n debe servir para varias implementaciones diferentes.
    - Adem�s, muestra que algunas operaciones s�lo tienen sentido para una implementaci�n
      espec�fica, como ocurre con las operaciones Matrix_Sparse<E>>::getDefault() y su
      "seteador" Matrix_Sparse<E>>::setgetDefault().
*/
    if (m * n == m_rows * m_cols) {
        m_size = 0; // desecha todos los valores anteriores
    }
    else {
        unsigned row = 0, col = 0;
        for (unsigned k=0; k<m_size; ++k) {
            // M(i,j) <==> m_val[ (i * m_cols) + j ] ; // si almacena los valores por filas
            // M(i,j) <==> m_val[ i + (j * m_rows) ] ; // si almacena los valores por columnas

            unsigned lineal = (m_I[k] * m_cols) + m_J[k]; // por filas
        //  unsigned lineal = m_I[k] + (m_j[k] * m_rows); // por columnas
            m_I[k] = lineal / n; // lineal % m;
            m_J[k] = lineal % n; // lineal / m;
        }
    }
    m_rows = m; // cambia las dimensiones de la matriz
    m_cols = n;

#endif
}

/** Le ajusta las dimensiones a la matriz.
    - Si ocurre que <code> (m*n) == rows()*cols() </code> hace
      lo mismo que har�a \c reSize(m,n).
    - En caso contrario, no hace nada.
*/
template <class E>
inline void Matrix_Sparse<E>::reShape(unsigned m, unsigned n) {
    if ( m * n == m_rows * m_cols ) {
        reSize(n,m);
    }
}

/// Retorna una referencia al elemento [i,j] de la matriz ( \c const ).
/// - \c M(i,j) significa lo que en arreglos se denota con \c M[i][j].
/// - <code>val = M(i,j); // M(i,j) es un "rvalue" (const)</code>
template <class E>
inline const E& Matrix_Sparse<E>::operator () (unsigned i, unsigned j) const {
    assert( "Matrix_Sparse<E>::operator()()" && (i < rows()) );
    assert( "Matrix_Sparse<E>::operator()()" && (j < cols()) );

    for (unsigned k=0; k<m_size; k++) {
        if ( m_I[k] == i ) {
            if ( m_J[k] == j ) {
                return m_val[k];
            }
        }
    }
    return m_same;
/*  NOTA
    Como este m�todo es "const", de antemano se sabe que el programador no puede
    usarlo para modificar el valor de la matriz. Por eso, aunque el valor
    retornado sea una referencia a valor com�n por defecto m_same, de antemano
    el compilador asegura que ese valor no ser� modificado.

    Sin embargo, cuando el programador usa el m�todo hom�nimo operator()(i,j)
    que no es "const", es posible que el valor retornado s� sea modificado.
    En ese caso, ya no es correcto retornar una referencia al valor com�n "m_same".
    - Por eso, cuando se usa el hace referencia en el otro operator()(i,j) es
      necesario agregar una entrada en los vectores paralelos en aquellos casos
      en que no existe un valor diferente a "m_same" para (i,j).
    - Esto quiere decir que s� es posible que al utilizar la versi�n modificable
      de operator()(i,j) quede en el vector "m_val[]" un valor que es igual a
      "m_same". En el caso peor, podr�a ocurrir que todos los valores almacenados
      en el vector "m_val[]" sean todos iguales a "m_same".
    - Una forma de corregir esta anomal�a es "revisar despu�s" si existe un valor
      en el vector "m_val[]" que es igual a "m_same". Una forma eficiente de
      hacerlo es mantener el el Rep un puntero "m_last_change" que apunte al
      �ltimo valor "m_val[]" que la versi�n modificable de operator()(i,j) retorn�.
      En la siguiente invocaci�n a operator()(i,j), se puede verificar si hubo un
      ese valor anterior fue cambiado de manera que tiene almacenado "m_same".
*/
#if 0
    // Este c�digo podr�a ser agregado al principio de operator()(i,j)
    if (m_last_change != 0) {
        if ( *m_last_change == m_same ) { // el �ltimo uso de op()(i,j) dej� "m_same"
            if ( m_size == 1 ) { // Elimina los vectores viejos
                delete [] m_I;    m_I = 0;
                delete [] m_J;    m_J = 0;
                delete [] m_val;  m_val = 0;
                m_capacity = 0;
                m_size     = 0;
            }
            else { // intercambia este valor con el �ltimo
                m_size--;
                unsigned k = (m_last_change - m_val); // �ndice de "* m_last_change"
                *m_last_change = m_val[m_size];
                m_I[k] = m_I[m_size];
                m_J[k] = m_J[m_size];
            }
        }
        m_last_change = 0;
    }
#endif
}
/// Retorna una referencia al elemento [i,j] de la matriz.
/// - \c M(i,j) significa lo que en arreglos se denota con \c M[i][j].
/// - <code>M(i,j) = val; // M(i,j) es un "lvalue" (modificable)</code>
template <class E>
inline E& Matrix_Sparse<E>::operator() (unsigned i, unsigned j) {
    assert( "Matrix_Sparse<E>::operator()()" && (i < rows()) );
    assert( "Matrix_Sparse<E>::operator()()" && (j < cols()) );

    // Busca al elemento (i,j) en los vectores paralelos
    for (unsigned k=0; k<m_size; k++) {
        if ( m_I[k] == i ) {
            if ( m_J[k] == j ) {
            //  m_last_change = & m_val[k];
                return m_val[k];
            }
        }
    }

    assert( (m_size <= m_capacity) && " => Agotado m_val[] Matrix_Sparse<E>::operator()()" );
    if (m_size == m_capacity) {
        unsigned new_capacity = m_capacity;
        if (m_capacity % 16 == 0) {
            new_capacity += 16;
        }
        else {
            new_capacity /= 16; // Hace que el nuevo tama�o sea divisible por 16
            new_capacity *= 16;
            new_capacity += 16;
        }
        reSize( new_capacity ); // Agrega 16 porque 16 == 2*2*2*2 ...
    }
    // Agrega un nuevo valor modificable en los vectores paralelos
    m_I[m_size] = i;
    m_J[m_size] = j;
    m_val[m_size] = m_same;
//  m_last_change = & m_val[m_size];
    return m_val[m_size++];
}

/// Le cambia el tama�o m�ximo posible a la matriz.
/// - Le aumenta la cantidad de valores diferentes a \c getDefault().
/// - No hace nada cuando <code>size() < newsize</code>.
template <class E>
void Matrix_Sparse<E>::reSize(unsigned newsize) {
    if ( newsize < m_size ) { // hay m�s valores en uso que "newsize"
        // assert((m_size < newsize) && "=>Matrix_Sparse<E>::reSize()" );
        return;
    }
    unsigned * new_I   = new unsigned [ newsize ]; // Obtiene los vectores nuevos
    unsigned * new_J   = new unsigned [ newsize ];
    E        * new_val = new E        [ newsize ];
    if (m_capacity > 0) { // Copia los valores en uso
        for (unsigned k=0; k<m_size; ++k) {
            new_I[k]   = m_I[k];
            new_J[k]   = m_J[k];
            new_val[k] = m_val[k];
        }
        delete [] m_I;  // Elimina los vectores viejos
        delete [] m_J;
        delete [] m_val;
    }
    m_I = new_I;      // Instala los vectores nuevos
    m_J = new_J;
    m_val = new_val;
    m_capacity = newsize;
}

/** Le suma a \c "*this" la matriz \c "O".
    \pre
    - \c "*this" y \c "O" deben tener las mismas dimensiones
    - <code> rows() == O.rows() && cols() == O.cols() </code>

    \par Complejidad:
         O( <code> rows() * cols() </code> )

    \remarks
    - Esta es la implementaci�n de Matrix_Sparse<E> operator+( Matrix_Sparse<E>&, Matrix_Sparse<E> )
    - El compilador tiene problemas en compilar un funci�n amiga (<em>"friend"</em>)
      definida con plantillas si esa funci�n amiga no est� definida (implementada)
      dentro de la declaraci�n de la clase. Para solventar esta deficiencia existe
      este m�todo que realiza el trabajo, aunque es poco c�modo de usar.
*/
template <class E>
void Matrix_Sparse<E>::add( const Matrix_Sparse<E> & O ) {
    // verifica que las dos matrices sean del mismo tama�o
    assert( "Matrix_Sparse<E>::add()" && (rows() == O.rows()) && (cols() == O.cols()) );

    // crea una nueva matriz que va a ser la que se devuelve
    reSize(O.m_rows, O.m_cols);

    // Como las matrices son del mismo tama�o basta sumarlas entrada por entrada.

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
    - Esta es la implementaci�n de Matrix_Sparse<E> operator-( Matrix_Sparse<E>&, Matrix_Sparse<E> )
    - El compilador tiene problemas en compilar un funci�n amiga (<em>"friend"</em>)
      definida con plantillas si esa funci�n amiga no est� definida (implementada)
      dentro de la declaraci�n de la clase. Para solventar esta deficiencia existe
      este m�todo que realiza el trabajo, aunque es poco c�modo de usar.
*/
template <class E>
void Matrix_Sparse<E>::substract( const Matrix_Sparse<E> & O ) {
    // verifica que las dos matrices sean del mismo tama�o
    assert( "Matrix_Sparse<E>::substract()" && (rows() == O.rows()) && (cols() == O.cols()) );

    // crea una nueva matriz que va a ser la que se devuelve
    reSize(O.m_rows, O.m_cols);

    // Como las matrices son del mismo tama�o basta sumarlas entrada por entrada.

    value_type *pThis = m_val;
    value_type *pO    = & O.m_val[0];
    value_type *pEND  = &m_val[m_cols * m_rows];
    for ( ; pThis != pEND; ++pThis, ++pO) {
        *pThis -= *pO;
    }
    return;
}

/** Calcula la multiplicaci�n <code> A * B </code> y la almacena en \c "*this".
    - Las dimensiones de \c "*this" se ajustan de manera que:
      - <code> rows() == A.rows() && cols() == B.cols()</code>

    \pre
    - \c "A" y \c "B" deben tener dimensiones compatibles
    - <code> A.cols() == B.rows() </code>
    - La multiplicaci�n se hace [Fila x Columna], lo que implica que la cantidad
      de valores en la filas de \c "A" debe ser igual a la cantidad de columnas de \c "B"

    \par Complejidad:
         O( <code> A.cols() * B.cols() * A.cols() * A.capacity() * B.capacity() </code> )

    \remarks
    - Esta es la implementaci�n de Matrix_Sparse<E> operator*( Matrix_Sparse<E>&, Matrix_Sparse<E> )
    - El compilador tiene problemas en compilar un funci�n amiga (<em>"friend"</em>)
      definida con plantillas si esa funci�n amiga no est� definida (implementada)
      dentro de la declaraci�n de la clase. Para solventar esta deficiencia existe
      este m�todo que realiza el trabajo, aunque es poco c�modo de usar.
*/
template <class E>
void Matrix_Sparse<E>::multiply( const Matrix_Sparse<E> & A, const Matrix_Sparse<E> & B ) {
    // Verifica que las matrices se puedan multiplicar
    assert( (A.cols() == B.rows()) && " => Matrix_Sparse<E>::multiply()" );

    reSize( A.rows(), B.cols() );
    value_type sum;
    for (unsigned i=0; i<rows(); i++) {
        for (unsigned j=0; j<cols(); j++) {
            sum = value_type(); // sum = E(0);
            for (unsigned k=0; k<A.cols(); k++) {
                sum = sum + A(i,k) * B(k,j);
            }
            // this->(i,j) = sum; // produce un error de compilaci�n
            // this->operator()(i,j) = sum; // tambi�n funciona
            (*this)(i,j) = sum; // tambi�n funciona
        }
    }
    return;
}

/// Graba en el flujo \c COUT el valor de \c M[][].
template <class E>
std::ostream& operator<<(std::ostream& COUT, const Matrix_Sparse<E>& M) {
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
std::istream& operator>>(std::istream& CIN, Matrix_Sparse<E>& M) {
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

// Este [horrible] truco permite "compartir" el mismo archivo Matrix_Lib.h
// entre Matrix.h y Matrix_Sparse.h
// #define   Matrix Matrix_Sparse
// #include "Matrix_Lib.h"
// #undef    Matrix

#include "Matrix_Lib.h"

#endif // Matrix_Sparse_h
// EOF: Matrix_Sparse.h
