// str2list.h (C) 2011 adolfo@di-mare.com

/** \file  str2list.h
    \brief Funciones para crear listas y matrices a partir de hileras.

    \author Adolfo Di Mare <adolfo@di-mare.com>
    \date   2011
*/

#ifndef   str2list_h
#define   str2list_h   ///< Evita la inclusión múltiple

#include <list>    // std::list<>
#include <string>  // std::string
#include <cstdlib> // atoi() atol() atof()

/** Retorna \c true para todos los caracters considerados como
    espacios en blanco por \c str2list().
    - { <code> , () <> {} [] </code> } && \c isspace().
    - El punto \c char('.') no es considerado espacio en blanco
      ( pues forma parte de un número flotante: \c 1.0e22 ).
    - <code> false == iswhitespace_str2list( '.' ) </code>.

    \dontinclude test_str2list.cpp
    \skipline    test::iswhitespace()
    \until       }}
    \see         test_str2list::test_iswhitespace()
*/
template <class CHAR>
bool iswhitespace_str2list( CHAR ch ) {
    if ( ch==',' || isspace(ch) ) { return true; }

    if ( ch=='(' || ch==')' ) { return true; }
    if ( ch=='<' || ch=='>' ) { return true; }
    if ( ch=='{' || ch=='}' ) { return true; }
    if ( ch=='[' || ch==']' ) { return true; }
    return false;
//  NOTA: implementado con plantillas para evitar usar el archivo str2list.cpp
}

/** Almacena en \c "L<>" la lista de hileras construida en base a \c "str".
    - En \c "str" están los valores que serán almacenados en \c "L<>" separados
      por varios delimitadores.
    - Como delimitadores se usan los caracteres blancos y la coma.
    - Como delimitadores se usan los caracteres \c ispace() y \c ','.
    - El punto \c '.' no se interpreta como delimitador.
    - Los caracteres { <code> , () <> {} [] </code> } son interpretados como
      blancos y no verifica que estén balanceados o en parejas.
    - Ignora el paréntesis del principio y del final de \c "str".
    - Si al principio encuentra el caracter \c '|' procesa la hilera hasta que
      encuentra el siguiente caracter \c '|'.
    - No examina lo que está después de la barrita final \c '|'.
    - Siempre modifica el valor de \c "L<>".
    - Retorna la última posición de \c "str" que fue examinada.

    \dontinclude test_str2list.cpp
    \skipline    test::str2list::list()
    \until       }}
    \see         test_str2list::str2list_list()
*/
template <class STRING>
const char* str2list( std::list<STRING>& L , const char* str ) {
    L.clear();
    if ( str == 0) { return 0; }
    if (*str == 0) { return str; }
    while ( iswhitespace_str2list(*str) ) { ++str; } // skip leading blanks
    if (*str=='|') { ++str; } // skip first '|'

    std::string val; // next value to store into L
    const bool IN_WHITESPACE = false; const bool IN_STRING = !IN_WHITESPACE;
    bool state = IN_WHITESPACE;
    while ( (*str != 0) && (*str != '|') ) {
        if ( state == IN_WHITESPACE ) {
            if ( ! iswhitespace_str2list(*str) ) {
                state = IN_STRING;
                val = *str;
            }
        }
        else { // assert( state == IN_STRING );
            if ( iswhitespace_str2list(*str) ) {
                L.push_back( val );
                val.clear();
                state = IN_WHITESPACE;
            }
            else {
                val += *str;
            }
        }
        ++str;
    }
    if ( !val.empty() ) { L.push_back(val); }
    return str;
//  NOTA: implementado con plantillas para evitar usar el archivo str2list.cpp
}


/** Usa \c str2list(L,str) para retornar la lista de valores de \c "str".

    \dontinclude test_str2list.cpp
    \skipline    test::str2list::example()
    \until       }}
    \see         test_str2list::str2list_example()
*/
inline std::list<std::string> str2list( const char* str ) {
    std::list<std::string> L;
    str2list(L,str);
    return L;
}

/** Retorna la lista de valores construida en base a \c "str".
    Usa \c str2list() junto con \c ATON_wrap::ATON().
    \tparam N tipo de dato: { <code> int long double </code> }
    \tparam ATON_wrap Empaque \c ATON() alrededor del convertidor:
                 { <code> atoi() atol() atof() </code> }.
*/
template <class N, class ATON_wrap>
std::list<N> str2list_wrap( const char* str ) {
    std::list<std::string> L;
    std::list<N> RES;
    str2list(L,str);

    std::list< std::string >::const_iterator it;
    for ( it= L.begin(); it!=L.end(); ++it ) {
        RES.push_back( ATON_wrap::ATON(it->c_str()) );
    }
    return RES;
}

