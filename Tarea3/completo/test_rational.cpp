// test_rational.cpp (C) 2006 adolfo@di-mare.com

/** \file  test_rational.cpp
    \brief Programa de prueba para la clase \c rational<INT>

    \author Adolfo Di Mare <adolfo@di-mare.com>
    \date   2007
*/

#include "BUnit.h"
#include <iostream>
#include <climits> // INT_MAX

using namespace std;

// Escoja aquí cuál clase usa entre GranNum (Narvaja) y Number (Bosak)
// #define Bosak
// #define Narvaja
#if defined(Bosak)
    #include "number.h"      // Clase de números de longitud arbitraria
    typedef  Number HugeInt; // sinónimo para la clase HugeInt
#elif defined(Narvaja)
    #include "GranNum.h"     // Clase de números de longitud arbitraria
    typedef  BigNum HugeInt; // sinónimo para la clase HugeInt
#else
    typedef  long   HugeInt; // sinónimo para la clase HugeInt
#endif

#include "rational.h"

/// Prueba la clase \c rational<INT>.
template <class INT>
class test_rational : public TestCase {
protected:
    rational<INT> m_half, m_quarter, m_one;
    rational<INT> m_half_neg, m_quarter_neg, m_one_neg;
    rational<INT> m_sep, m_sep_neg;
    bool m_super_lerdo; ///< Indica si hay que probar con numerotototes.
    bool m_super_numerotes; ///< Indica si se corren las pruebas que duran mucho.
public:
    virtual void setUp(); ///< Establece el ambiente de prueba.
    /// Define si se corren o no las pruebas con super-numerotototes.
    void super_numerotes(bool sl = false) {
        m_super_numerotes = sl;
    }
    /// Define si se corren las pruebas que duran mucho.
    void super_lerdo(bool sl = false) {
        m_super_lerdo = sl;
        if (m_super_lerdo) {
            m_super_numerotes = true;
        }
    }
public:
    bool run();
    void rat_tst(); void run_rest();
    void test_quiebra_Narvaja();
    void test_constructor();
    void test_destructor();
    void test_set();
    void test_op_equal();
    void test_num_den();
    void test_swap();
    void test_op_add_equal();
    void test_op_add();
    void test_op_mult_equal();
    void test_op_mult();
    void test_op_minus();
    void test_fromString();
    void test_op_comp();
    void test_op_in();
    void test_op_out();
    void test_simplify();
    void test_check_ok();
    void test_mcd();
    void test_op_cpp();
}; // test_rational

/// Método principal de la prueba.
/// - Requiere que recién haya sido ejecutado \c setUp()
template <class INT>
bool test_rational<INT>::run() {
    test_quiebra_Narvaja();
    test_constructor();
    test_destructor();
    test_set();
    test_op_equal();
    test_num_den();
    test_swap();
    test_op_add_equal();
    test_op_add();
    test_op_mult_equal();
    test_op_mult();
    test_op_minus();
    test_fromString();
    test_op_comp();
    test_op_in();
    test_op_out();

    test_simplify();
    test_check_ok();
    test_mcd();
    test_op_cpp();
    rat_tst(); run_rest();
    return TestCase::wasSuccessful();
}

template <class INT>
void test_rational<INT>::setUp() {
    m_half.set(1,2);
    m_quarter.set(1,4);
    m_one.set(1);
    m_half_neg.set(-1,2);
    m_quarter_neg.set(-1,4);
    m_one_neg.set(-1);
    m_sep.set(1,7);
    m_sep_neg.set(-1,7);
    m_super_lerdo = false;
    m_super_numerotes = false;
}

template <class INT> INT pow( const INT & r, int n );
template <class INT> void grabador (ostream &COUT, const INT & r);
template <class INT> const char * aHilera(const INT& r);
template <class INT> const char * aHilera(const rational<INT>& r);
template <class INT> rational<INT>& set( rational<INT>& r, const char * num, const char * den = "1" );
template <class INT> INT& set( INT& num, const char * str );


/// Datos de prueba que sirven para quebrar la implementación \c BigNum de Narvaja.
template <class INT>
void test_rational<INT>::test_quiebra_Narvaja() {
    {{  // test::quiebra_Narvaja()
        rational<INT> r;       assertTrue( r.num() ==  0 && r.den() == 1 );
        r.set(  5*11,  2*11 ); assertTrue( r.num() ==  5 && r.den() == 2 ); // Aquí Narvaja falla (1)
        rational<INT> s(-1,2); assertTrue( s.num() == -1 && s.den() == 2 ); // Aquí Narvaja falla (2)
        rational<INT> t(1,-2); assertTrue( t.num() == -1 && t.den() == 2 );
        assertTrue( r != s );
        assertTrue( s == t );                                               // Aquí Narvaja falla (3)
        std::basic_ostringstream<char> ost; // receptor de salida
        {
            const char* str = aHilera(r);
            ost.str(""); ost << str; assertTrue( ost.str() == "2" );        // Aquí Narvaja falla (4)
            delete [] str;
            ost.str("");
            str = aHilera(s.num());
            ost << '['<< str;
            delete [] str;
            str = aHilera( s.den() ); //  el denominador está malo.
            ost << '/' << str << ']';
            delete [] str; // retorna la memoria que aHilera() obtuvo
            assertTrue(  ost.str() == "[-1/2]" );                           // Aquí Narvaja falla (5)
            assertFalse( ost.str() == "[-1/0]" );                           // Aquí Narvaja falla (6)
        }
        if (1) {
            ost.str(""); ost << r;                    // Aquí Narvaja se encicla
            ost.str(""); ost << s;                    // Aquí Narvaja se encicla
            ost.str(""); ost << rational<INT>(-1,2);  // Aquí Narvaja se encicla
        }
    }}
    {   // Resto de las pruebas
    }
}

/// Datos de prueba para los constructores de la clase \c rational<INT>.
template <class INT>
void test_rational<INT>::test_constructor() {
    {{  // test::constructor()
        rational<INT> r,s,t(1,-2);
        assertTrue( r == rational<INT>(0) );
        assertTrue( s == rational<INT>(0,32) );
        assertTrue( t == rational<INT>(-100,200) );
        rational<INT> q = t;
        assertTrue( q == t );
    }}
    {   // Resto de las pruebas
    }
}

