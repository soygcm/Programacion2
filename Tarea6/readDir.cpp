// readDir.cpp  (C) 2005 adolfo@di-mare.com

/** \file  readDir.cpp
    \brief Implementación para \c readDir.h.

    \author Adolfo Di Mare <adolfo@di-mare.com>
    \date   2010
*/

#include "readDir.h"

#include <cstring> // strlen() + strchr()

/// Establece como máscara de búsqueda \c dir.
/// - También establece el valor \c hasNext().
void readDir::setMask( const char * dir ) {
/*  NOTA
    Cuando la hilera "dir" contiene un slash '/' setMask() la transforma
    en un '\' que es lo que Windows espera. En este caso, hay que crear
    una copia de "dir" en donde aparece esa modificación. Como este valor
    queda almacenado en memoria dinámica, al destruir el objeto o al
    cambiarle su valor es necesario antes retornar la memoria dinámica
    a la que "m_pszMask", pues contiene una copia modificada del valor
    original de "dir".
*/
    // "m_bHasSlash" es un sinónimo de "m_bDestroyPtr"
    bool& m_bHasSlash = m_bDestroyPtr; // nombre local para mejorar la legibilidad

    if ( m_bHasSlash ) {
        delete [] ( const_cast<char*>(m_pszMask)); // se vuela el buffer
    }

    if ( dir == 0 ) {
        m_bHasSlash = false;
        m_bHasNext  = false;
        return;
    }
    else if( *dir == 0 ) { // hilera nula
        m_bHasSlash = false;
        m_bHasNext  = false;
        return;
    }

    if ( 0 == strchr(dir, '/') ) { // el nombre tiene un "slash"
        m_bHasSlash = false; // el destructor no liberará m_pszMask
        m_pszMask   = dir;
    }
    else {
        m_bHasSlash = true;     // hay que usar otra hilera sin '/'
        unsigned N = strlen( dir );
        char* dst = new char[N+1];
        m_pszMask = dst;
        for ( unsigned i=0; i<=N; ++i ) { // copia la hilera
            if ( *dir == '/' ) {
                *dst = '\\';        // corrige el "slash"
            }
            else {
                *dst = *dir;        // copia las demás
            }
            ++dst; ++dir;
        }
    }

    #if defined( _MSC_VER ) || defined( __GNUC__ )
        m_handle   = _findfirst( m_pszMask , & m_FILE_BLOCK_NEXT );
        m_bHasNext = ( m_handle != -1L );
        if ( !m_bHasNext ) { _findclose(m_handle); }
    #endif
    #ifdef __BORLANDC__
        int ALL = WILDCARDS | EXTENSION | FILENAME | DIRECTORY | DRIVE;
        m_bHasNext = (0 == findfirst( m_pszMask , & m_FILE_BLOCK_NEXT , ALL ) );
    #endif
}

/// Establece como máscara de búsqueda \c dir.
/// - Si al final \c dir tiene 1 o más caracteres \c '\\' los elimina.
/// - Si al final \c dir tiene 1 o más caracteres \c '/' los elimina.
/// - Los caracteres \c '\\' y \c '/' al final producen funcionamiento erróneo.
/// - También establece el valor \c hasNext().
void readDir::setMaskWithChop( const char * dir ) {
    // "m_bHasSlash" es un sinónimo de "m_bDestroyPtr"
    bool& m_bHasSlash = m_bDestroyPtr; // nombre local para mejorar la legibilidad

    if ( m_bHasSlash ) {
        delete [] ( const_cast<char*>(m_pszMask)); // se vuela el buffer
    }
    if ( dir == 0 ) {
        m_bHasSlash = false;
        m_bHasNext  = false;
        return;
    }
    else if( *dir == 0 ) { // hilera nula
        m_bHasSlash = false;
        m_bHasNext  = false;
        return;
    }
    unsigned N = strlen( dir );
    unsigned i,numberChopped = N-1; // marca cuántos hay que eliminar del final

    for ( i=N-1; i>0; --i ) {
        if ( dir[i]!='\\' && dir[i]!='/' ) {
            break;
        }
    }
    if ( i==0 && (dir[0]=='\\' || dir[0]=='/') ) {
        m_bHasSlash = false;
        m_bHasNext  = false;
        return;
    }

    numberChopped = N-(i+1);

    if ( numberChopped==0 && 0 == strchr(dir, '/') ) {
        m_bHasSlash = false; // el destructor no liberará m_pszMask
        m_pszMask   = dir;
    }
    else {
        m_bHasSlash = true;     // hay que usar otra hilera sin '/'
        N -= numberChopped;
        char* dst = new char[N+1];
        m_pszMask = dst;
        for ( unsigned i=0; i<N; ++i ) { // copia la hilera
            if ( *dir == '/' ) {
                *dst = '\\';        // corrige el "slash"
            }
            else {
                *dst = *dir;        // copia las demás
            }
            ++dst; ++dir;
        }
        *dst = 0; // EOS
    }

    #if defined( _MSC_VER ) || defined( __GNUC__ )
        m_handle   = _findfirst( m_pszMask , & m_FILE_BLOCK_NEXT );
        m_bHasNext = ( m_handle != -1L );
        if ( !m_bHasNext ) { _findclose(m_handle); }
    #endif
    #ifdef __BORLANDC__
        int ALL = WILDCARDS | EXTENSION | FILENAME | DIRECTORY | DRIVE;
        m_bHasNext = (0 == findfirst( m_pszMask , & m_FILE_BLOCK_NEXT , ALL ) );
    #endif
}


