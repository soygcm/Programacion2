// rational.h   (c) 2007 adolfo@di-mare.com

/** \file  rational.h
    \brief Declara el tipo \c "rational".
    - La clase \c rational implementa las operaciones aritméticas
      principales para números rationales.

    - <code> [1/3] == [2/6] ==  ...   [9/27] == ... </code>
    - <code> [1/3]  * [2/6] / [3/9] - [9/27] </code>

    - Permite usar racionales en cualquier sitio en donde se puedan
      usar valores numéricos.

    \author Adolfo Di Mare <adolfo@di-mare.com>
    \date   2005
*/

#ifndef rational_h
#define rational_h ///< Evita la inclusión múltiple

/// Definido por la biblioteca C++ estándar
namespace std { }

#include <iostream> // std::cin && std::cout
#include <list>     // std::list<>
// using namespace std;

/**  La clase \c rational implementa las operaciones aritméticas
     principales para números rationales.
     - <code> [1/3] == [2/6] ==  ...   [9/27] == ... </code>
     - <code> [1/3]  * [2/6] / [3/9] - [9/27] </code>
*/
template <class INT>
class rational {
private:
    std::list<INT> m_Rep; ///< Lista que contiene al numerador y al denominador
private:
/*  ELIMINADO:
    INT m_num; ///< Numerador
    INT m_den; ///< Denominador
*/

    void simplify();
public:
    typedef INT value_type; ///< Nombre estándar del tipo de la plantilla.
    /// Constructor de vector.
    /// \see test_rational<NUM>::test_constructor().
    rational() : m_Rep() { m_Rep.push_back(0); m_Rep.push_back(1); }
    /// Constructor a partir de un valor entero.
    rational(INT num) : m_Rep() { m_Rep.push_back(num); m_Rep.push_back(1); }
    /// Constructor a partir de un valor quedbrado.
    rational(INT num, INT den)
        : m_Rep() { m_Rep.push_back(num); m_Rep.push_back(den); simplify(); }
    /// Constructor de copia
    rational(const rational& o) : m_Rep() { m_Rep=o.m_Rep; }
    ~rational() { }      ///< Destructor

    void set( const INT& n=INT(0), const INT& d=INT(1) );  // Le cambia el valor a \c "*this"
public:
    /** Acceso al numerador.
    \dontinclude test_rational.cpp
    \skipline    test::num_den()
    \until       }}
    \see         test_rational<NUM>::test_num_den()
    */
    const INT& num() const { return *(m_Rep.begin()); }
    /// Acceso al denomirador (siempre >0).
    const INT& den() const { return *( ++(m_Rep.begin()) ) ; }
private:
    /// Versión modificadora de \c num().
    INT& num() { return *(m_Rep.begin()); }
    /// Versión modificadora de \c den().
    INT& den() { // return *( ++(m_Rep.begin()) ) ;
        typename std::list<INT>::iterator it = m_Rep.begin();
        ++it;
        return *it;
    }
public:
//  void setNum(const INT& n) { m_num=n; simplify(); }                     // FEO
//  void setDen(const INT& d) { m_den= ( d!=0 ? d : m_den) ; simplify(); } // FEO

    rational& operator= (const rational&);  // Asignación (copia)
    rational& operator= (INT);
    rational& swap ( rational& );

    rational& operator+=( const rational& );
    rational& operator-=( const rational& );
    rational& operator*=( const rational& );
    rational& operator/=( const rational& );

    rational operator-() const;              // menos unario

    // uso NUM para no caerle encima ("shadow") a INT que es el tipo de la plantilla
    template <class NUM> friend rational<NUM> operator+( const rational<NUM>&, const rational<NUM>& );
    template <class NUM> friend rational<NUM> operator-( const rational<NUM>&, const rational<NUM>& );
    template <class NUM> friend rational<NUM> operator*( const rational<NUM>&, const rational<NUM>& );
    template <class NUM> friend rational<NUM> operator/( const rational<NUM>&, const rational<NUM>& );

