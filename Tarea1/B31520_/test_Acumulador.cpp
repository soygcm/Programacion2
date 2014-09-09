// test_Acumulador.cpp (C) 2006 adolfo@di-mare.com

/** \file  test_Acumulador.cpp
    \brief Datos de prueba para la clase \c Acumulador.

    \author Adolfo Di Mare <adolfo@di-mare.com>
    \date   2006.
*/

#include "Acumulador.h"
#include "BUnit.h" // Módulo para prueba unitaria de programas

/// Datos de prueba para la clase \c Acumulador.
class test_Acumulador : public TestCase {
public:
    test_Acumulador() { setUp(); } ///< Constructor que invoca \c setUp()
    bool run();
private:
    void test_constructor();
    void test_copie_igual();
    void test_copie();
    void test_acumule();
    void test_total_cantidad();
    void test_borre();
    void test_Verifica_Suma();
    bool Verifica_Suma(long total, unsigned dim, long vec[]);
};

/// Ejecuta las pruebas.
bool test_Acumulador::run() {
    test_constructor();
    test_copie_igual();
    test_copie();
    test_acumule();
    test_total_cantidad();
    test_borre();
    test_Verifica_Suma();
    return wasSuccessful();
}

/// Cantidad de valores del vector \c V[].
/// - Esta una macro que no se puede implementar con plantillas.
#define DIM(V) (sizeof(V) / sizeof(*V)) // *V == *(V+0) == V[0]

/// Retorna \c true cuando la suma total de un acumulador es igual a \c total.
/// - En caso contrario, simplementer retorna \c false.
bool test_Acumulador::Verifica_Suma(long total, unsigned dim, long vec[]) {
    Acumulador A;
    assertTrue( check_ok( A ) );
    for (unsigned i=0; i<dim; ++i) {
        A.acumule( vec[i] );
        assertTrue( check_ok( A ) );
    }
    return ( A.total() == total );
}

/// Datos de prueba para el constructor y destructor de la clase.
void test_Acumulador::test_constructor() {
    {{  // test::constructor()
        Acumulador A;
        assertTrue( 0 == A.total() );
        assertTrue( 0 == A.cantidad() );
        long vec[] = { 1, 2, 3, 4,  5 };
        A.acumule( DIM(vec) , vec );
        assertTrue( 2+3+4 == A.total() );
        A.borre();
        assertTrue( 0 == A.total() );
    }}
    {   Acumulador A;
        {   // acumulador vacío
            A.borre();
            assertTrue( 0 == A.m_menor );
            assertTrue( A.m_menor == A.m_mayor );
            assertTrue( A.m_cantidad == 0 );
        }
    }
}

/// Datos de prueba para las operaciones de copia.
void test_Acumulador::test_copie() {
    {{  // test::copie()
        Acumulador A;
        long vec[] = { 1, 2, 3, 4,  5 };
        A.acumule( DIM(vec) , vec );
        Acumulador B; B = A;
        assertTrue( A.total()    == B.total() );
        assertTrue( A.cantidad() == B.cantidad() );
        assertTrue( A == B );
        A.borre();
        assertTrue( A != B );
        assertTrue( A.total()    != B.total() );
        assertTrue( A.cantidad() != B.cantidad() );
        B.borre();
        assertTrue( A == B );
    }}
}

/// Datos de prueba para las operaciones de copia y de comparación.
void test_Acumulador::test_copie_igual() {
    {{  // test::copie_igual()
        Acumulador A;
        long vec[] = { 1, 2, 3, 4,  5 };
        A.acumule( DIM(vec) , vec );
        Acumulador B = A;
        assertTrue( A.total()    == B.total() );
        assertTrue( A.cantidad() == B.cantidad() );
        assertTrue( A == B );
        A.borre();
        assertTrue( A != B );
        assertTrue( A.total()    != B.total() );
        assertTrue( A.cantidad() != B.cantidad() );
        B.borre();
        assertTrue( A == B );
    }}
}

/// Datos de prueba para \c Acumulador::borre().
void test_Acumulador::test_borre() {
    {{  // test::borre()
        Acumulador A;
        long vec[] = { 1, 2, 3, 4,  5 };
        A.acumule( DIM(vec) , vec );
        assertTrue( 2+3+4 == A.total() );
        assertTrue( 5 == A.cantidad() );
        A.borre();
        assertTrue( 0 == A.total() );
        assertTrue( 0 == A.cantidad() );
    }}
}