/// Establece como máscara de búsqueda \c dir.
/// - También establece el valor \c hasNext().
void readDir::setMask( const std::string& dir ) {
    unsigned N = dir.length();
    char* dst = new char[N+1];
    dir.copy( dst, N,0 );
    dst[N] = 0; // EOS
    setMask( dst );
    if ( m_bDestroyPtr ) { // m_pszMask contiene una copia de "dir"
        delete [] dst;     // deja solo la copia de "m_pszMask"
    }
    else {
        // assert( dst == m_bDestroyPtr );
        m_bDestroyPtr = true; // se asegura que "dst==m_pszMask" sea destruido
    }
    // assert( m_pszMask == dst );
}

time_t readDir::getTimeWrite() const {
    #if defined( _MSC_VER ) || defined( __GNUC__ )
        return m_FILE_BLOCK.time_write;
    #endif
    #ifdef __BORLANDC__
        //  | 15.....11 | 10........5 | 4.......0 |
        //      Hours        Minutes    Seconds/2
        struct borland_time {
           unsigned secs_2    : 5;   // 0..4   (5 bits)
           unsigned minutes   : 6;   // 5..10  (6 bits)
           unsigned hours     : 5;   // 1..15  (5 bits)
        };
        //  | 15..........9 | 8.....5 | 4.......0 |
        //     Years-1980      Month       Day
        struct borland_date {
           unsigned day       : 5;   // 0..4   (5 bits)
           unsigned month     : 4;   // 5..8   (4 bits)
           unsigned year_1980 : 7;   // 9..15  (7 bits)
        };
        struct borland_time * t;
        struct borland_date * d;

        t = (borland_time*) ((void*)m_FILE_BLOCK.ff_ftime);
        d = (borland_date*) ((void*)m_FILE_BLOCK.ff_fdate);

        struct tm result; // time_t
        result.tm_sec   = t->secs_2 * 2;
        result.tm_min   = t->minutes;
        result.tm_hour  = t->hours;
        result.tm_mday  = -1;
        result.tm_mon   = d->month;
        result.tm_year  = d->year_1980 + 1980 - 1900;
        result.tm_wday  = -1;
        result.tm_yday  = -1;
        result.tm_isdst = -1;
        return mktime( & result );
    #endif
}

const char* readDir::getName() const {
    #if defined( _MSC_VER ) || defined( __GNUC__ )
        return m_FILE_BLOCK.name;
    #endif
    #ifdef __BORLANDC__
        return m_FILE_BLOCK.ff_name;
    #endif
}

int readDir::getMaxPath() {
    #if defined( _MSC_VER ) || defined( __GNUC__ )
        // _MAX_PATH is defined as (260) bytes.
        return    sizeof( ((_finddata_t*)(0))->name ) -1;
        // return sizeof(m_FILE_BLOCK.name)-1;
    #endif
    #ifdef __BORLANDC__
        return    sizeof( ((struct ffblk*)(0))->ff_name ) -1;
        // return sizeof(m_FILE_BLOCK.ff_name)-1;
    #endif
}

/// Retorna \c true si \c argname es un archivo.
/// - Retorna \c false si \c argname no existe.
/// - Retorna \c false si \c argname es un directorio.
bool isfile( const char* argname ) {
    readDir hFile( argname );
    if ( hFile.hasNext() ) {
        hFile.next();
        bool weird = hFile.isSystem() || hFile.isLabel() || hFile.isSubDir();
        return !weird;
    }
    return false;
}

/// Retorna \c true si \c argname es un directorio.
/// - Retorna \c false cuando no existe.
bool isdir( const char* argname ) {
    readDir hFile( argname );
    if ( hFile.hasNext() ) {
        hFile.next();
        return hFile.isSubDir();
    }
    return false;
}

/// Si \c file es un archivo, retorna su extensión.
/// - Para \c "dir/ARCHIVO.ext" retorna \c "ext" (sin el punto \c '.').
/// - Retorna \c (char*)(0) si \c file no es un archivo.
/// - Retorna \c (char*)(0) si no hay extensión.
const char* pathExt( const char* file ) {
    if ( !isfile(file)  ) { return 0; }
    return lastExt( file );
}

/// Retorna el sufijo de \c name que está después del último punto \c '.'.
/// - Para \c "dir/ARCHIVO.ext" retorna \c "ext" (sin el punto \c '.').
/// - Retorna \c (char*)(0) si el último caracter de \c name es un punto \c '.'.
/// - Retorna \c (char*)(0) si no hay extensión.
const char* lastExt( const char* name ) {
    const char *last = name + strlen( name );
    const char *ext = last;
    while ( ext != name ) {
        if ( *ext == '.' ) { ++ext; return ((ext==last) ? 0 : ext); }
        if ( *ext == '/' || *ext=='\\' ) { return 0; }
        --ext;
    }
    return 0;
}


