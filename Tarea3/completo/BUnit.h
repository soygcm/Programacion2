// BUnit.h (C) 2008  adolfo@di-mare.com

/*
   THIS IS FREE SOFWTWARE.
   - Use at your own risk; acknowledge authorship, please.
   - You can never blame the author for your use of this software.
   - Released to the world under LGPLv3:
     http://www.gnu.org/licenses/lgpl-3.0.html
   ESTE PROGRAMA ES LIBRE.
   - Uselo baja su propia responsabilidad; reconozca la autoría, por favor.
   - Usted nunca podrá culpar al autor por su propio uso de este software.
   - Entregado al mundo bajo LGPLv3:
     http://www.gnu.org/licenses/lgpl-3.0.html
*/
/*
    Version 0: No TestSuite<>
    Version 1: English Doxygen
    Version 2: English+Spanish Doxygen
*/

#ifdef English_dox
/** \file  BUnit.h
    \brief [B]asic module for [unit] program testing.

    \author Adolfo Di Mare <adolfo@di-mare.com>
    \date   2008
*/
#endif
#ifdef Spanish_dox
/** \file  BUnit.h
    \brief Módulo [B]ásico para prueba [unit]aria de programas.

    \author Adolfo Di Mare <adolfo@di-mare.com>
    \date   2008
*/
#endif

#ifdef English_dox
    /// Doxygen English documentation.
    #define English_dox   "Doxygen English documentation"
   /// \def English_dox  ///< Marks English documentation blocks.
   /// \def BUnit_h      ///< Avoids multiple inclusion.
#endif
#ifdef Spanish_dox
    /// Documentación en español.
    #define Spanish_dox "Documentación en español"
   /// \def Spanish_dox  ///< Macro usado para que Doxygen genere documentación
   /// \def BUnit_h      ///< Evita la inclusión múltiple.
#endif

#ifndef BUnit_h
#define BUnit_h      // avoid multiple inclusion

#include <list>      // std::list
#include <string>    // class std::string
#include <sstream>   // class basic_ostringstream<T,Ch>
#include <cstring>   // memcpy()+strlen()
#include <typeinfo>  // class std::type_info ==> char* typeid().name();
#include <algorithm> // find()

namespace std {} // using namespace std;

/// Escuela de Ciencias de la Computación e Informática.
/// \see http://www.ecci.ucr.ac.cr
namespace ECCI { }

// Private class to hold test cases that didn´t succeed.
class TestCaseFailure {
private:
    const char * m_fname;  // file where the failure ocurred
    int          m_lineno; // line for the failure
    const char * m_label;  // descriptive message for the failure
    bool m_destroy_label;  // true when "m_label" requires destruction
private:
    TestCaseFailure() : // almost never used
        m_fname(""), m_lineno(0), m_label(""), m_destroy_label(false)
    { } // always sets "m_destroy_label" to "false"
    TestCaseFailure(const char * fname, int line, const char * label, bool destroy )
        : m_fname(fname), m_lineno(line), m_label(label), m_destroy_label(destroy)
    { }
public:
    ~TestCaseFailure() { if (m_destroy_label) { delete [] m_label; } }
    TestCaseFailure( const TestCaseFailure& o ) {
        m_fname = o.m_fname; m_lineno = o.m_lineno; m_label = o.m_label;
        m_destroy_label = o.m_destroy_label;
        // avoid double destruction of "m_label"
        const_cast<TestCaseFailure*>(&o)->m_destroy_label = false;
    //  const_cast<bool>(o.m_destroy_label) = false; // won't compile
    }
    void operator = ( TestCaseFailure& o ) {
        m_fname = o.m_fname; m_lineno = o.m_lineno; m_label = o.m_label;
        m_destroy_label = o.m_destroy_label;
        o.m_destroy_label = false; // evita doble destrucción de "m_label"
    }
    template <class TestCase> friend void do_toXML(
        const TestCase * tc , std::basic_ostringstream<char> & ost );
    template <class TestCase> friend void do_toString(
        const TestCase * tc , std::basic_ostringstream<char> & ost );
    #ifdef English_dox
         /// Test case.
    #endif
    #ifdef Spanish_dox
         /// Caso de prueba.
    #endif
    friend class TestCase;
}; // TestCaseFailure


// Abstract base class for every test case ==>
// - It's always mandatory to reimplement \c TestCase::run().
class TestCase {
protected:
    int          m_pass;    // number successful tests
    int          m_failure; // number of non successful tests [broken assert_XXX()]
    const char * m_name;    // name for the test case
    bool         m_test_suite_destroy; // true means destroy from dynamic memory
    std::list<TestCaseFailure> m_failureList; // container for al non successful tests
public:
    TestCase(const char * name=0);
    virtual ~TestCase() { }
    virtual bool run() = 0; // must implement in derived class
    void runBare();
    bool Run()     { return run(); } // same as run()
    bool runTest() { return run(); }
    virtual void setUp();
    virtual void tearDown();
    int  countTestCases() const { return 1; } // more than 1 for TestSuite<>
    // errorCount() == 0 ==> "error" means "wrong exception"
    int  runCount() const { return successCount()+failureCount()+errorCount(); }
    virtual int failureCount() const { return m_failure; }
    int  errorCount() const { return 0; }
    virtual int successCount() const { return m_pass; }
    // true when all runs have been a success
    bool wasSuccessful() const { return successCount() == runCount(); }
    virtual void reset(); // resets al counter back to 0
public:
    std::string getName() const;
    void setName( const char * name=0 );
    virtual const std::string toString() const;
    virtual const std::string summary() const;
    virtual const std::string toXML() const;
    const std::string report() const { return summary() + '\n' + toString(); }
    const std::string failureString() const { return toString(); }
    template <class T> static std::string toString( const T & val );
protected:
    void recordSuccess() { ++m_pass; } // record another successful test
    void recordFailure( // (char*) version
        const char* label, const char* fname, int lineno,
        bool must_copy=false );
    void recordFailure( // std::string version
        const std::string& label, const char* fname, int lineno );
    void testThis( // (char*) version ==> don't destroy label
        bool cond, const char*        label, const char* fname, long lineno,
        bool must_copy=false );
    void testThis( // string<> version ==> create copy of "label" in dynamic memory
        bool cond, const std::string& label, const char* fname, long lineno );
    template <class TestCase> friend
    void do_toXML( const TestCase * tc , std::basic_ostringstream<char> & ost );
    template <class TestCase> friend
    void do_toString( const TestCase * tc , std::basic_ostringstream<char> & ost );
    template <class TestCase> friend class TestSuite; ///< Colección de pruebas.
    int  nPass()  const { return m_pass; }    // Deprecated: it's here for
    int  nError() const { return m_failure; } // compatibility with Allison's work
private:
    virtual bool iAmTestSuite() const { return false; } // I am NOT a TestSuite<>
    TestCase(const TestCase&);            // trick to prevent copy on init
    TestCase& operator=(const TestCase&); // trick to prevent copying
    #ifdef English_dox
         /// Test case class for \c BUnit.h
    #endif
    #ifdef Spanish_dox
          /// Clase de prueba para \c BUnit.h
    #endif
    friend class test_BUnit;
}; // TestCase

#ifdef Spanish_dox
/// Sinónimo de \c TestCase.
#endif
#ifdef English_dox
/// Synonym for \c TestCase.
#endif
typedef TestCase TestCase_is_base_for_TestSuite;

// Test collection.
template <class TestCase>
class TestSuite : public TestCase_is_base_for_TestSuite {
public:
    typedef std::list<TestCase_is_base_for_TestSuite*> container_type;
    typedef container_type::iterator iterator;
    typedef const container_type::iterator cons_iterator;
    typedef const container_type const_container_type;
private:
    container_type m_allTest; // holds [pointers to] failed tests
public:
    TestSuite(const char * name=0): TestCase(name), m_allTest() { }
    virtual ~TestSuite();
    bool addTest(TestCase& T);
    bool addTest(TestCase* T);
    void addSuite( TestSuite& SSS );
    bool run(); // implemented: TestSuite<> is not an abstract class
    void runBare();
    int  countTestCases() const { return int(m_allTest.size()); }
    int  failureCount() const;
    int  successCount() const;
    void reset();
    const_container_type& allTests() const { return m_allTest; }
    const std::string toString() const;
    const std::string summary() const;
    const std::string toXML() const;
private:
    bool iAmTestSuite() const { return true; }
    TestSuite(const TestSuite& DONTcopy);
    TestSuite& operator=(const TestSuite& DONTcopy); // forbid copying
}; // TestSuite

/************************\
**************************
**                      **
**       BUnit.h        **
**                      **
**************************
\************************/

#ifdef English_dox
    /// Doxygen English documentation.
    #define English_dox   "Doxygen English documentation"
   /// \def English_dox  ///< Marks English documentation blocks.
#endif
#ifdef Spanish_dox
    /// Documentación en español.
    #define Spanish_dox "Documentación en español"
   /// \def Spanish_dox  ///< Macro usado para que Doxygen genere documentación
#endif