    template <class NUM> friend bool operator==( const rational<NUM>&, const rational<NUM>& );
    template <class NUM> friend bool operator<(  const rational<NUM>&, const rational<NUM>& );
    template <class NUM> friend bool operator!=( const rational<NUM>&, const rational<NUM>& );
    template <class NUM> friend bool operator<=( const rational<NUM>&, const rational<NUM>& );
    template <class NUM> friend bool operator>=( const rational<NUM>&, const rational<NUM>& );
    template <class NUM> friend bool operator>(  const rational<NUM>&, const rational<NUM>& );

    template <class NUM> friend rational<NUM>& operator++( rational<NUM> & r );       // ++r
    template <class NUM> friend rational<NUM>  operator++( rational<NUM> & r , int ); // r++
    template <class NUM> friend rational<NUM>& operator--( rational<NUM> & r );
    template <class NUM> friend rational<NUM>  operator--( rational<NUM> & r , int );

    template <class NUM> friend std::ostream& operator<< (std::ostream &, const rational<NUM>& );
    template <class NUM> friend std::istream& operator>> (std::istream &,       rational<NUM>& );
    rational& fromString (const char* nStr);

    template <class NUM> friend double real   (const rational<NUM>& );   // Conversión a real
    template <class NUM> friend long   integer(const rational<NUM>& );   // Conversión a entero

    template <class NUM> friend bool check_ok( const rational<NUM>& r ); // Ok()
//  excluidos porque producen ambigüedad con operadores aritméticos
//  template <class NUM> operator double () { return double(m_num) / double(m_den); }
//  template <class NUM> operator NUM    () { return        m_num  /        m_den ; }
    template <class NUM> friend class test_rational; ///< Datos de prueba para la clase.
}; // rational

template <class NUM>
NUM mcd(NUM x, NUM y); // Calcula el Máximo Común Divisor

/// Sinónimo de \c mcd(x,y).
template <class INT>
inline INT gcd( const INT& x, const INT& y ) { return mcd(x,y); }

/** Cambia el valor del número rational a \c "n/d".
    \pre <code> d != 0 </code>.

    \dontinclude test_rational.cpp
    \skipline    test::set()
    \until       }}
    \see         test_rational<NUM>::test_set()
*/
template <class INT>
inline void rational<INT>::set( const INT& n, const INT& d ) {
    num() = n;
    #ifdef NDEBUG
        m_den = d;
    #else
        if ( d==INT(0) ) {
            den() = INT(1);
        }
        else {
            den() = d;
        }
    #endif
    simplify();
}

/** Copia desde \c "o".
    - El valor anterior de \c "*this" se pierde.

    \par Complejidad:
         O( \c 1 )
    \returns *this
    \see http://www.di-mare.com/adolfo/binder/c04.htm#sc05

    \dontinclude test_rational.cpp
    \skipline    test::op_equal()
    \until       }}
    \see         test_rational<NUM>::test_op_equal()
*/
template <class INT>
inline rational<INT>& rational<INT>::operator=( const rational<INT>& o ) {
    num() = o.num(),
    den() = o.den();

//  sobra invocar a "simplify()" pues "o" ya está simplificado
    return *this;
}

/** Intercambia los valores de \c "*this" y \c "o".
      \par Complejidad:
         O( \c 1 )
    \returns *this
    \see http://www.di-mare.com/adolfo/binder/c04.htm#sc08

    \dontinclude test_rational.cpp
    \skipline    test::swap()
    \until       }}
    \see         test_rational<NUM>::test_swap()
*/
template <class INT>
inline rational<INT>& rational<INT>::swap( rational<INT>& o ) {
    #if 1
        rational tmp = o;
        o = *this;        // o.operator=( *this );
        *this = tmp;
    #else
        // Esto NO funciona para objetos, métodos virtuales, etc.
        char tmp[ sizeof( *this ) ];
        memcpy( tmp,  & o,   sizeof( *this ) ); // tmp = o;
        memcpy( & o,  this,  sizeof( *this ) ); // o = *this;
        memcpy( this, tmp,   sizeof( *this ) ); // *this = tmp;
        // Esto violenta la integridad del Rep
    #endif
    return *this;
}