#if 0 // programa de ejemplo

#include <iostream> // cout
#include <iomanip> // setw()

/// Graba en \c std::cout todos los nombres de archivos del directorio actual.
/// - Ejemplo de uso de la clase \c readDir.
void ejemplo() {{ // test::ejemplo()
    using namespace std;
    readDir hFile("../");
    hFile.setMask( "../*.*" ); // listado del directorio padre
    while ( hFile.hasNext() ) {
        hFile.next(); // siguiente nombre de archivo
        cout << ( hFile.isReadOnly() ? 'r' : '.' );
        cout << ( hFile.isArchive()  ? 'a' : '.' );
        cout << ( hFile.isSystem()   ? 's' : '.' );
        cout << ( hFile.isSubDir()   ? 'd' : '.' );
        cout << ( hFile.isHidden()   ? 'h' : '.' ) << ' ';
        {
            time_t t = hFile.getTimeWrite();
            struct tm ts = *( gmtime( & t ) ); // saca una copia ¡rápido!
            char  str[25];

            strftime( str, sizeof(str), "%Y", & ts );
            cout << str << '-'; // %Y ==> Year
            strftime( str, sizeof(str), "%m", & ts );
            cout << str << '-'; // %m ==> Month as a decimal number (01-12)
            strftime( str, sizeof(str), "%d", & ts );
            cout << str << ' '; // %d ==> Day of the month (01-31)

            strftime( str, sizeof(str), "%H", & ts );
            cout << str << ':'; // %H ==> Hour in 24h format (00-23)
            strftime( str, sizeof(str), "%M", & ts );
            cout << str << ':'; // %M ==> Minute (00-59)
            strftime( str, sizeof(str), "%S", & ts );
            cout << str << ' '; // %S ==> Second (00-61)
        }
        cout << ' ' << setw(10) << hFile.size();

        cout << ' ' << setw(00) << hFile.getName() << endl;
    }
}}

/** Programa principal.
    - Es conveniente para ver cómo usar las versiones Borland y Microsoft
      de las rutinas de lectura de directorios.
*/
int main(int argc, char* argv[]) {
    ejemplo();
    return 0;
}
#endif // programa de ejemplo

#if 0
> Al revisar estas líneas del readDir:
>    readDir hFile("..\\*.*");
>    hFile.setMask( "./Hijo/*.*" ); // listado del directorio padre
> sé que son para crear la máscara y abrir el archivo, pero no entiendo los
> argumentos que le esta enviando, ni cómo puedo abrir un directorio que no
> sea en el que esta en ese momento, porque ocupo abrir 2 directorios
> diferentes para hacer las comparaciones.

Cuando una ejecuta un programa debe hacerlo en algún directorio. Ese
directorio se llama el directorio actual (o current directory). Con
respecto al directorio actual es que se hacen las referencia a los
otros directorios.

.            ==> directorio actual
..           ==> directorio padres
hijo         ==> directorio hijo
.\hijo       ==> lo mismo: directorio hijo
.\hijo\nieto ==> Subdirectorio nieto del hijo
C:\          ==> raíz del disco C: [referencia fija]

Si quiero comparar los archivos del subirectorios Hijo con Nieta
puedo hacerlo así:
    readDir Hijo, Nieta;
    Hijo.setMask(  ".\\Hijo\\*.*" );
    Nieta.setMask( ".\\Hijo\Nieta\\*.*" );

Es necesario usar doble '\\' porque la letra '\\' es el caracter
de escape: '\n' es cambio de línea, '\t' es tabulador y usar
'\' un error, pues falta la segunda comilla: '\''.

Esto sirve para iterar sobre nombres de los archivos en los 2
directorios. Es necesario incluir el "*.*" para definir cuáles
archivos van a ser encontrados por los iteradores. En este caso,
"*.*" significa "cualquier nombre con cualquier extensión", pero
si solo necesito los empacados ".zip" usaría estos seteadores:
    readDir Hijo, Nieta;
    Hijo.setMask(  ".\\Hijo\\*.zip" );
    Nieta.setMask( ".\\Hijo\\Nieta\\*.zip" );

En Unix los nombres de los directorios se deben separar con '/'
por lo que readDir permite usar esta sintáxis:
    readDir Hijo, Nieta;
    Hijo.setMask(  "./Hijo/*.zip" );
    Nieta.setMask( "./Hijo/Nieta/*.zip" );

A mi me gusta mucho más la forma Unix de escribir los nombres de
subdirectorios, porque es menos confusa.

>    readDir hFile("..\\*.*");
>    hFile.setMask( "./Hijo/*.*" ); // listado del directorio padre

En este ejemplo la variable "hfile" queda lista para leer el directorio
padre, pero luego el setMask() la setea de nuevo para ver el subidrectorio
hijo. Eso se vale y funciona bien.

#endif

// EOF: readDir.cpp