/// Datos de prueba para el destructor de la clase \c rational<INT>.
template <class INT>
void test_rational<INT>::test_destructor() {
    return test_constructor();
}

/// Datos de prueba para \c rational<INT>::set().
template <class INT>
void test_rational<INT>::test_set() {
    {{  // test::set()
        rational<INT> r;
        assertTrue( r == rational<INT>( 0 ) );
        r.set(-3, -4);
        assertTrue( r == rational<INT>( 3*11 , 4*11 ) );
    }}
    {   // Resto de las pruebas
    }
}

/// Datos de prueba para \c rational<INT>::num() y \c rational<INT>::den().
template <class INT>
void test_rational<INT>::test_num_den() {
    {{  // test::num_den()
        rational<INT> r;       assertTrue( r.num() ==  0 && r.den() == 1 );
        r.set( -1*13, -4*13 ); assertTrue( r.num() ==  1 && r.den() == 4 );
        r.set( -2*11,  5*11 ); assertTrue( r.num() == -2 && r.den() == 5 );
        r.set(  2*17, -6*17 ); assertTrue( r.num() == -1 && r.den() == 3 );
    }}
    {   // Resto de las pruebas
    }
}

/// Datos de prueba para \c rational<INT>::operator=().
template <class INT>
void test_rational<INT>::test_op_equal() {
    {{  // test::op_equal()
        rational<INT> r(1), s(18,56); assertTrue( r == rational<INT>(1) );
        r = s;                        assertTrue( r == ( r / ( r / s ) ) );
        r = 34;                       assertTrue( r == rational<INT>(34) );
        s = -3;                       assertTrue( s == rational<INT>(-3) );
    }}
    {   // Resto de las pruebas
    }
}

/// Datos de prueba para \c rational<INT>::swap().
template <class INT>
void test_rational<INT>::test_swap() {
    {{  // test::swap()
        rational<INT> r, s(18,56);  assertTrue( r == rational<INT>(0) );
        r.swap( s );                assertTrue( s == rational<INT>(0) );
        assertTrue( r == rational<INT>(18,56) );
    }}
    {   // Resto de las pruebas
    }
}

/// Datos de prueba para \c rational<INT>::operator +=() y \c rational<INT>::operator -=() .
template <class INT>
void test_rational<INT>::test_op_add_equal() {
    {{  // test::op_add_equal()
        rational<INT> add(0), sub(0);
        for ( int i=20; i>=-20; --i ) {
            add += rational<INT>(i-i, 20*i+1);
            sub -= rational<INT>(i-i, 20*i+1);
        }
        assertTrue( add == sub );
    }}
    {   // Resto de las pruebas
    }
}

/// Datos de prueba para \c rational<INT>::operator +() y \c rational<INT>::operator -() .
template <class INT>
void test_rational<INT>::test_op_add() {
    {{  // test::op_add()
        rational<INT> add(0), sub(0);
        for ( int i=20; i>=-20; --i ) {
            add = add + rational<INT>(i-i, 20*i+1);
            sub = sub - rational<INT>(i-i, 20*i+1);
        }
        assertTrue( add == sub );
    }}
    {   // Resto de las pruebas
    }
}

/// Datos de prueba para \c rational<INT>::operator *=() y \c rational<INT>::operator /=().
template <class INT>
void test_rational<INT>::test_op_mult_equal() {
    {{  // test::op_mult_equal()
        rational<INT> mlt(1), div(1);
        for ( int i=15; i>=-15; --i ) {
            mlt *= rational<INT>(17*i-1, 13*i+1);
            div /= rational<INT>(13*i+1, 17*i-1);
        }
        assertTrue( mlt == div );
    }}
    {   // Resto de las pruebas
    }
}

/// Datos de prueba para \c rational<INT>::operator *() y \c rational<INT>::operator /().
template <class INT>
void test_rational<INT>::test_op_mult() {
    {{  // test::op_mult()
        rational<INT> mlt(1), div(1);
        for ( int i=15; i>=-15; --i ) {
            mlt = mlt * rational<INT>(17*i-1, 13*i+1);
            div = div / rational<INT>(13*i+1, 17*i-1);
        }
        assertTrue( mlt == div );
    }}
    {   // Resto de las pruebas
    }
}

/// Datos de prueba para \c rational<INT>::operator -().
template <class INT>
void test_rational<INT>::test_op_minus() {
    {{  // test::op_minus()
        rational<INT> half_n(1,-2), quarter_n(-1,4);
        assertTrue( - half_n == rational<INT>(-1) * half_n );
        assertTrue( half_n * half_n == - quarter_n );
        assertTrue( half_n * half_n * half_n == - quarter_n * half_n );
    }}
    {   // Resto de las pruebas
    }
}

/// Datos de prueba para \c rational<INT>::test_fromString().
template <class INT>
void test_rational<INT>::test_fromString() {
    {{  // test::fromString()
        rational<INT> r, half(1,2), quarter(1,4);
        assertTrue( - half  == r.fromString(  "[---12/24]" ) );
        assertTrue( quarter == r.fromString( "[ -03/ -12 ]") );
        r.fromString( "[ -+-+-+-+- 4 / -- -+ -- 32  ]" );
        assertTrue( r == rational<INT>(1,8) );
    }}
    {   // Resto de las pruebas
    }
}

/// Datos de prueba para \c rational<INT>::test_simplify().
template <class INT>
void test_rational<INT>::test_simplify() {
    {{  // test::simplify()
        rational<INT> half(1,2);
        half.num() *= 23;
        half.den() *= 23;
        assertTrue( half != rational<INT>(23, 23*2) );
        half.simplify();
        assertTrue( half == rational<INT>(23, 23*2) );
    }}
    {   // Resto de las pruebas
    }
}

