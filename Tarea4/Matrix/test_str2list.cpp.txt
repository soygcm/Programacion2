// test_str2list.cpp   (C) 2009 adolfo@di-mare.com

/** \file  test_str2list.cpp
    \brief Programa de prueba para \c str2list() y \c str2matrix().

    \author Adolfo Di Mare <adolfo@di-mare.com>
    \date   2009
*/

#include "BUnit.h"
#include <iostream>  // BORREME !!!
#include "str2list.h"

#include <iostream>  // cout
#include <iomanip>   // setw()

/// Prueba \c str2list() y \c str2matrix().
class test_str2list : public TestCase {
public:
    bool run();
    void str2list_example();
    void test_iswhitespace();
    void str2list_list();
    void test_makeList_long();
    void test_intlist();
    void test_doublelist();
    void test_makeList_char();
    void test_str2matrix();
    void test_intmatrix();
    void test_longmatrix();
    void test_doublematrix();
    void test_charmatrix();
};

/// <code> main() { test->run(); } </code>
bool test_str2list::run() {
    str2list_example();
    test_iswhitespace();
    str2list_list();
    test_makeList_long();
    test_intlist();
    test_doublelist();
    test_makeList_char();
    test_str2matrix();
    test_intmatrix();
    test_longmatrix();
    test_doublematrix();
    test_charmatrix();
    return wasSuccessful();
};

/// Test -> \c iswhitespace().
void test_str2list::test_iswhitespace() {
{{  // test::iswhitespace()
    const char blank = ' ';
    assertTrue( iswhitespace_str2list(blank) );  assertTrue( iswhitespace_str2list(',') );

    assertTrue( ! iswhitespace_str2list('.') );  assertFalse( iswhitespace_str2list('.') );
    assertTrue( ! iswhitespace_str2list('\0') ); assertFalse( iswhitespace_str2list('\0') );

    assertTrue( iswhitespace_str2list('(') );    assertTrue( iswhitespace_str2list(')') );
    assertTrue( iswhitespace_str2list('[') );    assertTrue( iswhitespace_str2list(']') );
    assertTrue( iswhitespace_str2list('{') );    assertTrue( iswhitespace_str2list('}') );
    assertTrue( iswhitespace_str2list('<') );    assertTrue( iswhitespace_str2list('>') );

    assertTrue( iswhitespace_str2list('\r') );   assertTrue( iswhitespace_str2list('\n') );
    assertTrue( iswhitespace_str2list('\f') );
    assertTrue( iswhitespace_str2list('\t') );   assertTrue( iswhitespace_str2list('\v') );
}}
}

