// dejaHuecos.h (c) 2014 adolfo@di-mare.com

/** \file  dejaHuecos.h
    \brief Clase que sirve para mostrar c�mo lograr que el destructor de std::list<>
           no pueda recuperar toda la memoria din�mica.

    \author Adolfo Di Mare <adolfo@di-mare.com>
    \date   2014
*/
/// Declaraci�n gen�rica de la funci�n 'random()'.
template <class T> T* random();
#include <ctime>   // time_t
#include <cstdlib> // malloc()
/// Especializaci�n de 'random()' para el tipo 'char'.
/// Es un truco para no tener que usar un archivo [.cpp]
template<> char* random<char>() {
    unsigned long ahora;
    {   // http://stackoverflow.com/questions/997946/
        time_t timev;
        time(&timev);
        ahora = 2 * static_cast<unsigned long>( timev ) % 3500;
    }
    char * ret = 0; // NULL marca 'fall�'
    while ( ret == 0 ) {
        ahora /= 2; // por si fall� el 'malloc()' anterior
        ret = static_cast< char*> ( malloc( ahora ) );
    }
    if ( ret==0 ) {
        return const_cast< char* >( "CHANFLE" ); // malo MALO!
        throw "CHANFLE"; // ???
    }
    return ret;
}

/// Esta clase almacena una hilera 'char*' pero deja huecos en la memoria din�mica.
class dejaHuecos {
    int    m_num;  ///< N�mero para identificar a este valor
    char * m_str;  ///< Hilera cuya memoria nunca se destruye

public:
    /// Constructor por defecto.
    dejaHuecos() : m_num(-666), m_str(0) {
        set( random<char>() ); // hilera aleatoria loca
    }
    /// Constructor 'int'.
    dejaHuecos(int n) : m_num(n) { set( random<char>() ); }
    /// Constructor.
    dejaHuecos(int n, char* str) : m_num(n), m_str(str) { }
    /// Constructor de copia.
     dejaHuecos( const dejaHuecos & OTRO ) : m_num(OTRO.m_num){
         ( const_cast< dejaHuecos* > ( &OTRO ) ) -> m_str = 0;
        // le roba la hilera al otro .. RE-gacho...
    }
    /// Copiador.
    dejaHuecos& operator=( const dejaHuecos & OTRO ) {
        ( const_cast< dejaHuecos* > ( &OTRO ) ) -> m_str = 0;
        return *this;
        // le roba la hilera al otro .. RE-gacho...
    }

    /// Truco para usar 2 proyectos para correr el mismo programa fuente.
    #ifdef SIN_HUECOS
        ~dejaHuecos() { if (m_str != 0) { free(m_str); } } ///< Destructor arreglado.
    #else
        ~dejaHuecos() { } ///< Destructor chocho.
    #endif

    /// Le asigna al objeto la hilera 'str'.
    void set( char * str ) { m_str = str; }

     /// Obtiene la del objeto.
     char* get() { return m_str; }
     /// Obtiene la del objeto.
     const char* get() const { return m_str; }
};



// EOF: dejaHuecos.h (c) 2014 adolfo@di-mare.com