/// Datos de prueba para \c check_ok( const rational<INT>& ).
template <class INT>
void test_rational<INT>::test_check_ok() {
    {{  // test::check_ok()
        rational<INT> r, *nul=0;      assertFalse( check_ok(*nul) );
        r.num() =  2;  r.den() =  0;  assertFalse( check_ok(  r ) );
        r.num() =  2;  r.den() = -1;  assertFalse( check_ok(  r ) );
        r.num() =  0;  r.den() =  2;  assertFalse( check_ok(  r ) );
        r.num() = 31;  r.den() = 31;  assertFalse( check_ok(  r ) );
        r.simplify();                 assertTrue ( check_ok(  r ) );
    }}
    {   // Resto de las pruebas
    }
}

/// Datos de prueba para todos los operadores de comparación de \c rational<INT>.
template <class INT>
void test_rational<INT>::test_op_comp() {
    {{  // test::op_comp()
        rational<INT> neg_half(-1,2), quarter(1,4);
        assertTrue( neg_half == -(-neg_half) );
        assertTrue( neg_half <  quarter  );
        assertTrue( quarter  >  neg_half );
        assertTrue( neg_half <= quarter  );
        assertTrue( quarter  >= neg_half );
        assertTrue( neg_half != quarter  );
    }}
    {   // Resto de las pruebas
    }
}

/// Datos de prueba para \c operator<<().
template <class INT>
void test_rational<INT>::test_op_out() {
    {{  // test::op_out()
        std::basic_ostringstream<char> ost; // receptor de salida
        ost.str(""); ost << rational<INT>(-1,2);  assertTrue( ost.str() == "[-1/2]" );
        ost.str(""); ost << rational<INT>(-12);   assertTrue( ost.str() == "[-12]"  );
        ost.str(""); ost << rational<INT>(1-1,8); assertTrue( ost.str() == "[0]"    );
        ost.str(""); // Borra el receptor de salida
        ost << rational<INT>(-1,2) << rational<INT>(-12) << rational<INT>(1-1,8);
        assertTrue( ost.str() == "[-1/2][-12][0]" );
    }}
    {   // Esto también funciona pero es incómodo declarar 1a variable en cada caso
        std::basic_ostringstream<char> r_ost, s_ost, t_ost;
        r_ost << rational<INT>(-1,2);  assertTrue( r_ost.str() == "[-1/2]" );
        s_ost << rational<INT>(-12);   assertTrue( s_ost.str() == "[-12]"  );
        t_ost << rational<INT>(1-1,8); assertTrue( t_ost.str() == "[0]"    );
    }
    {   // Resto de las pruebas
    }
}

/// Datos de prueba para \c operator>>().
template <class INT>
void test_rational<INT>::test_op_in() {
    {{  // test::op_in()
        std::basic_istringstream<char> ist( "[-1/2] [-12] [0]" );
        rational<INT> r(0); ist >> r;  assertTrue( r == rational<INT>(-1,2) );
        rational<INT> s(1); ist >> s;  assertTrue( s == rational<INT>(-12)  );
        rational<INT> t(2); ist >> t;  assertTrue( t == rational<INT>(0)    );

        ist.str( "[ -+-+-+-+- 4 / -- -+ -- 32  ]" );
        rational<INT> u(3); ist >> u;  assertTrue( u == rational<INT>(1,8) );
    }}
    {   // Resto de las pruebas
    }
}

/// Datos de prueba para la función \c mcd().
template <class INT>
void test_rational<INT>::test_mcd() {
    {{  // test::mcd()
        assertTrue( 1 == mcd(1,2) );
        assertTrue( 2*3*5 == mcd( 2*2*2*2 * 3*3 * 5*5, 2*3*5 ) );
        assertTrue( 30 == mcd( -3600, -30 ) );
    }}
    {   // Resto de las pruebas
    }
}

/// Datos de prueba para los incrementadores \c c++ y \c --c.
template <class INT>
void test_rational<INT>::test_op_cpp() {
    {{  // test::op_cpp()
        rational<INT> r(3,2);

        assertTrue( r++ == rational<INT>(3,2) );
        assertTrue( r   == rational<INT>(5,2) );

        assertTrue( r-- == rational<INT>(5,2) );
        assertTrue( r   == rational<INT>(3,2) );

        assertTrue( --r == rational<INT>(1,2) );
    }}
    {   // Resto de las pruebas
        rational<INT> r(4,3);

        assertTrue( r++ == rational<INT>(4,3) );
        assertTrue( r   == rational<INT>(7,3) );

        assertTrue( r-- == rational<INT>(7,3) );
        assertTrue( r   == rational<INT>(4,3) );

        assertTrue( --r == rational<INT>(1,3) );
    }
    {   rational<INT> r(4,3);

        assertTrue( r-- == rational<INT>(4,3) );
        assertTrue( r   == rational<INT>(1,3) );

        assertTrue( r++ == rational<INT>(1,3) );
        assertTrue( r   == rational<INT>(4,3) );

        assertTrue( --r == rational<INT>(1,3) );
    }
}