/// Test -> \c makeList_long().
void test_str2list::test_makeList_long() {
    {{  // test::makeList_long()
        std::list<long> Li, Lj;
        Li = makeList_long( "()" );      assertTrue( Li.empty() );
        Li = makeList_long( " (  ) " );  assertTrue( Li.empty() );

        Li = makeList_long( "(1 2 3 4 5)" );
        for ( int j=1; j<=5; ++j ) {
            Lj.push_back( j );
        }
        assertTrue( Li == Lj );

        assertTrue( makeList_long( "( ... )" ) == makeList_long( "( 0 )" ) );
        assertTrue( makeList_long( "( ... )" ).size() == 1 );

        assertTrue( makeList_long( "(0)" ) == makeList_long( "( - )" ) );
        assertTrue( makeList_long( "(0)" ) == makeList_long( "( + )" ) );
        assertTrue( makeList_long( "()" )  == makeList_long( "(  )" ) );

        assertTrue( makeList_long( "( 1 2 3 4 5 )" ) == makeList_long( "(1,2,3,4,5)" ) );
        assertTrue( makeList_long( "{ 1,2,3,4,5 }" ) == makeList_long( "[1 2 3 4 5]" ) );

        Li = makeList_long( "( 1,22,333,4444,5555 )" );
        Lj = makeList_long(  "(1,22,333,4444,5555)" );
        assertTrue( Li == Lj );

        Li = makeList_long( "(1 , - + 22,  -333, - 4444, --5555)" );
        Lj = makeList_long( "(1,  0,0,22,  -333, 0,4444,      0)" );
        assertTrue( Li == Lj );

        Li = makeList_long( "(  1,  22,  333,  4444,  5555 )" );
        Lj = makeList_long( "| [1] [22] [333] [4444] [5555] |... ignore");
        assertTrue( Li == Lj );

        Li = makeList_long( "(  1,  22,  333,  4444,  5555, 0,   0 )" );
        Lj = makeList_long( "( [1] [22] [333] [4444] [5555] )etc ...");
        assertTrue( Li == Lj );
    }}
{   // Resto de las pruebas
    void print_list( const char* str );
    if (false) { // Estas son las que fallan con la nueva versión de makeList_long()
        assertTrue( makeList_long( "()" )  == makeList_long( "( ... )" ) );
        assertTrue( makeList_long( "( 1 2 3 4 5 )" ) == makeList_long( "(1.2.3.4.5)" ) );

        std::list<long> Li, Lj;
        Li = makeList_long( "(1 , - + 22,  -333, - 4444, --5555)" );
        Lj = makeList_long( " (1,0,0,22,-333,0,4444,5555)" );
        assertTrue( Li == Lj );

        Li = makeList_long( "( ++101 --202 +-+-303 -+-+404 -+-+--505 )" );
        assertTrue( Li == Lj );

        assertTrue( makeList_long( "() " )  == makeList_long( " ( ... ) " ) );
        assertTrue( makeList_long( " ()" )  == makeList_long( " ( ... ) " ) );
    }
    if (true) { // Estas sirven para ver en qué falla la nueva versión de makeList_long()
        assertTrue( makeList_long( "( ... )" ) == makeList_long( "( [...] )" ) );
        assertTrue( makeList_long( "( ... )" ) == makeList_long( "( 0 )" ) );
        assertTrue( makeList_long( "( ... )" ).size() == 1 );

        std::list<long> Li, Lj;
        if (false) {
            print_list( "( ... )" );
            print_list( "(1 , - + 22,  -333, - 4444, --5555)" );
            print_list( " (1,0,0,22,-333,0,4444,5555)" );
            print_list( "( ++101 --202 +-+-303 -+-+404 -+-+--505 )" );

        }
        Li = makeList_long( "(1 , - + 22,  -333, - 4444, --5555)" );
        Lj = makeList_long( "| [1] [-] [+] [22] [-333] [-] [4444] [--5555] | ==> (#8)");
        assertTrue( Li == Lj );

        Li = makeList_long( " (1,0,0,22,-333,0,4444,5555)" );
        Lj = makeList_long( "| [1] [0] [0] [22] [-333] [0] [4444] [5555] | ==> (#8)" );
        assertTrue( makeList_long( " 1 0 0 22 -333 0 4444 5555 ") == Lj );
        assertTrue( Li == Lj );

        Li = makeList_long( "( ++101 --202 +-+-303 -+-+404 -+-+--505 )" );
        Lj = makeList_long( "| [++101] [--202] [+-+-303] [-+-+404] [-+-+--505] | ==> (#5)");
        assertTrue( Li == Lj );
        assertTrue( Lj == makeList_long( " 0 0 0 0 0 " ) );
    }
    {   // Resto de las pruebas
        std::list<long> Li, Lj;
        Li = makeList_long( "( 101 202 303 404 505 )" );
        for ( int j=101; j<=505; j+=101 ) {
            Lj.push_back( j );
        }
        assertTrue( Li == Lj );
        Li = makeList_long( "( ++101 --202 +-+-303 -+-+404 -+-+--505 )" );
        Li = makeList_long( "( 101 202 303 404 505 )" );
        assertTrue( Li == Lj );

        assertTrue( makeList_long( " (0)" ) == makeList_long( " ( - ) " ) );
        assertTrue( makeList_long( " (0)" ) == makeList_long( " ( + ) " ) );
        assertTrue( makeList_long( " ()" )  == makeList_long( " (  ) " ) );
        assertTrue( makeList_long( "(0) " ) == makeList_long( " ( - ) " ) );
        assertTrue( makeList_long( "(0) " ) == makeList_long( " ( + ) " ) );
        assertTrue( makeList_long( "() " )  == makeList_long( " (  ) " ) );

        Li = makeList_long( "( +-1 -+2 ++3 --4 5 )" );
        Lj = makeList_long( "(+-1,-+2,++3,--4,5)" );
        assertTrue( Li == Lj );
        makeList_long( "( +-+1 -+-2 -++-3 -+-4 5 )" );
        makeList_long(   "(+-1,+2,++3,--4,5)" );
        assertTrue( Li == Lj );
    }
}
}

/// Test -> \c intlist().
void test_str2list::test_intlist() {
{{  // test::intlist()
        std::list<int> Li, Lj;
        Li = intlist( "()" );      assertTrue( Li.empty() );
        Li = intlist( " (  ) " );  assertTrue( Li.empty() );

        Li = intlist( "(1 2 3 4 5)" );
        for ( int j=1; j<=5; ++j ) {
            Lj.push_back( j );
        }
        assertTrue( Li == Lj );

        assertTrue( intlist( "( ... )" ) == intlist( "( 0 )" ) );
        assertTrue( intlist( "( ... )" ).size() == 1 );

        assertTrue( intlist( "(0)" ) == intlist( "( - )" ) );
        assertTrue( intlist( "(0)" ) == intlist( "( + )" ) );
        assertTrue( intlist( "()" )  == intlist( "(  )" ) );

        assertTrue( intlist( "( 1 2 3 4 5 )" ) == intlist( "(1,2,3,4,5)" ) );
        assertTrue( intlist( "{ 1,2,3,4,5 }" ) == intlist( "[1 2 3 4 5]" ) );

        Li = intlist( "( 1,22,333,4444,5555 )" );
        Lj = intlist(  "(1,22,333,4444,5555)" );
        assertTrue( Li == Lj );

        Li = intlist( "(1 , - + 22,  -333, - 4444, --5555)" );
        Lj = intlist( "(1,  0,0,22,  -333, 0,4444,      0)" );
        assertTrue( Li == Lj );

        Li = intlist( "(  1,  22,  333,  4444,  5555 )" );
        Lj = intlist( "| [1] [22] [333] [4444] [5555] |... ignore");
        assertTrue( Li == Lj );

        Li = intlist( "(  1,  22,  333,  4444,  5555, 0,   0 )" );
        Lj = intlist( "( [1] [22] [333] [4444] [5555] )etc ...");
        assertTrue( Li == Lj );
}}
}

