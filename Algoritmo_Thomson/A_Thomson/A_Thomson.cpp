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
#include "fin_proceso.h"
#include "grafica_html.h"

//ENTRADA PARA EL PROGRAMA EN  EL TXT, COLOCAR EXPRESION REGULAR PARA EL INICIO DE OPERACIONES
//Luis Fernando Paxel Cojolón
//Jorge David Nuñez
//Oliver Jorgen Kies
//Diego Ricardo D Leon
//Marlon Alexander Barrios

using namespace std;

struct estructura
{
	int id_inicio = -1;
	char tt;
	int fin_id = -1;
};


vector<estructura> nfa;
ofstream fout("body.txt", ios::out);


bool letra_correcta(string s)
{
	stack<char> bracket;
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] >= 'a' && s[i] <= 'z')
			continue;
		else if (s[i] == '|' || s[i] == '*' || s[i] == '.')
			continue;
		else if (s[i] == '(')
			bracket.push('(');
		else if (s[i] == ')')
		{
			if (bracket.empty())
				return false;
			else
				bracket.pop();
		}
		else
			return false;
	}
	if (!bracket.empty())
		return false;
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] == '|')
		{
			if (i == 0 || i == s.length() - 1)
				return false;
			else if (s[i + 1] == '|' || s[i + 1] == '*' || s[i + 1] == '.' || s[i + 1] == ')')
				return false;
		}
		else if (s[i] == '.')
		{
			if (i == 0 || i == s.length() - 1)
				return false;
			else if (s[i + 1] == '|' || s[i + 1] == '*' || s[i + 1] == '.' || s[i + 1] == ')')
				return false;
		}
		else if (s[i] == '*')
		{
			if (i == 0)
				return false;
			else if (s[i + 1] == '*')
				return false;
		}
	}
	return true;
}

int simbolo_estatico(char c)
{
	if (c == '*'){
		return 3;
	}else if (c == '.'){
		return 2;
	}else if (c == '|'){
		return 1;
	}else{
		return -1;
	}
}

string Analizar_simbolo(string expresion)
{
	string infix;
	for (int i = 0; i < expresion.length(); i++){
		char tmp = expresion[i];
		char next;
		if (i == expresion.length() - 1){
			next = '\0';
		}else{
			next = expresion[i + 1];
		}
		if (((tmp != '(' && tmp != '.' && tmp != '|') || tmp == ')' || tmp == '*') && (next != ')' && next != '*' && next != '|' && next != '.' && next != '\0')){
			infix = infix + tmp + '.';
		}else{
			infix = infix + tmp;
		}
	}
	return infix;
}

string Analizar_simbolo_antes(string infix)
{
	stack<char> op;
	string suffix;
	for (int i = 0; i < infix.length(); i++)
	{
		char tmp = infix[i];
		if (tmp == '(')
		{
			op.push(tmp);
		}
		else if (tmp == ')')
		{
			while (op.top() != '(')
			{
				suffix = suffix + op.top();
				op.pop();
			}
			op.pop();
		}
		else if (tmp == '*' || tmp == '.' || tmp == '|')
		{
			while (!op.empty() && op.top() != '(' && simbolo_estatico(tmp) <= simbolo_estatico(op.top()))
			{
				suffix = suffix + op.top();
				op.pop();
			}
			op.push(tmp);
		}
		else
		{
			suffix = suffix + tmp;
		}
	}
	while (!op.empty())
	{
		suffix = suffix + op.top();
		op.pop();
	}
	return suffix;
}

