// readDir.h    (C) 2005 adolfo@di-mare.com

/** \file  readDir.h
    \brief Clase portable para leer los nombres de archivos del directorio actual.

    - \c _MSC_VER: Identifica al compilador C++ de Microsoft.
    - \c __BORLANDC__: Identifica al compilador C++ de Borland.
    - \c __GNUC__: Identifica al compilador GNU C++

    \author Adolfo Di Mare <adolfo@di-mare.com>
    \date   2010
*/

#ifndef   _readDir
#define   _readDir

#if defined( _MSC_VER ) || defined( __GNUC__ )
    #include <io.h>  // _findfirst(), _findnext(), etc.
#endif
#ifdef __BORLANDC__
    #include <dir.h>
    #include <dos.h> // para FA_DIREC
    #if (__BORLANDC__ <= 0x0410)  //   Identifica a BC++ v3.1 y anterior
        typedef int bool;
        const int false = 0;
        const int true  = ! false;
        #define using
        #define namespace
        #define std
    #endif
#endif

#include <ctime> // time_t
#include <string>

/** Módulo portable para leer los nombres de los archivos de un directorio.
    \test

    \dontinclude readDir.cpp
    \skipline    test::ejemplo()
    \until       }}
    \see         ejemplo()
*/
class readDir {
public:
    readDir( const char * pszMask = 0 );
    readDir( const std::string& dir );
    ~readDir(  ); ///< Destructor.
    bool hasNext() const; ///< Retorna \c "true" cuando hay más archivos por procesar.
    /// Avanza al siguiente nombre de archivo.
    /// \pre \c setMask() debe haber sido invocado previamente.
    const readDir& next();

    void setMask( const char * dir );
    void setMaskWithChop( const char * dir );
    void setMask( const std::string& dir );
    /// Retorna la máscara establecida por \c setMask();
    const char* getMask( ) const { return m_pszMask; }

    long size() const; ///< Tamaño del archivo.

    bool isNormal()   const; ///< \c "true" si el archivo es "Normal".
    bool isReadOnly() const; ///< \c "true" si el archivo es "Read Only".
    bool isHidden()   const; ///< \c "true" si el archivo es "Hidden".
    bool isSystem()   const; ///< \c "true" si el archivo es "System".
    bool isLabel()    const; ///< \c "true" si el archivo es "Label".
    bool isSubDir()   const; ///< \c "true" si el archivo es un sub-directorio.
    bool isArchive()  const; ///< \c "true" si el archivo es "Archive".
    const char* getName() const; ///< Nombre del archivo.
    static int getMaxPath(); ///< Tamaño máximo del nombre retornado por \c getName().
    time_t getTimeWrite() const; ///< Momento de la última escritura al archivo.
private:
    const char* m_pszMask;   ///< Máscara de búsqueda: "*.*"
    bool        m_bDestroyPtr; ///< \c "true" cuando es necesario retornar la memoria de \c m_pszMask.
    bool        m_bHasNext;  ///< \c "true" cuando todavía no se han agotado los archivos del directorio.
    // Campos que dependen del compilador usado
    #if defined( _MSC_VER ) || defined( __GNUC__ )
        _finddata_t m_FILE_BLOCK;      ///< Contiene los datos del archivo en proceso.
        _finddata_t m_FILE_BLOCK_NEXT; ///< Datos del siguiente archivo.
        intptr_t    m_handle;          ///< Indice numérico para llamados subsecuentes.
    #endif
    #ifdef __BORLANDC__
        struct ffblk m_FILE_BLOCK_NEXT; ///< Datos del siguiente archivo.
        struct ffblk m_FILE_BLOCK;      ///< Contiene los datos del archivo en proceso.
    #endif
};

// hasNext()+next() es el nombre usado para los iteradores Java.
inline bool readDir::hasNext() const {
    return m_bHasNext;
}

#include <cstring> // memcpy()

