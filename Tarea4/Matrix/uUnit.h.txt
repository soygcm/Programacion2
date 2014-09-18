// uUnit.h (C) 2011 adolfo@di-mare.com

/** \file   uUnit.h
    \brief  [u]Micro module for [Unit] program testing.

    \author Adolfo Di Mare <adolfo@di-mare.com>
    \date   2011
    \see    http://www.di-mare.com/adolfo/p/BUnitXP.htm

\code
    #include "uUnit.h" // assertTrue() && assertFalse()

    // Example
    int main( int argc , const char* argv[] ) {
        std::cout << "TestCase [uUnit.h] yesYES" << std::endl;
        assertTrue(  1==2 ); // ok?
        assertFalse( 1==2 ); // ok!
        return test_myImp(argc , argv);
    }
\endcode
*/

#ifndef ADOLFO_UUNIT
#define ADOLFO_UUNIT

// #include "BUnit.h" // http://www.di-mare.com/adolfo/p/BUnit.htm
// #include "BUnit.h" // http://www.di-mare.com/adolfo/p/BUnitXP.htm

/// "C:/DIR/SubDir/file_name.ext" ==> "file_name.ext"
static // "static" avoids "function already defined" linkage error
const char* remove_dir_name( const char* str )
{
    const char *p = str;     // p = str+strlen(str);
    while ( *p != 0 ) { ++p; }
    while ( *p != '/' && *p != '\\' && *p != ':' ) {
        if ( p == str ) { return str; }
        --p;
    }
    return p+1;
/*
    Each object file will contain a local compiled copy of this function
    because it is declared 'static'. This duplication is the price paid to
    avoid using a uUnit.cpp file.
*/
}

#ifdef __cplusplus
    #include <iostream> // std::cout
    template <class T> inline // inline avoids 'multiple definition' warning
    void testThis( bool cond, const T* label, const T* fname, long lineno );
    template <> inline
    void testThis<char>(
        bool cond, const char* label, const char* fname, long lineno
    ) {
        if (!cond) {
            std::cout << "=\\_fail: " << label << " \n=/ (";
            std::cout << lineno << ") " << remove_dir_name(fname) << "\n";
        }
    }
#else
    #include <stdio.h>  // printf()
    static
    void testThis(
        int cond, const char* label, const char* fname, long lineno
    ) {
        if (!cond) {
            printf( "%s%s%s",   "=\\_fail: " , label , " \n=/ (" );
            printf( "%ld%s%s%s", lineno , ") " , remove_dir_name(fname),"\n" );
        }
    }
#endif


#ifdef NDEBUG
    #define assertTrue(x)   ((void)0)
    #define assertFalse(x)  ((void)0)
#else /* debugging enabled */
    /// (cond ? () : cout << "cond" )
    #define assertTrue(cond)  testThis(!!(cond),     #cond,     __FILE__,__LINE__ )
    /// (!(cond) ? () : cout << "!" << (cond)" )
    #define assertFalse(cond) testThis( !(cond),"!(" #cond ")", __FILE__,__LINE__ )
#endif

#endif

// EOF: uUnit.h
