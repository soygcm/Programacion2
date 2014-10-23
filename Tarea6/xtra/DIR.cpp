// DIR.cpp  (C) 2005 adolfo@di-mare.com

/** \file  DIR.cpp
    \brief Programa que muestra cómo leer los nombres de archivos
           del directorio actual.

           - \c _MSC_VER: Identifica al compilador C++ de Microsoft.
           - \c __BORLANDC__: Identifica al compilador C++ de Borland.
           - \c __GNUC__: Identifica al compilador GNU C++

    \author Adolfo Di Mare <adolfo@di-mare.com>
    \date   2005
*/

int WINDOWS_main(int argc, char* argv[]);
int Borland_main(int argc, char* argv[]);
int DOS_main(int argc, char* argv[]);

/** Programa principal.
    - Es conveniente para ver cómo usar las versiones Borland y Microsoft
      de las rutinas de lectura de directorios.
*/
int main(int argc, char* argv[]) {
    #if defined( _MSC_VER ) || defined( __GNUC__ )
        return WINDOWS_main(argc, argv);
    #endif // _MSC_VER

    #ifdef __BORLANDC__
        int B = Borland_main(argc, argv);
        int D = DOS_main(argc, argv);
        return ( (B!=0) ? B : D );
    #endif // ! __BORLANDC__
} // main()



#if defined( _MSC_VER ) || defined( __GNUC__ )

#include <io.h>  // _findfirst(), _findnext(), etc.
#include <iostream>

/// Definido por la biblioteca C++ estándar
namespace std {} // Esta es la forma de lograr que Doxygen lo documente
using namespace std;

/// Versión "Microsoft C++" del programa que graba en la salida estándar los nombres de los archivos del directorio actual.
int WINDOWS_main(int argc, char* argv[]) {
    _finddata_t FILE_BLOCK;  // "struct" que contiene los datos de un archivo
    long hFile;

    if ( -1L == (hFile = _findfirst( "./*.*", &FILE_BLOCK )) ) {
        cout << "No hay archivos en el directorio\n";
    }
    else {
        do {
            cout << FILE_BLOCK.name;
            if( FILE_BLOCK.attrib & _A_SUBDIR ) {
                cout << " [DIR]";
            }
            cout << endl;
        } while ( 0 == _findnext( hFile, &FILE_BLOCK ) );
    }

    _findclose( hFile ); // You must call _findclose() after you are finished using either the _findfirst() or _findnext() function
    return 0;
} // main()

#endif // ! _MSC_VER

#ifdef __BORLANDC__

#include <dos.h> // para FA_DIREC
#include <dir.h>
#include <stdio.h>

/// Versión "Borland C++ v3.1" del programa que graba en la salida estándar los nombres de los archivos del directorio actual.
int Borland_main(int argc, char* argv[]) {
    struct ffblk FILE_BLOCK;
    int done;
    printf("Directory listing of ./*.*\n");

    int ALL =
        WILDCARDS | EXTENSION | FILENAME | DIRECTORY | DRIVE;
    done = findfirst("./*.*",&FILE_BLOCK, ALL);

    // OJO: Es en findfirst() adonde de definen los tipos de entrada
    //      del directorio a buscar

    while( !done ) {
        printf("  %s", FILE_BLOCK.ff_name);
        if( FILE_BLOCK.ff_attrib & FA_DIREC ) { // OJO: FA_DIREC es de <dos.h>
            printf(" [DIR]");
        }
        printf("\n");
        done = findnext(&FILE_BLOCK);
    }

    return 0;
} // Borland_main()

#include <dos.h>
#include <stdio.h>

/// Versión DOS para "Borland C++" del programa que graba en la salida estándar nombres de los nombres de los archivos del directorio actual.
int DOS_main(int argc, char* argv[]) {
    struct find_t FILE_BLOCK;
    int done;
    printf("Directory listing of ./*.*\n");

    int FA_ALL =
        FA_RDONLY | FA_HIDDEN | FA_SYSTEM | FA_LABEL | FA_DIREC | FA_ARCH;

    done = _dos_findfirst("./*.*",FA_ALL,&FILE_BLOCK);

    // OJO: Es en _dos_findfirst() adonde de definen los tipos de entrada
    //      del directorio a buscar

    while( !done ) {
        printf("  %s", FILE_BLOCK.name);
        if( FILE_BLOCK.attrib & FA_DIREC ) {
            printf(" [DIR]");
        }
        printf("\n");
        done = _dos_findnext(&FILE_BLOCK);
    }

    return 0;
} // DOS_main()

#endif // __BORLANDC__

// EOF: DIR.cpp