/// Test -> \c doublelist().
void test_str2list::test_doublelist() {
{{  // test::doublelist()
        std::list<double> Li, Lj;
        Li = doublelist( "()" );      assertTrue( Li.empty() );
        Li = doublelist( " (  ) " );  assertTrue( Li.empty() );

        Li = doublelist( "(1. 2. 3. 4. 5.)" );
        for ( double j=1; j<=5; ++j ) {
            Lj.push_back( j );
        }
        assertTrue( Li == Lj );

        assertTrue( doublelist( "( ... )" ) == doublelist( "( 0 )" ) );
        assertTrue( doublelist( "( ... )" ).size() == 1 );

        assertTrue( doublelist( "(0)" ) == doublelist( "( - )" ) );
        assertTrue( doublelist( "(0)" ) == doublelist( "( + )" ) );
        assertTrue( doublelist( "()" )  == doublelist( "(  )" ) );

        assertTrue( doublelist( "( 1. 2. 3. 4. 5. )" ) == doublelist( "(1,2,3,4,5)" ) );
        assertTrue( doublelist( "{ 1.,2.,3.,4.,5. }" ) == doublelist( "[1 2 3 4 5]" ) );

        Li = doublelist( "( 1,22,333,4444,5555 )" );
        Lj = doublelist(  "(1,22,333,4444,5555)" );
        assertTrue( Li == Lj );

        Li = doublelist( "(1 , - + 22,  -333, - 4444, --5555)" );
        Lj = doublelist( "(1,  0,0,22,  -333, 0,4444,      0)" );
        assertTrue( Li == Lj );

        Li = doublelist( "(  1,  22,  333,  4444,  5555 )" );
        Lj = doublelist( "| [1] [22] [333] [4444] [5555] |... ignore");
        assertTrue( Li == Lj );

        Li = doublelist( "(  1,  22,  333,  4444,  5555, 0,   0 )" );
        Lj = doublelist( "( [1] [22] [333] [4444] [5555] )etc ...");
        assertTrue( Li == Lj );
}}
}

/// Test -> \c str2list().
void test_str2list::str2list_list() {
{{  // test::str2list::list()
    std::list<std::string> Li,Lj,Lk,Ll;
    str2list( Li , "( 1 22 333 4444 5555 )" );
    str2list( Lj , "< 1,22,333,4444,5555 > | del " );
    str2list( Lk , "{ 1 22 333 4444 5555 } | del " );
    str2list( Ll , "[ 1.22.333.4444.5555 ] | del " );    // '.' NO '.'
    std::list<std::string> :: const_iterator itI,itJ,itK,itL;
    itI = Li.begin(); itJ = Lj.begin(); itK = Lk.begin(); itL = Ll.begin();
    assertTrue( *itI == "1"    ); assertTrue( *itI++ == *itJ++ );
    assertTrue( *itI == "22"   ); assertTrue( *itI++ == *itJ++ );
    assertTrue( *itI == "333"  ); assertTrue( *itI++ == *itJ++ );
    assertTrue( *itI == "4444" ); assertTrue( *itI++ == *itJ++ );
    assertTrue( *itI == "5555" ); assertTrue( *itI++ == *itJ++ );
    assertTrue( 5 == Li.size() ); assertTrue(  5 == Lj.size()  );

    assertTrue(  Li == Lj && Lj == Lk && ((Lk != Ll)) ); // '.' NO '.'
    assertTrue( *itL == "1.22.333.4444.5555" );          // '.' NO '.'
    assertTrue( 1 == Ll.size() && Ll.size() < 2 );
}}
}

#include <iterator> // std::ostream_iterator<char>

/// Test -> \c makeList_char().
void test_str2list::test_makeList_char() {
{{ // test::charlist()
    std::list<char> Lcon,Lsin;
    Lcon = charlist( " { 'a' ' ' 'c' '4' '\'' '.' } " );
    Lsin = charlist( " (  a  ' '  c   4  '''   .  ) " );
    //                    1   2   3   4   5    6
    assertTrue( Lcon == Lsin );
    assertTrue( Lcon.size() == 6 );

    std::list<char>::const_iterator it;
    it = Lcon.begin();
    for ( unsigned i=0; i<Lcon.size()-1; ++i ) { ++it; }
    assertTrue( *it == '.' );
    --it;
    assertTrue( *it == '\'' );
}}
{
    std::list<char> Lcon,Lsin;
    Lcon = charlist( " { 'a' ' ' 'c' '4' '\'' '.' } " );
    Lsin = charlist( " (  a  ' '  c   4  '''   .  ) " );
    //                    1   2   3   4   5    6
    if (false) {
    //  http://www.devx.com/getHelpOn/10MinuteSolution/20410/1954?pf=true
        std::copy( Lcon.begin(), Lcon.end(), std::ostream_iterator<char> (std::cout, ",") );
    }
}
}


