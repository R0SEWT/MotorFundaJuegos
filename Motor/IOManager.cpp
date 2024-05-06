#include "IOManager.h"
#include <fstream>
#include "Error.h"

bool IOManager::readFileToBuffer(string filepath, vector<unsigned char>& buffer)
{
    ifstream file(filepath, ios::binary);
    if (file.fail()) {
        perror(filepath.c_str());
        fatalError("No se puede abrir el archivo " + filepath);
        return false;
    }

    //get TotalSize
    file.seekg(0, ios::end);        // te permite moverte por el archivo (a la pos indicada)
    int fileSize = file.tellg();    // current pos

    //ios::beg <- fin header (filesize = tTotal - tCabezera)
    file.seekg(0, ios::beg);
    fileSize -= file.tellg();

    // vector del mismo tamano del archivo
    buffer.resize(fileSize);
    file.read((char*)&(buffer[0]), fileSize);   // casteamos porque ifstream.read espera un char
    // le pasamos el vec[0] para que realice el volcado 
    // del file iterando a la vez en el vec y en el file 
    // (empieza en ios::beg) , hasta fileSize
    file.close();

    return true;
}