/// Antiguo programa de prueba para la clase \c rational<INT>.
template <class INT>
void test_rational<INT>::rat_tst() {
    rational<INT> r,       // r.rational<INT>();
           s(1),    // s.rational<INT>(1);
           t = 1,   // t.rational<INT>(1);
           u(1,2);  // u.rational<INT>(1,2);

    assertTrue( r == rational<INT>(0) );
    assertTrue( s == rational<INT>(1) );
    assertTrue( t == s );
    assertTrue( u == rational<INT>(1,2) );

    rational<INT>      a = t;
    assertTrue( a == t );
               a = t;
    assertTrue( a == t );

    (a += s * rational<INT>(2)) += rational<INT>(4);
    assertTrue( a == rational<INT>(7) );
    assertTrue( s == rational<INT>(1) );
    assertTrue( rational<INT>(1) == s );

    a = rational<INT>(4, -5);
    rational<INT> b(-10, 6);
    a *= b;

    assertTrue( a == rational<INT>( 4*-10,-5*6) );

    a=b;
    assertTrue( a == b );
    assertTrue( b == a );

    a = rational<INT>(4, -5);
    a += b;
    assertTrue( a == rational<INT>( 4*6 + -5*-10, -5*6 ) );

    a -= b;
    assertTrue( a == rational<INT>(-4, 5) );

    a = b;
    assertTrue( a == b );

    a -= a;
    assertTrue( a == rational<INT>(0) );
    assertTrue( rational<INT>(0) == a );

    rational<INT> c(15);
    assertTrue( rational<INT>(15) == c );

    a /= c;
    assertTrue( a == rational<INT>(0) );
    assertTrue( rational<INT>(0) == a );

    a = 1234567l;
    assertTrue( a == rational<INT>(1234567l) );

    long d = 4;
    a = d;
    assertTrue( a == rational<INT>(d) );
    assertTrue( rational<INT>(d) == a );

    a += (b = rational<INT>(5));
    assertTrue( a == rational<INT>(d+5) );
    assertTrue( b == rational<INT>(5) );

    a = rational<INT> (4, -5);
    b = rational<INT> (5, -4);
    assertTrue( a*b == rational<INT>(1) );
    assertTrue( a/b == rational<INT>( -4*4 , -5*5 ) );
    assertTrue( b/a == rational<INT>( -5*5 , -4*4 ) );

    rational<INT> tot = rational<INT>(0);
    for (int i = 0; i <= 50; ++i) {
        tot += rational<INT>(i, i+1);
    }
    {
        bool good_sum = false;
        good_sum = good_sum || TestCase::toString(tot) == "[2668385009430893323/8818708306321991968]";
        good_sum = good_sum || TestCase::toString(tot) == "[64263385/206760032]";
        good_sum = good_sum || TestCase::toString(tot) == "[3067/4896]";
        assertTrue_Msg( "sum(0==>50 { rational<INT>(i, i+1) }", good_sum );
    }

    assertTrue( rational<INT>( 1,4) == rational<INT>().fromString("[ 1 / - - 4 ]"));
    assertTrue( rational<INT>(-1,4) == rational<INT>().fromString("[-1 / - - 4 ]"));
    assertTrue( rational<INT>(-1,4) == rational<INT>().fromString("[ - 1 / -- 4 ]"));
    assertTrue( rational<INT>( 1,4) == rational<INT>().fromString("[ - 1 / -  4 ]"));

    {   // selection_sort()

        rational<INT> VEC[17];
        size_t  nVEC = sizeof(VEC) / sizeof(*VEC);

        size_t i;
        srand( 1492 );
        for (i=0; i<nVEC; ++i) {
            rational<INT> r(rand(), rand());
            VEC[i] = r;
            assertTrue( check_ok( VEC[i] ) );
            assertTrue( r == VEC[i] );
        }

        selection_sort(VEC, nVEC);
        assertTrue( Ordenado(VEC, nVEC) );
    }
}

/// Retorna un racional construido a partir de los valores almacenados en \c num y \c den.
/// - Los valores negativos comienzan con el caracter \c '-' al principio de la hilera.
template <class INT>
rational<INT> rat( const char* num , const char * den = "1" ) {
    rational<INT> RES;
    return set( RES, num, den );
}

