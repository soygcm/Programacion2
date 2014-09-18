// Matrix_Map.h Copyright (C) 2004  adolfo@di-mare.com

/** \file  Matrix_Map.h
    \brief Declaraciones y definiciones para la clase \c Matrix_Map.

    \author Adolfo Di Mare <adolfo@di-mare.com>
    \date   2004

    - Why English names??? ==> http://www.di-mare.com/adolfo/binder/c01.htm#sc04
*/

#ifndef   Matrix_Map_h
#define   Matrix_Map_h

#include <cassert> // assert()
#include <list>
#include <map>
#include <utility>


/// Definido por la biblioteca C++ estándar
namespace std {} // Está acá para que Doxygen lo documente

/// Matriz chirrisquitica de adolfo@di-mare.com
namespace Mx {

/** Esta es una clase matriz muy chirrisquitica almacenada como una matriz rala implementada con Map.
    - La matriz tiene tamaño \c rows() x \c cols().
    - Se le puede cambiar el tamaño dinámicamente con el método \c reSize().
    - La clase \c E debe incluir un neutro para la adición, cuyo valor debe poderse
      obtener invocando el convertidor \c Matrix_Map<E>::value_type().
    - Las operaciones aritméticas "+" "-" "*" deben estar definidas para
      \c Matrix_Map<E>::value_type y debe existir el valor \c Matrix_Map<E>::value_type().
    \see http://www.oonumerics.org/oon/
*/
template <class E>
class Matrix_Map {
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
    Matrix_Map(unsigned m = 1, unsigned n = 1);
    Matrix_Map(const Matrix_Map& o); ///< Constructor de copia
    /// Matriz escalar de valor \c V.
    Matrix_Map(const value_type V) : m_same() { reSize(1,1); (*this)(0,0) = V; }
    ~Matrix_Map();
public:
    unsigned rows() const { return  m_rows; } ///< Cantidad de filas de la matriz
    unsigned cols() const { return m_cols; } ///< Cantidad de columnas de la Matriz
    unsigned size()  const { return rows() * m_cols; } ///< Cantidad de valores almacenados en la matriz
    unsigned count() const { return size(); } ///< Cantidad de valores almacenados en la matriz
    /// Cantidad máxima posible de valores diferentes que pueden ser almacenados en la matriz
    size_type capacity() const { return size(); }
public:
    Matrix_Map& operator = (const Matrix_Map &o) { return copy(o); } ///< Sinónimo de \c this->copy(o)
    Matrix_Map& copy( const Matrix_Map &o );
    Matrix_Map& move( Matrix_Map &o );
    Matrix_Map& swap( Matrix_Map &o );
public:
    /// ¿¿¿ (p == q) ???
    friend bool operator == (const Matrix_Map &p, const Matrix_Map &q) { return p.equals(q); }
    /// ¿¿¿ (p != q) ???
    friend bool operator != (const Matrix_Map &p, const Matrix_Map &q) { return !(p==q); }
    bool equals( const Matrix_Map & o ) const; ///< ¿¿¿ (*this==o) ???
    /// Retorna \c true si \c "o" comparte sus valores con \c "*this"
    bool same( const Matrix_Map & o ) const { return this == &o; }
private:
    void add( const Matrix_Map & );
    void substract( const Matrix_Map & );
    void multiply( const Matrix_Map &, const Matrix_Map & );
public:
    friend Matrix_Map operator + (const Matrix_Map& A, const Matrix_Map& B)
    { Matrix_Map Res = A; Res.add(B); return Res; } ///< Retorna \c A+B
    friend Matrix_Map operator - (const Matrix_Map& A, const Matrix_Map& B)
    { Matrix_Map Res = A; Res.substract(B); return Res; } ///< Retorna \c A-B
    friend Matrix_Map operator * (const Matrix_Map& A, const Matrix_Map& B)
    { Matrix_Map Res;  Res.multiply(A, B); return Res; } ///< Retorna \c A*B
public:
    reference operator () (unsigned, unsigned);
    const_reference operator () (unsigned, unsigned) const;
public:
    void reSize( unsigned, unsigned);
    void reShape(unsigned, unsigned);
public:
    void setDefault(const E& same);
    /// Valor almacenado en la mayor parte de la \c Matrix_Map
    const E& getDefault() { return m_same; }
    /// Ajusta la matriz para que pueda almacenar \c n valores diferentes a \c getDefault().
    void reserve(size_type _Count);
    void reSize(unsigned newsize);
    void clear();