/** Asignación desde un \c "INT".
    \dontinclude test_rational.cpp
    \skipline    test::op_equal()
    \until       }}
    \see         test_rational<NUM>::test_op_equal()
*/
template <class INT>
inline rational<INT>& rational<INT>::operator= ( INT entero ) {
    num() = entero;
    den() = INT(1);
    return *this;
//  el prototipo debiera ser operator= ( const INT& entero ) ...
}

/** Multiplica \c "*this" por \c "num".
    \dontinclude test_rational.cpp
    \skipline    test::op_mult_equal()
    \until       }}
    \see         test_rational<NUM>::test_op_mult_equal()
*/
template <class INT>
inline rational<INT>& rational<INT>::operator*=( const rational<INT>& rt ) {
    num() *= rt.num();
    den() *= rt.den();
    simplify();
    return *this;
}

/** Divide \c "*this" por el valor de \c "num".
    \pre (num != 0)

    \dontinclude test_rational.cpp
    \skipline    test::op_mult_equal()
    \until       }}
    \see         test_rational<NUM>::test_op_mult_equal()
*/
template <class INT>
inline rational<INT>& rational<INT>::operator/=( const rational<INT>& num ) {
    num() *= num.den();
    den() *= num.num();
    simplify();
    return *this;
}

/** \c "-x".
    - Menos unario.
    - Calcula y retorna el valor \c "-x".

    \dontinclude test_rational.cpp
    \skipline    test::op_minus()
    \until       }}
    \see         test_rational<NUM>::test_op_minus()
*/
template <class INT>
inline rational<INT> rational<INT>::operator-() const {
    rational tmp = (*this);  // tmp.rational( *this );
    tmp.m_num = - tmp.m_num;
    return tmp;
}

/** ¿ x == y ?.
    \dontinclude test_rational.cpp
    \skipline    test::op_comp()
    \until       }}
    \see         test_rational<NUM>::test_op_comp()
*/
template <class NUM>
inline bool operator==( const rational<NUM> &x, const rational<NUM> &y ) {
    return (x.m_num == y.m_num) && (x.m_den == y.m_den);
/*  Nota:
    Como los números racionales siempre están simplificados, no puede
    ocurrir que [1/1] está almacenado como [3/3] y en consecuencia
    basta comparar los valores campo por campo para determinar si se
    da o no la igualdad.
*/
}

/// ¿ x < y ?
template <class NUM>
inline bool operator<( const rational<NUM> &x, const rational<NUM> &y ) {
    return (x.m_num * y.m_den) < (x.m_den * y.m_num);
/*  Nota:
    Una desigualdad de fracciones se preserva siempre que se
    multiplique a ambos lados por un número positivo. Por eso:
            [a/b] < [c/d]          <==>
    (b*d) * [a/b] < [c/d] * (b*d)  <==>
    (b/b) * (d*a) < (b*c) * (d/d)  <==>
            (d*a) < (b*c)

    [a/b] > [c/d] <==> [(b*d)*a/b] > [(b*d)*c/d] <==> [d*a] > [b*c]

    Debido a que el denominador siempre es un número positivo, el
    trabajo de comparar 2 racionales se puede lograr haciendo 2
    multiplicaciones de números enteros, en lugar de convertirlos
    a punto flotante para hacer la división, que es hasta un orden
    de magnitud más lento.
*/
//  return double(x.m_num) / double(x.m_den) < double(y.m_num) / double(y.m_den);
}

/// ¿ x > y ?
template <class NUM>
inline bool operator>( const rational<NUM> &x, const rational<NUM> &y ) {
    return (y < x);
}