#ifdef English_dox
/** \mainpage

\section sec-01 [B]asic module for [unit] program testing.

Header file \c BUnit.h supports writing modules for unit program testing.
The model used for this implementation is similar to that described in
the article "The Simplest Automated Unit Test Framework That Could
Possibly Work "by Chuck Allison, which you can get here:
- http://www.stickyminds.com/getfile.asp?ot=XML&id=3129&fn=XDD3129filelistfilename1.pdf
- http://www.google.com/search?num=100&as_q=Simplest+Unit+Test+Allison
- http://search.yahoo.com/search?n=100&p=Simplest+Unit+Test+Allison

- To simplify to a minimum the task of writing test cases, the full
  implementation of \c BUnit is contained within the single header
  file \c BUnit.h, so testing requieres only this directive:
  - <strong>\#include "BUnit.h"</strong>
- This framework is similar to JUnit, because it is presumed that
  in the future students will use Java as their primary language
  to develop programs (as C++ is used only for deep training in
  programming techniques).
  \see http://search.yahoo.com/search?n=100&p=JUnit+Java
- As C++ does not have a mechanism similar to Java's "reflection",
  for each failed test the exact test condition is recorded
  using an invocation to macro \c BUnit_TEST() that also records
  the filename \c __FILE__ and the line \c __LINE__ for that test.
  \see http://search.yahoo.com/search?n=100&p=reflection+Java
- In JUnit class \c TestCase is a subclass of \c Assert; in BUnit no
  \c Assert class exist but to maintain compatibility with JUnit similar
  functionality is provided with macros whose names begin with "assert".
  \see http://search.yahoo.com/search?n=100&p=JUnit+Java+Assert+method
- To simplify this framework no difference is made between a test that
  is not successfull and one that didn´t succeed because the correct
  exception was not raised. This is a contrast with JUnit where an
  "error" is a test case that raised the wrong exception. This
  explains why \c TestCase::errorCount() always returns zero \c 0.
    \code
    // Failure vs Error in JUnit
    public void test_Fail () {
        try {
            new ArrayList(10).get( 11 );
            fail("Error if IndexOutOfBoundsException is not thrown" ); // error
        }
        catch (IndexOutOfBoundsException success) { } // Ok
    }
    public void test_Failure() {
        assertTrue( 1 == 2 ); // failure
    }
    \endcode
- In \c BUnit.h no difference is made between "failures" and "errors".
  - En \c JUnit an "error" means that the wrong exception was raised.
  - En \c JUnit a "failure" means that an assertion was false.
  - \see http://osdir.com/ml/java.junit.user/2002-06/msg00114.html
  - \see http://search.yahoo.com/search?n=100&p=junit+difference+failure+error
- In JUnit, what's the difference between a failure and an error?
  - Assertions are used to check for the possibility of failures,
    therefore failures are anticipated with invokations similar to \c assertTrue().
  - Errors are unanticipated problems resulting in uncaught exceptions
    being propagated from a JUnit test method.
  - \see http://www.cs.waikato.ac.nz/~bernhard/314/junit3.8.1/doc/faq/faq.htm#tests_9
  - \see http://www.cs.wm.edu/~noonan/junit/doc/faq/faq.htm#tests_9

\dontinclude test_BUnit.cpp
\skipline    test::Allison()
\until       }}
\see         test_BUnit::test_Allison()
*/
#endif
#ifdef Spanish_dox
/** \mainpage

\section sec-01 Módulo [B]ásico para prueba [unit]aria de programas.

El archivo de encabezado \c BUnit.h apoya la escritura de módulos de prueba de
unitaria programas. El modelo que se usa para esta implementación es
similar al expuesto en este artículo "The Simplest Automated Unit
Test Framework That Could Possibly Work" de Chuck Allison, que se
puede obtener aquí:
- http://www.stickyminds.com/getfile.asp?ot=XML&id=3129&fn=XDD3129filelistfilename1.pdf
- http://www.google.com/search?num=100&as_q=Simplest+Unit+Test+Allison
- http://search.yahoo.com/search?n=100&p=Simplest+Unit+Test+Allison

- Para simplificar al mínimo la tarea de programar los casos de prueba,
  la implementación completa de \c BUnit está contenida en el archivo
  de encabezado \c BUnit.h, por lo que para hacer pruebas basta poner
  esta directiva:
  - <strong>\#include "BUnit.h"</strong>
- Este marco de pruebas es similar a JUnit, pues se presume que en
  el futuro los estudiantes usarán Java como su lenguaje principal
  para desarrollo de programas (pues usan C++ sólo para adiestrarse
  profundamente en técnicas de programación).
  \see http://search.yahoo.com/search?n=100&p=JUnit+Java
- Como el lenguaje C++ no tiene un mecanismo similar al de "reflexión"
  de Java, en cada prueba fallida se registra la condición de prueba
  exacta, obtenida a través de una invocación de la macro \c BUnit_TEST()
  que registra el nombre del archivo \c __FILE__ y el renglón \c __LINE__
  de la prueba.
  \see http://search.yahoo.com/search?n=100&p=reflection+Java
- En JUnit la clase \c TestCase es una subclase de \c Assert; en BUnit no
  existe la clase \c Assert pero para mantener compatibilidad con JUnit
  sí se provée funcionalidad similar usando macros cuyos nombres comienzan
  con "assert".
  \see http://search.yahoo.com/search?n=100&p=JUnit+Java+Assert+method
- Para simplificar este marco de pruebas no se usa la clase \c TestResult
  que sirve para acumular resultados de las pruebas. En su lugar, se puede
  obtener una hilera enorme \c std::string que contiene el registro de
  todas las pruebas que no tuvieron éxito invocando el método
  \c TestCase::toString() o \c TestCase::toXML().
- Para simplificar esta plataforma de pruebas no se hace diferencia entre
  un caso de prueba de prueba no existoso y uno que no tiene éxito porque no
  se ha levantado la excepción adecuada. Esto contrasta con JUnit, que
  llama "error" a un caso de prueba que ha levantado la excepción equivocada.
  Por eso \c TestCase::errorCount() siempre retorna cero \c 0.
    \code
    // Falla vs Error en JUnit
    public void test_Error() {
        try {
            new ArrayList(10).get( 11 );
            fail("Error si no tira IndexOutOfBoundsException" ); // error
        }
        catch (IndexOutOfBoundsException success) { } // Ok
    }
    public void test_Failure() {
        assertTrue( 1 == 2 ); // falla
    }
    \endcode
- En \c BUnit.h no se hace diferencia entre "fallas" y "errores".
  - En \c JUnit un "error" se produce cuando no se levanta la excepción adecuada.
  - En \c JUnit una "falla" se produce cuando una aserción resulta falsa.
  - \see http://osdir.com/ml/java.junit.user/2002-06/msg00114.html
  - \see http://www.cs.waikato.ac.nz/~bernhard/314/junit3.8.1/doc/faq/faq.htm#tests_9
  - \see http://search.yahoo.com/search?n=100&p=junit+difference+failure+error
- In JUnit, ¿cuál es la diferencia entre fallas y errores?
  - Las aseveraciones son usadas para verificar si existen fallas, y por eso las
    fallas son previstas con invocaciones similares a \c assertTrue().
  - Los errores son problemas no previstos que resultan de excepciones no captadas
    por los método de prueba JUnit.
  - \see http://www.cs.waikato.ac.nz/~bernhard/314/junit3.8.1/doc/faq/faq.htm#tests_9
  - \see http://www.cs.wm.edu/~noonan/junit/doc/faq/faq.htm#tests_9

\dontinclude test_BUnit.cpp
\skipline    test::Allison()
\until       }}
\see         test_BUnit::test_Allison()
*/
#endif

#ifdef Spanish_dox
    // using namespace std;
    /// Definido por la biblioteca C++ estándar.
    namespace std {} // It's here for Doxygen to document it
#endif
#ifdef English_dox
    // using namespace std;
    /// Defined by the C++ estándar library.
    namespace std {} // Está acá para que Doxygen lo documente
#endif

/// Escuela de Ciencias de la Computación e Informática.
/// \see http://www.ecci.ucr.ac.cr
namespace ECCI { }

/************************\
**************************
**                      **
**   TestCaseFailure    **
**                      **
**************************
\************************/

#ifdef English_dox
/** \class TestCaseFailure
    \brief Private class to hold test cases that didn´t succeed.
*/
/** \var   const char * TestCaseFailure::m_fname;
    \brief Name of the file where the failure was produced.
*/
/** \var   int TestCaseFailure::m_lineno;
    \brief Line number in the file where the failure was produced.
*/
/** \var   const char * TestCaseFailure::m_label;
    \brief Descriptive message for the failure.
*/
/** \var   bool TestCaseFailure::m_destroy_label;
    \brief Holds \c "true" if the destructor should return the dynamic memory for \c m_label.
*/
/** \fn    TestCaseFailure::TestCaseFailure();
    \brief Default constructor.
*/
/** \fn    TestCaseFailure::TestCaseFailure(const char * fname, int line, const char * label, bool destroy );
    \brief Private constructor that can set \c m_label to \c true.
*/
#endif
#ifdef Spanish_dox
/** \class TestCaseFailure
    \brief Clase privada que contiene los datos de cada prueba no exitosa.
*/
/** \var   const char * TestCaseFailure::m_fname;
    \brief Nombre del archivo en donde se produjo el error.
*/
/** \var   int TestCaseFailure::m_lineno;
    \brief Número de línea del archivo en donde se produjo el error.
*/
/** \var   const char * TestCaseFailure::m_label;
    \brief Mensaje descriptivo del error.
*/
/** \var   bool TestCaseFailure::m_destroy_label;
    \brief Contiene \c "true" si el destructor debe retornar la memoria dinámica de \c m_label.
*/
/** \fn    TestCaseFailure::TestCaseFailure();
    \brief Constructor por defecto.
*/
/** \fn    TestCaseFailure::TestCaseFailure(const char * fname, int line, const char * label, bool destroy );
    \brief Constructor Privado que puede inicializar \c m_label en \c true.
*/
#endif

#ifdef English_dox
/** \fn    TestCaseFailure::~TestCaseFailure();
    \brief Destructor.
*/
/** \fn    TestCaseFailure::TestCaseFailure( const TestCaseFailure& o );
    \brief Copy constructor.
*/
/** \fn    void TestCaseFailure::operator = ( TestCaseFailure& o );
    \brief Copy operator used to insert into contanier.
*/
#endif

#ifdef Spanish_dox
/** \fn    TestCaseFailure::~TestCaseFailure()
    \brief Destructor.
*/
/** \fn    TestCaseFailure::TestCaseFailure( const TestCaseFailure& o );
    \brief Constructor de copia.
*/
/** \fn    void TestCaseFailure::operator = ( TestCaseFailure& o )
    \brief Copiador usado al insertar en el contenedor.
*/
#endif

/************************\
**************************
**                      **
**      TestCase        **
**                      **
**************************
\************************/

#ifdef English_dox
/** \class TestCase
    \brief Every test case is an instance derived from this abstract class.
     - It's always mandatory to reimplement \c TestCase::run().
*/
/** \fn    int TestCase::m_pass;
    \brief Number of successful tests.
*/
/** \fn    int TestCase::m_failure;
    \brief Number of test that produced failed.
*/
/** \fn    const char * TestCase::m_name;
    \brief Test case name.
*/
/** \fn    bool TestCase::m_test_suite_destroy;
    \brief Holds \c "true" if the test case is stored in dynamic memory.
*/
/** \fn    std::list<TestCaseFailure> TestCase::m_failureList;
    \brief Container where test cases that produced failures are stored.
*/
#endif

#ifdef Spanish_dox
/** \class TestCase
    \brief Cada caso de prueba es una instancia derivada de esta clase abstracta.
     - Siempre es obligatorio reimplementar \c TestCase::run().
*/
/** \fn    int TestCase::m_pass;
    \brief Cantidad de pruebas exitosas.
*/
/** \fn    int TestCase::m_failure;
    \brief Cantidad de pruebas que han fallado.
*/
/** \fn    const char * TestCase::m_name;
    \brief Nombre del caso de prueba.
*/
/** \fn    bool TestCase::m_test_suite_destroy;
    \brief Contiene \c "true" si la prueba está almacenada en memoria dinámica.
*/
/** \fn    std::list<TestCaseFailure> TestCase::m_failureList;
    \brief Contenedor para almacenar las pruebas que han producido fallas.
*/
#endif

