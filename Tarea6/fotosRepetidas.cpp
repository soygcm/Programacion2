// fotosRepetidas.h (c) 2014 

/** \file  fotosRepetidas.cpp
    \brief Programa que compara dos carpetas de fotos, para determinar si estan repetidas

    \author Gabriel Castañaza <gcmelmejor@gmail.com>
    \date   2014
*/

#include <iostream>      // std::cout, etc.
#include <list>          // std::list<>
#include <dirent.h>
#include <string>

using namespace std;

/**
    Guarda todas las fotos (archivos que terminan con .jpg) que estan en una carpeta \c parent 
    y en un subdirectorio \c subDirName. En una lista \c list<string>
*/
void getPhotos(const char *parent, const char *subDirName, list<string> &list){

    DIR *dir;
    char parentEdit[1024];

    strcpy(parentEdit, parent);

    char *newParent = strcat(parentEdit, subDirName);

    dir = opendir ( strcat(newParent, "/") );

    if (dir != NULL) {

        struct dirent *ent;
        ent = readdir (dir);
        while (ent != NULL) {
//            printf ("%s \n", ent->d_name);

            if (ent->d_type == DT_REG){		// if entry is a regular file
                string extension = "jpg";
                string fname = ent->d_name;	// filename
                // if filename's last characters are extension
                if (fname.find(extension, (fname.length() - extension.length())) != string::npos){

                    list.push_back(fname);		// add filename to results vector
                }
            }

            if (ent->d_type == DT_DIR) {
                bool punto = strcmp(ent->d_name, ".") == 0;
                bool puntoPUnto = strcmp(ent->d_name, "..") == 0;
                if ( !punto && !puntoPUnto ) {
                    getPhotos(newParent, ent->d_name, list);
                }
            }
            ent = readdir (dir);
        }
        closedir (dir);
    }

}

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
void fotosRepetidas(list<string> &listAlbum, list<string> &listCamera){

    list<string> listOnlyCamera, listOnlyAlbum, listBoth;

    list<string>::iterator i;

	for( i = listAlbum.begin(); i != listAlbum.end(); ++i){

        bool siEsta = false;

        list<string>::iterator j;
        for( j = listCamera.begin(); j != listCamera.end(); ++j){

            if ( iequals( onlyImportant(*i), onlyImportant(*j) ) ) {
                listBoth.push_back(*i);
                siEsta = siEsta || true;
            }
        }

        if (!siEsta) {
            listOnlyAlbum.push_back(*i);
        }

    }

	for( i = listCamera.begin(); i != listCamera.end(); ++i){

        bool siEsta = false;

        list<string>::iterator j;
        for( j = listAlbum.begin(); j != listAlbum.end(); ++j){

            if ( onlyImportant(*i) == onlyImportant(*j) ) {
                siEsta = siEsta || true;
            }

        }

        if (!siEsta) {
            listOnlyCamera.push_back(*i);
        }

    }



    cout << "**** Fotos solo en la camara ****" << endl;
	for( i = listOnlyCamera.begin(); i != listOnlyCamera.end(); ++i)
		cout << *i << endl;
	cout << endl;

    cout << "**** Fotos solo en el album ****" << endl;
	for( i = listOnlyAlbum.begin(); i != listOnlyAlbum.end(); ++i)
		cout << *i << endl;
	cout << endl;

    cout << "**** Fotos en ambos ****" << endl;
    for( i = listBoth.begin(); i != listBoth.end(); ++i)
		cout << *i << endl;
	cout << endl;

}

/**
    Compara dos carpetas para mostrar que archivos estan solo en la primera,
    solo en la segunda o en ambas carpetas.
*/
int main(int argc, char **argv) {

    list<string> albumPhotos, cameraPhotos;

    char albumDirectory[] = "/Users/gabriel/Proyectos/UCR/IV-Semestre/Progra2/Programacion2/Tarea6/Carpetas-Prueba/muchos/ALBUM";
    char cameraDirectory[] = "/Users/gabriel/Proyectos/UCR/IV-Semestre/Progra2/Programacion2/Tarea6/Carpetas-Prueba/muchos/DCIM";

    getPhotos(albumDirectory, "", albumPhotos);
    getPhotos(cameraDirectory, "", cameraPhotos);

    fotosRepetidas(albumPhotos, cameraPhotos);

    return 0;
}



// EOF: huequeador.h (c) 2014 adolfo@di-mare.com