/// ¿ x != y ?
template <class NUM>
inline bool operator!=( const rational<NUM>& x, const rational<NUM>& y ) {
    return !(x == y);
}

/// ¿ x <= y ?
template <class NUM>
inline bool operator<=( const rational<NUM>& x, const rational<NUM>& y ) {
    return !(y < x);
}

/// ¿ x >= y ?
template <class NUM>
inline bool operator>=( const rational<NUM>& x, const rational<NUM>& y ) {
    return !(x < y);
}

/// Convertidor a punto flotante.
template <class NUM>
inline double real(const rational<NUM>& num) {
    return double (num.m_num) / double (num.m_den);
}

/// Convertidor a punto fijo.
template <class NUM>
inline long integer(const rational<NUM>& num) {
    return long  ( num.m_num / num.m_den );
}

#if 0
    /// Convertidor a punto fijo
    template <class NUM>
    inline rational<NUM>::operator NUM() {
        return NUM (m_num / m_den);
    }
#endif

#include  <cstdlib>
#include  <cctype>     // isdigit()

/** Verifica la invariante de la clase \c rational.
    \par <em>Rep</em> Modelo de la clase:
    \code
    +---+
    | 3 | <==  m_num == numerador del número racional
    +---+
    |134| <==  m_den == denominador del número racional
    +---+
    \endcode
    - http://www.di-mare.com/adolfo/binder/c03.htm#k1-Rep

    \remark
    Libera al programador de implementar el método \c Ok()
    - http://www.di-mare.com/adolfo/binder/c04.htm#sc11


    \dontinclude test_rational.cpp
    \skipline    test::check_ok()
    \until       }}
    \see         test_rational<NUM>::test_check_ok()
*/
template <class NUM>
bool check_ok( const rational<NUM>& r ) {
    if ( &r != 0 ) {
        // Ok
    }
    else {
        /// - Invariante: ningún objeto puede estar almacenado en la posición nula.
        return false;
    }

    if ( r.m_den > NUM(0) )  {
        // Ok
    }
    else {
        /// - Invariante: el denominador debe ser un número positivo.
        return false;
    }
    if (r.m_num == NUM(0)) {
        if ( r.m_den == NUM(1) ) {
            /// - Invariante: el cero debe representarse con denominador igual a "1".
            return true;
        }
        else {
            return false;
        }
    }
    if ( mcd(r.m_num, r.m_den) == NUM(1) ) {
        // Ok
    }
    else {
        /// - Invariante: el numerador y el denominador deben ser primos relativos.
        return false;
    }
    return true;
}

/** Verifica la invariante de la clase \c rational.
    \remark
    Esta implementación nos se le mete al <em>Rep</em>
    (casi siempre no es posible implementar una función como ésta).
      - http://www.di-mare.com/adolfo/binder/c03.htm#k1-Rep
    \remark
    Libera al programador de implementar el método \c Ok()
    - http://www.di-mare.com/adolfo/binder/c04.htm#sc11
*/
template <class NUM>
bool check_ok_no_Rep( const rational<NUM>& r ) {
    if ( ! (&r != 0) ) {
        /// - Invariante: ningún objeto puede estar almacenado en la posición nula.
        return false;
    }

    if ( ! (r.den() > NUM(0)) )  {
        /// - Invariante: el denominador debe ser un número positivo.
        return false;
    }
    if (r.num() == NUM(0)) {
        if ( r.den() == NUM(1) ) {
            /// - Invariante: el cero debe representarse con denominador igual a "1".
            return true;
        }
        else {
            return false;
        }
    }
    if ( ! ( mcd(r.num(), r.den()) == NUM(1) ) ) {
        /// - Invariante: el numerador y el denominador deben ser primos relativos.
        return false;
    }
    return true;
}