#ifdef English_dox
/** \fn    virtual TestCase::~TestCase();
    \brief Destructor.
*/
/** \fn    virtual bool TestCase::run() = 0;
    \brief <strong>[virtual]</strong> ==> Executes test and returns \c "false" if not successful.
    <strong>[***]</strong> It is always mandatory to redefine method \c run().
    \see runBare().
*/
/** \fn    bool TestCase::Run();
    \brief Synonym for \c run().
*/
/** \fn    bool TestCase::runTest();
    \brief Synonym for \c run().
*/
/** \fn    int  TestCase::countTestCases() const;
    \brief 1 == Number of test cases.
    The value returned always is one \c 1 because class \c TestCase
    represents a single test case. For the container \c TestSuite<>
    the value returned can be bigger than \c 1.
    - A "test case" is a class, derived from class \c TestCase.
    - A "test run" gets counted when either a specific test succeeds or fails.
    - A "test run" gets counted when protected method TestCase::testThis() is
      executed, either directly or through any BUnit \c "assert()" macro, as
      \c assertTrue(), \c fail_Msg(), \c assertEquals_Delta(), or others like
      \c BUnit_SUCCESS() or \c BUnit_TEST().
    - A "test case" ussually has many "test runs". A collection of "test cases"
      resided within an instance derived from class \c TestSuite<>.
*/
/** \fn    int  TestCase::runCount() const;
    \brief Number of test runs.
    - Synonym for <code>successCount()+failureCount()+errorCount()</code>.
    \see reset().
*/
/** \fn    virtual int TestCase::failureCount() const;
    \brief Number of test runs that failed. \see reset().
*/
/** \fn    int  TestCase::errorCount() const;
    \brief Always returns \c 0 (cero): "Number of errors".
    - BUnit does not maintain separate counts for "errors" and "failures".
    \see http://osdir.com/ml/java.junit.user/2002-06/msg00114.html
*/
/** \fn    virtual int TestCase::successCount() const;
    \brief Number of successful test runs. \see reset().
*/
/** \fn    bool TestCase::wasSuccessful() const;
    \brief Returns \c "true" when all test runs where successful.
    - Synonym for <code>(successCount() == runCount())</code>
*/
/** \fn    std::string TestCase::getName() const;
    \brief Gets the test case name. \see setName().
*/
/** \fn    const std::string TestCase::report() const;
    \brief Returns string \c summary() followed by \c toString().
*/
/** \fn    const std::string TestCase::failureString() const;
    \brief Synonym for \c toString().
*/
/** \fn    void TestCase::recordSuccess();
    \brief Records the test run as a success.
*/
/** \fn    int  TestCase::nPass()  const;
    \brief Synonym for \c successCount() [OBSOLETE]. \deprecated
*/
/** \fn    int  TestCase::nError() const;
    \brief Synonym for \c failureCount() [OBSOLETE]. \deprecated
*/
/** \fn    virtual bool TestCase::iAmTestSuite() const;
    \brief Returns \c false for \c TestCase.
*/
/** \fn    TestCase::TestCase(const TestCase&);
    \brief This \c "private" declaration forbids test case copying.
*/
/** \fn    TestCase& TestCase::operator=(const TestCase&);
    \brief This \c "private" declaration forbids test case copying.
*/
#endif
#ifdef Spanish_dox
/** \fn    virtual TestCase::~TestCase();
    \brief Destructor.
*/
/** \fn    virtual bool TestCase::run() = 0;
    \brief <strong>[virtual]</strong> ==> Ejecuta la prueba y retorna \c "false" si produce error.
    <strong>[***]</strong> Siempre es necesario redefinir el método \c run().
    \see runBare().
*/
/** \fn    bool TestCase::Run();
    \brief Sinónimo de \c run().
*/
/** \fn    bool TestCase::runTest();
    \brief Sinónimo de \c run().
*/
/** \fn    int  TestCase::countTestCases() const;
    \brief 1 == Cantidad de casos de prueba.
    El valor retornado siempre es uno \c 1 porque la clase \c TestCase
    representa un único caso de pruebas. Para el contenedor
    \c TestSuite<> el valor retornado puede ser mayor a \c 1.
    - Un "caso de prueba" es una clase derivada de la clase \c TestCase.
    - Una "prueba" se cuenta cuando tiene éxito o fracasa.
    - Una "prueba" se cuenta cuando el método protegido TestCase::testThis() es
      ejecutado, ya sea directamente o a través de cualquiera de los macros
      \c "assert()" de BUnit, como lo son \c assertTrue(), \c fail_Msg(),
      \c assertEquals_Delta(), u otros como \c BUnit_SUCCESS() or \c BUnit_TEST().
    - Un "caso de prueba" generalmente incluye muchas "pruebas". Una colección de
      "casos de prueba" reside en una instancia derivada de la clase \c TestSuite<>.
*/
/** \fn    int  TestCase::runCount() const;
    \brief Cantidad total de pruebas realizadas.
    - Sinónimo de <code>successCount()+failureCount()+errorCount()</code>.
    \see reset().
*/
/** \fn    virtual int TestCase::failureCount() const;
    \brief Cantidad de pruebas que fallaron. \see reset().
*/
/** \fn    int  TestCase::errorCount() const;
    \brief Siempre retorna \c 0 (cero): "Cantidad de errores".
    - BUnit no se contabilizan aparte los "errores" de las "fallas".
    \see http://osdir.com/ml/java.junit.user/2002-06/msg00114.html
*/
/** \fn    virtual int TestCase::successCount() const;
    \brief Cantidad de pruebas exitosas. \see reset().
*/
/** \fn    bool TestCase::wasSuccessful() const;
    \brief Retorna \c "true" si todas las pruebas han sido exitosas.
    - Sinónimo de <code>(successCount() == runCount())</code>
*/
/** \fn    std::string TestCase::getName() const;
    \brief Obtiene el nombre de la prueba. \see setName().
*/
/** \fn    const std::string TestCase::report() const;
    \brief Retorna la hilera encabezado \c summary() seguido \c toString().
*/
/** \fn    const std::string TestCase::failureString() const;
    \brief Sinónimo de \c toString().
*/
/** \fn    void TestCase::recordSuccess();
    \brief Registra como exitoso el resultado de una prueba.
*/
/** \fn    int  TestCase::nPass()  const;
    \brief Sinónimo de \c successCount() [OBSOLETO]. \deprecated
*/
/** \fn    int  TestCase::nError() const;
    \brief Sinónimo de \c failureCount() [OBSOLETO]. \deprecated
*/
/** \fn    virtual bool TestCase::iAmTestSuite() const;
    \brief Retorna \c false para \c TestCase.
*/
/** \fn    TestCase::TestCase(const TestCase&);
    \brief Esta declaración \c "private" prohibe la copia de casos de prueba.
*/
/** \fn    TestCase& TestCase::operator=(const TestCase&);
    \brief Esta declaración \c "private" prohibe la copia de casos de prueba.
*/
#endif

#ifdef Spanish_dox
/** Constructor.
    Si no se indica el nombre en \c "name", después el nombre se obtiene
    invocando <code>typeid(*this).name()</code>.
*/
#endif
#ifdef English_dox
/** Constructor.
    If no name is given to this constructor in \c "name", later the
    name will be obtained invoking <code>typeid(*this).name()</code>.
*/
#endif
/** \dontinclude test_BUnit.cpp
    \skipline    test::constructor()
    \until       }}
    \see         test_BUnit::test_constructor()
*/
inline TestCase::TestCase(const char * name) :
    m_pass(0), m_failure(0), m_name(name),
    m_test_suite_destroy(false), m_failureList() { }

#ifdef Spanish_dox
/** Elimina todas las pruebas realizadas.
    - Anula los contadores de pruebas porque deja la clase en el estado
      inicial que tuvo al ser construida.
    - Borra el registro de pruebas no exitosas.
    - No borra el nombre de la prueba.
*/
#endif
#ifdef English_dox
/** Discards all test runs.
    - Resets to cero al counters because the test case is left in
      the state it had when initially contructed.
    - Deletes the record for not successful tests.
    - Does not change the test case name.
*/
#endif
/** \dontinclude test_BUnit.cpp
    \skipline    test::reset()
    \until       }}
    \see         test_BUnit::test_reset()
*/
inline void TestCase::reset() {
    m_pass =  m_failure = 0;
    m_failureList.clear();
}

inline std::string TestCase::getName() const {
    return ( m_name != 0 ? m_name : typeid(*this).name() );
}

#ifdef Spanish_dox
/** Le cambia el nombre a la prueba por \c "name".
    - Si \c "name" es una hilera o puntero nulo, después
      usa <code>typeid(*this).name()</code> para obtener
      el nombre de la prueba.
*/
#endif
#ifdef English_dox
/** Sets the test case name to \c "name".
    - When \c "name" is a null string or pointer, later
      <code>typeid(*this).name()</code> will be invoked
      to get the test´s name.
*/
#endif
/** \dontinclude test_BUnit.cpp
    \skipline    test::setName()
    \until       }}
    \see         test_BUnit::test_setName()
*/
inline void TestCase::setName( const char * name ) {
    m_name = name;
}

#ifdef Spanish_dox
/** Ejecuta la prueba <code>setUp(); run(); tearDown();</code>.
    - A diferencia de \c run(), este método sí establece el ambiente
      de prueba invocando \c setUp() y \c tearDown() antes y después
      de hacer la prueba.
*/
#endif
#ifdef English_dox
/** Excecutes the test run <code>setUp(); run(); tearDown();</code>.
    - In contrast to \c run(), this method will setup the environment
      for the test run invoking \c setUp() and \c tearDown() before and
      after the test.
*/
#endif
/** \dontinclude test_BUnit.cpp
    \skipline    test::run()
    \until       }}
    \see         test_BUnit::test_run()
*/
inline void TestCase::runBare( ) { setUp(); run(); tearDown(); }

#ifdef Spanish_dox
/** Establece el ambiente para la ejecución de la prueba.
    Esta clase existe para mejorar la compatibilidad con JUnit.
    \see TestCase::setUp()
*/
#endif
#ifdef English_dox
/** Sets the environment for the test run.
    This clase exists to improve compatibility with JUnit.
    \see TestCase::setUp()
*/
#endif
typedef TestCase TestFixture;

#ifdef Spanish_dox
/** Establece el ambiente en que se realizará la prueba.
    - Como \c TestCase::run() es un método abstracto, para facilitar la
      programación lo usual es que el programador no incluya invocaciones
      a \c TestCase::setUp() y \c TestCase::tearDown() pues es más fácil
      dejar que lo haga \c TestSuite<TestCase>::runBare().
    - A diferencia de \c TestCase::runBare(), el método \c TestCase::run()
      no establece el ambiente de prueba porque no invoca ni a
      \c TestCase::setUp() antes de la prueba ni a \c TestCase::tearDown()
      después de la prueba.
    - \c TestSuite<TestCase>::runBare() invoca los métodos
      \c TestCase::setUp() y \c TestCase::tearDown() cuando ejecuta cada
      prueba.
    - Si el programador cliente quiere que cada prueba se ejecute luego de
      establecer el ambiente de la prueba, lo más práctico es que agregue
      sus pruebas a una colección de pruebas \c TestSuite para ejecutarlas
      con \c TestSuite<TestCase>::runBare().
*/
#endif
#ifdef English_dox
/** Sets the environment for the test run.
    - As \c TestCase::run() is an abstract method, to facilitate
      programming it is usual for the programmer not to invoke
      \c TestCase::setUp() and \c TestCase::tearDown() because it is
      easier to have \c TestSuite<TestCase>::runBare() do it.
    - In contrast with \c TestCase::runBare(), method  \c TestCase::run()
      will not establish the test environment because it does not
      invoke neither \c TestCase::setUp() before the test run nor
      \c TestCase::tearDown() after the test run.
    - \c TestSuite<TestCase>::runBare() invokes methods
      \c TestCase::setUp() and \c TestCase::tearDown() when the test run
      is executed.
    - If the client programmer wants each test run  to stablish it test
      environment, it is easier to put all test cases into a test collection
      \c TestSuite and execute all of them invoking
      \c TestSuite<TestCase>::runBare().
*/
#endif
inline void TestCase::setUp() { }