inline const readDir& readDir::next() {
    #if defined( _MSC_VER ) || defined( __GNUC__ )
        memcpy( &m_FILE_BLOCK, &m_FILE_BLOCK_NEXT, sizeof(m_FILE_BLOCK_NEXT) );
        m_bHasNext = ( -1 != _findnext( m_handle, & m_FILE_BLOCK_NEXT ) );
        if ( !m_bHasNext ) { _findclose(m_handle); }
    #endif
    #ifdef __BORLANDC__
        memcpy( &m_FILE_BLOCK, &m_FILE_BLOCK_NEXT, sizeof(m_FILE_BLOCK_NEXT) );
        m_bHasNext = ( 0 == findnext( &m_FILE_BLOCK ) );
    //  if ( !m_bHasNext) { findclose(m_handle); }
    #endif
    return *this;
}

/// Constructor que lee el directorio \c "dir".
inline readDir::readDir( const char * dir ) {
    m_bDestroyPtr = false;
    setMask( dir );
}

/// Constructor que lee el directorio \c "dir".
inline readDir::readDir( const std::string& dir ) {
    m_bDestroyPtr = false;
    setMask( dir );
}

inline readDir::~readDir(  ) {
    if ( m_bDestroyPtr ) { // el valor original usado por setMask() tenía al menos 1 slash
        delete [] ( const_cast<char*>(m_pszMask)); // se vuela el buffer
    };
    #if defined( _MSC_VER ) || defined( __GNUC__ )
        if ( m_bHasNext ) { _findclose(m_handle); }
    #endif
}

inline long readDir::size() const {
    #if defined( _MSC_VER ) || defined( __GNUC__ )
        return m_FILE_BLOCK.size;
    #endif
    #ifdef __BORLANDC__
        return m_FILE_BLOCK.ff_fsize;
    #endif
}

inline bool readDir::isNormal() const {
    #if defined( _MSC_VER ) || defined( __GNUC__ )
        return (m_FILE_BLOCK.attrib & _A_NORMAL);
    #endif
    #ifdef __BORLANDC__
        return (m_FILE_BLOCK.ff_attrib & FA_NORMAL);
    #endif
}

inline bool readDir::isReadOnly() const {
    #if defined( _MSC_VER ) || defined( __GNUC__ )
        return (m_FILE_BLOCK.attrib & _A_RDONLY);
    #endif
    #ifdef __BORLANDC__
        return (m_FILE_BLOCK.ff_attrib & FA_RDONLY);
    #endif
}

inline bool readDir::isHidden() const {
    #if defined( _MSC_VER ) || defined( __GNUC__ )
        return (m_FILE_BLOCK.attrib & _A_HIDDEN);
    #endif
    #ifdef __BORLANDC__
        return (m_FILE_BLOCK.ff_attrib & FA_HIDDEN);
    #endif
}

inline bool readDir::isSystem() const {
    #if defined( _MSC_VER ) || defined( __GNUC__ )
        return (m_FILE_BLOCK.attrib & _A_SYSTEM);
    #endif
    #ifdef __BORLANDC__
        return (m_FILE_BLOCK.ff_attrib & FA_SYSTEM);
    #endif
}

inline bool readDir::isLabel() const {
    #if defined( _MSC_VER ) || defined( __GNUC__ )
        #ifdef _A_VOLID
            return (m_FILE_BLOCK.attrib & _A_VOLID);
        #else
            return (m_FILE_BLOCK.attrib & (_A_SYSTEM<1));
        #endif
    #endif
    #ifdef __BORLANDC__
        return (m_FILE_BLOCK.ff_attrib & FA_LABEL);
    #endif
}

inline bool readDir::isSubDir() const {
    #if defined( _MSC_VER ) || defined( __GNUC__ )
        return (m_FILE_BLOCK.attrib & _A_SUBDIR);
    #endif
    #ifdef __BORLANDC__
        return (m_FILE_BLOCK.ff_attrib & FA_DIREC);
    #endif
}

inline bool readDir::isArchive() const {
    #if defined( _MSC_VER ) || defined( __GNUC__ )
        return (m_FILE_BLOCK.attrib & _A_ARCH);
    #endif
    #ifdef __BORLANDC__
        return (m_FILE_BLOCK.ff_attrib & FA_ARCH);
    #endif
}

bool isfile( const char* argname );
bool isdir(  const char* argname );
const char* pathExt( const char* file );
const char* lastExt( const char* name );

#endif // _readDir
// EOF: readDir.h
