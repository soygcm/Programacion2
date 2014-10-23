// dirArbol.cpp  (C) 2005 adolfo@di-mare.com

/** \file  dirArbol.cpp
    \brief Recursivamente lista los nombres de los subdirectorios..

    \author Adolfo Di Mare <adolfo@di-mare.com>
    \date   2008
*/

#include "readDir.h"

#include <iostream> // cout
#include <iomanip> // setw()
#include <string>

/// Graba en \c std::cout todos los nombres de archivos del directorio actual.
/// - Ejemplo de uso de la clase \c readDir.
void dirRecursivo( bool flag, const std::string& dir , int indent ) {{ // test::dirRecursivo()
    if ( dir.empty() ) {
        return;
    }
    if ( dir=="." || dir==".." ) {
        return;
    }
    for ( int i=0; i<indent; ++i ) { std::cout << ' '; }
    std::cout << "<-(1)-* " << dir << std::endl;

    readDir hDir( dir + "/*.*" );
    while ( hDir.hasNext() ) {
        hDir.next(); // siguiente nombre de archivo
        if ( hDir.isSubDir() ) {
            bool    salga  = ( 0 == strcmp( hDir.getName(), "."  ) );
            salga = salga || ( 0 == strcmp( hDir.getName(), ".." ) );
            if ( salga ) { continue; }
            for ( int i=0; i<indent; ++i ) { std::cout << ' '; }
            std::cout << "<-(2)-* " << dir << std::endl;
            if ( flag ) {
                dirRecursivo( false, /*dir + '\\' + */ /* */ hDir.getName() , indent+2 );
            }
            else {
                dirRecursivo( false, dir + '\\' + /* */ hDir.getName() , indent+2 );
            }
        }
    }
}}

/** Programa principal.
    - Es conveniente para ver cómo usar las versiones Borland y Microsoft
      de las rutinas de lectura de directorios.
*/
int main( int argc, char* argv[] ) {
    using namespace std;
//  const char* exe = argv[0];
    std::string dir = ( argc >= 2 ? argv[1] : "*.*" );
//  std::cout << dir << std::endl;
    std::cout << '[' << dir << ']' << std::endl;
    readDir hDir( dir );
    while ( hDir.hasNext() ) {
        hDir.next(); // siguiente nombre de archivo
        if ( hDir.isSubDir() ) {
            std::cout << "*-(0)->" << hDir.getName() << endl;
            dirRecursivo( false, hDir.getName(), 2 );
        }
        #if 0
        #endif
    }
    return 0;
}

// EOF: dirArbol.cpp