#ifdef Spanish_dox
/// Destruye el ambiente de prueba.
#endif
#ifdef English_dox
/// Destroys the test environment.
#endif
inline void TestCase::tearDown() {}

#ifdef Spanish_dox
/** Efectúa la prueba y registra su resultado.
    - Si la prueba fue exitosa sólo incrementa la cantidad de éxitos
      \c successCount().
    - Si la prueba no tuvo éxito reporta en \c toString() ese hecho.
    - El resultado de la prueba es \c "cond".
    - Los valores \c "fname" y \c "lineno" indican el archivo y el renglón
      en donde se ejecuta la prueba.
    - Usualmente los valores de \c "fname" y \c "lineno" se obtienen con
      las macros globales \c "__FILE__" y \c "__LINE__".
    - El valor \c "must_copy" indica que es necesario hacer una copia de la
      hilera \c "label", copia que será destruida cuando el registro de
      pruebas no exitosas sea borrado.
    - En la mayor parte de los casos, la hilera \c "label" es una constante
      generada por el preprocesador al usar la macro \c \#cond y por eso su
      memoria no debe ser retornada.
    Este método es invocado usando la macro \c BUnit_TEST().
*/
#endif
#ifdef English_dox
/** Executes the test and records its result.
    - If the test is successful only increases the numbero of success
      \c successCount().
    - If the test is not successful reports in \c toString() this fact.
    - The result of the test is \c "cond".
    - Values \c "fname" and \c "lineno" show the file and line of the
      executed test.
    - Usuallyy \c "fname" and \c "lineno" are values obtained with global
      macros \c "__FILE__" and \c "__LINE__".
    - The value for \c "must_copy" indicates whether it is necessary to
      make a copy of string \c "label"; this copy will be destroyed when
      the record of not successful test is deleted.
    - Most of the time string \c "label" is a constant genertated by the
      preprocessor when using macro \c \#cond its memory must not be
      returned.
    This method is invoked using macro \c BUnit_TEST().
*/
#endif
/** \dontinclude test_BUnit.cpp
    \skipline    test::testThis()
    \until       }}
    \see         test_BUnit::test_testThis()
*/
inline void TestCase::testThis(
    bool cond, const char * label, const char* fname, long lineno, bool must_copy )
{
    if (cond) {
        recordSuccess();
    }
    else {
        recordFailure( label, fname, lineno, must_copy );
    }
}

#ifdef Spanish_dox
/// Sinónimo de \c testThis().
#endif
#ifdef English_dox
/// Synonym for \c testThis().
#endif
inline void TestCase::testThis(
    bool cond, const std::string& label, const char* fname, long lineno )
{
    testThis( cond, label.c_str(), fname, lineno, true /* must_copy == true */ );
/*  NOTE: In the class instance the name for the test case is stored as
    a (char*). If a std::string is used for "label", it must be converted to
    a (char*) that will have to be destroyed when the test instance gets
    destroyed. This is why parameter "must_copy" must be set to "true".
*/
}

#ifdef Spanish_dox
/** Registra que la prueba no tuvo éxito.
    - Los valores \c "fname" y \c "lineno" indican el archivo y el renglón
      en donde se ejecuta la prueba.
    - Usualmente los valores de \c "fname" y \c "lineno" se obtienen invocando
      las macros globales \c "__FILE__" y \c "__LINE__".
    - El valor \c "must_copy" indica que es necesario hacer una copia de la
      hilera \c "label" en memoria dinámica. Este memoria dinámica será destruida
      cuando el caso de prueba sea destruido o cuando el método \c TestCase::reset()
      sea invocado.
    - En la mayor parte de los casos, la hilera \c "label" es una constante
      generada por el preprocesador al usar la macro \c \#cond; como esta hilera
      constante no está almacenada en la memoria dinámica no debe ser destruida.

    Este método es invocado usando la macro \c BUnit_FAILURE().
*/
#endif
#ifdef English_dox
/**  Records that the test run did not succeed.
    - Values \c "fname" and \c "lineno" indicate the file and line where
      the test run is executed.
    - The values form \c "fname" and \c "lineno" are usuallly obtained
      invoking global macros \c "__FILE__" and \c "__LINE__".
    - \c "must_copy" forces a copy of string label to be created in dynamic
      memory. This dynamic memory will be destroyed when the test case gets
      destroyed or when method \c TestCase::reset() gets invoked.
    - Oftentimes string \c "label" is a constant string generated by the
      preprocessor macro using \c \#cond; this string constant is not stored
      in dynamic memory and it must not be destroyed.

    This method is invoked using macro \c BUnit_FAILURE().
*/
#endif
inline void TestCase::recordFailure(
    const char* label, const char* fname, int lineno, bool must_copy )
{
    if ( must_copy ) {
        size_t len = strlen(label)+1;
        char* str = new char[ len ]; // copies the failure message
        memcpy( str, label, len );
        label = str;
    }
    m_failureList.push_back( TestCaseFailure ( fname, lineno, label, must_copy ) );
    m_failure++;
/*  NOTE: When this method is invoked directly it is because "label" is a
    constant (char*) string, produced by one of the BUnit_TEST() macro using
    operator "#" (as in #cond). As this is a constant string, it it not
    necessary to return its memory when the instance of "TestCase" gets
    destroyed and, as result of this, field "m_destroy_label" in the
    corresponding "TestCaseFailure" instance must be false.

    In contrast, when "label" is generated from std::string, the method
    recordFailure() to be invoked is not this. Intead the method invoked will be
    the other version of recordFailure() where parameter "label" is a std::string.
    That other version always uses "true" for "must_copy", because a (char*) copy
    of the original std::string is stored in the "TestCaseFailure" instance, and
    that value should be destroyed when that instance gets destroyed. This fact
    gets recorded in field "TestCaseFailure::m_destroy_label."

    This means that no extra work is necessary to handle the two different cases
    (char*) vs std::string, as the compiler chooses correctly which version of
    recordFailure() to use.
*/
/*  NOTA: Cuando este método es invocado directamente es porque "label" es una
    hilera constante (char*), producida por alguna de las macro BUnit_TEST()
    mediante el operador "#" (como en #cond). Como esa hilera es constante, no
    hace falta retornar su memoria cuando la instancia de "TestCase" sea destruida
    y, en consecuencia, el campo "m_destroy_label" de la instancia de
    "TestCaseFailure" correspondiente debe ser "false".

    En contraposición, si la hilera "label" es generada a partir de std::string, el
    método recordFailure() que será invocado no es éste. Más bien será la otra
    versión de recordFailure() la invocada, pues en esa "label" es de tipo
    std::string. Esa otra versión siempre usa "true" para "must_copy", pues una
    copia (char*) del valor original std::string queda almacenada en la instancia
    "TestCaseFailure" y ese valor debe ser destruido cuando esa instancia sea
    destruida. Este hecho quda registrado en el campo "TestCaseFailure::m_destroy_label."

    Esto quiere decir que no hace falta hacer trabajo adicional para manejar los
    dos casos diferentes "char*" vs "std::string", pues el compilador escoge
    correctamente siempre cuál versión de recordFailure() que hay que usar.
*/
}

#ifdef Spanish_dox
/// Registra que la prueba no tuvo éxito.
/// - En \c BUnit.h no se hace diferencia entre "fallas" y "errores".
#endif
#ifdef English_dox
/// Records that the test did not succeed.
/// - En \c BUnit.h there is no difference between "failures" and "errors".
#endif
inline void TestCase::recordFailure(
    const std::string& label, const char* fname, int lineno )
{
    recordFailure( label.c_str(), fname, lineno, true /* must_copy == true */ );
/*  NOTE: as in here the (char*) version of recordFailure() is invoked for it to
    make a copy of "label.c_str()", argument "must_copy" must be set to "true".
    In this way field "TestCaseFailure::m_destroy_label" will be se to "true",
    and the (char*) memory for the copy of string  "label.c_str()" will be
    returned when the record for the "TestCaseFailure" is destroyed.
*/
/*  NOTA: como aquí se invoca a la versión (char*) de recordFailure() para que
    haga una copia de "label.c_str()", el argumento "must_copy" debe ser "true".
    De esta manera el campo "TestCaseFailure::m_destroy_label" tomará el valor
    "true" y, en consecuencia, la memoria (char*) en donde quedó la copia de la
    hilera "label.c_str()" será retornada cuando el registro de la prueba
    "TestCaseFailure" sea destruido.
*/
}

#ifdef Spanish_dox
/// Hilera "enooorme" que contiene copia del registro de pruebas no exitosas, separados por \c "\n".
#endif
/// Huuuge string that holds a copy of non successfull test, separated by \c "\n".
#ifdef English_dox
#endif
/** \code
    =\_fail: 1 == 0
    =/ (125) X:/DIR/SubDir/test_BUnit.cpp
    =\_fail: 4 == 0
    =/ (128) X:/DIR/SubDir/test_BUnit.cpp
    \endcode
*/
inline const std::string TestCase::toString() const {
//  typedef basic_ostringstream<char> ostringstream;
    std::basic_ostringstream<char> ost; // ostringstream ost;
    do_toString( this , ost );
    return ost.str();
}

#ifdef Spanish_dox
/// Le agrega a \c ost la hilera de todas las pruebas no exitosas de \c *tc.
#endif
#ifdef English_dox
/// Adds to \c ost the string of al unsuccessful test from  \c *tc.
#endif
template <class TestCase>
void do_toString( const TestCase * tc , std::basic_ostringstream<char> & ost ) {
    std::list<TestCaseFailure>::const_iterator it = tc->m_failureList.begin();
    for ( ; it != tc->m_failureList.end(); ++it) {
        ost << "=\\_fail: " << it->m_label << " \n=/ ("
            << it->m_lineno << ") " << it->m_fname << "\n";
    }
    return;
/*  NOTA: La implementación de "TestCase::toString()" está hecha invocando esta
    función emplantillada para que sea el compilador el responsable de remover
    versiones duplicadas de esta misma rutina, las que se producen cuando este
    archivo de encabezado ("BUnit.h") es incluido en varios archivos "*.cpp".
    Todo esto hay que hacerlo para que "BUnit.h" quepa, completo, en un único
    archivo de encabezado.
*/
/*  NOTE: the implementation for "TestCase:toString()" is made invoking this
    template function for the compiler to be responsible of removing duplicate
    versions of this same routine, which would be produced when this header file
    ("BUnit.h") gets included in several "*.cpp" files. All this must be done
    for "BUnit.h" to fit, completely,  in a single header file.
*/
}