void Cargar_transicion(string suffix)
{
	vector<estructura>().swap(nfa);
	int ID = -1;
	stack<int> beginst;
	stack<int> endst;

	int contador = 0;
	for (int i = 0; i < suffix.length(); i++)
	{
		char tmp = suffix[i];
		if (tmp >= 97 && tmp <= 122)
		{
			ID++;
			estructura e;
			e.tt = tmp;
			e.id_inicio = ID;
			beginst.push(ID);
			ID++;
			e.fin_id = ID;
			nfa.push_back(e);
			endst.push(ID);
		}
		else if (tmp == '|')
		{
			ID++;
			estructura e1;
			estructura e2;
			estructura e3;
			estructura e4;
			e1.tt = e2.tt = e3.tt = e4.tt = 'e';
			e1.id_inicio = e2.id_inicio = ID;
			e2.fin_id = beginst.top();
			beginst.pop();
			e1.fin_id = beginst.top();
			beginst.pop();
			e4.id_inicio = endst.top();
			endst.pop();
			e3.id_inicio = endst.top();
			endst.pop();
			beginst.push(ID);
			ID++;
			endst.push(ID);
			e3.fin_id = e4.fin_id = ID;
			nfa.push_back(e1);
			nfa.push_back(e2);
			nfa.push_back(e3);
			nfa.push_back(e4);
		}
		else if (tmp == '.')
		{
			int temp = endst.top();
			endst.pop();
			for (int i = 0; i < nfa.size(); ++i)
				if (nfa[i].id_inicio == beginst.top())
					nfa[i].id_inicio = endst.top();
			beginst.pop();
			endst.pop();
			endst.push(temp);
		}

		else if (tmp == '*')
		{
			ID++;
			estructura e1;
			estructura e2;
			estructura e3;
			estructura e4;
			e1.tt = e2.tt = e3.tt = e4.tt = 'e';
			e1.id_inicio = ID;
			e3.id_inicio = ID;
			e1.fin_id = beginst.top();
			e4.fin_id = beginst.top();
			beginst.pop();
			e2.id_inicio = endst.top();
			e4.id_inicio = endst.top();
			endst.pop();
			beginst.push(ID);
			ID++;
			endst.push(ID);
			e2.fin_id = ID;
			e3.fin_id = ID;
			nfa.push_back(e1);
			nfa.push_back(e2);
			nfa.push_back(e3);
			nfa.push_back(e4);
		}
		else
		{
			cout << "error" << endl;
		}
	}
	

	sort(nfa.begin(), nfa.end(), [](estructura a, estructura b)
		{ return a.id_inicio < b.id_inicio; });
	gotoxy(20, 0); cout << "INICIO AUTOMATA FINITO NO DETERMINISTA";
	gotoxy(20, 1); cout << "--------------------------------------";
	gotoxy(20, 2); cout << "|id_inicio ";
	gotoxy(35, 2); cout << "|id_transicion";
	gotoxy(50, 2); cout << "|id_fin |";
	 
	fout << "<section class = \"table__body\">" << endl;
	fout << "<table>" << endl;
	fout << "<thead>\n<tr>" << endl;
	fout << "<th>Contador<span class = \"icon-arrow\">&UpArrow; </span></th>" << endl;
	fout << "<th>EstadoInicio <span class = \"icon-arrow\">&UpArrow; </span></th>" << endl;
	fout << "<th>Transicion <span class = \"icon - arrow\">&UpArrow; </span></th>" << endl;
	fout << "<th>EstadoFin<span class = \"icon-arrow\">&UpArrow; </span></th>" << endl;
	fout << "</tr>\n</thead><tbody>" << endl;

	
	int a = 3, b = 3, c = 3, d = 3;
	for (int i = 0; i < nfa.size(); i++)
	{
		contador++;
		fout << "<tr>\n<td>" << contador << "</td>\n" << "\n<td>" <<nfa[i].id_inicio << "</td>" << "\n<td>" << nfa[i].tt << "</td>" << "\n<td>" << nfa[i].fin_id << "</td>" << "\n</tr>" << endl;

		gotoxy(20, a++); cout << "|" << nfa[i].id_inicio;
		gotoxy(35, b++); cout << "|" << nfa[i].tt;
		gotoxy(50, c++); cout << "|" << nfa[i].fin_id;
		gotoxy(58, d++); cout << "|";
		//cout << nfa[i].id_inicio << "\t" << nfa[i].tt << "\t" << nfa[i].fin_id << endl;
	}
	cout << "\n\t\t    --------------------------------------\n\n";
	fout << "</tbody></table>" << endl;
	 
}

