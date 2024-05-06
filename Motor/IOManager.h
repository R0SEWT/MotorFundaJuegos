#pragma once
#include<iostream>
#include<string>
#include<vector>
using namespace std;

// helper para leer archivos de bytes y transformarlos a imagenes
class IOManager
{
public:
	//metodo estatico 
	// ------------------------
	// es independiente de los objetos -> no se necesita instanciar para crearla
	// se accesa mediante ::  -> IOManger::readFileToBuffer
	// los metodos estaticos se usan unicamente para acceder a atributos estaticos

	static bool readFileToBuffer(string filepath,
		vector<unsigned char>& buffer);

};