#ifdef Spanish_dox
/// Hilera XML que contiene una copia de las pruebas no exitosas.
#endif
#ifdef English_dox
/// XML string that holds a copy of all unsuccessful tests.
#endif
/** \code
    <fail file="X:/DIR/SubDir/test_BUnit.cpp" line="125" message="1 == 0"/>
    <fail file="X:/DIR/SubDir/test_BUnit.cpp" line="128" message="4 == 0"/>
    \endcode
*/
inline const std::string TestCase::toXML() const {
//  typedef basic_ostringstream<char> ostringstream;
    std::basic_ostringstream<char> ost; // ostringstream ost;
    do_toXML( this , ost );
    return ost.str();
}

#ifdef Spanish_dox
/// Le agrega a \c ost la hilera de todas las pruebas no exitosas de \c *tc en formato XML.
#endif
#ifdef English_dox
/// Adds to \c ost the string of al unsuccessful test from  \c *tc in XML format
#endif
template <class TestCase>
void do_toXML( const TestCase * tc , std::basic_ostringstream<char> & ost ) {
    std::list<TestCaseFailure>::const_iterator it = tc->m_failureList.begin();
    for ( ; it != tc->m_failureList.end(); ++it ) {
        ost << "<fail file=\"" << it->m_fname
            << "\" line=\"" << it->m_lineno
            << "\" message=\"" << it->m_label  << "\"/>" << std::endl;
    }
    return;
/*  NOTE: the implementation for "TestCase:toXML()" is made invoking this
    template function for the compiler to be responsible of removing duplicate
    versions of this same routine, which would be produced when this header file
    ("BUnit.h") gets included in several "*.cpp" files.
*/
/*  NOTA: La implementación de "TestCase::toXML()" está hecha invocando a la
    función emplantillada para que sea el compilador el responsable de remover
    versiones duplicadas de esta misma rutina, las que se producen cuando este
    archivo de encabezado ("BUnit.h") es incluido en varios archivos "*.cpp".
*/
}

/// [BUnit] ==> BUnit.h
#define TEST_BUnit()
#undef  TEST_BUnit

#ifdef Spanish_dox
/** [BUnit] Efectúa la prueba \c "cond y registra el resultado.
    - Es más elegante usar \c assertTrue() que hace lo mismo.
    - Si la prueba \c "cond" tiene éxito invoca el método
      \c TestCase::recordSuccess().
    - Si la prueba \c "cond" no tiene éxito invoca el método
      \c TestCase::recordFailure().
*/
#endif
#ifdef English_dox
/** [BUnit] Executes test \c "cond and records its result.
    - It is more elegant to use \c assertTrue() for the same purpose.
    - If test \c "cond" is successful method \c TestCase::recordSuccess()
      is invoked.
    - If test \c "cond" is not successful, method\c TestCase::recordFailure()
      is invoked.
*/
#endif
/** \dontinclude test_BUnit.cpp
    \skipline    test::BUnit_macro()
    \until       }}
    \see         test_BUnit::test_BUnit_macro()
*/
#define BUnit_TEST(cond) testThis( cond, #cond, __FILE__, __LINE__ )

#ifdef Spanish_dox
/// [BUnit] Macro similar a \c BUnit_TEST() que usa el mensaje \c "msg".
/// - El mensaje \c "msg" debe ser una hilera (char*) literal o un objeto
///   que pueda convertirse en <code>(const char *)</code>.
#endif
#ifdef English_dox
/// [BUnit] Macro similar to \c BUnit_TEST() that uses mesaje \c "msg".
/// - Mensaje \c "msg" must be a literal (char*) string or and object that
///   can be converted into a <code>(const char *)</code>.
#endif
#define BUnit_TEST_Msg(msg, cond) testThis( cond, msg, __FILE__, __LINE__ )

#ifdef Spanish_dox
/** [BUnit] Registra como "falla" el resultado de una prueba.
    - El programador cliente es quien determinó que la prueba no tuvo éxito
      y quiere registrar ese hecho.
    - Está implementado comun una macro que sirve de empaque para invocar
      el método \c TestCase::recordFailure().
*/
#endif
#ifdef English_dox
/** [BUnit] Records as "failure" the result of a test.
    - The client programmer determined that the test did not succeed
      and wants to record this fact.
    - It is implemented as a macro wrapper to invoke method
      \c TestCase::recordFailure().
*/
#endif
/** \see BUnit_TEST()
    \code
    if (22==33) {
        BUnit_FAILURE("! (22==33)"); // records failure
    }
    \endcode
*/
#define BUnit_FAILURE(msg) recordFailure( msg, __FILE__, __LINE__ )

#ifdef Spanish_dox
/** [BUnit] Registra como "exitoso" el resultado de una prueba.
    - El programador cliente es quien determinó que la prueba
      fue exitosa y quiere registrar ese hecho.
    - Generalmente se usa después de atrapar una excepción que
      se supone debió ser lanzada como resultado de la prueba.
    - Está implementado como una macro que invoca el método
      \c TestCase::recordSuccess().
    \see BUnit_TEST()
*/
#endif
#ifdef English_dox
/** [BUnit] Records as "successful" the result of a test.
    - The client programmer determined that the test did succeed
      and wants to record this fact.
    - Usually used after catching an exception that should have
      been raised as result of the test.
    - It is implemented as a macro wrapper to invoke method
      \c TestCase::recordSuccess().
    \see BUnit_TEST()
*/
#endif
#define BUnit_SUCCESS() recordSuccess()