/// Método complementario de prueba.
/// - Esto lo ejecita \c run().
template <class INT>
void test_rational<INT>::run_rest() {
    {   //  A54641.cpp
        assertTrue(  rational<INT>(0) * rational<INT>(0,2) == rational<INT>(0,4) );

        assertTrue( rat<INT>( "100" , "2" ) * rat<INT>( "2" , "100" ) == rat<INT>( "1" , "1" ) );
        assertTrue( rat<INT>( "99" , "99" ) * rat<INT>( "99" , "99" ) == rat<INT>( "1" ) );

        assertTrue( m_half / rat<INT>( "1" , "1" ) == rat<INT>( "128" , "256" ) );
        assertTrue( rat<INT>( "10" , "40" )/rat<INT>( "10" , "40" )  == rat<INT>( "1" ) );

        assertTrue( m_quarter / rat<INT>( "2" )  == rat<INT>( "1" , "8" ) );
        assertTrue( m_quarter / m_half == rat<INT>( "2" , "4" ) );

        assertTrue( rat<INT>( "75" , "100" ) / rat<INT>( "100" , "75" ) == rat<INT>( "75" , "100" ) * rat<INT>( "75" , "100" ));

        assertTrue( ( m_half + m_quarter ) == rat<INT>( "3" , "4" ) );

        assertTrue( ( m_one + m_quarter ) == rat<INT>( "5" , "4" ) );
        assertTrue( ( rat<INT>( "75" , "100" )+= rat<INT>( "0" ) )== rat<INT>( "75" , "100" )  );

        assertTrue( ( m_half + m_quarter) == rational<INT>(3,4) );
        assertTrue( ( m_half + rational<INT>(85,40000)) == rational<INT>(40170,80000) );

        assertTrue(  (m_one + m_quarter) == rational<INT>(5,4) );
        assertTrue(  (rational<INT>(75,100)+= rational<INT>(0) )== rational<INT>(75,100)  );

        assertTrue(   (m_quarter + rational<INT>(-1)) ==  rational<INT>(-3,4) );
        assertTrue(   (rational<INT>(50,37)+rational<INT>(50,37) ) == rational<INT>(2)*rational<INT>(50,37) );

        assertTrue((  m_half - m_quarter )== m_quarter );
        assertTrue((  rational<INT>(10,40) - rational<INT>(10,40) ) == rational<INT>(0) );

        assertTrue( ( -m_half + m_quarter)== rational<INT>(-1,4) );
        assertTrue( ( rational<INT>(-75,100) )== m_quarter-m_one  );

        assertTrue( ( rational<INT>(75,100) - rational<INT>(75)) == -rational<INT>(7425,100) );
        assertTrue( ( m_quarter - rational<INT>(1) )== rational<INT>(-3,4) );

        assertTrue(  m_half * rational<INT>(0,2) == rational<INT>(0,4) );
        assertTrue(  m_half * rational<INT>(0,2) == rational<INT>(0) );

        assertTrue(  rational<INT>(0) * rational<INT>(0,2) == rational<INT>(0,4) );
        assertEquals( rat<INT>("1", "1") ,  m_one );
        assertEquals( rat<INT>("1", "2") ,  m_half );
        assertEquals( rat<INT>("1", "4") ,  m_quarter );
        assertTrue(  m_half / rational<INT>(1,1) == rational<INT>(13,26) );
        assertTrue(  rational<INT>(10,40)/rational<INT>(10,40)  == rational<INT>(1) );

        assertTrue(  m_quarter / rational<INT>(2)  == rational<INT>(1,8) );
        assertTrue(  m_quarter / m_half == rational<INT>(2,4));

        assertTrue( (rational<INT>(75,100) /= rational<INT>(100,75)) == rational<INT>(75*75,100*100) );

        assertTrue(   (rational<INT>(50,37)+=rational<INT>(50,37) ) == rational<INT>(2)*rational<INT>(50,37) );

        assertTrue((  rational<INT>(10,40) -= rational<INT>(10,40) ) == rational<INT>(0) );

        assertTrue( ( rational<INT>(-3,4) -= m_quarter   )== rational<INT>(-1) );
        assertTrue( ( -m_half + m_quarter)== rational<INT>(-1,4) );

        assertTrue( ( rational<INT>(75,100) -= rational<INT>(75)) ==  rational<INT>(75*1-75*100, 1*100) );

        assertTrue(  (rational<INT>(99,99) *= rational<INT>(0,2)) == rational<INT>(0,4) );

        assertTrue(  (rational<INT>(100,2) *= rational<INT>(2,100)) == rational<INT>(1,1) );
        assertTrue(  (rational<INT>(99,99) *= rational<INT>(99,99)) == rational<INT>(1) );
        assertTrue(  (rational<INT>(10,40) /= rational<INT>(10,40))  == rational<INT>(1) );

        assertTrue( (rational<INT>(75,100) *= rational<INT>(100,75)) == rational<INT>(75,75) );

        assertTrue(  m_quarter < m_half );
        assertTrue(  rational<INT>(-10,40)  < rational<INT>(10,40) );

        assertFalse(  m_quarter   < m_quarter );
        assertFalse(  rational<INT>(4) < rational<INT>(40,10)  );

        assertFalse( rational<INT>(20,40) < rational<INT>(40,80) );
        assertFalse(   rational<INT>(0,100) < rational<INT>(0) );

        assertFalse( ( m_half-m_quarter ) != rational<INT>(10,40) );
        assertFalse(  rational<INT>(10,40)  != m_quarter );

        assertFalse(  m_one-m_quarter   != rational<INT>(75,100) );
        assertFalse(  rational<INT>(-75,100) != (m_quarter-m_one ) );

        assertTrue(  rational<INT>(75,100) !=  (m_quarter-m_one) );
        assertFalse(  rational<INT>(0,100) != rational<INT>(0) );

        assertTrue(  m_quarter <= m_half );
        assertTrue(  rational<INT>(-10,40)  <= rational<INT>(10,40) );

        assertTrue(  m_quarter   <= m_quarter );
        assertTrue(  rational<INT>(4) <= rational<INT>(40,10)  );
        assertTrue( rational<INT>(20,40) <= rational<INT>(40,80) );
        assertTrue(   rational<INT>(0,100) <= rational<INT>(0) );
        assertTrue(  m_quarter >= m_quarter  );
        assertTrue(  rational<INT>(10,40)  >= -rational<INT>(10,40) );

        assertTrue(  m_one >= m_quarter );
        assertTrue(  rational<INT>(-75,100) >= m_quarter-m_one  );

        assertTrue( rational<INT>(79,57) >=  rational<INT>(75,150) );
        assertTrue( rational<INT>(125,100) >= m_one+m_quarter );

        assertTrue(  m_quarter >= m_quarter  );
        assertTrue(  rational<INT>(10,40)  >= -rational<INT>(10,40) );

        assertTrue(  m_one >= m_quarter );
        assertTrue(  rational<INT>(-75,100) >= m_quarter-m_one  );

        assertTrue( rational<INT>(79,57) >=  rational<INT>(75,150) );
        assertTrue( ( rational<INT>(10,40) = m_one) == m_one );
        assertTrue( rational<INT>(-75,100) == (m_quarter-m_one ) );

        assertEquals( rat<INT>("1", "1") ,  m_one );
        assertEquals( rat<INT>("1", "2") ,  m_half );
        assertEquals( rat<INT>("1", "4") ,  m_quarter );
    }

    {   // A33984
        rational<INT> m_five_neg(-5), m_half_neg(1,-2), m_cero(0);
        rational<INT> m_quarter_neg(-1,4), m_one_neg(-1);
        //1) Suma de dos numeros racionales positivos  con simplificacion
        assertTrue(m_half + m_quarter == rational<INT>(30,40) );
        assertTrue(rational<INT>(30,40) == m_quarter + m_half );
        //2) Suma de entero positivo  + racional positivo con simplificacion
        assertTrue(m_one + m_quarter == rational<INT>(125,100) );
        //3) Suma de racional positivo + entero positivo
        assertTrue(rational<INT>(125,100) == m_quarter+m_one );
        //4) Suma de entero negativo con racional positivo
        assertTrue(m_five_neg + m_half == rational<INT>(-9,2) );
        //5)Suma de racional positivo + entero negativo
        assertTrue(rational<INT>(-9,2)== m_half+m_five_neg);
        //6) Suma de entero negativo con racional negativo con simplificacion
        assertTrue(m_five_neg + m_quarter_neg == rational<INT>(-21,4) );
        //7) Suma de racional negativo + entero negativo
        assertTrue(rational<INT>(-20,16) == m_quarter_neg + m_one_neg);
        //8) Suma de entero positivo con racional negativo
        assertTrue(m_one + m_quarter_neg == rational<INT>(3,4));
        //9) Suma de racional negativo + entero positivo
        assertTrue(rational<INT>(3,4) == m_quarter_neg + m_one);
        //10) Suma de racionales negativos
        assertTrue(m_half_neg + m_quarter_neg == rational<INT>(-3,4));
        assertTrue(rational<INT>(-3,4) == m_quarter_neg+m_half_neg);
        //11) Suma de enteros negativos con simplicacion
        assertTrue(m_one_neg + m_five_neg == rational<INT>(-30,5));
        assertTrue(rational<INT>(-30,5) == m_five_neg + m_one_neg);
        //12)Suma de racional positivo + cero
        assertTrue(m_half + m_cero == rational<INT>(1,2));
        assertTrue(rational<INT>(1,2) == m_cero + m_half);
        //13)Suma de racional negativo + cero
        assertTrue(m_half_neg + m_cero == rational<INT>(-1,2));
        //14)Suma de cero + racional negativo
        assertTrue(rational<INT>(-1,2) == m_cero + m_half_neg);
        //15)Suma de entero positivo + cero
        assertTrue(m_one+ m_cero == rational<INT>(1));
        //16)Suma de cero + entero positivo
        assertTrue(rational<INT>(1) == m_cero + m_one);
        //17)Suma de entero negativo + cero
        assertTrue(-m_one+ m_cero == rational<INT>(-1));
        //18)Suma de cero + entero negativo
        assertTrue(rational<INT>(-1) == m_cero + m_one_neg);
        //19)Suma de ceros
        assertTrue(m_cero+ m_cero == rational<INT>(0));
        assertTrue(rational<INT>(0) == m_cero+ m_cero);
        //20)Suma de INT_MAX + cero
        assertTrue(rational<INT>(INT_MAX) + m_cero == rational<INT>(INT_MAX));
        //21)Suma de cero + INT_MAX
        assertTrue(rational<INT>(INT_MAX) ==m_cero + rational<INT>(INT_MAX));
        //22)Suma de - INT_MAX + cero
        assertTrue(rational<INT>(-INT_MAX) + m_cero == rational<INT>(-INT_MAX));
        //23)Suma de cero +- INT_MAX
        assertTrue(rational<INT>(-INT_MAX) == m_cero + rational<INT>(-INT_MAX));
        //24)Suma de dos racionales que de cero
        assertTrue(m_half_neg + m_half == rational<INT>(0));
        assertTrue(rational<INT>(0) == m_half + m_half_neg);
        //25)Suma de dos enteros que de cero
        assertTrue(m_one + m_one_neg == rational<INT>(0));
        assertTrue(rational<INT>(0) == m_one_neg+ m_one);
        //26)Suma de rational<INT>(INT_MAX,INT_MAX) + rational<INT>(INT_MAX,INT_MAX)
        assertTrue(rational<INT>(INT_MAX,INT_MAX) + rational<INT>(INT_MAX,INT_MAX) == rational<INT>(2));
        //27)Suma de rational<INT>(INT_MAX,INT_MAX) + rational<INT>(-INT_MAX,INT_MAX)
        assertTrue(rational<INT>(INT_MAX,INT_MAX) + rational<INT>(-INT_MAX,INT_MAX) == rational<INT>(0));
        //28)Suma cero + cero
        assertTrue(m_cero + m_cero == rational<INT>(0));

        assertEquals( rat<INT>("1", "1") ,  m_one );
        assertEquals( rat<INT>("1", "2") ,  m_half );
        assertEquals( m_one_neg ,  - m_one );
        assertEquals( m_half_neg , - m_half );
        assertEquals( - m_one_neg ,  m_one );
        assertEquals( - m_half_neg , m_half );
        assertTrue( m_one_neg ==  - m_one );
        assertTrue( m_half_neg == - m_half );
        assertTrue( - m_one_neg ==  m_one );
        assertTrue( - m_half_neg == m_half );
        assertTrue( m_one_neg ==  rational<INT>(-1) );
        assertTrue( m_half_neg == rational<INT>(-1,2) );

        assertTrue( m_half_neg + m_half == rational<INT>(0) );
        assertTrue( rational<INT>(0) == m_one_neg + m_one );

        assertTrue( (m_half_neg + m_half) == rational<INT>(0));
        assertTrue(  rational<INT>(0) == (m_one_neg + m_one) );
        assertTrue(  rat<INT>("0") == (m_one_neg + m_one) );
        assertEquals( (m_half_neg + m_half) , rational<INT>(0));
        assertEquals(  rational<INT>(0) , (m_one_neg + m_one) );
    }

    {   // datos de prueba viejos
        assertTrue( m_half+m_quarter == rational<INT>(30,40) );
        assertTrue( rational<INT>(30,40) == m_quarter+m_half );

        assertTrue( m_one+m_quarter == rational<INT>(125,100) );
        assertTrue( rational<INT>(125,100) == m_quarter+m_one );

        assertTrue(  rational<INT>(1,3) != rational<INT>(33,100)  );

        assertTrue(  m_half-m_quarter == rational<INT>(10,40) );
        assertTrue(  rational<INT>(10,40)  == -m_quarter+m_half );

        assertTrue(  m_one-m_quarter   == rational<INT>(75,100) );
        assertTrue(  rational<INT>(-75,100) == m_quarter-m_one  );

        assertEquals( rational<INT>(-75,100) ,  m_quarter-m_one );
        assertTrue(   rational<INT>(125,100) == m_one+m_quarter );

        assertTrue(  m_half * m_quarter == rational<INT>(125,1000) );
        assertTrue(  rational<INT>(125,1000)  == m_quarter * m_half );

        assertTrue(  m_one * m_quarter   == rational<INT>(10,40) );
        assertTrue(  rational<INT>(10,40) == m_quarter *  m_one  );

        assertTrue(  rational<INT>(25,100) == m_one * m_quarter );

        assertTrue(  m_half / m_quarter == rational<INT>(100,50) );
        assertTrue(  rational<INT>(50,100)  == m_quarter / m_half );

        assertTrue(  m_one / m_quarter   == rational<INT>(40,10) );
        assertTrue(  rational<INT>(10,40) == m_quarter /  m_one  );

        assertTrue(  rational<INT>(100, 100) == m_half / m_half );
    }

    if (m_super_lerdo) {   // 2^1024 * 3 ^ 512;
        INT deDOS=1, deTRES=1;
        for (int i=1; i<=1024; ++i ) {
            INT elOTRO = (2 * deDOS);
            assertTrue( deDOS < elOTRO );
            deDOS = elOTRO;
        }
        for (int i=1; i<=512; ++i ) {
            deTRES *= 3;
        }
        assertTrue( mcd<INT>(deDOS, deTRES) == 1 );
    }

    if (m_super_lerdo) {   // Progresión geométrica
        int n = 1024;  //         1         2        3          4
        INT r; set( r , "1234567890123456789012345678901234567890" );
        INT a; set( a , "9876543210987654321098765432109876543210" );

        INT SUMA = 0;
        INT u = a;
        for ( int i=0; i<n; ++i ) {
            SUMA += u;
            u *= r;
        }
        assertTrue( SUMA == a * ( pow(r,n) - 1 ) / ( r-1 ) );
    }

    if (m_super_numerotes) {
        rational<INT> r,s,t;
        set(r, "10000000000000000", "4" );
        set(s, "100000000", "2" );
        set(t, "100000000", "2" );
        assertTrue( r == s * t );
        assertTrue( s * t == r );
    }

    if (m_super_numerotes) {   //  A54641.cpp
        assertTrue( rat<INT>("10000000000000000", "4" ) == rat<INT>("100000000", "2" ) * rat<INT>("100000000", "2" ) );
        assertTrue( rat<INT>( "900" , "2" ) * rat<INT>( "100000000000" ) == rat<INT>( "90000000000000" , "2" ) );
        assertTrue( rat<INT>( "1" , "1000000" )/rat<INT>( "1000000" , "1" ) == rat<INT>( "1" , "1000000" )*rat<INT>( "1" , "1000000" ) );
        assertTrue(  (rat<INT>("900","2")  *= rat<INT>("100000000000")) == rat<INT>("90000000000000","2") );
        assertTrue( (rat<INT>("1","1000000") /= rat<INT>("1000000","1")) ==  rat<INT>("1","1000000000000") );
        assertTrue( - rat<INT>("8000000000000000") == ( rational<INT>() - rat<INT>("8000000000000000") ) );
        assertTrue( rational<INT>(20,100) == rat<INT>("20000000000","100000000000") );
        assertTrue( ( rat<INT>( "1" , "2" ) + rat<INT>( "85" , "40000" ) ) == rat<INT>( "40170" , "80000" ) );
        assertTrue( ( m_half + rat<INT>( "85" , "40000" ) ) == rat<INT>( "40170" , "80000" ) );
    }

    // assertTrue(  rational<INT>(100, 100) != m_half / m_half );
    // assertFalse( rational<INT>(100, 100) == m_half / m_half );
}