/// Empaque \c ATON() alrededor de la función \c atoi().
struct str2list_int_wrap {
    /// Retorna \c atoi(str).
    static int ATON( const char * str ) { return atoi(str); }
};

/// Empaque \c ATON() alrededor de la función \c atol().
struct str2list_long_wrap {
    /// Retorna \c atol(str).
    static long ATON( const char * str ) { return atol(str); }
};

/// Empaque \c ATON() alrededor de la función \c atof().
struct str2list_double_wrap {
    /// Retorna \c atof(str).
    static double ATON( const char * str ) { return atof(str); }
};

inline std::list<int> intlist( const char* str ) {
    return str2list_wrap<int, str2list_int_wrap>( str );
} ///< Sinónimo de \c makeList_int().

/** Retorna la lista de enteros \c (int) construida en base a \c "str".
    Usa \c str2list() junto con \c atoi().

    \dontinclude test_str2list.cpp
    \skipline    test::intlist()
    \until       }}
    \see         test_str2list::test_intlist()
*/
inline std::list<int> makeList_int( const char* str ) {
    return str2list_wrap<int, str2list_int_wrap>( str );
}


inline std::list<long> longlist( const char* str ) {
    return str2list_wrap<long, str2list_long_wrap>( str );
} ///< Sinónimo de \c makeList_long().

/** Retorna la lista de enteros \c (long) construida en base a \c "str".
    Usa \c str2list() junto con \c atol().

    \dontinclude test_str2list.cpp
    \skipline    test::makeList_long()
    \until       }}
    \see         test_str2list::test_makeList_long()
*/
inline std::list<long> makeList_long( const char* str ) {
    return str2list_wrap<long, str2list_long_wrap>( str );
}


/** Retorna la lista de flotantes construida en base a \c "str".
    Usa \c str2list() junto con \c atof().

    \dontinclude test_str2list.cpp
    \skipline    test::doublelist()
    \until       }}
    \see         test_str2list::test_doublelist()
*/
inline std::list<double> doublelist( const char* str ) {
    return str2list_wrap<double, str2list_double_wrap>( str );
}

std::list<char> charlist( const char* str ) {
    std::list<std::string> Lstr;
    std::list<char> Lchar;
    str2list(Lstr,str);

    std::list< std::string >::const_iterator it;
    for ( it= Lstr.begin(); it!=Lstr.end(); ++it ) {
        if  ( (*it)[0] == '\'' ) {
            if ( it->size() == 1 ) {
                Lchar.push_back( ' ' ); // blank
                ++it;
            }
            else {
                Lchar.push_back( (*it)[1] );
            }
        }
        else {
            Lchar.push_back( (*it)[0] );
        }
    }

    return Lchar;
} ///< Sinónimo de \c makeList_char().

/** Retorna la lista de caracteres \c (char) construida en base a \c "str".
    Usa \c str2list() pero se brinca las comillas simples \c (').
    - No necesita que cada letra venga entre comillas simples.
    - Si alguno de los valores tiene más de un caracter, usa el primero e ignora los demás.

    \dontinclude test_str2list.cpp
    \skipline    test::charlist()
    \until       }}
    \see         test_str2list::test_makeList_char()
*/
inline std::list<char> makeList_char( const char* str ) {
    return charlist( str );
}

/** Almacena en \c "M<<>>" la matriz de hileras construida en base a \c "str".
    - Cada renglón de la matriz debe estar entre caracteres \c '|'.
    - Para obtener cada renglón se usa \c str2list().
    - La matriz se retorna como una lista de listas de hileras.
    - Ignora sublistas nulas pues \c "M<<>>" nunca contiene sublistas nulas.
    - Siempre modifica el valor de \c "M<<>>".
    - Retorna la longitud de la sublista más grande de \c "M<<>>" (para
      calcular cuántas columnas tiene la matriz).

    \dontinclude test_str2list.cpp
    \skipline    test::str2matrix()
    \until       }}
    \see         test_str2list::test_str2matrix()
*/
template <class STRING>
size_t str2matrix( std::list< std::list<STRING> >& M , const char* str ) {
    M.clear();
    if ( str == 0) { return 0; }
    if (*str == 0) { return 0; }

    size_t mx = 0; // max sublist length
    M.push_back( std::list<STRING>() );
    while ( *str != 0 ) {
        typename std::list<STRING> & L = M.back(); // L: last list in M<<>>
        str = str2list( L , str );
        if ( *str!=0 ) { str++; }
        mx = ( L.size()<mx ? mx : L.size() ); // keep max
        if ( ! M.back().empty() ) {
            M.push_back( std::list<STRING>() );
        }
    }
    if ( M.back().empty() ) { M.pop_back(); }
    return mx;
//  NOTA: implementado con plantillas para evitar usar el archivo str2list.cpp
}

