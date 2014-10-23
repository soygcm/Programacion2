// blnksplt.cpp (C) 2010 adolfo@di-mare.com

/** \file  blnksplt.cpp
    \brief Breaks long lines in space or tab characters.

    Reads input files and breaks up long lines.
    - Breaks lines in space or tab characters only.
    - Long lines with no spaces or tabs will not be broken up.
    - Adds MSDOS cr+lf to break up lines.
    - Never removes characters from files.

    \date 2010
    \author Adolfo Di Mare <adolfo@di-mare.com>
*/

#include <fstream>
#include <iostream>
#include <cctype>
#include "readDir.h"
using namespace std;

const int DEFAULT_LENGTH = 78; ///< Default line length.
const int MIN_LENGTH     = 16; ///< Minimun line length.
#undef MSDOS_support ///< No implemented

/// Output the usage message
void echo_help( const char *argv[] ) {
    cout << "Breaks long lines in space or tab characters." << endl << endl;
    cout << "Usage " << argv[0] << " [OPTION] ";
    cout << "[drive:][path][file] ... [drive:][path][file]" << endl;
    cout << "-lNNN      Max Line Length. Default is " << DEFAULT_LENGTH
                       << " characters." << endl;
    cout << "--lineNNN  Longer lines will be split at space or tab characters." << endl;
    cout << "           Line Length must be at least " << MIN_LENGTH << '.' << endl;
    cout << "EXAMPLE:" << endl << "        ";
    cout << argv[0] << " --line66 *.txt *.doc" << endl;
    return;
}

/// Holds command line options.
struct option {
    unsigned length; ///< Line length.
    /// Constructor.
    option(): length(DEFAULT_LENGTH) { }
};

int do_options( int argc, const char *argv[], option& op );
const char* get_temp_file_name( const char* argv[] );
void do_wrap( const char* TEMP_FILE , const char* FILE , const option& op );

/// Main program.
int main(int argc, const char *argv[]) {
    option op;
    int i = do_options( argc, argv , op );
    if ( i==argc ) { echo_help(argv); return 1; }
    const char* TEMP_FILE = get_temp_file_name( argv );
    if ( 0==TEMP_FILE ) { return 1; }
    for ( ; i<argc; ++i ) {
        readDir file( argv[i] );
        while ( file.hasNext() ) {
            file.next(); // process next file
            do_wrap( TEMP_FILE , file.getName() , op );
        }
    }
    remove( TEMP_FILE ); delete [] TEMP_FILE;
    return 0;
}

#include <cstdio>  // remove() && rename()
#include <cassert> // assert()

/// Breaks every line in \c FILE.
/// - Lines will at most be \c op.length characters in length.
/// - File \c TEMP_FILE in the current directory will be used
///   for intermediate data.
/// - At the end, the old \c FILE will be replaced with \c TEMP_FILE.
void do_wrap( const char* TEMP_FILE , const char* FILE , const option& op ) {
    ifstream inFile(  FILE      , ios_base::binary | ios_base::in );
    ofstream outFile( TEMP_FILE , ios_base::binary | ios_base::out );
    cout << "[" << op.length << "] "  << FILE<< ' ';
    if ( !inFile.is_open() ) {
        cout << FILE << " [ERROR: cannot open input FILE]" << endl;
        return;
    }
    if ( !outFile.is_open() ) {
        inFile.close();
        cout << TEMP_FILE << " [ERROR: cannot open temporary FILE]" << endl;
        return;
    }
    string line(""); // CR=='\r'  LF=='\n'
    int lastSpace(-1), col(-1); bool addedCRLF(false);
    while ( !inFile.fail() ) {
        char ch; inFile.get(ch);
        if ( inFile.fail() ) { break; }
        line += ch; col++; assert( line.length()-1 == unsigned(col) );
        // cout << '(' << ch << '.' << col << ')'; if (col%22==0) { cout << endl; }
        if ( ch==' '|| ch=='\t' ) { lastSpace = col; }
        if ( ch=='\n' ) {
            if ( addedCRLF && line=="\r\n" ) { addedCRLF = false; }
            else { outFile << line; } // dont´t repeat end of line
            line = ""; lastSpace = col = -1;
        }
        else if ( line.length() >= op.length ) {
            if ( lastSpace<0 ) {
                outFile << line;
                col = -1; line = ""; addedCRLF = false;
            }
            else {
                addedCRLF = true;
                outFile << line.substr(0, lastSpace+1);
                outFile << "\r\n"; // adds CR+LF to break up line
                line = line.substr(lastSpace+1);
                col -= lastSpace+1;
            }
            lastSpace = -1;
            // cout << endl << '[' << line[0] << '.' << col << ']';
        }
    }   // File.fail() ==> http://www.horstmann.com/cpp/pitfalls.html
    if ( line != "" ) {
        outFile << line;
    }
    inFile.close();
    outFile.close();
    if ( 0 != remove( FILE ) ) {
        cout << "ERROR[117]: cannot remove " << FILE;
    }
    else if ( 0 != rename( TEMP_FILE, FILE ) ) {
        cout << "ERROR[120]: cannot create " << FILE;
    }
    cout << endl;
}