int findbegin() {
	int id_inicio = -1;
	for (int i = 0; i < nfa.size(); i++) {
		int flag = 0;
		for (int j = 0; j < nfa.size(); j++) {
			if (nfa[i].id_inicio == nfa[j].fin_id) {
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			id_inicio = nfa[i].id_inicio;
			break;
		}
	}
	return id_inicio;
}

int findend() {
	int fin_id = -1;
	for (int i = 0; i < nfa.size(); i++) {
		int flag = 0;
		for (int j = 0; j < nfa.size(); j++) {
			if (nfa[i].fin_id == nfa[j].id_inicio) {
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			fin_id = nfa[i].fin_id;
			break;
		}
	}
	return fin_id;
}

int main() {
	//ABRIR EL ARCHIVO PARA LA EXPRESION REGULAR
	string direccion_archivo;
	cout << "Ingrese la direccion del archivo: ";
    getline(cin, direccion_archivo);

	ifstream fin(direccion_archivo.c_str());

	if (!fin) {
		cout << "Error al abrir el archivo :(" << endl;
		return 1;
	}
	int n = 0;
	while (!fin.eof()) {
		n++;
		string expresion;
		fin >> expresion;
		if (fin.fail())
			break;
		if (letra_correcta(expresion)) {
			string infix = Analizar_simbolo(expresion);
			string suffix = Analizar_simbolo_antes(infix);
			Cargar_transicion(suffix);
		

			fout << "<h6>Expresion regular: " << expresion << "</h6>" << endl;
			cout << "\t\t\tExpresion Regular: " << expresion << "" << endl;

			fout << "<h6>Expresion de sufijo: " << suffix << "</h6>" << endl;
			cout << "\t\t\tExpresion de sufijo:" << suffix << "" << endl;
			fout << "<h6>Comienzo del estado: " << findbegin() << "</h6>" << endl;
			cout << "\t\t\tComienzo del estado: " << findbegin() << "" << endl;
			fout << "<h6>Fin del estado: " << findend() << "</h6>" << endl;
			cout << "\t\t\tFin del estado: " << findend() << "" << endl;

			fout << "<h4>AFND</h4>" << endl;
			cout << "\t\t\tAFND:" << endl;
			fout << " <pre class=\"mermaid\"> \nflowchart LR\n";
			//CREACION DE LA GRAFICA DEL AUTOMATA
			for (int i = 0; i < nfa.size(); i++)
			{
				if (nfa[i].id_inicio == findbegin())
				{


					fout << nfa[i].id_inicio << "((" << nfa[i].id_inicio << ":Inicio))--" << nfa[i].tt << "-->" << nfa[i].fin_id << "((" << nfa[i].fin_id << "))" << endl;

					cout << "\t\t\t" << nfa[i].id_inicio << "--" << nfa[i].tt << "-->" << nfa[i].fin_id << endl;
				}
				else if (nfa[i].fin_id == findend())

				{

					fout << nfa[i].id_inicio << "((" << nfa[i].id_inicio << "))--" << nfa[i].tt << "-->" << nfa[i].fin_id << "(" << nfa[i].fin_id << ":Fin)" << endl;
					cout << "\t\t\t" << nfa[i].id_inicio << "--" << nfa[i].tt << "-->" << nfa[i].fin_id << endl;
				}
				else
				{

					fout << nfa[i].id_inicio << "((" << nfa[i].id_inicio << "))--" << nfa[i].tt << "-->" << nfa[i].fin_id << "((" << nfa[i].fin_id << "))" << endl;
					cout << "\t\t\t" << nfa[i].id_inicio << "--" << nfa[i].tt << "-->" << nfa[i].fin_id << endl;
				}
			}

		}
		else
		{

			fout << "<h6>Expresion Regular: " << expresion << " no encontrada</h6>" << endl;
			cout << "\t\t\tExpresion Regular: " << expresion << " no encontrada" << endl;
		}
	}


	fout.close();
	fin.close();

	Operaciones_archivos op;

	int retorno = op.InsertarHTML("AFND.html", "header.txt", "body.txt", "footer.txt");
	if (retorno == 1) {
		cout << "\n\t\t\t" << "GRAFICA AFND CREADO CORRECTAMENTE" << endl;
	}

	

	inicio(direccion_archivo);

	int retorno2 = op.InsertarHTML("AFD.html", "header.txt", "body2.txt", "footer.txt");
	if (retorno2 == 1) {
		cout << "\n\t\t\t" << "GRAFICA AFD CREADO CORRECTAMENTE" << endl;
	}
	return 0;
}