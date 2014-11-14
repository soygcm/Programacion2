// fotosRepetidas.h (c) 2014

/** \file  fotosRepetidas.cpp
    \brief Programa que compara dos carpetas de fotos, para determinar si estan repetidas

    \author Gabriel CastaÒaza <gcmelmejor@gmail.com>
    \date   2014
*/

#include <iostream>      // std::cout, etc.
#include <dirent.h>
#include <string>
#include "CppSQLite3.h"

using namespace std;
template <typename T> std::stringstream logging_expansion ( T const * value );

/**
 Toma un nombre de imagen ejemplo: "DS599003_3329 hola.jpg"
 y deja solamente la parte "DS599003_3329" borrando desde el espacio
 para adelante.
 Tambien toma un nombre de imagen ejemplo: "DS599003_3329.jpg"
 y deja solamente la parte "DS599003_3329" borrando desde el punto
 para adelante.
 */
string onlyImportant(string &fileName){

    string fname = fileName;

    unsigned long firstSpace = fname.find_first_of(" ");
    unsigned long firstDot = fname.find_first_of(".");

    fname = fname.substr(0, firstSpace);
    fname = fname.substr(0, firstDot);

    return fname;

}

/**
    Guarda todas las fotos (archivos que terminan con .jpg) que estan en una carpeta \c parent
    y en un subdirectorio \c subDirName. En una lista \c list<string>
*/
void getPhotos(const char *parent, const char *subDirName, CppSQLite3DB& DB, const char *table){

    DIR *dir;
    char parentEdit[1024];

    strcpy(parentEdit, parent);

    char *newParent = strcat(parentEdit, subDirName);

    dir = opendir ( strcat(newParent, "/") );

    if (dir != NULL) {

        struct dirent *ent;
        ent = readdir (dir);
        while (ent != NULL) {

            if (ent->d_type == DT_REG){		// if entry is a regular file
                string extension = "jpg";
                string fname = ent->d_name;	// filename
                // if filename's last characters are extension
                if (fname.find(extension, (fname.length() - extension.length())) != string::npos){

                    string DMSString;

                    DB.execDML("begin transaction;");
                    DMSString = "insert into "+ string(table) +" values('" + fname + "','"+ onlyImportant(fname) +"');";
                    DB.execDML( DMSString.data() );
                    DB.execDML("commit transaction;");
                }
            }

            if (ent->d_type == DT_DIR) {
                bool punto = strcmp(ent->d_name, ".") == 0;
                bool puntoPUnto = strcmp(ent->d_name, "..") == 0;
                if ( !punto && !puntoPUnto ) {

                    getPhotos(newParent, ent->d_name, DB, table);

                }
            }
            ent = readdir (dir);
        }
        closedir (dir);
    }

}

/**
    compara dos \c <string> ignorando mayusculas y minusculas.
*/
bool iequals(const string &a, const string &b)
{
    unsigned long sz = a.size();
    if (b.size() != sz)
        return false;
    for (unsigned int i = 0; i < sz; ++i)
        if (tolower(a[i]) != tolower(b[i]))
            return false;
    return true;
}

/**
    a partir de dos listas de nombres de archivos, imprime tres listas:
    - Con solo los que estan en la primera lista
    - Con solo los que estan en la segunda lista
    - Con los nombres de archivos que estan en ambos
*/
void fotosRepetidas(CppSQLite3DB& DB, char *albumDirectory, char *cameraDirectory){

    CppSQLite3Query q;

    cout << "**** (ALBUM except CAMARA) Fotos solo en: " << cameraDirectory << endl;
    q = DB.execQuery("SELECT codigo FROM ALBUM "
                     " EXCEPT "
                     " SELECT codigo FROM CAMARA;");
    while (!q.eof()){
        cout << "\t" << q.fieldValue(0) << endl;
        q.nextRow();
    }
    cout << endl;

    cout << "**** (CAMARA except ALBUM) Fotos solo en: " << albumDirectory << endl;
    q = DB.execQuery("SELECT codigo FROM CAMARA "
                     " EXCEPT "
                     " SELECT codigo FROM ALBUM;");
    while (!q.eof()){
        cout << "\t" << q.fieldValue(0) << endl;
        q.nextRow();
    }
    cout << endl;

    cout << "**** (ALBUM INTERSECT CAMARA) en ambos directorios ****" << endl;
    q = DB.execQuery("SELECT codigo FROM ALBUM "
                     " INTERSECT "
                     " SELECT codigo FROM CAMARA;");
    while (!q.eof()){
        cout << "\t" << q.fieldValue(0) << endl;
        q.nextRow();
    }
    cout << endl;

}



void createTables(CppSQLite3DB& DB) {

    DB.execDML( "DROP TABLE IF EXISTS ALBUM" );
    DB.execDML( "DROP TABLE IF EXISTS CAMARA" );

    DB.execDML( "CREATE TABLE [ALBUM]( "
               "[nombre]  VARCHAR(26) NOT NULL, "
               "[codigo]  VARCHAR(26) NOT NULL "
               ");" );
    DB.execDML( "CREATE TABLE [CAMARA]( "
               "[nombre]  VARCHAR(26) NOT NULL, "
               "[codigo]  VARCHAR(26) NOT NULL "
               ");" );

}

/**
    Compara dos carpetas para mostrar que archivos estan solo en la primera,
    solo en la segunda o en ambas carpetas.
*/
int main(int argc, char **argv) {

    /// Archivo que contiene la base de datos completa.
    const char* gszDBfile = "FOTOS.db";

    CppSQLite3DB DB;
    DB.open(gszDBfile);

    try {
        createTables(DB);
    }catch (CppSQLite3Exception& e) {
        cerr << e.errorCode() << ":" << e.errorMessage() << endl;
    }

    char albumDirectory[1024];
    char cameraDirectory[1024];

    cout << "Directorio de la memoria flash: ";
    cin >> cameraDirectory;
    cout << endl;

    cout << "Directorio de la computadora: ";
    cin >> albumDirectory;
    cout << endl;

    getPhotos(albumDirectory, "", DB, "ALBUM");
    getPhotos(cameraDirectory, "", DB, "CAMARA");

    fotosRepetidas(DB, albumDirectory, cameraDirectory);

    return 0;
}



// EOF: huequeador.h (c) 2014 adolfo@di-mare.com