#ifdef __GNU__
    #define NO_long_long
#endif

/// Programa principal [viejo] desde donse se invocan todas las pruebas
int main_viejo() {
    #ifdef NO_long_long
        if (1) { // código para ver si aHilera() funciona
            const char * n = aHilera<long long>(  12345678900 );
            const char * m = aHilera<long long>( 987654321001 );
            const char * p = aHilera<long long>(  -8765432100 );
            cout << n << endl;
            cout << m << endl;
            cout << p << endl;
            delete [] n;
            delete [] m;
            delete [] p;
        }
        if (1) { // código para ver si grabador() funciona
            long long
                n =  12345678900,
                m = 987654321001,
                p =  -8765432100;
            grabador( cout , n ); cout << endl;
            grabador( cout , m ); cout << endl;
            grabador( cout , p ); cout << endl;
        }
    #endif

    if (1) { // código para ver si aHilera( rational<INT> ) funciona
        rational<HugeInt> r(-12,5);
        const char * c = aHilera( r );
        cout << r << " ==> " << c << endl;
        delete [] c;
    }
    if (1) { // código para ver que "cout <<" funciona
        rational<HugeInt>
            n_five ( 5 ),
            n_five_neg ( -5 ),
            n_half_neg(1,-2),
            n_cero ( 0 ),
            n_quarter_neg(-1,4),
            n_one_neg ( -1 );
        cout << n_cero << endl;
        cout << n_five << endl;
        cout << n_quarter_neg << endl;
        cout << n_one_neg << endl;
        cout << n_half_neg << endl;
        cout << n_five_neg << endl;
    }
    return 0;
}