/// Test -> \c str2list().
void test_str2list::str2list_example() {
{{  // test::str2list::example()
    assertTrue( str2list("").empty() && "list<>.empty() !!!" );
    assertTrue( str2list("")    == str2list("()") );
    assertTrue( str2list("()")  == str2list("  |   | del ") );
    assertTrue( str2list("{}").size() == 0 );
    assertTrue( str2list("[0]") == str2list("| 0 |") );

    assertTrue( str2list("(0,1,,  ,,2)")  == str2list("( 0 1 2 )") );
    assertTrue( str2list("( 1 2 3 4 5 )") == str2list("| 1 2 3 4 5 |") );
    assertTrue( str2list("(,'c',,'j',)" ) == str2list( "< 'c' 'j' >") );

    assertTrue( str2list(" |a| etc... ")  == str2list("( a )") );
    assertTrue( str2list(" |a| etc... ").size() == 1 );
    assertTrue( str2list(" |a| etc... ")  == str2list("| a | #1") );
}}
{
    assertTrue( str2list(" |a| etc... ")            == str2list("| a |            #1") );
    assertTrue( str2list("")                        == str2list("| |              #0") );
    assertTrue( str2list("()")                      == str2list("| |              #0") );
    assertTrue( str2list("[0]")                     == str2list("| 0 |            #1") );

    assertTrue( str2list("(0,1,22)")                == str2list("| 0 1 22 |       #3") );
    assertTrue( str2list("(0,1,22)").size() == 3 );

    assertTrue( str2list("( 1 2 3 4 5 )")           == str2list("| 1 2 3 4 5 |    #5") );
    assertTrue( str2list("( 1 2 3 4 5 )").size() == 5 );

    assertTrue( str2list(" | 22 | etc... ")         == str2list("| 22 |           #1") );
    assertTrue( str2list("|| etc... ")              == str2list("| |              #0") );
    assertTrue( str2list("   22|  |etc|")           == str2list("| 22 |           #1") );
    assertTrue( str2list(" |1| ")                   == str2list("| 1 |            #1") );
    assertTrue( str2list(" | 2 | ")                 == str2list("| 2 |            #1") );
    assertTrue( str2list("1")                       == str2list("| 1 |            #1") );
    assertTrue( str2list("[-1]")                    == str2list("| -1 |           #1") );
    assertTrue( str2list("[1] [2] [3] [4] [5]")     == str2list("| 1 2 3 4 5 |    #5") );
    assertTrue( str2list("{ [1] [2] [3] [4] [5] }") == str2list("| 1 2 3 4 5 |    #5") );
    assertTrue( str2list(" | 22 | |etc|")           == str2list("| 22 |           #1") );
    assertTrue( str2list(" |22 |  |etc|")           == str2list("| 22 |           #1") );
    assertTrue( str2list(" | 22|  |etc|")           == str2list("| 22 |           #1") );
    assertTrue( str2list("   22 | |etc|")           == str2list("| 22 |           #1") );
    assertTrue( str2list("  22 |  |etc|")           == str2list("| 22 |           #1") );
    assertTrue( str2list("   22|  |etc|")           == str2list("| 22 |           #1") );
    assertTrue( str2list("(1,2,3,4,5)")             == str2list("| 1 2 3 4 5 |    #5") );
    assertTrue( str2list("( ... )")                 == str2list("| ... |          #1") );
    assertTrue( str2list("( - )")                   == str2list("| - |            #1") );
    assertTrue( str2list("( 1 - 22 )")              == str2list("| 1 - 22 |       #3") );
    assertTrue( str2list("(1,0,0,22,-333,0)")       == str2list("| 1 0 0 22 -333 0 | #6") );
    assertTrue( str2list("(1 , - - 22,  -333, - )") == str2list("| 1 - - 22 -333 - | #6") );
    assertTrue( str2list("( 0, 4444, 5555 )")       == str2list("| 0 4444 5555 |  #3") );
    assertTrue( str2list("( - 4444, --5555)")       == str2list("| - 4444 --5555 | #3") );
}
{
    assertTrue( str2list( "()" )          == str2list( "()" ) );
    assertTrue( str2list( "[-1]" )        == str2list( "-1" ) );
    assertTrue( str2list( " |2| " )       == str2list( " { 2 } " ) );
    assertTrue( str2list( "(1,2,3,4,5)" ) == str2list( "( 1 2 3 4 5 )" ) );

    assertTrue( str2list( "(1,22,333)")   == str2list( "( 1,22,333 )" ) );
    assertTrue( str2list( "(1,-22,333)")  == str2list( "( 1 -22 333 )" ) );

    assertTrue( str2list( "..." )          == str2list( "( ... )" ) );
    assertTrue( str2list( "(0)" )         == str2list( "( 0 )" ) );
    assertTrue( str2list( "(1,0,22)" )    == str2list( "( 1 0 22 )" ) );

    assertTrue( str2list( "(1,-,-,22,-333,-)" ) == str2list( "(1 , - - 22,  -333, - )" ) );
    assertTrue( str2list( "( 0, 4444, 5555 )" ) == str2list( "( 0 4444, 5555)" ) );
}
{
    assertTrue( str2list ( "{ 3, 3, 16}"        )  == str2list ( "(3 3 16)"         ) );

    assertTrue( str2list ( "(-, 5, 84, -5)"     )  == str2list ( "(- 5 84 -5)"      ) );
    assertTrue( str2list ( "(5, 7, 89, 65 100)" )  == str2list ( "(5 7 89 65 100)"  ) );
    assertTrue( str2list ( "(2.0, 6, 76.)"      )  == str2list ( "(2.0 6 76.)"      ) );
    assertTrue( str2list ( "('a',-6, 6, 7)"     )  == str2list ( "('a' -6 6 7)"     ) );
    assertTrue( str2list ( "('$', 5, 7, 0)"     )  == str2list ( "( '$' 5 7 0)"     ) );
    assertTrue( str2list ( "(al, 5, 6)"         )  == str2list ( "(al 5 6)"         ) );
    assertTrue( str2list ( "(casa, 5, 6)"       )  == str2list ( "(casa 5 6)"       ) );
    assertTrue( str2list ( "(---6, 78, 98)"     )  == str2list ( "[ ---6 78 98 ]"   ) );
    assertTrue( str2list ( "( 4  33  758)"      )  == str2list ( "| 4 33 758 | ..!" ) );
    assertTrue( str2list ( "(-4, 44, - --,-7)"  )  == str2list ( "(-4 44 - -- -7)"  ) );
    assertTrue( str2list ( "..."                )  == str2list ( "(...)"            ) );
    assertTrue( str2list ( "(k, l, 0, 8)"       )  == str2list ( "(k l 0 8)"        ) );
}
}