/** Calcula el Máximo Común Divisor de los números \c "x" y \c "y".
    - <code> mcd(x,y) >= 1 </code> siempre.
    - MCD <==> GCD: <em> Greatest Common Divisor </em>.

    \pre
    <code> (y != 0) </code>

    \remark
    Se usa el algoritmo de Euclides para hacer el cálculo.

    \par Ejemplo:
    \code
    2*3*5 == mcd( 2*2*2*2 * 3*3 * 5*5, 2*3*5 )
       30 == mcd( -3600, -30 )
    \endcode

    \dontinclude test_rational.cpp
    \skipline    test::mcd()
    \until       }}
    \see         test_rational<NUM>::test_mcd()
*/
template <class NUM>
NUM mcd(NUM x, NUM y) {
    NUM g = (x < NUM(0) ? -x : x); // trabaja con valores positivos
    NUM r = (y < NUM(0) ? -y : y); // "r" es el resto
    NUM temp;

    do {
        temp = r;
        r    = g % r;
        g    = temp;
    } while (NUM(0) != r);

    return g;
}

/** Simplifica el numerador y el denomidador.
    - Transforma el número rational de manera que el numerador y el
      denominador sean primos relativos, asegurando además que el
      denominador es siempre positivo.
    - Si <code>(m_num==0) ==> (m_den==1)</code>.
    - Simplifica la fracción para que \c m_num y \c m_den sean números
      primos relativos ie, <code>mcd(m_num,m_den) == 1</code>.
    - Asegura que \c m_den sea un número positivo.
    - Restaura la invariante de la clase \c rational.

    \dontinclude test_rational.cpp
    \skipline    test::simplify()
    \until       }}
    \see         test_rational<NUM>::test_simplify()
*/
template <class NUM>
void rational<NUM>::simplify() {
    if (num() == NUM(0)) {
       den() = NUM(1);
       return;
    }
    NUM divisor = mcd(num(), den());
    if (divisor > NUM(1)) {   // ==> (divisor != 0)
        num() /= divisor;
        den() /= divisor;
    }
    if (den() < NUM(0)) {
        num() = -num();
        den() = -den();
    }
}

/** Le suma a \c "*this" el valor de \c "otro".
    \dontinclude test_rational.cpp
    \skipline    test::op_add_equal()
    \until       }}
    \see         test_rational<NUM>::test_op_add_equal()
*/
template <class INT>
rational<INT>& rational<INT>::operator+=( const rational<INT>& otro ) {
    num()  = num() * otro.den() + den() * otro.num();
    den() *= otro.den();
    simplify();
    return *this;
}

/** Le resta a \c "*this" el valor de \c "otro".
    \dontinclude test_rational.cpp
    \skipline    test::op_add_equal()
    \until       }}
    \see         test_rational<NUM>::test_op_add_equal()
*/
template <class INT>
rational<INT>& rational<INT>::operator-=( const rational<INT>& otro ) {
    INT oldm_den = den();
    INT oldm_num = num();
    INT d       = otro.den();
    INT n       = otro.num();

    den() *= d;
    num() = oldm_num * d - oldm_den * n;
    simplify();

    return *this;
}

/** Graba el valor de \c "r" en el flujo \c "COUT".
    - Graba el valor en el formato [num/den].
    - En particular, este es el operador que se invoca
      cuando se usa, por ejemplo, este tipo de instrucción:
     \code
          cout << r << q;
     \endcode

    \dontinclude test_rational.cpp
    \skipline    test::op_out()
    \until       }}
    \see         test_rational<NUM>::test_op_out()
*/
template <class NUM>
std::ostream& operator<<( std::ostream &COUT, const rational<NUM>& r ) {
    if ( r.den() == NUM(1) ) { // no hay parte fraccional
        return COUT << "[" << r.num() << "]" ;
    } else {
        return COUT << "[" << r.num() << "/" << r.den() << "]" ;
    }
}