/** Almacena en \c MATRIX[][] la matriz construida en base a \c "str".
    Usa \c str2matrix() junto con \c ATON_wrap::ATON().
    \tparam N tipo de dato: { <code> int long double </code> }
    \tparam ATON_wrap Empaque \c ATON() alrededor del convertidor:
                 { <code> atoi() atol() atof() </code> }
    \tparam Nrows  Cantidad de filas de \c MATRIX[][].
    \tparam Ncols  Cantidad de columnas de \c MATRIX[][].
*/
template <class N, class ATON_wrap, unsigned Nrows, unsigned Ncols>
void matrix2list_wrap( N MATRIX[Nrows][Ncols] , const char* str ) {
    std::list< std::list<std::string> > Mlist;
    size_t NCOLS = str2matrix( Mlist , str ); // Mlist <== str
    size_t NROWS = Mlist.size();

    NROWS = ( NROWS<Nrows ? NROWS : Nrows ); // take min(NROWS,Nrows)
    NCOLS = ( NCOLS<Ncols ? NCOLS : Ncols ); // take min(NCOLS,Ncols)

    typename std::list< std::list<std::string> > :: const_iterator it;
    typename            std::list<std::string>   :: const_iterator jt;

    it = Mlist.begin();
    for ( size_t i=0; i<NROWS; ++i,++it ) {
        jt = it->begin();
        for ( size_t j=0; j<NCOLS; ++j ) {
            if ( jt==it->end() ) { MATRIX[i][j] = 0; }
            else {
                MATRIX[i][j] = ATON_wrap::ATON( jt->c_str() );
                jt++;
            }
        }
    }
}

/** Almacena en \c M[][] la matriz de enteros \c (int) construida en base a \c "str".
    Usa \c str2matrix() junto con \c atoi().

    \dontinclude test_str2list.cpp
    \skipline    test::intmatrix()
    \until       }}
    \see         test_str2list::test_intmatrix()
*/
template <unsigned Nrows, unsigned Ncols>
inline void intmatrix( int M[Nrows][Ncols] , const char* str ) {
    return matrix2list_wrap<int, str2list_int_wrap, Nrows, Ncols>( M , str );
}

/** Almacena en \c M[][] la matriz de enteros \c (long) construida en base a \c "str".
    Usa \c str2matrix() junto con \c atol().
    \dontinclude test_str2list.cpp
    \skipline    test::longmatrix()
    \until       }}
    \see         test_str2list::test_longmatrix()
*/
template <unsigned Nrows, unsigned Ncols>
inline void longmatrix( long M[Nrows][Ncols] , const char* str ) {
    return matrix2list_wrap<long, str2list_long_wrap, Nrows, Ncols>( M , str );
}

/** Almacena en \c M[][] la matriz de flotantes construida en base a \c "str".
    Usa \c str2matrix() junto con \c atof().
    \dontinclude test_str2list.cpp
    \skipline    test::doublematrix()
    \until       }}
    \see         test_str2list::test_doublematrix()
*/
template <unsigned Nrows, unsigned Ncols>
inline void doublematrix( double M[Nrows][Ncols] , const char* str ) {
    return matrix2list_wrap<double,str2list_double_wrap,Nrows,Ncols>( M,str );
}

#include <cstring> // memset()

/** Almacena en \c M[][] la matriz de caracteres construida en base a \c "str".
    Usa \c str2matrix() pero se brinca las comillas simples \c (').
    - No necesita que cada letra venga entre comillas simples.
    - Si alguno de los valores tiene más de un caracter, usa el primero e ignora los demás.

    \dontinclude test_str2list.cpp
    \skipline    test::charmatrix()
    \until       }}
    \see         test_str2list::test_charmatrix()
*/
template <unsigned Nrows, unsigned Ncols>
inline void charmatrix( char M[Nrows][Ncols] , const char* str ) {
    std::list< std::list<std::string> > Mstr;
    str2matrix(  Mstr, str );
    memset( M, ' ', sizeof(M) ); // llena la matriz de blancos

    unsigned i,j; // paralelos (i<->it) && (j<->jt)
    std::list< std::list<std::string> >::const_iterator it;
    std::list<std::string>::const_iterator jt;
    for ( i=0,it=Mstr.begin(); (i<Nrows && it!=Mstr.end()); ++i,++it ) {
        const std::list<std::string>& Mrenglon = (*it);
        for ( j=0,jt=Mrenglon.begin(); (j<Ncols && jt!=Mrenglon.end()); ++j,++jt ) {
            if  ( (*jt)[0] == '\'' ) {
                if ( jt->size() == 1 ) {
                    M[i][j] = ' '; // blank
                    ++jt;
                }
                else {
                    M[i][j] = (*jt)[1];
                }
            }
            else {
                M[i][j] = (*jt)[0];
            }
        }
    }
    return;
}

#endif // str2list_h

// EOF: str2list.h