/// Imprime por filas el valor de \c "M".
template <class MAT>
void print( const char* name, MAT & M ) {
    std::cout << std::endl << name
              << '[' << M.rows() << ',' << M.cols() << ']'
              << std::endl;
    for (unsigned i=0; i < M.rows(); ++i) {
        for (unsigned j=0; j < M.cols(); ++j) {
            std::cout << " " << std::setw(6) << M(i,j);
        }
        std::cout << std::endl;
    }
}

/// Prints the contents in \c L.
void print( const std::list< std::string >& L ) {
    std::list< std::string >::const_iterator it;
    for ( it= L.begin(); it!=L.end(); ++it ) {
        std::cout << " [" << (*it) << ']';
    }
    std::cout << std::endl;
}

/// Uses \c str2list() to print list values.
void print_list( const char* str ) {
    std::list< std::string > L;
    str2list( L, str );
    #if 1
        // print list values
        std::cout << '\"'<< str << "\"\n";
        std::list< std::string >::const_iterator it;
        for ( it= L.begin(); it!=L.end(); ++it ) {
            std::cout << " [" << (*it) << ']';
        }
        std::cout << " ==> (#" << L.size() << ")" << std::endl << std::endl;
    #else
        // generate the assertTrue() statements
        std::cout << "assertTrue( str2list(\"" << str << "\") == str2list(\"|";
        std::list< std::string >::const_iterator it;
        for ( it= L.begin(); it!=L.end(); ++it ) {
            std::cout << " " << (*it);
        }
        std::cout << " | #" << L.size() << "\") );" << std::endl << std::endl;
    #endif
}

#include <iostream>
#include <iomanip>

/// Prints by rows in \c std::cout the contents of \c MATRIX[][].
/// Uses \c w chars to print each value.
template <class atype, unsigned Nrows, unsigned Ncols>
void printMatrix( atype MATRIX[Nrows][Ncols] , unsigned w=3 ) {
    for (unsigned i=0; i < Nrows; ++i) {
        for (unsigned j=0; j < Ncols; ++j) {
            std::cout << std::setw(w) << MATRIX[i][j];
        }
        std:: cout << std::endl;
    }
}

/// Stores all zeroes in \c MATRIX[][].
template <class atype, unsigned Nrows, unsigned Ncols>
void setValue( atype MATRIX[Nrows][Ncols] , atype val ) {
    for (unsigned i=0; i < Nrows; ++i) {
        for (unsigned j=0; j < Ncols; ++j) {
            MATRIX[i][j] = val;
        }
    }
}

/// Stores all zeroes in \c MATRIX[][].
template <class atype, unsigned Nrows, unsigned Ncols>
void setZero( atype MATRIX[Nrows][Ncols] ) {
    setValue<atype,Nrows,Ncols>( MATRIX, atype(0) );
}