/** Lee del flujo de texto \c "CIN" el valor de \c "r".
    \pre
    El número rational debe haber sido escrito usando
    el formato "[r/den]", aunque es permisible usar
    algunos blancos.
    - Se termina de leer el valor sólo cuando encuentra \c "]".
    - <code> [ -+-+-+-+- 4 / -- -+ -- 32  ] </code> se lee como
      <code> [1/8] </code>

    \dontinclude test_rational.cpp
    \skipline    test::op_in()
    \until       }}
    \see         test_rational<NUM>::test_op_in()
*/
template <class NUM>
std::istream& operator>>( std::istream &CIN, rational<NUM>& r ) {
    char ch;  // valor leido, letra por letra, de "CIN"
    const NUM DIEZ = NUM(10);

    bool es_positivo = true;    // manejo de los signos + y -

    // se brinca todo hasta el primer dígito
    do {
        CIN >> ch;
        if (ch == '-') {  // cambia de signo
            es_positivo = !es_positivo;
        }
    } while (!isdigit(ch));

    // se traga el numerador
    r.num() = NUM(0);
    while (isdigit(ch)) { // convierte a GranNum: izq --> der
        r.num() = DIEZ * r.num() + (ch-'0');
        CIN >> ch;
    }

    // se brinca los blancos después del numerador
    while (isspace(ch)) {
        CIN >> ch;
    }

    if (ch ==']') { // es un número entero
        r.den() = NUM(1);
    }
    else {
        do {  // se brinca todo hasta el denominador
            CIN >> ch;
            if (ch == '-') {
                es_positivo = !es_positivo;
            }
        } while (!isdigit(ch));

        // se traga el denominador
        r.den() = NUM(0);
        while (isdigit(ch)) {
            r.den() = DIEZ * r.den() + (ch-'0');
            CIN >> ch;
        }

        // El programa se duerme si en el flujo de entrada
        // NO aparece el caracter delimitador final "]",
        // pues la lectura termina hasta encontrar el "]".
        while (ch != ']') {
            CIN >> ch;
        }
    }   // corrección: Andrés Arias <e980300@anubis.ecci.ucr.ac.cr>


    // le cambia el signo, si hace falta
    if (! es_positivo) {
        r.num() = -r.num();
    }
    #ifndef NDEBUG
        if ( r.den() == NUM(0) ) {
            r.den() = NUM(1);
        }
    #endif

    r.simplify();
    return CIN;
/*
    no detecta errores...
    [1/0] lo lee y no se queja
    [ !#!#!$#@! 3/ aaaa 4  jajaja ] lo lee como [3/4]
    ... pero no se supone que el usuario cometa errores...
*/
}

/** Establece el varlor de \c "*this" a partir de la hilera \c "nStr".
    \pre \c "nStr" debe estar escrita en el formato "[num/den]".

    \dontinclude test_rational.cpp
    \skipline    test::fromString()
    \until       }}
    \see         test_rational<NUM>::test_fromString()
*/
template <class NUM>
rational<NUM>& rational<NUM>::fromString (const char* nStr) {
    char ch;  // valor obtenido, caracter por caracter, de "nStr"
    const NUM DIEZ = NUM(10);

    bool es_positivo = true;    // manejo de los signos + y -

    // se brinca todo hasta el primer dígito
    do {
        ch = *nStr; nStr++;
        if (ch == '-') {  // cambia de signo
            es_positivo = !es_positivo;
        }
    } while (!isdigit(ch));

    // se traga el numerador
    NUM num = NUM(0);
    while (isdigit(ch)) { // convierte a <NUM>: izq --> der
        num = DIEZ * num + NUM(ch-'0');
        ch = *nStr; nStr++;
    }

    // se brinca los blancos después del numerador
    while (isspace(ch)) {
        ch = *nStr; nStr++;
    }

    NUM den;
    if (ch ==']') { // es un número entero
        den = NUM(1);
    }
    else {
        do {  // se brinca todo hasta el denominador
            ch = *nStr; nStr++;
            if (ch == '-') {
                es_positivo = !es_positivo;
            }
        } while (!isdigit(ch));

        // se traga el denominador
        den = NUM(0);
        while (isdigit(ch)) {
            den = DIEZ * den + NUM(ch-'0');
            ch = *nStr; nStr++;
        }
        // Ya no importa si aparece o no el ']' del final del número
    }

    // le cambia el signo, si hace falta
    if (! es_positivo) {
        num = -num;
    }
    set( num, den );
    return *this;
}

