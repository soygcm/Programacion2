// dejaHuecos.h (c) 2014 adolfo@di-mare.com

/** \file  dejaHuecos.h
    \brief Clase que sirve para mostrar cómo lograr que el destructor de std::list<>
           no pueda recuperar toda la memoria dinámica.

    \author Adolfo Di Mare <adolfo@di-mare.com>
    \date   2014
*/
/// Declaración genérica de la función 'random()'.
#include <cstdlib> // malloc()

/// Esta clase almacena una hilera 'char*' pero deja huecos en la memoria dinámica.
class dejaHuecos {
    int    m_num;  ///< Número para identificar a este valor
    char * m_str;  ///< Hilera cuya memoria nunca se destruye

public:
    /// Constructor por defecto.
    dejaHuecos() : m_num(-666), m_str(0) {
        set( obtenerMemoria() );
    }
    /// Constructor 'int'.
    dejaHuecos(int n) : m_num(n) {
        set( obtenerMemoria() );
    }
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
        ~dejaHuecos() {
            if (m_str != 0) {
                free(m_str);
            }
        } ///< Destructor arreglado.
    #else
      ~dejaHuecos() { } ///< Destructor chocho.
    #endif

    /// Le asigna al objeto la hilera 'str'.
    void set( char * str ) {
        std::cout << m_num << ": " << (void *) str << '\n';
        m_str = str;
    }

     /// Obtiene la del objeto.
     char* get() { return m_str; }
     /// Obtiene la del objeto.
     const char* get() const { return m_str; }
private:
    char* obtenerMemoria();
};

/// Especialización de 'random()' para el tipo 'char'.
/// Es un truco para no tener que usar un archivo [.cpp]
char *dejaHuecos::obtenerMemoria() {
    
    long numerote = 1000000000000;
    char * ret = 0; // NULL marca 'fallé'
    ret = (char *) malloc( numerote ) ;
    if ( ret == 0 ) {
        throw "CHANFLE";
        return ret;
    }
    return ret;
}



// EOF: dejaHuecos.h (c) 2014 adolfo@di-mare.com
