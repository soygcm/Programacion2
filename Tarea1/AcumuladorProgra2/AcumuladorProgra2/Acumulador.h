#ifndef Acumulador_h
#define Acumulador_h ///< Evita la inclusi—n multiple

class Acumulador{
    
    private:long m_total; ///< Total acumulado hasta el momento.
	long m_cantidad;
	long m_menor;
    long m_mayor;
    
    /** Constructor de vector.
        \dontinclude test_Acumulador.cpp
        \skipline    test::constructor()
        \until       }}
        \see         test_Acumulador::test_constructor()
    */
    public:Acumulador():m_total(0),m_cantidad(0),m_menor(0),m_mayor(0){
        /*borre();*/
    }
    
    ~Acumulador(){
        
    }
    
    Acumulador(const Acumulador&o){
        *this=o;
    }
    
    const Acumulador& operator=(const Acumulador&o){
        m_total=o.m_total;
        m_cantidad=o.m_cantidad;
        m_menor
        =o.m_menor;
        m_mayor=o.m_mayor;
        return*this;
    }
    
    void borre(){
        m_total = m_cantidad = m_menor = m_mayor = 0;
    }
    
    long total() const{
        return((m_cantidad <= 2) ? 0 : m_total-m_menor-m_mayor);
    }
    
    long cantidad() const{
        return m_cantidad;
    }
    
    void acumule(long val){
        
        m_total += val;
        m_cantidad++;
        
        if(m_cantidad > 1){
            m_menor =( val < m_menor ? val : m_menor);
            m_mayor =( val > m_mayor ? val : m_mayor);
        }
        else{
            m_menor = m_mayor = val;
        }
    }
    
    void acumule( unsigned n, const long * val){
        
        for(unsigned i=0; i<n; ++i){
            acumule(val[i]);
        }
    }
    
    friend bool operator == (const Acumulador&A,const Acumulador&B){
        return( A.m_total == B.m_total && A.m_cantidad == B.m_cantidad && A.m_menor==B.m_menor &&A.m_mayor==B.m_mayor);
        
    }
    
    friend bool operator!=(const Acumulador&A, const Acumulador&B){
        return!(A==B);
    }
    
    friend bool check_ok( const Acumulador&A);
    friend class test_Acumulador;
};
/* bool */
#endif
