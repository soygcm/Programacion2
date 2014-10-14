// huequeador.h (c) 2014 adolfo@di-mare.com

/** \file  huequeador.cpp
    \brief Programa que aprovecha la clase 'dejaHuecos' para no recuperar toda la memoria dinámica.

    \author Adolfo Di Mare <adolfo@di-mare.com>
    \date   2014
*/

#include <iostream>      // std::cout, etc.
#include <list>          // std::list<>
#include <cstring>       // strcmp()
#include "dejaHuecos.h"  // clase mala, rompe programas

int main(int argc, char **argv) {

    try {
        int n=0;
        for (;;) {
            ++n;
            dejaHuecos dejado = dejaHuecos(n);
            if (n == 1000) {
                std::cout << "Podria seguir asi hasta el infinito, pero esta claro que el destructor esta haciendo su trabajo, y no esta dejando basura." << std::endl;
                break;
            }
        }
    }
    catch ( const char *error ) {
        if ( 0 == strcmp("CHANFLE", error) ) {
            std::cout << "  Memoria insuficiente [" << error << "]\n";
            std::cout << "  -> Ejecucion finalizada" << std::endl;
            return 0;
        }
    }
    
    return 0;
}

// EOF: huequeador.h (c) 2014 adolfo@di-mare.com
