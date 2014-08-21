#ifndef Acumulador_h
#define Acumulador_h ///< Evita la inclusión multiple

/** Contiene la suma de números quitando al más grande y al más pequeño.
 
    En algunas aplicaciones conviene contar con la clase Acumulador que sirve para sumar un montón de valores numéricos, pero eliminando del total tanto al más grande como al más pequeño de todos los valores. Por ejemplo, el acumulador aplicado a los valores (1,5,2,2,3,3,4,5) sólo sumaría 5+2+2+3+3+4 y eliminaría el 1 y alguno de los 5's, que son los valores el menor y el mayor de toda la secuencia.
*/

class Acumulador{
    
    private:long m_total;  ///<  Total acumulado hasta el momento.
	long m_cantidad;       ///<  Número de valores agregados al acumulador.
	long m_menor;          ///<  Valor menor agregado hasta el momento.
    long m_mayor;          ///<  Valor mayor agregado hasta el momento.
    
    /** Constructor de vector.
        \dontinclude test_Acumulador.cpp
        \skipline    test::constructor()
        \until       }}
        \see         test_Acumulador::test_constructor()
    */
    public:Acumulador():m_total(0),m_cantidad(0),m_menor(0),m_mayor(0){
        /*borre();*/
    }
    
    /** Destructor. */
    ~Acumulador() {}
    
    /** Constructor de copia.
        \dontinclude test_Acumulador.cpp
        \skipline    test::copie_igual()
        \until       }}
        \see         test_Acumulador::test_copie_igual()
     */
    Acumulador(const Acumulador&o) {
        *this=o;
    }
    
    /** Copiador.
        \dontinclude test_Acumulador.cpp
        \skipline    test::copie()
        \until       }}
        \see         test_Acumulador::test_copie()
     */
    const Acumulador& operator=(const Acumulador&o) {
        m_total     = o.m_total;
        m_cantidad  = o.m_cantidad;
        m_menor     = o.m_menor;
        m_mayor     = o.m_mayor;
        return *this;
    }
    
    /** Inicializa en cero el acumulador.
        \dontinclude test_Acumulador.cpp
        \skipline    test::borre()
        \until       }}
        \see         test_Acumulador::test_borre()
     */
    void borre(){
        m_total = m_cantidad = m_menor = m_mayor = 0;
    }
    
    /** Retorna el valor total acumulado hasta el momento.
        \dontinclude test_Acumulador.cpp
        \skipline    test::total_cantidad()
        \until       }}
        \see         test_Acumulador::test_total_cantidad()
     */
    long total() const {
        return((m_cantidad <= 2) ? 0 : m_total-m_menor-m_mayor);
    }
    
    /** Retorna el número de valores agregados al acumulador.
        \dontinclude test_Acumulador.cpp
        \skipline    test::total_cantidad()
        \until       }}
        \see         test_Acumulador::test_total_cantidad()
    */
    long cantidad() const {
        return m_cantidad;
    }
    
    /** Agrega el valor <code> val </code> al acumulador.
        \dontinclude test_Acumulador.cpp
        \skipline    test::acumule()
        \until       }}
        \see         test_Acumulador::test_acumule()
    */
    void acumule ( long val ) {
        
        m_total += val;
        m_cantidad++;
        
        if(m_cantidad > 1) {
            m_menor =( val < m_menor ? val : m_menor);
            m_mayor =( val > m_mayor ? val : m_mayor);
        }
        else {
            m_menor = m_mayor = val;
        }
    }

    /** Agrega los primeros <code> n </code> valores del vector <code> val[] </code> al acumulador.
        \dontinclude test_Acumulador.cpp
        \skipline    test::acumule()
        \until       }}
        \see         test_Acumulador::test_acumule()
    */
    void acumule( unsigned n, const long * val) {
        
        for(unsigned i=0; i<n; ++i) {
            acumule(val[i]);
        }
    }
    
    /// ¿ A == B ?. 
    friend bool operator == (const Acumulador&A,const Acumulador&B) {
        return( A.m_total == B.m_total && A.m_cantidad == B.m_cantidad && A.m_menor==B.m_menor &&A.m_mayor==B.m_mayor);
        
    }
    
    /** ¿ A != B ?.
        \dontinclude test_Acumulador.cpp
        \skipline    test::copie_igual()
        \until       }}
        \see         test_Acumulador::test_copie_igual()
    */
    friend bool operator!=(const Acumulador&A, const Acumulador&B) {
        return!(A==B);
    }
    
    friend bool check_ok( const Acumulador&A);

    /// Datos de prueba para la clase Acumulador.
    friend class test_Acumulador;
};
/* bool */
#endif