#include <cstdio> // isdigit()

/// Toma los dígitos de \c str y los asigna como valor de \c num.
/// - Solo incluye los números que están al principio de la hilera \c str.
/// - Los valores negativos comienzan con el caracter \c '-' al principio de la hilera.
/// - Retorna \c num.
template <class INT>
INT& set( INT& num, const char * str ) {
    num = INT(0);
    if ( str==0 ) {
        return num;
    }
    bool neg; // indica que el  número es negativo
    if ( *str == '-') {
        neg = true;
        str++;
    }
    else  {
        neg = false;
    }
    INT DIEZ = INT(10);
    while ( isdigit(*str) ) {
        num *= DIEZ;
        num += INT( (*str) - '0' );
        str++;
    }
    if (neg) {
        num = -num;
    }
    return num;
}


/// Toma los dígitos de \c num y \c den y los asigna como valor de \c r.
/// - Solo incluye los números que están al principio de las hileras.
/// - Los valores negativos comienzan con el caracter \c '-' al principio de la hilera.
/// - Retorna \c r.
template <class INT>
rational<INT>& set( rational<INT>& r, const char * num, const char * den ) {
    INT NUM, DEN;
    set( NUM, num );
    set( DEN, den );
    r.set( NUM, DEN );
    return r;
}

/// Calcula y retorna el residuo \c ( n % m ).
template <class INT>
inline INT operator% ( const INT& n , const INT& m ) {
    INT temp = n/m;
    return INT( n - temp * m );
/*  Nota:
    Supongamos que ya tenemos un algoritmo op/(n,m) que calcula la
    división entera (n/m). A partir de ese algoritmo de división se
    puede calcular el residuo usando una multiplicación y una resta.

    El valor que el algoritmo de la división d==(n/m) tiene la
    siguiente propiedad:
    - n == d*m + r, en donde 0 <= r < m.

    A partir de esta ecuación se puede hacerla siguiente deducción:
    n == d*m + r             <==>
    d*m + r == n             <==>
    r == n - (d*m)           <==>  d == op/( n , m )
    r == n - (op/(n,m) * m)

    O sea que es posible calcular el residuo usando la fórmula
    (n - (op/(n/m) * m)
*/
}