/// Set the values in \c MATRIX[][] as consecutive numbers from \c 10.
/// Stores values by rows.
template <class atype, unsigned Nrows, unsigned Ncols>
void setRectangularRow( atype MATRIX[Nrows][Ncols] ) {
    unsigned k = 10;
    for (unsigned i=0; i < Nrows; ++i) {
        for (unsigned j=0; j < Ncols; ++j) {
            MATRIX[i][j] = atype( k++ );
        }
    }
}

/// Invoca print_list() de muchas formas diferentes
void do_print_list() {
    print_list( " |a| etc... " );
    print_list( "" );
    print_list( "()" );
    print_list( "[0]" );
    print_list( "(0,1,22)" );
    print_list( "( 1 2 3 4 5 )" );

    print_list( " | 22 | etc... " );
    print_list( "|| etc... " );
    print_list( "   22|  |etc|" );
    print_list( " |1| " );
    print_list( " | 2 | " );
    print_list( "1" );
    print_list( "[-1]" );
    print_list( "[1] [2] [3] [4] [5]" );
    print_list( "{ [1] [2] [3] [4] [5] }" );

    print_list( " | 22 | |etc|" );
    print_list( " |22 |  |etc|" );
    print_list( " | 22|  |etc|" );
    print_list( "   22 | |etc|" );
    print_list( "  22 |  |etc|" );
    print_list( "   22|  |etc|" );
    print_list( "(1,2,3,4,5)" );
    print_list( "( ... )" );
    print_list( "( - )" );
    print_list( "( 1 - 22 )" );
    print_list( "(1,0,0,22,-333,0)" );
    print_list( "(1 , - - 22,  -333, - )" );
    print_list( "( 0, 4444, 5555 )" );
    print_list( "( - 4444, --5555)" );
}

/// Test -> intmatrix().
void test_str2list::test_intmatrix() {
{{  // test::intmatrix()
    int M[4][5];       //   [0][1][2][3][4]
    intmatrix<4,5>( M , " |  1 //  3  4  5 | "   // [0]
                        " | ::  7  8  9 10 | "   // [1]
                        " | 11 12 13 .. 15 | "   // [2]
                        " | 16 17 // 19 20 | " );// [3]

    assertTrue( M[0][0] == 1 );  assertTrue( M[2][1] == 12 );
    assertTrue( M[2][3] == 0 );  assertTrue( M[1][0] ==  0 );

    int k=1;
    for ( size_t i=0; i<4; ++i ) {
        for ( size_t j=0; j<5; ++j ) {
            assertTrue( M[i][j]==k++ || M[i][j]==0 );
        }
    }
}}
{   // Resto de las pruebas
    int M[3][3]; setZero<int,3,3>( M );
    intmatrix<3,3>( M , " |  1  2  3  4  5 | "    // |  1  2  3 | [0]
                        " |  6  7  8  9 10 | "    // |  6  7  8 | [1]
                        " | 11 12 13 14 15 | "    // | 11 12 13 | [2]
                        " | 16 17 18 19 20 | " ); //   [0][1][2]
//  printMatrix<int,3,3>( M );
    assertTrue( M[2][1] == 12 );
    int k=1;
    for ( size_t i=0; i<3; ++i ) {
        for ( size_t j=0; j<3; ++j ) {
            if( M[i][j]!=k ) {
                std::cout << k << "== k != M[" << i << "][" << j
                          << "] == " << M[i][j] << std::endl;
            }
            assertTrue(M[i][j]==k++ );
        }
        k += (3-1);
    }
}
{   // Resto de las pruebas
    int M[7][8]; setZero<int,7,8>( M );
    intmatrix<7,8>( M , " |  1 //  3  4  5 | "
                        " | ::  7  8  9 10 | "
                        " | 11 12 13 .. 15 | "
                        " | 16 17 // 19 20 | " );
//  printMatrix<int,7,8>( M );
    assertTrue( M[2][1] == 12 );
    int k=1;
    for ( size_t i=0; i<4; ++i ) {
        for ( size_t j=0; j<5; ++j ) {
            assertTrue( M[i][j]==k++ || M[i][j]==0 );
        }
    }
}
{   // Resto de las pruebas
    int M[8][7]; setZero<int,8,7>( M );
    intmatrix<8,7>( M , " |  1  :  :  :  : | "
                        " |  :  2  :  :  : | "
                        " |  :  :  3  :  : | "
                        " |  :  :  :  4  : | " );
//  printMatrix<int,8,7>( M ); std::cout << std::endl;
    assertTrue( M[3][3] == 4  );
    int k=1;
    for ( size_t i=0; i<8; ++i ) {
        for ( size_t j=0; j<7; ++j ) {
            if ( i==j && i<4 ) {
                assertTrue( M[i][j]==k++ );
            }
            else {
                assertTrue( M[i][j]==0 );
            }
        }
    }
}
{   // Resto de las pruebas
    int M[8][7]; setZero<int,8,7>( M );
    intmatrix<8,7>( M , "  |  1             |  "
                        "  |  .  2  ,  ,  . |  "
                        "  |  .  .  3  ,  . |  "
                        "  |  .  .  .  4  . |  " );
//  printMatrix<int,8,7>( M ); std::cout << std::endl;
    assertTrue( M[1][1] == 2  );
    int k=1;
    for ( size_t i=0; i<8; ++i ) {
        for ( size_t j=0; j<7; ++j ) {
            if ( i==j && i<4 ) {
                assertTrue( M[i][j]==k++ );
            }
            else {
                assertTrue( M[i][j]==0 );
            }
        }
    }
}
}