/// Datos de prueba para \c Acumulador::acumule().
void test_Acumulador::test_acumule() {
    {{  // test::acumule()
        Acumulador A;
        for ( long i=5; i>=0; --i ) {
            A.acumule( i );
        }
        assertTrue( 4+3+2+1 == A.total() );
        assertTrue( A.cantidad() == 6 );
        A.borre();
        assertTrue( 0 == A.total() );
        assertTrue( 0 == A.cantidad() );
    }}
    {   Acumulador A;
        {   // acumulación de puros ceros
            A.borre();
            assertTrue( 0 == A.total() );
            for ( long i=5; i>=0; --i ) {
                A.acumule( 0 );
            }
            assertTrue( 0+0+0+0 == A.total() );
            assertTrue( A.cantidad() == 6 );
        }
        {   // acumulación de solo un número
            A.borre();
            A.acumule( 12 );
            assertTrue( 12 == A.m_menor );
            assertTrue( A.m_menor == A.m_mayor );
            assertTrue( 0 == A.total() );
        }
        {   // acumulación ascendente de 2 números
            A.borre();
            A.acumule( 7 ); A.acumule( 11 );
            assertTrue(  7 == A.m_menor );
            assertTrue( 11 == A.m_mayor );
            assertTrue(  2 == A.m_cantidad );
            assertTrue(  0 == A.total() );
        }
        {   // acumulación descendente de 2 números
            A.borre();
            A.acumule( 11 ); A.acumule( 7 );
            assertTrue(  7 == A.m_menor );
            assertTrue( 11 == A.m_mayor );
            assertTrue(  2 == A.m_cantidad );
            assertTrue(  0 == A.total() );
        }
        {   // acumulación ascendente de 3 números
            A.borre();
            A.acumule( 7 ); A.acumule( 9 ); A.acumule( 11 );
            assertTrue(  7 == A.m_menor );
            assertTrue( 11 == A.m_mayor );
            assertTrue(  3 == A.m_cantidad );
            assertTrue(  9 == A.total() );
        }
        {   // acumulación descendente de 3 números
            A.borre();
            A.acumule( 11 ); A.acumule( 9 ); A.acumule( 7 );
            assertTrue(  7 == A.m_menor );
            assertTrue( 11 == A.m_mayor );
            assertTrue(  3 == A.m_cantidad );
            assertTrue(  9 == A.total() );
        }
        {   // vec[] en orden ascendente
            long vec[] = { -2, -1, -0, +0 , +1, +2, +3, +4  };
            A.borre();
            A.acumule( DIM(vec) , vec );
            assertTrue( A.total() == (-1) + (-0) + (+0) + (+1) + (+2) + (+3) );
            assertTrue( 8 == A.cantidad() );
        }
        {   // vec[] en orden decendente
            long vec[] = { +4, +3, +2, +1, +0, -0, -1, -2 };
            A.borre();
            A.acumule( DIM(vec) , vec );
            assertTrue( A.total() == (-1) + (-0) + (+0) + (+1) + (+2) + (+3) );
            assertTrue( 8 == A.cantidad() );
        }
        {   // vec[] en orden alocado
            long vec[] = { -1, -2, +1, +0, -0, +4, +3, +2 };
            A.borre();
            A.acumule( DIM(vec) , vec );
            assertTrue( A.total() == (-1) + (-0) + (+0) + (+1) + (+2) + (+3) );
            assertTrue( 8 == A.cantidad() );
        }
    }
}

/// Método base que ejecuta todas las pruebas de la clase.
void test_Acumulador::test_total_cantidad() {
    {{  // test::total_cantidad()
        Acumulador A;  //   2   3   4    5   6   7   8
        long vec[] = { -2, -1, -0, +0 , +1, +2, +3, +4  };
        A.acumule( DIM(vec) , vec );
        assertTrue( A.total() == (-1) + (-0) + (+0) + (+1) + (+2) + (+3) );
        assertTrue( 8 == A.cantidad() && 8 == DIM(vec) );
    }}
}

/// Corre varias pruebas específicas.
void test_Acumulador::test_Verifica_Suma() {
    {   assertTrue( 35 ==  2  + 3 + 4 + 5 + 6 + 7 + 8 );
        long vec[] = { 1 , 2 ,  3 , 4 , 5 , 6 , 7 , 8 , 9 };
        assertTrue( Verifica_Suma( 35, DIM(vec), vec ) );
    }

    {   long vec[] = { 1 };
        assertTrue( Verifica_Suma( 0, DIM(vec), vec ) );
    }

    {   long vec[] = { 1, 2 };
        assertTrue( Verifica_Suma( 0, DIM(vec), vec ) );
    }

    {   long vec[] = { 1, 2, 1 };
        assertTrue( Verifica_Suma( 1, DIM(vec), vec ) );
    }

    {   long vec[] = { 2, 2, 1 };
        assertTrue( Verifica_Suma( 2, DIM(vec), vec ) );
    }

    {   assertTrue( 12 ==  2 + 2 + 2 + 2 + 2 + 2  );
        long vec[] = { 1 , 2 , 2 , 2 , 2 , 2 , 2 , 2 };
        assertTrue( Verifica_Suma( 12, DIM(vec), vec ) );
    }

    {   // Esta es la que genera 1 error
        assertFalse(13 ==  2 + 2 + 2 + 2 + 2 + 2  );
        long vec[] = { 1 , 2 , 2 , 2 , 2 , 2 , 2 , 2 };
        assertTrue( Verifica_Suma( 13, DIM(vec), vec ) );
    }
}

#include <iostream> // cout

/** Este es el programa principal.
   - Primero crea la prueba \c Test de tipo \c test_Acumulador.
   - Luego hace la prueba, invocando \c test_Acumulador::run().
   - Por último graba en \c cout los resultados usando \c Report().
*/
int main() {
    
    std::cout << "Acumulador Test: \n";
    
    test_Acumulador tester;       // crea la prueba
    tester.run();                 // la ejecuta
    std::cout << tester.report(); // graba en "cout" los resultados
    return 0;
}

// EOF: test_Acumulador.cpp