/** \c "x+y".
    - Calcula y retorna la suma \c "x+y".

    \dontinclude test_rational.cpp
    \skipline    test::op_add()
    \until       }}
    \see         test_rational<NUM>::test_op_add()
*/
template <class NUM>
rational<NUM> operator+( const rational<NUM> &x, const rational<NUM> &y ) {
    NUM res_num, res_den;
    res_den = x.m_den * y.m_den;
    res_num = x.m_num * y.m_den + x.m_den * y.m_num;

    return rational<NUM>(res_num, res_den);
}

/** \c "x-y".
    - Calcula y retorna la resta \c "x-y".

    \dontinclude test_rational.cpp
    \skipline    test::op_add()
    \until       }}
    \see         test_rational<NUM>::test_op_add()
*/
template <class NUM>
rational<NUM> operator-( const rational<NUM> &x, const rational<NUM> &y ) {
    NUM res_num, res_den;
    res_den = x.den() * y.den();
    res_num = x.num() * y.den() - x.den() * y.num();

    return rational<NUM>(res_num, res_den);
}

/** \c "x*y".
    - Calcula y retorna la multiplicación \c "x*y".

    \dontinclude test_rational.cpp
    \skipline    test::op_mult()
    \until       }}
    \see         test_rational<NUM>::test_op_mult()
*/
template <class NUM>
rational<NUM> operator*( const rational<NUM> &x, const rational<NUM> &y ) {
    NUM res_num, res_den;
    res_num = x.m_num * y.m_num;
    res_den = x.m_den * y.m_den;

    return rational<NUM>(res_num, res_den);
}

/** \c "x/y".
    - Calcula y retorna la división \c "x/y".

    \pre <code> y != 0 </code>
    \dontinclude test_rational.cpp
    \skipline    test::op_mult()
    \until       }}
    \see         test_rational<NUM>::test_op_mult()
*/
template <class NUM>
rational<NUM> operator/( const rational<NUM> &x, const rational<NUM> &y ) {
    NUM res_num, res_den;
    #ifdef NDEBUG
        res_num = x.m_num * y.m_den;
        res_den = x.m_den * y.m_num;
        return rational<NUM>(res_num, res_den);
    #else
        if (NUM(0) != y.num()) {
            res_num = x.num() * y.den();
            res_den = x.den() * y.num();
            return rational<NUM>(res_num, res_den);
        }
        else {
            return rational<NUM>(NUM(0),NUM(1));
        }
    #endif
}

/** \c ++r.
    \dontinclude test_rational.cpp
    \skipline    test::op_cpp()
    \until       }}
    \see         test_rational<NUM>::test_op_cpp()
*/
template <class NUM>
inline rational<NUM>& operator++( rational<NUM> & r ) {
    r += rational<NUM>(NUM(1));
    return r;
}

/// \c r++.
template <class NUM>
inline rational<NUM> operator++( rational<NUM> & r , int ) {
    rational<NUM> tmp = r;
    r += rational<NUM>(NUM(1));
    return tmp;
}

/** \c --r.
    \dontinclude test_rational.cpp
    \skipline    test::op_cpp()
    \until       }}
    \see         test_rational<NUM>::test_op_cpp()
*/
template <class NUM>
inline rational<NUM>& operator--( rational<NUM> & r ) {
    r -= rational<NUM>(NUM(1));
    return r;
}

/// \c r--.
template <class NUM>
inline rational<NUM> operator--( rational<NUM> & r , int ) {
    rational<NUM> tmp = r;
    r -= rational<NUM>(NUM(1));
    return tmp;
}

#endif // rational_h

// EOF: rational.h