    template<class T> friend bool  check_ok( const Matrix_Map<T>& M );
    template<class T> friend class test_Matrix_Map; ///< Datos de prueba para la clase
private:
    typedef
        std::map<
            std::pair<unsigned,unsigned>,
            E
        // ,std::less< std::pair<unsigned,unsigned> >
        > Diccionario;
    Diccionario m_dicc; ///< Vector que contiene los valores de la matiz.
    unsigned  m_rows; ///< Cantidad de filas de la matriz
    unsigned  m_cols; ///< Cantidad de columnas de la matriz
    E         m_same; ///< Valor almacenado en la mayor parte de la \c Matrix_Map
}; // Matrix_Map

/** Verifica la invariante de la clase.
    - El campo \c m_same indica cuál es el valor que se repite más en toda la matriz.
    - Usualmente \c m_same es el neutro aditivo \c value_type().
    - No existe un constructor explícito para darle a \c m_same su valor inicial, que
      es siempre inicializado en \c value_type(). Para cambiarlo es necesario invocar
      el método \c setgetDefault().
    - El vector \c m_dicc contiene las listas de valores almacenados en la matriz.
      Cualquiera de estas listas puede estar vacía; aún todas pueden ser listas vacías.
    - La cantidad de columnas de la matriz es el valor fijo es \c m_cols.
    - El largo de las listas de valores cambia cuando se hace referencia a un valor M(i,j)
      mediante la versión que no es \c const del \c operator()(i,j). O sea, que es
      ese método el encargado de agregar valores en \c m_dicc, pues el operador
      homónimo \c const operator()(i,j) nunca agrega nada y, como es \c const, en
      general retorna una referencia constante a \c m_same.
    - Es posible que la matriz tenga dimensiones nulas, lo que implica que el vector
      \c m_dicc está vacío.
    - No hace falta alacenar la cantidad de valores de la matriz porque se siempre es
      \c m_VL.size().
      el valor \c 0 (cero) al campo \c m_capacity.

    \par <em>Rep</em> Modelo de la clase
\code
std::map<std::pair<unsigned,unsigned>,  E  , std::less<std::pair<unsigned,unsigned> > >

   m_dicc      IT
               /\
              /  \
             /    \
        First      second
      _____________________
     /                     \
    | std::pair <0,1>, 'a' |
     \_____________________/        0 1 2 3 4 5 6
           M(0,1)=='a'          0 / - a - - - - - \
                                1 | - - - - - - - |
      _____________________     2 | - - - - - - - |
     /                     \    3 | - - - - - b - |
    | std::pair <3,5>, 'b' |    4 \ - - - - - - - /
     \_____________________/
           M(3,5)=='b'

        m_same == '-'   m_rows == 5   m_cols == 7
\endcode

    - http://www.di-mare.com/adolfo/binder/c03.htm#k1-Rep
    \remark
    Libera al programador de implementar el método \c Ok()
    - http://www.di-mare.com/adolfo/binder/c04.htm#sc11
*/
template<class T>
bool check_ok( const Matrix_Map<T>& M ) {
    if ( ! (&M != 0 ) ) {
        /// - Invariante: ningún objeto puede estar almacenado en la posición nula.
        return false;
    }
    if ( M.m_cols == 0 ) { // m_cols es el "marcador" que determina el valor de los demás
        if ( ! ((M.m_dicc).empty()) ) {
            return false; /// - Invariante: <code>(m_cols == 0) <==> (m_dicc.empty())</code>
        }
    }

    if ( ! check_ok( M.m_same ) ) {
        return false; /// - Invariante: <code>check_ok( M.m_same )</code>
    }
    for (unsigned i=0; i<M.rows(); ++i) {
        for (unsigned j=0; j<M.cols(); ++j) {
            if ( ! check_ok( M(i,j) ) ) {
                return false; /// - Invariante: <code>check_ok( M(i,j) )</code>
            }
        }
    }
    return true;
}

/// Define el escalar que por defecto está en todas las entradas de la \c Matrix_Map.
/// - Si <code>same != getDefault()</code> la matriz queda vacía.
/// - De lo contrario, nada ocurre.
template<class E>
inline void Matrix_Map<E>::setDefault(const E& same) {
    if ( m_same != same ) {
        m_same = same;
    }
}
/** \fn template <class E>inline Matrix_Map<E>::Matrix_Map(const value_type V);

    \brief Constructor a partir de \c Matrix_Map<E>::value_type(V).

    - La matriz resultante es una matriz escalar, de dimensiones 1x1,
      y su valor es \c "V"
*/

/** Constructor de vector.
    - Obtiene suficiente memoria dinámica para almacenas los
      <code> n * m </code> valores de la matriz.
    - Si \c "value_type" tiene un constructor de vector, lo
      usar para inicializar cada uno de los valores de la matriz;
      de lo contrario, los deja tal cual están en la memoria.
    - Si \c "value_type" es uno de los tipos escalares básicos,
      como lo son \c int o \c float, los valores almacenados
      en la matriz quedan tal cual están y no son inicializados.
    \pre
     -  <code> m * n > 0  </code>
     -  <code> (m > 0) && (n > 0) </code>
*/
template <class E>
inline Matrix_Map<E>::Matrix_Map(unsigned m, unsigned n)
    : m_dicc(), m_rows(m), m_cols(n), m_same()
{
    if (m == 0 || n == 0) {
        m_cols = 0;
        m_rows = 0;
    }
//  m_same = E(); // Usa el número "cero" como neutro tipo "E"
}
/// constructor de copia
template <class E>
inline Matrix_Map<E>::Matrix_Map(const Matrix_Map<E>& o) {
    copy( o );
    return;
}

/// Destructor
template <class E>
inline Matrix_Map<E>::~Matrix_Map() { }

template <class E>
bool Matrix_Map<E>::equals( const Matrix_Map & o ) const {
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
    - Luego de la copia, cuando el valor de \c "o" cambia, el de \c "*this" no cambiará,
      y viceversa, pues la copia es una copia profunda; no es superficial.
    - Si \c "*this" es \c "o" entonces su valor no cambia.
    - Después de esta operación siempre ocurre que <code> *this == o </code>.

    \par Complejidad:
         O( <code>  rows() * cols() </code> )

    \returns *this
    \see http://www.di-mare.com/adolfo/binder/c04.htm#sc05
*/
template <class E>
Matrix_Map<E>& Matrix_Map<E>::copy( const Matrix_Map<E> &o ) {
    if (this == &o) { // evita auto-borrado
        return *this;
    }
    this->m_dicc   = o.m_dicc;
    this->m_cols = o.cols();
    this->m_rows = o.rows();
    this->m_same = o.m_same;
    return *this;
}

/** Traslada el valor de \c "o" a \c "*this".
  - El valor anterior de \c "*this" se pierde.
  - El nuevo valor de \c "*this" es el que \c "o" tuvo.
  - \c "o" queda en el estado en que lo dejaría \c Erase().
  - Si \c "*this" es \c "o" entonces su valor no cambia.
  - En general, después de esta operación casi
    nunca ocurre que <code> (*this == o) </code>

    \par Complejidad:
         O( <code>  rows() * cols() </code> )

    \returns \c *this

    \see http://www.di-mare.com/adolfo/binder/c04.htm#sc07
*/
template <class E>
Matrix_Map<E>& Matrix_Map<E>::move( Matrix_Map<E> &o ) {
     if (this == &o) { // evita auto-borrado
        return *this;
    }
    this->clear();

    for (
        typename Diccionario::iterator it = o.m_dicc.begin();
        it != o.m_dicc.end();
        it++
    ) {
            this->m_dicc[ it->first ]= it->second;
            this->m_dicc.insert( Diccionario::value_type( it->first , it->second ) );
    }
    this->m_cols = o.m_cols;
    this->m_rows = o.m_rows;
    this->m_same = o.m_same;
    o.clear();
    return *this;
}

/** Intercambia los valores de \c "*this" y \c "o".
    - Debido a que algunos métodos retornan copias del valor de \c "*this" en
      lugar de una referencia, como ocurre con \c Matrix_Map::Child(), a veces
      \c swap() no tiene el resultado esperado por el programador.
    - Por ejemplo, si se invoca <code> T.Child(i). swap( T.Child(j) ) </code>
      el resultado no es intercambiar los hijos, sino más bien intercambiar
      los valores de los sub-árboles temporales \c T.Child(i) y \c T.Child(j).
      La forma correcta de intercambiar hijos es usar \c Graft().

      \par Complejidad:
         O( \c 1 )

    \returns *this

    \see http://www.di-mare.com/adolfo/binder/c04.htm#sc08
*/
template <class E>
inline Matrix_Map<E>& Matrix_Map<E>::swap( Matrix_Map<E> &o ) {
    std::swap( this->m_dicc , o.m_dicc );
    std::swap( this->m_cols , o.m_cols );
    std::swap( this->m_same , o.m_same );
    std::swap( this->m_rows , o.m_rows );
    return *this;
}

/** Le cambia las dimensiones a la matriz.
    - En algunos casos los valores almacenados en la matriz no quedan
      todos iguales a \c Matrix_Map<E>::value_type().
    \pre <code> (m > 0) && (n > 0) </code>
*/
template <class E>
void Matrix_Map<E>::reSize(unsigned m, unsigned n) {
    if ( m == 0 || n == 0) {
        return;
    }

    m_rows = m;
    m_cols = n;
    m_dicc.clear();
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
    |   inicializaría CERO == Matrix_Map<E>::value_type().    |
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
*/
template <class E>
inline void Matrix_Map<E>::reShape(unsigned m, unsigned n) {
    if ( m * n == m_rows * m_cols ) {
        reSize(n,m);
    }
}

/// Deja el valor de \c *this en el valor en que lo
/// inicializa el constructor de vector.
template <class E>
void Matrix_Map<E>::clear() {
        m_dicc.clear();
        m_same = value_type();
        m_rows = m_cols = 1;
}

/// Retorna una referencia al elemento [i,j] de la matriz ( \c const ).
/// - \c M(i,j) significa lo que en arreglos se denota con \c M[i][j].
/// - <code>val = M(i,j); // M(i,j) es un "rvalue" (const)</code>
template <class E>
const E& Matrix_Map<E>::operator () (unsigned i, unsigned j) const {
    assert( "Matrix_Map<E>::operator()()" && (i < rows()) );
    assert( "Matrix_Map<E>::operator()()" && (j < cols()) );

    typename Diccionario::const_iterator it;
    std::pair<unsigned,unsigned> i_j (i,j);
    it = m_dicc.find(i_j);
    if ( it == m_dicc.end() ) {
        return m_same;
    }
    else {
        return it->second;
    }

/*  NOTA
    Como este método es "const", de antemano se sabe que el programador no puede
    usarlo para modificar el valor de la matriz. Por eso, aunque el valor
    retornado sea una referencia a valor común por defecto m_same, de antemano
    el compilador asegura que ese valor no será modificado.

    Sin embargo, cuando el programador usa el método homónimo operator()(i,j)
    que no es "const", es posible que el valor retornado sí sea modificado.
    En ese caso, ya no es correcto retornar una referencia al valor común "m_same".
    - Por eso, cuando se usa el hace referencia en el otro operator()(i,j) es
      necesario agregar una entrada en los vectores paralelos en aquellos casos
      en que no existe un valor diferente a "m_same" para (i,j).
    - Esto quiere decir que sí es posible que al utilizar la versión modificable
      de operator()(i,j) quede en el vector "m_dicc" un valor que es igual a
      "m_same". En el caso peor, podría ocurrir que todos los valores almacenados
      en el vector "m_dicc" sean todos iguales a "m_same".
    - Una forma de corregir esta anomalía es "revisar después" si existe un valor
      en el vector "m_dicc" que es igual a "m_same". Una forma eficiente de
      hacerlo es mantener el el Rep un puntero "m_last_change" que apunte al
      último valor "m_dicc" que la versión modificable de operator()(i,j) retornó.
      En la siguiente invocación a operator()(i,j), se puede verificar si hubo un
      ese valor anterior fue cambiado de manera que tiene almacenado "m_same".
*/
}

/// Retorna una referencia al elemento [i,j] de la matriz.
/// - \c M(i,j) significa lo que en arreglos se denota con \c M[i][j].
/// - <code>M(i,j) = val; // M(i,j) es un "lvalue" (modificable)</code>
template <class E>
inline E& Matrix_Map<E>::operator() (unsigned i, unsigned j) {
    assert( "Matrix_Map<E>::operator()()" && (i < rows()) );
    assert( "Matrix_Map<E>::operator()()" && (j < cols()) );

    // Busca al elemento M(i,j)
    typename Diccionario::iterator it;
    std::pair<unsigned,unsigned> i_j (i,j);
    it = m_dicc.find(i_j);
    if ( it == this->m_dicc.end() ) {
        std::pair<typename Diccionario::iterator,bool> IT;
        IT = this->m_dicc.insert (
            typename Diccionario::value_type (
                std::pair<unsigned,unsigned>(i,j),
                m_same
            )
        );
        it = IT.first;
    //  m_dicc[i_j] = m_same;
    }

    return it->second; // m_dicc[i_j];
}

/// Le cambia el tamaño máximo posible a la matriz.
/// - Le aumenta la cantidad de valores diferentes a \c getDefault().
/// - No hace nada cuando <code>size() < newsize</code>.
template <class E>
inline void Matrix_Map<E>::reSize(unsigned newsize) {
}

/** Le suma a \c "*this" la matriz \c "O".
    \pre
    - \c "*this" y \c "O" deben tener las mismas dimensiones
    - <code> rows() == O.rows() && cols() == O.cols() </code>

    \par Complejidad:
         O( <code> rows() * cols() </code> )

    \remarks
    - Esta es la implementación de Matrix_Map<E> operator+( Matrix_Map<E>&, Matrix_Map<E> )
    - El compilador tiene problemas en compilar un función amiga (<em>"friend"</em>)
      definida con plantillas si esa función amiga no está definida (implementada)
      dentro de la declaración de la clase. Para solventar esta deficiencia existe
      este método que realiza el trabajo, aunque es poco cómodo de usar.
*/
template <class E>
void Matrix_Map<E>::add( const Matrix_Map<E> & O ) {
    // verifica que las dos matrices sean del mismo tamaño
    assert( "Matrix_Map<E>::add()" && (rows() == O.rows()) && (cols() == O.cols()) );

    // Como las matrices son del mismo tamaño basta sumarlas entrada por entrada.
    for ( unsigned i=0 ; i<rows() ; ++i ) {
        for ( unsigned j=0 ; j<cols() ; ++j ) {
            this->operator()(i,j) += O(i,j);
        }
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
    - Esta es la implementación de Matrix_Map<E> operator-( Matrix_Map<E>&, Matrix_Map<E> )
    - El compilador tiene problemas en compilar un función amiga (<em>"friend"</em>)
      definida con plantillas si esa función amiga no está definida (implementada)
      dentro de la declaración de la clase. Para solventar esta deficiencia existe
      este método que realiza el trabajo, aunque es poco cómodo de usar.
*/
template <class E>
void Matrix_Map<E>::substract( const Matrix_Map<E> & O ) {
    // verifica que las dos matrices sean del mismo tamaño
    assert( "Matrix_Map<E>::substract()" && (rows() == O.rows()) && (cols() == O.cols()) );

    for ( unsigned i=0 ; i<rows() ; ++i ) {
        for ( unsigned j=0 ; j<cols() ; ++j ) {
            this->operator()(i,j) -= O(i,j);
        }
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
         O( <code> A.cols() * B.cols() * A.cols() * A.capacity() * B.capacity() </code> )

    \remarks
    - Esta es la implementación de Matrix_Map<E> operator*( Matrix_Map<E>&, Matrix_Map<E> )
    - El compilador tiene problemas en compilar un función amiga (<em>"friend"</em>)
      definida con plantillas si esa función amiga no está definida (implementada)
      dentro de la declaración de la clase. Para solventar esta deficiencia existe
      este método que realiza el trabajo, aunque es poco cómodo de usar.
*/
template <class E>
void Matrix_Map<E>::multiply( const Matrix_Map<E> & A, const Matrix_Map<E> & B ) {
    // Verifica que las matrices se puedan multiplicar
    assert( (A.cols() == B.rows()) && " => Matrix_Map<E>::multiply()" );

    reSize( A.rows(), B.cols() );
    value_type sum;
    for (unsigned i=0; i<rows(); i++) {
        for (unsigned j=0; j<cols(); j++) {
            sum = E();
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
std::ostream& operator<<(std::ostream& COUT, const Matrix_Map<E>& M) {
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
std::istream& operator>>(std::istream& CIN, Matrix_Map<E>& M) {
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
// entre Matrix.h y Matrix_Map.h
// #define   Matrix Matrix_Map
// #include "Matrix_Lib.h"
// #undef    Matrix

#include "Matrix_Lib.h"

#endif // Matrix_Map_h
// EOF: Matrix_Map.h