/// Test -> str2matrix().
void test_str2list::test_str2matrix() {
{{  // test::str2matrix()
    std::list< std::list<std::string> > M;
    size_t sz = str2matrix( M , "  | 99             |  "    // L0
                                "  |  .  2  ,  ,  : |  "    // L1
                                "  |  .  .  3  ,  . |  "    // L2
                                "  |  a  b  c  4  . |  " ); // L3
    std::list< std::list<std::string> > :: const_iterator itL = M.begin();
    const std::list<std::string> & L0 = *itL; itL++;
    const std::list<std::string> & L1 = *itL; itL++;
    const std::list<std::string> & L2 = *itL; itL++;
    const std::list<std::string> & L3 = *itL; itL++;
    assertTrue( itL==M.end() && M.size() == 4 && sz == L3.size() );

    assertTrue( L0 == str2list( "  ( 99             )  " ) ); // L0
    assertTrue( L1 == str2list( "  [  .  2  ,  ,  : ]  " ) ); // L1
    assertTrue( L2 == str2list( "  <  .  .  3  ,  . >  " ) ); // L2
    assertTrue( L3 == str2list( "  {  a  b  c  4  . }  " ) ); // L3

    assertTrue( L0.size() == 1 && L0.back() == "99" );
    assertTrue( L1.size() == 3 && L1.back() ==  ":" );
    assertTrue( L2.size() == 4 && L2.front() == "." );
    assertTrue( L3.size() == 5 && L3.front() == "a" );

    assertTrue( L0 == str2list( "(99)"         ) ); // L0
    assertTrue( L1 == str2list( "(. 2 :)"      ) ); // L1
    assertTrue( L2 == str2list( "(. . 3 .)"    ) ); // L2
    assertTrue( L3 == str2list( "( a b c 4 .)" ) ); // L3
}}
{   // Resto de las pruebas
    std::list< std::list<std::string> > M;
    size_t sz = str2matrix( M , "  | 99             |"    // L0
                                "  |  .  2  ,  ,  : |"    // L1
                                "  |  .  .  3  ,  . |"    // L2
                                "  |  a  b  c  4  . |" ); // L3
    assertTrue( ! M.back().empty() && M.size() == 4 && sz == 5 );
    M.pop_back();
    assertTrue( ! M.back().empty() && M.size() == 3 );
}
{   // Resto de las pruebas
    std::list< std::list<std::string> > M;
    size_t sz = str2matrix( M , " | 1 | "
                                " | 2 | "
                                " | 3 | "
                                " | 4 | ");
    assertTrue( ! M.back().empty() && M.size() == 4 && sz == 1 );
}
{   // Resto de las pruebas
    std::list< std::list<std::string> > M;
    size_t sz = str2matrix( M , "|1||2||3||4|");
    assertTrue( ! M.back().empty() && M.size() == 4 && sz == 1 );
}
{   // Resto de las pruebas
    std::list< std::list<std::string> > M;
    size_t sz = str2matrix( M , "1|2|3|4");
    assertTrue( ! M.back().empty() && M.size() == 4 && sz == 1 );
}
{   // Resto de las pruebas
    std::list< std::list<std::string> > M;
    size_t sz = str2matrix( M , "[1|2|3|4 5]");
    assertTrue( ! M.back().empty() && M.size() == 4 && sz == 2 );
}
}

/// Test -> longmatrix().
void test_str2list::test_longmatrix() {
{{  // test::longmatrix()
    long M[4][5];       //   [0][1][2][3][4]
    longmatrix<4,5>( M , " |  1 //  3  4  5 | "   // [0]
                         " | ::  7  8  9 10 | "   // [1]
                         " | 11 12 13 .. 15 | "   // [2]
                         " | 16 17 // 19 20 | " );// [3]

    assertTrue( M[0][0] == 1 );  assertTrue( M[2][1] == 12 );
    assertTrue( M[0][1] == 0 );  assertTrue( M[3][2] ==  0 );

    long k=1;
    for ( size_t i=0; i<4; ++i ) {
        for ( size_t j=0; j<5; ++j ) {
            assertTrue( M[i][j]==k++ || M[i][j]==0 );
        }
    }
}}
{   // Resto de las pruebas
    long M[4][5];
    longmatrix<4,5>( M, " |  1  2  3  4  5 | "
                        " |  6  7  8  9 10 | "
                        " | 11 12 13 14 15 | "
                        " | 16 17 18 19 20 | "  );
    int n = 1;
    for ( int i=0; i<4; ++i ) {
        for ( int j=0; j<5; ++j ) {
            if (false) { M[2][3] = -1; }
            std::string fail; // dice adónde falla str2matrix()
            fail  =   TestCase::toString( M[i][j] ) + " == M";
            fail += '[' + TestCase::toString(i) + ']';
            fail += '[' + TestCase::toString(j) + ']' + " != ";
            fail +=       TestCase::toString(n);
            assertTrue_Msg( fail , M[i][j] == n );
            ++n;
        }
    }
    //  =\_fail: -1 == M[2][3] != 14
    //  =/ (688) test_str2list.cpp
}
}