/// Gets program options from the command line.
/// - Returns the index of the first parameter that is not an option.
/// - Outputs an error message whenever an error condition is detected.
/// - Signals and error condition returning \c argc.
int do_options( int argc, const char *argv[], option& op ) {
    int i;
    for ( i=1; i<argc; ++i ) {
        string argI( argv[i] );
        if ( argI[0] != '-' || argI.length()<3 ) {
            // options come before file names
            break; // ==> return i;
        }
        argI[1] = tolower( argI[1] ); argI[2] = tolower( argI[2] );
        // option "--line nnn"
        if ( 'l' == argI[1] || 'l' == argI[2] ) {
            int j=2;
            while ( ! isdigit(argI[j]) ) { ++j; } // skip non digits
            int length = 0;
            while ( isdigit(argI[j]) ) {
                length = 10*length + (argI[j]-'0');
                ++j;
            }
            if ( length<=MIN_LENGTH ) {
                cout << "ERROR: must provide length bigger than " << MIN_LENGTH << endl;
                echo_help( argv );
                i = argc;
                return argc;
            }
            else {
                op.length = length; // store new length
            }
        }
        #ifdef MSDOS_support
            argI[1] = tolower( argI[1] ); argI[2] = tolower( argI[2] );
            // option "--dos"
            if ( 'd' == argI[1] || 'd' == argI[2] ) {
                op.unix = false;
            }
        #endif
    }
    return i;
}

#include <sstream>   // class basic_ostringstream<T,Ch>

/// Returns a string that has the value of \c val.
/// - \c toString() with standard C++
template <class T>
string toString( const T & val ) {
//  typedef basic_ostringstream<char> ostringstream;
    std::basic_ostringstream<char> temp; // ostringstream temp;
    temp << val;
    return temp.str( );
}

#include <climits> // INT_MAX

/// Finds a file name that doesn't exists in the current directory.
/// - Uses \c "name" as the file name, and extension is a number.
/// - The invoking routine must delete the string invoking \c delete[].
const char* get_temp_file_name( const char* argv[] ) {
    for ( int i=0; i<INT_MAX; ++i ) {
        string file_name = argv[0];
        file_name += '.' + toString(i);
        ifstream FILE ( file_name.c_str() , ios_base::in );
        if ( ! FILE.is_open() ) {
            char * ret = new char[file_name.length()+1];
            strcpy( ret , file_name.c_str() );
            return ret;
        }
        FILE.close();
    }
    cout << "ERROR: cannot create temporary file" << endl;
    echo_help( argv );
    return 0;
}

// EOF: blnksplt.cpp
