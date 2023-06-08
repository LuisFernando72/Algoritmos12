#include <stdarg.h>
#include <iostream>
#include <vector>
#include <conio.h>
#include <Windows.h>
#include <fstream>
#include <stdio.h>
#include <cstdlib>
#include <stack>
#include <algorithm>
#include <queue>
#include <string>
#include <string.h>
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include "casa1.h"
#include <cstring> 

using namespace std;

string aa(string cadena);
string bb(string cadena);
string cc(string cadena);

#pragma warning(disable:4996)


int main() {
  uno();

	string lectura;                                  
	string cadena = "ab|*a.b.b.#.";  
 
}


string aa(string cadena) {
	string lectura;                                             // Aquí vamos a ir leyendo cada valor por separado
	stringstream input_stringstream(cadena);
	int contador = 0;
	while (getline(input_stringstream, lectura, '*')) {
		/* if (lectura.size() < 4) {*/
		contador++;
		cout << "Un valor: " << contador << " " << lectura << "-> *" << lectura.size() << endl;

		/* }*/
	}

	return "";
}

string bb(string cadena) {
	string lectura;                                             // Aquí vamos a ir leyendo cada valor por separado
	stringstream input_stringstream(cadena);                    // Convertir la cadena a un stream
	int contador = 0;
	while (getline(input_stringstream, lectura, '|')) {
		contador++;
		cout << "Un valor: " << contador << " " << lectura << "-> |" << endl;

	}

	return "";
}

string cc(string cadena) {
	string lectura;                                             // Aquí vamos a ir leyendo cada valor por separado
	stringstream input_stringstream(cadena);                    // Convertir la cadena a un stream
	int contador = 0;
	while (getline(input_stringstream, lectura, '.')) {
		contador++;
		cout << "Un valor: " << contador << " " << lectura << "-> ." << endl;

	}

	return "";
}