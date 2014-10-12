// huequeador.h (c) 2014 adolfo@di-mare.com

/** \file  huequeador.cpp
    \brief Programa que aprovecha la clase 'dejaHuecos' para no recuperar toda la memoria dinámica.

    \author Adolfo Di Mare <adolfo@di-mare.com>
    \date   2014
*/

#include <iostream>      // std::cout, etc.
#include <list>          // std::list<>
#include "dejaHuecos.h"  // clase mala, rompe programas

int main(int argc, char **argv) {
    if ( argc!=1 ) {
        std::cout << argv[0] << " no usa parametros\n";
        std::cout << "  -> Ejecucion finalizada" << std::endl;
        return 1;
    }

    int cont[4]; // contadores para ejecutar varias veces lo mismo
    #define DIM(v) (sizeof(v)/sizeof(*v)) ///< Dimensión de 'v[]'.
    for ( unsigned k=0; k<DIM(cont); ++k ) {

        try {
            int n=0;
            std::list< dejaHuecos > Lhuecos;
            for (;;) {
                ++n;
                std::list< dejaHuecos >::const_iterator last = Lhuecos.back();
                Lhuecos.push_back( dejaHuecos(n) );

                // se sale cuando se le acaba la memoria
                if ( 0 == strcmp ( last->get(), Lhuecos.back() ) ) { break; }
                }
            }
        }
        catch ( const char * CHANFLE ) {
            static char chanfle = { 'C','H','A','N','F','L','E',  \0 };
            if ( 0 != strcmp( CHANFLE , chanfle ) ) {
                std::cout << "  Exepción loca [" << CHANFLE << "[\n";
                std::cout << "  Debiera ser [ "  << chanfle << "[\n";
                std::cout << "  -> Ejecucion finalizada" << std::endl;
                return 2;
            }
        }
    }

    for ( unsigned k=1; k<DIM(cont); ++k ) {
        if ( cont[k-1] != cont[k] ) {
            std::cout << argv[0] << " deja huecos en la memoria dinamica\n";
            std::cout << "  -> Ejecucion finalizada" << std::endl;
            return 3;
        }
    }
    return 0;
}

// EOF: huequeador.h (c) 2014 adolfo@di-mare.com