/// Especialización para que aHilera<INT>() funcione con racionales.
template <class INT>
const char * aHilera(const rational<INT>& r) {
    INT temp = r.num() / r.den();
    return aHilera(temp);
    /*  Nota
        Como "r" es un número racional, no es posible "sacarle" los
        dígitos usando el truco del módulo diez:
            digito = numero % 10; // último "digito" de "numero"
        Cada vez que se usa la división, la clase rational<> produce un
        valor quebrado que nunca llegará a sercero, por lo que algoritmo
        aHilera( rational<INT> ) nunca retorna si se le usa con un
        argumento racional.
        - Por eso hace falta esta especialización de la plantilla.
    */
}

/// Construye una hilera que tiene el valor de \c r en dígitos decimales.
/// - Al terminar de usar la hilera, es responsabilidad del programador
///   usuario retornar la memoria de la hilera retornada.
/// \pre \c INT debe ser una clase entera (no puede ser \c rational<INT>).
template <class INT>
const char * aHilera(const INT& r) {
    int signo = 0; // cantidad de campos para almacenar el signo: { 0 , 1 }
    if ( r == INT(0) ) {
        char * res = new char[2];
        res[0] = '0';
        res[1] =  0 ;
        return res;
    }
    else if ( r<INT(0) ) {
        signo = 1; // el '-' ocupa un campo en la hilera de retorno
    }
    INT DIEZ = 10, digito;
    INT tmp = ( signo==0 ? r : 0-r );
    int n=0;  // cantidad de dígitos de "r"
    while ( tmp != INT(0) ) {
        tmp /= DIEZ;
        ++n;
    }
    char * res = new char[n+1+signo];
    res[n+signo] = 0;
    if (signo == 1) {
        res[0] = '-';
        ++res; // se "brinca" el signo
        tmp = 0-r;
    }
    else {
        tmp = r;
    }
    while ( n>0 ) {
        digito = tmp % DIEZ;
        tmp /= DIEZ;
        --n;
        for ( int j=0; j<10; ++j ) { // no usa el convertidor <INT> ==> <int>
            if ( digito == INT(j) ) {
                res[n] = char('0'+j); // encuentra cuál dígito escoger
                break;
            }
        }
    }
    if (signo == 1) {
        --res; // se "des-brinca" el signo
    }
    return res;
}

/// Graba el valor de \c "n" en el flujo \c "COUT".
template <class INT>
void grabador (ostream &COUT, const INT & r) {
    if ( r < INT(0) ) {
        COUT << "-";
        grabador( COUT , -r );
        return;
    }
    else if ( r == INT(0) ) {
        COUT << "0";
        return;
    }
    INT DIEZ = 10;
    INT tmp = r;
    INT digito, vuelto = 0;
    int n=0;
    while ( tmp != INT(0) ) {
        digito = tmp % DIEZ; // en "vuelto" quedan los dígitos
        tmp /= DIEZ;         // de "tmp" en orden inverso
        vuelto *= DIEZ;
        vuelto += digito;
        ++n;
    }
    for ( int i=0; i<n; ++i ) {
        digito = vuelto % DIEZ;
        vuelto /= DIEZ;
        for ( int j=0; j<10; ++j ) {
            if ( digito == INT(j) ) {     // encuentra cuál dígito escoger
                COUT << char('0'+j);
                break;
            }
        }
    }
    // este algoritmo funciona con cualquier valor entero
}

/// Calcula y retorna <code> r^n == r * ... * r  </code> (\c n veces).
template <class INT>
INT pow( const INT & r, int n ) {
    INT res = 1;
    for ( int i=0; i<n; ++i ) {
        res *= r;
    }
    return res;
}

/// Método de ordenamiento de selección.
/// - Deja el arregle \c A[] ordenado.
/// - Trabaja en el rango \c [0..n-1].
template <class T>
void selection_sort( T* A, size_t n) {
    for ( size_t i=0, m=n-1; i<m; ++i ) {
        size_t idx = i;
        T      min = A[idx];
        for (size_t j=i+1; j<n; ++j) {
            /* compare each key with current min */
            if (A[j] < min) {
                idx = j;
                min = A[idx];
            }
        }
        T tmp  = A[i];   // Intercambia(A[i], A[idx])
        A[i]   = A[idx];
        A[idx] = tmp;
    }
}
/// Retorna "true" cuando el vector está ordenado ascendentemente.
/// - Revisa desde \c A[0] hasta \c A[N-1].
template <class T>
bool Ordenado (T* A, size_t n) {
    for (size_t i = 0; i < n-1; ++i) {
        if (A[i] > A[i+1]) {
            return false;
        }
    }
    return true;
} // Ordenado()

#undef TEST_BIG_NUM

#if defined(_INCLUDED_NUMBER_H) && defined(USE_SMART_PTR)
    // Clase Number Krzysztof Bosak
    #define TEST_BIG_NUM
#endif

#if defined(GranNum_h) && defined(FALSE)
    // Clase BigNum de Guillermo Narvaja
    #define TEST_BIG_NUM
#endif

#ifdef TEST_BIG_NUM
#endif

/// Programa principal desde donse se invocan todas las pruebas
int main() {
    if (1) {
        test_rational<HugeInt>  test_rational_Instance;
        test_rational_Instance.setUp();
        test_rational_Instance.super_lerdo(false);
        test_rational_Instance.run();
        cout << test_rational_Instance.report() << endl;
    //  cout << test_rational_Instance.toXML();
    }
    if (1) {
        test_rational<long long>  test_rational_Instance;
        test_rational_Instance.setUp();
        test_rational_Instance.run();
        cout << test_rational_Instance.report() << endl;
    }
    if (1) {
        test_rational<int>  test_rational_Instance;
        test_rational_Instance.setUp();
        test_rational_Instance.run();
        cout << test_rational_Instance.report() << endl;
    }
    if (1) {
        test_rational<short>  test_rational_Instance;
        test_rational_Instance.setUp();
        test_rational_Instance.run();
        cout << test_rational_Instance.report() << endl;
    }
    if (0) { int main_viejo(); main_viejo(); }
    return 0;
}

// EOF: test_rational.cpp