/// Test -> doublematrix().
void test_str2list::test_doublematrix() {
{{  // test::doublematrix()
    double M[4][5];       //   [0] [1] [2] [3] [4]
    doublematrix<4,5>( M , " | 10. ///  30 40. 5.e1  | "   // [0]
                           " |   . 70.  80 90. 100.  | "   // [1]
                           " | 110 120 130 ..  150.  | "   // [2]
                           " | 160 170 /// 190 2.e2  | " );
    assertTrue( M[2][1] == 120.0 );
    assertTrue( M[1][0] == 0.0 && M[2][3] == 0.0 );
    double k=10.0;
    for ( size_t i=0; i<4; ++i ) {
        for ( size_t j=0; j<5; ++j ) {
            assertTrue( M[i][j]==k || M[i][j]==0 );
            k += 10.0;
        }
    }
}}
{   // Resto de las pruebas
    double M[4][3]; setValue<double,4,3>( M,-1 );
    doublematrix<4,3>( M , " 11. 22. | 33.0 0.44e2" );
//  printMatrix<double,4,3>( M,4 ); std::cout << std::endl;

    assertTrue( M[0][0]==11.0 && M[0][1]==22.0 );   assertTrue( M[0][2]==-1.0 );
    assertTrue( M[1][0]==33.0 && M[1][1]==44.0 );   assertTrue( M[1][2]==-1.0 );

    assertTrue( M[2][0]==-1.0 && M[2][1]==-1.0 );   assertTrue( M[2][2]==-1.0 );
    assertTrue( M[3][0]==-1.0 && M[3][1]==-1.0 );   assertTrue( M[3][2]==-1.0 );
}
{   // Resto de las pruebas
    double M[4][5];
    doublematrix<4,5>( M , "  11. "
                           "| 22. "
                           "| 33.0 "
                           "| 0.44e2 " );
    assertTrue( M[0][0] == 11.0 );
    assertTrue( M[1][0] == 22.0 );
    assertTrue( M[2][0] == 33.0 );
    assertTrue( M[3][0] == 44.0 );
}
{   // Resto de las pruebas
    double M[6][1];
    doublematrix<6,1>( M , "  11e-4 "
                           "| 220.e-5 "
                           "| 33.0e-4 "
                           "| 0.44e-2 " );
    assertTrue( M[0][0] == 0.0011 );
    assertTrue( M[1][0] == 0.0022 );
    assertTrue( M[2][0] == 0.0033 );
    assertTrue( M[3][0] == 0.0044 );
}
{   // Resto de las pruebas
    long M[4][5];
    longmatrix<4,5>( M , " | 1 | "
                         " | 2 | "
                         " | 3 | "
                         " | 4 | ");
    long k = 1;
    for ( int i=0; i<4; ++i ) {
        for ( int j=0; j<1; ++j ) {
            assertTrue(  M[i][j] == k ); k++;
        }
    }
}
}

/// Test -> \c charmatrix().
void test_str2list::test_charmatrix() {
{{ // test::charmatrix()
    char A[5][3];
    charmatrix<5,3>( A , " | 1 a V | "
                         " | 2 b W | "
                         " | 3 c X | "
                         " | 4 d Y | "
                         " | 5 e Z | " );

    char D[5][3];
    charmatrix<5,3>( D , " | 2 c Y | "
                         " | 3 d Z | "
                         " | 4 e V | "
                         " | 5 a W | "
                         " | 1 b X | " );

    for ( int j=0; j<5; ++j ) {
        assertTrue( A[j][0] == (j)+'1' );
    }
    for ( int j=0; j<5; ++j ) {
        assertTrue( A[j][1] == (j)+'a' );
    }
    for ( int j=0; j<5; ++j ) {
        assertTrue( A[j][2] == (j)+'V' );
    }
}}
{
}
}

/// Programa principal.
int main() {
    if (true) {
        test_str2list tester;
        tester.setUp();
        tester.run();
        std::cout << tester.report();
    }
    if (false) {
        int M[4][5];
        setZero<int,4,5>( M );
        intmatrix<4,5>( M , " | 1  2  3  4  5 | " );
        printMatrix<int,4,5>( M );
        std::cout << std::endl;

        setRectangularRow<int,4,5>( M );
        intmatrix<4,5>( M , " |  1 | 2 | 3 | 4 | 5 | " );
        printMatrix<int,4,5>( M );
    }
    if ( false ) { do_print_list(); }
}

// EOF: test_str2list.cpp