#ifdef Spanish_dox
/// [BUnit] Efectúa la prueba para determinar si <code>(expected == actual)</code>.
/// - Si la prueba tiene éxito invoca el método \c TestCase::recordSuccess().
/// - De otra manera invoa \c TestCase::recordFailure().
/// - Esta es una una macro que invoca el método \c TestCase::testThis().
///  \see BUnit_TEST()
#endif
#ifdef English_dox
/// [BUnit] Executes the test to determine if <code>(expected == actual)</code>.
/// - On success invokes method \c TestCase::recordSuccess().
/// - Otherwise invokes \c TestCase::recordFailure().
/// - This is a macro to invoke method \c TestCase::testThis().
///  \see BUnit_TEST()
#endif
#define BUnit_EQUAL(expected, actual) \
        testThis( (expected) == (actual), #expected " == " #actual, __FILE__, __LINE__ )

#ifdef Spanish_dox
/// [BUnit] Efectúa la prueba para determinar si <code>expected == actual</code>.
/// - Si la prueba tiene éxito invoca el método
///   \c TestCase::recordSuccess().
/// - Si la prueba no tiene éxito invoca el método
///   \c TestCase::recordFailure().
/// - Esta es una una macro que invoca el método
///   \c TestCase::testThis().
/// - Registra el mensaje \c "MSG" si la prueba no tiene éxito.
/// \see BUnit_TEST()
#endif
#ifdef English_dox
/// [BUnit] Executes the test to determine if <code>(expected == actual)</code>.
/// - On success invokes method \c TestCase::recordSuccess().
/// - Otherwise invokes \c TestCase::recordFailure().
/// - This is a macro to invoke method \c TestCase::testThis().
/// - Records messaje \c "MSG" if the test is not successful.
///  \see BUnit_TEST()
#endif
#define BUnit_EQUAL_Msg(MSG, expected, actual) \
        testThis( (expected) == (actual), MSG, __FILE__, __LINE__ )

#ifdef Spanish_dox
/// Retorna un hilera que contiene el nombre, cantidad de éxitos y fallas.
#endif
#ifdef English_dox
/// Returns a string with the name, number of successes and failures.
#endif
inline const std::string TestCase::summary() const {
    std::string res = "TestCase [" + getName() + ']';
    res += " (OK: "   + TestCase::toString( successCount() ) + ')';
    res += " (FAIL: " + TestCase::toString( failureCount() ) + ')';
    return res;
}

/************************\
**************************
**                      **
**      TestSuite       **
**                      **
**************************
\************************/
// template <class TestCase> class TestSuite
#ifdef Spanish_dox
/** \class TestSuite
    \brief Colección de pruebas.
    - Este un contenedor simple con una interfaz similar a la de la clase
      \c TestCase.
    - Cada una de las pruebas de la colección es ejecutada mediante la
      invocación a su método \c TestCase::run().
    - El método \c TestSuite<TestCase>::runBare() es similar a
      \c TestSuite<TestCase>::run(), pero antes de hacer cada prueba establece
      el ambientede ejecución de la prueba invocando a \c TestCase::setUp()
      y a \c TestCase::tearDown() después de realizarla.
    - Debido al uso de los métodos \c setUp() y \c tearDown() para ejecutar
      las pruebas de la colección, cada prueba queda "aisalda" de las demás,
      pues para cada una se estable el ambiente de ejecución que promete la
      clase \c TestCase.
    - Está implementado como una plantilla para evitar usar un archivo
      \c "BUnit.cpp", de manera que para probar programas baste incluir
      únicamente este archivo \c "BUnit.h".

    \par Truco para evitar usar el archivo \c "BUnit.cpp"

    - Al implementar la plantilla \c TestSuite<> usé como parámetro el identificador
      \c TestCase, que también es el nombre de la clase de la que \c TestSuite<> se
      deriva. Este doble uso del mismo identificador confunde al compilador, porque
      no puede deducir que el parámetro \c TestCase también es el nombre de la clase
      base. Por eso, uso un sinónimo para nombrar a la clase base de \c TestSuite<>.
      Escogí el identificador \c TestCase_is_base_for_TestSuite  porque es
      descriptivo y bastante largo, por lo que es muy poco probable que un
      programador cliente de \c BUnit.h necesite usarlo.
    - La clase \c TestSuite<> está implementada usando plantillas para evitar
      forzar al programador cliente de \c "BUnit" a usar 2 archivos, uno de
      encabezado \c "BUnit.h" y otro de implementación \c "BUnit.cpp". Como no
      hace falta usar el archivo \c "BUnit.cpp", implementar programas de
      prueba es muy sencillo pues al programar las pruebas solo se necesita
      agregar este único renglón:
      - <code>\#include "BUnit.h"</code>
    - Si toda la implementación de \c "BUnit" está contenida en el archivo de
      encabezado de \c "BUnit.h", puede ocurrir que algunas rutinas queden
      duplicadas en el programa objeto cuando el encabezado \c "BUnit.h" es
      incluido en varios archivos \c ".cpp" diferentes en el mismo proyecto.
      En estos casos, los métodos que no son <em>"inline"</em> quedan
      definidos en varios archivos y, en consecuencia, el editor de
      eslabonamiento (<em>"linker"</em>) debe reportar que el mismo módulo
      objeto está presente en varios archivos objeto. Cuando se usan
      plantillas, el sistema de compilacición se encarga de manejar esta
      duplicación, lo que releva al programador de esta engorrosa
      responsabilidad. Esto explica por qué \c TestSuite<> es una clase
      emplantillada.
    - La otra forma de lograr evitar la duplicación de módulos objetos es
      obligar a los programadores cliente a incluir en su proyecto el archivo
      \c "BUnit.cpp" en donde estarían las definiciones de las funciones que
      no son <em>"inline"</em>.

    - En otras palabras, el requerimiento de lograr que todas la
      implementación de \c BUnit.h esté metida en un sólo archivo obliga a
      implementar \c TestSuite<> con plantillas, lo que a también motiva el
      uso de \c TestCase como nombre del parámetro para la plantilla
      \c TestSuite<>.
    - Dicho de otra manera: la razón por la que \c TestSuite<> está
      implementada con plantillas es lograr que la responsabilidad de eliminar
      duplicados de métodos y funciones caiga en el sistema de compilación,
      pues de lo contrario sería necesario que los programadores cliente incluyan
      en su proyecto el archivo de implementación \c "BUnit.cpp", lo que haría
      más complicado el uso de \c BUnit. Este truco de programación hace que
      sea trabajo del compilador manejar el código de implementación que de
      otra manera habria habido que poner dentro de un archivo de
      implementación \c "BUnit.cpp".
*/
#endif
#ifdef English_dox
/** \class TestSuite;
    \brief Test collection.
    - This is a container with an interface similar to class \c TestCase.
    - Each test case in the collection is executed invoking its \c TestCase::run()
      method.
    - Method \c TestSuite<TestCase>::runBare() is similar to
      \c TestSuite<TestCase>::run(), but before each test the test environment
      for the test is established invoking \c TestCase::setUp() and
      \c TestCase::tearDown() afterwards.
    - Because of the use of methods \c setUp() and \c tearDown() to exectute test
      in the collection, each tests becomes "isolated" from the rest, because
      each stablishes its execution environment promised by class \c TestCase.
    - This implementation uses templates to avoid using a file \c "BUnit.cpp",
      to allow program testis using a single include of file \c "BUnit.h".

    \par Trick to avoid using file \c "BUnit.cpp"

    - When implementing the \c TestSuite<> template I used as parameter identifier
      \c TestCase, which is also the name for the class from where \c TestSuite<>
      is derived. This dual use of the same identifier confuses the compiler,
      because it can not figure out that parameter \c TestCase is also the name
      for the base class. Therefore, I use a synonym to name the base class for
      \c TestSuite<>. I chose identifier \c TestCase_is_base_for_TestSuite
      because it is descriptive and quite long, so it is very unlikely that client
      programmer of \c BUnit.h would ever need to use it.
    - Class \c TestSuite<> is implemented using templates to avoid forcing the
      client programmer  of \c "BUnit" to use 2 files, a header file
      \c "BUnit.h" and another to hold the implementation \c "BUnit.cpp." As
      there is no need to use the file \c "BUnit.cpp",  implementing test
      programs is easy because programming test cases only requires adding
      this single line:
      - <code>\#include "BUnit.h"</code>
    - If the entire implementation of \c "BUnit" is contained in  header file
      \c "BUnit.h", it may happen that some routines get duplicated in the
      object code when header file \c "BUnit.h" is included from several
      different \c ".cpp" files in the same project. In these cases, methods
      that are not <em>"inline"</em> are defined in several files and,
      consequently, the linkage editor (<em>"linker"</em>) should report that
      the same  object module is present in several object files. When
      templates are used, the compiling system is responsible for managing
      this duplicating what relieves the programmer of this cumbersome
      responsibility. This explains why \c TestSuite<> is a template class.
    - Another way to avoid duplication of object modules it to force the
      client programmers to include in their project file \c "BUnit.cpp"
      containing all definition for non <em>"inline"</em> functions.

    - In other words, the requirement to make all the implementatation of
      \c BUnit.h to fit into a single file forces implementing
      \c TestSuite<> with templates, which also motivates the use
      \c TestCase as the name name of the parameter template
      \c TestSuite<>.
    - Put in another way: the reason why \c TestSuite<> is implemented with
      templates is to ensure that the responsibility for removing duplicate
      functions and methods belongs to the compiling system, as otherwise the
      client programmers would be required to include in their proyect
      implementation file \c "BUnit.cpp", which would make more complicated
      using \c BUnit. This programming trick puts the compiler in charge of
      handling the implementation coide that otherwise would have been put in
      an implementation file\c "BUnit.cpp."
*/
#endif

#ifdef Spanish_dox
/** \fn    template <class TestCase> typedef TestSuite<TestCase>::container_type;
    \brief Tipo del contenedor (escoja el que prefiera).
*/
/** \fn    template <class TestCase> typedef TestSuite<TestCase>::iterator;
    \brief Iterador del contenedor.
*/
/** \fn    template <class TestCase> typedef TestSuite<TestCase>::cons_iterator;
    \brief Iterador del contenedor (\c const).
*/
/** \fn    template <class TestCase> typedef TestSuite<TestCase>::const_container_type;
    \brief Tipo del contenedor (\c const).
*/
/** \fn    template <class TestCase> container_type TestSuite<TestCase>::m_allTest;
    \brief Contenedor en donde se almacenan los punteros a las pruebas.
*/
#endif
#ifdef English_dox
/** \fn    template <class TestCase> typedef TestSuite<TestCase>::container_type;
    \brief Container type (choice your preferred).
*/
/** \fn    template <class TestCase> typedef TestSuite<TestCase>::iterator;
    \brief Container iterador.
*/
/** \fn    template <class TestCase> typedef TestSuite<TestCase>::cons_iterator;
    \brief Container iterador (\c const).
*/
/** \fn    template <class TestCase> typedef TestSuite<TestCase>::const_container_type;
    \brief Container type (\c const).
*/
/** \fn    template <class TestCase> container_type TestSuite<TestCase>::m_allTest;
    \brief Container to store pointers to tests.
*/
#endif

/** \fn    template <class TestCase> TestSuite<TestCase>::TestSuite(const char * name=0);
    \brief Constructor
*/

#ifdef Spanish_dox
/** \fn    template <class TestCase> TestSuite<TestCase>::TestSuite(const char * name=0);
    \brief Constructor.
*/
/** \fn    template <class TestCase> int  TestSuite<TestCase>::countTestCases() const;
    \brief Cantidad de casos de prueba.
*/
/** \fn    template <class TestCase> int TestSuite<TestCase>::failureCount() const;
    \brief Cantidad de pruebas que fallaron. \see reset().
*/
/** \fn    template <class TestCase> int TestSuite<TestCase>::successCount() const;
    \brief Cantidad de pruebas exitosas. \see reset().
*/
/** \fn    template <class TestCase> void TestSuite<TestCase>::reset();
    \brief Elimina todas las pruebas realizadas.
*/
/** \fn    template <class TestCase> TestSuite<TestCase>::const_container_type& TestSuite<TestCase>::allTests() const;
    \brief Referencia al contenedor con todos los casos de prueba.
*/
/** \fn    template <class TestCase> const std::string TestSuite<TestCase>::toString() const;
    \brief Hilera "enooorme" que contiene copia del registro de pruebas no exitosas, separados por \c "\n".
*/
/** \fn    template <class TestCase> const std::string TestSuite<TestCase>::toXML() const;
    \brief Hilera XML que contiene una copia de las pruebas no exitosas.
*/
/** \fn    template <class TestCase> bool TestSuite<TestCase>::iAmTestSuite() const { return true; }
    \brief Retorna \c true solo para la clase derivada \c TestSuite<>.
*/
/** \fn    template <class TestCase> TestSuite<TestCase>::TestSuite(const TestSuite& DONTcopy);
    \brief Esta declaración \c "private" prohibe la copia de casos de prueba.
*/
/** \fn    template <class TestCase> TestSuite<TestCase>::TestSuite& TestSuite<TestCase>::operator=(const TestSuite& DONTcopy);
    \brief Esta declaración \c "private" prohibe la copia de casos de prueba.
*/
#endif
#ifdef English_dox
/** \fn    template <class TestCase> TestSuite<TestCase>::TestSuite(const char * name=0);
    \brief Constructor.
*/
/** \fn    template <class TestCase> int  TestSuite<TestCase>::countTestCases() const;
    \brief Number of test cases.
*/
/** \fn    template <class TestCase> int TestSuite<TestCase>::failureCount() const;
    \brief Number of test runs that failed. \see reset().
*/
/** \fn    template <class TestCase> int TestSuite<TestCase>::successCount() const;
    \brief Number of successful test runs. \see reset().
*/
/** \fn    template <class TestCase> void TestSuite<TestCase>::reset();
    \brief Discards all test runs.
*/
/** \fn    template <class TestCase> TestSuite<TestCase>::const_container_type& TestSuite<TestCase>::allTests() const;
    \brief Reference to the container where al the test runs are stored.
*/
/** \fn    template <class TestCase> const std::string TestSuite<TestCase>::toString() const;
    \brief Huuuge string that holds a copy of non successfull test, separated by \c "\n".
*/
/** \fn    template <class TestCase> const std::string TestSuite<TestCase>::toXML() const;
    \brief XML string that holds a copy of all unsuccessful tests.
*/
/** \fn    template <class TestCase> bool TestSuite<TestCase>::iAmTestSuite() const { return true; }
    \brief Returns \c true only for derived class \c TestSuite<>.
*/
/** \fn    template <class TestCase> TestSuite<TestCase>::TestSuite(const TestSuite& DONTcopy);
    \brief This \c "private" declaration forbids test case copying.
*/
/** \fn    template <class TestCase> TestSuite<TestCase>::TestSuite& TestSuite<TestCase>::operator=(const TestSuite& DONTcopy);
    \brief This \c "private" declaration forbids test case copying.
*/
#endif

#if 0
    #ifdef Spanish_dox
    /// Acumula los resultados de pruebas que no tienen éxito.
    #endif
    #ifdef English_dox
    /// Holds results for non successful tests.
    #endif
    class TestResult { }; // not declared
/*  NOTE
    As the class TestSuite<> is derived from TestCase, it also
    contains a list of tests that have not been successful. That list
    is always empty, because the failure logging is carried out
    individually, in each instance of TestCase stored in the
    TestSuite<> instance. Should BUnit had a TestResult class it would
    not be necessary to keep this empty lista.
    - To simplify BUunit it contains no TestResult class.
*/
/*  NOTA
    Como la clase TestSuite<> está derivada de TestCase, también
    contiene una lista de pruebas que no han tenido éxito. Esa lista
    siempre está vacía, pues el registro de fallas se lleva
    individualmente, en cada una de las instancias de TestCase
    almacenadas en la instancia de TestSuite. Si BUnit tuviera una
    clase TestResult no sería necesario mantener esta lista vacía.
    - Para simplificar BUnit no contiene la clase TestResult.
*/
#endif

#ifdef Spanish_dox
/** Agrega la prueba \c "*pT" a la colección \c "*this" (y luego la destruirá).
    - Si <code>(pT == 0)</code> no agrega nada y retorna \c "false".
    - Es incorrecto que la misma prueba esté más de una vez en la colección.
    - Destruirá \c "*pT" cuando \c "*this" sea destruido.
    - El parámetro \c "pT" es un puntero.
    - Si \c "pT" es una colección de pruebas el efecto es similar a invocar
      \c addSuite(*pT) directamente.
*/
#endif
#ifdef English_dox
/** Adds test \c "*pT" to collection \c "*this" (and later it will be destroyed).
    - If <code>(pT == 0)</code> nothing is added and returns \c "false".
    - It is incorrect for the same test to be in more than one collection.
    - Will destroy \c "*pT" when \c "*this" is destroyed.
    - The \c "pT" parameter is a pointer.
    - If \c "pT" is a test colection the effect is similar to invoking
      \c addSuite(*pT) directly.
*/
#endif
/** \dontinclude test_BUnit.cpp
    \skipline    test::addTest()
    \until       }}
    \see         test_BUnit::test_addTest()
*/
template <class TestCase>
bool TestSuite<TestCase>::addTest(TestCase* pT) {
    if (pT == 0) {
        return false;
    }
    if ( pT->iAmTestSuite() ) {  // ==> addSuite()
        addSuite( *( reinterpret_cast< TestSuite<TestCase>* > ( pT ) ) );
        return true;
    }
    else if ( m_allTest.end() != std::find(m_allTest.begin(), m_allTest.end(), pT) ) {
        return false; // never trust the client programmer
    }
    else {
        m_allTest.push_back( pT );
        pT->m_test_suite_destroy = true;
        return true;
    }
}

#ifdef Spanish_dox
/** Agrega la prueba \c "T" a la colección  \c "*this" (no la destruirá).
    - No destruirá \c "T" cuando \c "*this" sea destruido.
    - El parámetro \c "T" es una referencia.
    - Si \c "T" es una colección de pruebas el efecto es similar a invocar
      \c addSuite(T) directamente.
*/
#endif
#ifdef English_dox
/** Adds test \c "T" to collection \c "*this" (and later it will not be destroyed).
    - Will not destroy \c "T" when \c "*this" is destroyed.
    - The \c "T" parameter is a reference.
    - If \c "pT" is a test colection the effect is similar to invoking
      \c addSuite(T) directly.
*/
#endif
/** \dontinclude test_BUnit.cpp
    \skipline    test::addTest()
    \until       }}
    \see         test_BUnit::test_addTest()
*/
template <class TestCase>
bool TestSuite<TestCase>::addTest(TestCase& T) {
    if ( T.iAmTestSuite() ) { // ==> addSuite()
        addSuite( *( reinterpret_cast< TestSuite<TestCase>* > ( & T ) ) );
        return true;
    }
    else if ( addTest(& T) ) {
        T.m_test_suite_destroy = false;
        return true;
    }
    else {
        return false;
    }
}

#ifdef Spanish_dox
/** Traslada todas las pruebas de la colección \c "SSS" y las agrega a \c "*this".
    - Deja a \c "SSS" vacía.
    - No copia las pruebas de \c SSS para evitar los problemas que surgen cuando
      tanto \c "SSS" como \c "*this" tratan de destruir la misma prueba.
    - No es posible hacer una jerarquía de pruebas pues todas quedan aplanadas
      en un sola colección.
*/
#endif
#ifdef English_dox
/** Moves all test from collection \c "SSS" and ads them to \c "*this".
    - Leaves \c "SSS" empty.
    - Will not copy tests form \c SSS to avoid problems that come up when
      both \c "SSS" and \c "*this" try to destroy the same test.
    - It is not possible to make a test hierarchy because all become
      flattened in a single collection.
*/
#endif

template <class TestCase>
inline void TestSuite<TestCase>::addSuite( TestSuite& SSS ) {
    if (&SSS == 0) { // SSS = (TestSuite<TestCase>*)(0);
        return;
    }
    if ( this == &SSS ) {
        return; // avoid self-copy
    }
    while ( ! SSS.m_allTest.empty() ) {
        iterator it = SSS.m_allTest.begin();
        if ( m_allTest.end() == std::find(m_allTest.begin(), m_allTest.end(), *it) ) {
            m_allTest.splice( m_allTest.end(), SSS.m_allTest, it );
        }
        else {
            SSS.m_allTest.pop_front(); // discards duplicate
        }
    }
    SSS.reset(); // reset counters
}

#ifdef Spanish_dox
/** Destructor.
    - Solo destruye aquellas pruebas que hayan sido agregadas con
      \c TestSuite<TestCase>::addTest(TestCase* T) (puntero).
    - No destruye las pruebas que fueron agregadas con
      \c TestSuite<TestCase>::addTest(TestCase& T) (referencia).
*/
#endif
#ifdef English_dox
/** Destructor.
    - Will destroy only those test that where addes with
      \c TestSuite<TestCase>::addTest(TestCase* T) (pointer).
    - Will not destroy those test that where addes with
      \c TestSuite<TestCase>::addTest(TestCase& T) (reference).
*/
#endif
template <class TestCase>
TestSuite<TestCase>::~TestSuite() {
    container_type::const_iterator it = m_allTest.begin();
    while ( it != m_allTest.end() ) {
        if ( (*it)->m_test_suite_destroy ) {
            delete (*it);
        }
        ++it;
    }
    m_allTest.clear();
}

#ifdef Spanish_dox
/** Invoca \c TestCase::run() para cada prueba en la colección.
    - No invoca los métodos \c TestCase::setUp() y \c TestCase::tearDown()
      cuando ejecuta cada prueba.
    - No invoca \c TestSuite<TestCase>::reset() antes de cada prueba.
    - Retorna \c "true" cuando todas las pruebas han tenido éxito (inclusive
      aquellas que fueron corridas antes de la invocación actual).
*/
#endif
#ifdef English_dox
/** Invokes \c TestCase::run() for each test in the collection.
    - Will not invoke methods \c TestCase::setUp() and \c TestCase::tearDown()
      when executing each test.
    - Will not invoke \c TestSuite<TestCase>::reset() before each test.
    - Returns \c "true" when al test have been successful (even those
      that were run before this invocation).
*/
#endif
template <class TestCase>
bool TestSuite<TestCase>::run() {
    bool res = true;
    container_type::const_iterator it = m_allTest.begin();
    for ( ; it != m_allTest.end(); ++it ) {
        (*it)->run(); // hace la prueba
    }
    return wasSuccessful();
}

#ifdef Spanish_dox
/** Invoca \c TestCase::runBare() para cada prueba en la colección.
    - Si invoca \c TestSuite<TestCase>::reset() antes de hacer cada prueba.
    - Si invoca \c TestCase::setUp() antes de cada prueba
    - Si invoca \c TestCase::tearDown() después de cada prueba.
*/
#endif
#ifdef English_dox
/** Invokes \c TestCase::runBare() for each test in the collection.
    - Will invoke \c TestSuite<TestCase>::reset() before each test.
    - Will invoke \c TestCase::setUp() before each test.
    - Will invoke \c TestCase::tearDown() after each test.
*/
#endif
/** \dontinclude test_BUnit.cpp
    \skipline    test::run()
    \until       }}
    \see         test_BUnit::test_run()
*/
template <class TestCase>
void TestSuite<TestCase>::runBare() {
    bool res = true;
    container_type::const_iterator it = m_allTest.begin();
    for ( ; it != m_allTest.end(); ++it ) {
        (*it)->reset();    // reset counters
        (*it)->setUp();    // setup test environment
        (*it)->run();      // run test
        (*it)->tearDown(); // reset test environment
    }
}

template <class TestCase>
int TestSuite<TestCase>::failureCount() const {
    int n;
    const_container_type::const_iterator it = m_allTest.begin();
    for ( n = 0 ; it != m_allTest.end(); ++it ) {
        n += (*it)->failureCount();
    }
    return n;
/*  NOTE: there is no choice but to count every test, as the client
    programmer could have run a test outside the collection, which
    would make counter TestSuite<TestCase>:m_failure to be outdated,
    because no mechanism is implemented for a "TestCase" to "tell"
    its "TestSuite<>" that it just executed "run()":
*/
#if 0
    class MyTest : public TestCase { ... };
    TestSuite<TestCase> SSS;
    MyTest thisTest, *ptr_test = new MyTest;
    SSS.addTest( thisTest );
    SSS.addTest( ptr_test );
    SSS.run();      // exec [thisTest] + [*ptr_test]
    thisTest.run(); // exec [thisTest] ==> SSS ???
#endif
/*  NOTA: no queda más que contar todas las pruebas actuales, pues
    el programador cliente puede ejecutar alguna de las pruebas
    fuera de la colección, lo que haría que el contador
    TestSuite<TestCase>::m_failure quedara desactualizado, pues
    no está implementado un mecanismo para que el "TestCase" le
    "avise" a su "TestSuite<>" que acaba de ejecutar "run()".
*/
}

template <class TestCase>
int TestSuite<TestCase>::successCount() const {
    int n;
    const_container_type::const_iterator it = m_allTest.begin();
    for ( n = 0 ; it != m_allTest.end(); ++it ) {
        n += (*it)->successCount();
    }
    return n;
}


template <class TestCase>
void TestSuite<TestCase>::reset() {
    const_container_type::const_iterator it = m_allTest.begin();
    for ( ; it != m_allTest.end(); ++it ) {
        (*it)->reset();
    }
}

#ifdef Spanish_dox
/// Retorna un hilera que contiene el nombre, cantidad de éxitos y fallas.
#endif
#ifdef English_dox
/// Returns a string with the name, number of successes and failures.
#endif
template <class TestCase>
inline const std::string TestSuite<TestCase>::summary() const {
    typedef TestCase_is_base_for_TestSuite TC;
    std::string res = "TestSuite [" + getName() + ']';
    res += " (OK: "   + TC::toString( successCount() ) + ')';
    res += " (FAIL: " + TC::toString( failureCount() )   + ')';
    return res;
}

template <class TestCase>
const std::string TestSuite<TestCase>::toString() const {
    std::string failureString;
    TestSuite<TestCase>::container_type::const_iterator it;
    it = allTests().begin();
    for ( ; it != allTests().end(); ++it ) {
        failureString += (*it)->toString() ;
    }
    return failureString;
}

template <class TestCase>
const std::string TestSuite<TestCase>::toXML() const {
    std::string XMLstring;
    TestSuite<TestCase>::container_type::const_iterator it;
    it = allTests().begin();
    for ( ; it != allTests().end(); ++it ) {
        XMLstring += (*it)->toXML() ;
    }
    return XMLstring;
}

/************************\
**************************
**                      **
**       Macros         **
**                      **
**************************
\************************/


#ifdef Spanish_dox
#endif
#ifdef English_dox
#endif

#ifdef BUnit_DEFINE_check_ok
    /* These are macros used by Adolfo to define the invariant for a
       class. Many times the invariant checking function check_ok()
       does not exist which makes these functions irrelevant:
       - http://www.di-mare.com/adolfo/p/Rep.htm#sc-02
    */
    /* Estas son las macros usadas por Adolfo al definir la invariante
       de una clase. Muchas veces la función check_ok() usada para
       verificar invariante no existe, lo que hace irrelevantes estas
       definiciones:
       - http://www.di-mare.com/adolfo/p/Rep.htm#sc-02
    */
    #ifdef Spanish_dox
        /// Declaración genérica para \c check_ok().
    #endif
    #ifdef English_dox
        /// Generic declaration for \c check_ok().
    #endif
    template <class T> bool check_ok( const T& );
    inline bool check_ok( const double &        ) { return true; } ///< <code>check_ok() == true</code>
    inline bool check_ok( const float &         ) { return true; } ///< <code>check_ok() == true</code>
    inline bool check_ok( const long double &   ) { return true; } ///< <code>check_ok() == true</code>
    inline bool check_ok( const signed char &   ) { return true; } ///< <code>check_ok() == true</code>
    inline bool check_ok( const signed int &    ) { return true; } ///< <code>check_ok() == true</code>
    inline bool check_ok( const signed long &   ) { return true; } ///< <code>check_ok() == true</code>
    inline bool check_ok( const unsigned char & ) { return true; } ///< <code>check_ok() == true</code>
    inline bool check_ok( const unsigned int&   ) { return true; } ///< <code>check_ok() == true</code>
    inline bool check_ok( const unsigned long & ) { return true; } ///< <code>check_ok() == true</code>
#endif

#ifndef BUnit_DEFINE_check_ok // trick to make Doxygen document BUnit_DEFINE_check_ok
    #ifdef Spanish_dox
        /// Si esta macro existe las funciones \c check_ok() quedan definidas para los tipos básicos.
    #endif
    #ifdef English_dox
        /// If this macro exist functions for \c check_ok() will be defined for the basic types.
    #endif
    #define BUnit_DEFINE_check_ok
    #undef  BUnit_DEFINE_check_ok
#endif

#ifdef Spanish_dox
/// Retorna una hilera \c std::string contruida desde el valor de \c val.
/// - \c toString() with standard C++
#endif
#ifdef English_dox
/// Returns a \c std::string constructed form value \c val.
/// - \c toString() with standard C++
#endif
template <class T>
std::string TestCase::toString( const T & val ) {
//  typedef basic_ostringstream<char> ostringstream;
    std::basic_ostringstream<char> temp; // ostringstream temp;
    temp << val;
    return temp.str( );
}

/// [CppUnit] Macros propios de \c CppUnit http://cppunit.sourceforge.net/doc/lastest
#define CPPUNIT_BUnit()
#undef  CPPUNIT_BUnit

/// [CppUnit] Assertions that a condition is true.
/// \see http://cppunit.sourceforge.net/doc/lastest/group___assertions.html#ga0
#define CPPUNIT_ASSERT(condition) BUnit_TEST(condition)

/// [CppUnit] Assertion with a user specified message.
/// \see http://cppunit.sourceforge.net/doc/lastest/group___assertions.html#ga1
#define CPPUNIT_ASSERT_MESSAGE(message, condition) assertTrue_Msg(message,condition)

/// [CppUnit] Fails with the specified message.
/// \see http://cppunit.sourceforge.net/doc/lastest/group___assertions.html#ga2
#define CPPUNIT_FAIL(message) BUnit_FAILURE(message)

/// [CppUnit] Asserts that two values are equals.
/// \see http://cppunit.sourceforge.net/doc/lastest/group___assertions.html#ga3
#define CPPUNIT_ASSERT_EQUAL(expected, actual) BUnit_EQUAL(expected, actual)

/// [CppUnit] Asserts that two values are equals, provides additional messafe on failure.
/// \see http://cppunit.sourceforge.net/doc/lastest/group___assertions.html#ga4
#define CPPUNIT_ASSERT_EQUAL_MESSAGE(message, expected, actual) \
        BUnit_EQUAL_Msg(message, expected, actual)

/// [CppUnit] Macro for primitive value comparisons.
/// \see http://cppunit.sourceforge.net/doc/lastest/group___assertions.html#ga5
#define CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, actual, delta) \
        assertEquals_Delta(expected, actual, delta)

/// [CppUnit] Asserts that the given expression throws an exception of the specified type.
/// \see http://cppunit.sourceforge.net/doc/lastest/group___assertions.html#ga6
#define CPPUNIT_ASSERT_THROW(expression, ExceptionType) \
        do {                                            \
            bool cpputExceptionThrown_ = false;         \
            try {                                       \
                expression;                             \
            } catch ( const ExceptionType & ) {         \
                cpputExceptionThrown_ = true;           \
            }                                           \
                                                        \
            if ( cpputExceptionThrown_ ) {              \
                break;                                  \
            }                                           \
            BUnit_FAILURE(                              \
                "Expected exception: " #ExceptionType ) \
        } while ( false )

/// [CppUnit] Asserts that the given expression does not throw any exceptions.
/// \see http://cppunit.sourceforge.net/doc/lastest/group___assertions.html#ga7
#define CPPUNIT_ASSERT_NO_THROW(expression)                   \
        do {                                                  \
            try {                                             \
                expression;                                   \
            } catch ( ... ) {                                 \
                BUnit_FAILURE("Unexpected exception caught"); \
            }                                                 \
        } while ( false )

/// [CppUnit] Asserts that an assertion fail.
/// \see http://cppunit.sourceforge.net/doc/lastest/group___assertions.html#ga8
#define CPPUNIT_ASSERT_ASSERTION_FAIL(assertion)               \
        CPPUNIT_ASSERT_THROW( assertion, CPPUNIT_NS::Exception )

/// [CppUnit] Asserts that an assertion pass.
/// \see http://cppunit.sourceforge.net/doc/lastest/group___assertions.html#ga9
#define CPPUNIT_ASSERT_ASSERTION_PASS(assertion) \
        CPPUNIT_ASSERT_NO_THROW( assertion )

#define JUnit_BUnit()
#undef  JUnit_BUnit

/// [JUnit] Macros propios de \c JUnit http://junit.sourceforge.net/javadoc/junit/framework/Assert.html
///  Asserts that two objects are equal.
/// \see http://junit.sourceforge.net/javadoc/junit/framework/Assert.html#assertEquals(java.lang.Object,%20java.lang.Object)
#define assertEquals(          EXPECTED, ACTUAL ) BUnit_EQUAL(EXPECTED, ACTUAL)
///  Asserts that two objects are equal (with message).
/// \see http://junit.sourceforge.net/javadoc/junit/framework/Assert.html#assertEquals(java.lang.String,%20java.lang.Object,%20java.lang.Object)
#define assertEquals_Msg( MSG, EXPECTED, ACTUAL ) BUnit_EQUAL_Msg(MSG, EXPECTED, ACTUAL)

/// [JUnit] Asserts that a condition is true.
/// \see http://junit.sourceforge.net/javadoc/junit/framework/Assert.html#assertTrue(boolean)
#define assertTrue(           CONDITION ) \
        testThis( CONDITION, #CONDITION, __FILE__, __LINE__ )
/// [JUnit] Asserts that a condition is true (with message).
/// \see http://junit.sourceforge.net/javadoc/junit/framework/Assert.html#assertTrue(java.lang.String,%20boolean)
#define assertTrue_Msg( MSG,  CONDITION ) \
        testThis( CONDITION,  MSG,       __FILE__, __LINE__ )

/// [JUnit] Asserts that a condition is false.
/// \see http://junit.sourceforge.net/javadoc/junit/framework/Assert.html#assertFalse(boolean)
#define assertFalse(          CONDITION ) \
        testThis( !(CONDITION), "!(" #CONDITION ")", __FILE__, __LINE__ )
/// [JUnit] Asserts that a condition is false (with message).
/// \see http://junit.sourceforge.net/javadoc/junit/framework/Assert.html#assertFalse(java.lang.String,%20boolean)
#define assertFalse_Msg( MSG, CONDITION ) \
        testThis( !(CONDITION),       MSG,           __FILE__, __LINE__ )

#include <math.h> // fabs()
/// [JUnit] Asserts that two doubles are equal concerning a delta.
/// \see http://junit.sourceforge.net/javadoc/junit/framework/Assert.html#assertEquals(double,%20double,%20double)
#define assertEquals_Delta(EXPECTED, ACTUAL, DELTA ) \
        testThis( fabs( double(EXPECTED) -  double(ACTUAL) ) < double(DELTA), \
        "|"  #EXPECTED "-" #ACTUAL "| < " #DELTA,  __FILE__, __LINE__ )

/// [JUnit] Asserts that two doubles are equal concerning a delta (with message).
/// \see http://junit.sourceforge.net/javadoc/junit/framework/Assert.html#assertEquals(java.lang.String,%20double,%20double,%20double)
#define assertEquals_Delta_Msg( MSG, EXPECTED, ACTUAL, DELTA ) \
        testThis( fabs( double(EXPECTED) -  double(ACTUAL) ) < double(DELTA), \
        MSG, __FILE__, __LINE__ )

/// [JUnit] Asserts that an object is null.
/// \see http://junit.sourceforge.net/javadoc/junit/framework/Assert.html#assertNull(java.lang.Object)
#define assertNull(OBJECT)    \
        testThis( 0==&(OBJECT), "assertNull("    #OBJECT ")", __FILE__, __LINE__ )
/// [JUnit] Asserts that an object isn't null.
/// \see http://junit.sourceforge.net/javadoc/junit/framework/Assert.html#assertNotNull(java.lang.Object)
#define assertNotNull(OBJECT) \
        testThis( 0!=&(OBJECT), "assertNotNull(" #OBJECT ")", __FILE__, __LINE__ )

/// [JUnit] Asserts that two objects refer to the same object.
/// \see http://junit.sourceforge.net/javadoc/junit/framework/Assert.html#assertSame(java.lang.Object,%20java.lang.Object)
#define assertSame(THIS, THAT)      \
        testThis( &(THIS)==&(THAT), \
                   "assertSame("    #THIS ", " #THAT ")", __FILE__, __LINE__ )
///  Asserts that two objects do not refer to the same object.
/// \see http://junit.sourceforge.net/javadoc/junit/framework/Assert.html#assertNotSame(java.lang.Object,%20java.lang.Object)
#define assertNotSame(THIS, THAT)   \
        testThis( &(THIS)!=&(THAT), \
                   "assertNotSame(" #THIS ", " #THAT ")", __FILE__, __LINE__ )

#if 0
// This breaks code as fail() is a method for streams.
/// [JUnit] Fails a test with no message.
/// \see http://junit.sourceforge.net/javadoc/junit/framework/Assert.html#fail()
#define fail(     )     BUnit_FAILURE("FAIL")
#endif

/// [JUnit] Fails a test with the given message.
/// \see http://junit.sourceforge.net/javadoc/junit/framework/Assert.html#fail(java.lang.String)
#define fail_Msg( MSG ) BUnit_FAILURE(MSG)

#endif // BUnit_h

// EOF: BUnit.h
