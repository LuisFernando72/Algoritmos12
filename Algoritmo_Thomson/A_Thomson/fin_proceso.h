
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



using namespace std;
void gotoxy(int x, int y)
{
	HANDLE hcon;
	hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	SetConsoleCursorPosition(hcon, dwPos);

}


void inicio();
char Obtener_abc(int id);
string AbrirArchivo();


map<string, int> mapa;

struct edge1
{
	int cod_inicio = -1;
	char trans;
	int cod_fin = -1;
};

struct estado1
{
	int ID = -1;
	char trans;
	int nuevo_cod = -1;
	vector<int> vec_cierre;
	bool terminal = false;
};


vector<edge1> nfaa;
vector<estado1> dfaa;
bool E_permitido(string s)
{
	stack<char> bracket;
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] >= 'a' && s[i] <= 'z') // If letter
			continue;
		else if (s[i] == '|' || s[i] == '*' || s[i] == '.') // If operator
			continue;
		else if (s[i] == '(') // Check if the brackets are matched
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

int procS(char c)
{
	if (c == '*')
	{
		return 3;
	}
	else if (c == '.')
	{
		return 2;
	}
	else if (c == '|')
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

string evaluar(string expression)
{
	string infix;
	for (int i = 0; i < expression.length(); i++)
	{
		char tmp = expression[i];
		char next;
		if (i == expression.length() - 1)
		{
			next = '\0';
		}
		else
		{
			next = expression[i + 1];
		}
		if (((tmp != '(' && tmp != '.' && tmp != '|') || tmp == ')' || tmp == '*') && (next != ')' && next != '*' && next != '|' && next != '.' && next != '\0'))
		{
			infix = infix + tmp + '.';
		}
		else
		{
			infix = infix + tmp;
		}
	}
	return infix;
}

string evaluar_des(string infix) {
	stack<char> op;
	string suf;
	for (int i = 0; i < infix.length(); i++) {
		char tmp = infix[i];
		if (tmp == '(') {
			op.push(tmp);
		}else if (tmp == ')') {
			while (op.top() != '(')
			{
				suf = suf + op.top();
				op.pop();
			}
			op.pop();
		}else if (tmp == '*' || tmp == '.' || tmp == '|') {
			while (!op.empty() && op.top() != '(' && procS(tmp) <= procS(op.top())) {
				suf = suf + op.top();
				op.pop();
			}
			op.push(tmp);
		}else {
			suf = suf + tmp;
		}
	}while (  !op.empty()) {
		suf = suf + op.top();
		op.pop();
	}
	return suf;
}

vector<char> obtener_alfabeto(string a) {
	vector<char> alphabet;
	for (int i = 0; i < a.length(); i++) {
		if (a[i] >= 'a' && a[i] <= 'z') {
			alphabet.push_back(a[i]);
		}
	}
	sort(alphabet.begin(), alphabet.end());
	alphabet.erase(unique(alphabet.begin(), alphabet.end()), alphabet.end());
	cout << "\n\t\t\tAlfabeto:";
	for (int i = 0; i < alphabet.size(); i++) {
		cout << alphabet[i] << " ";
	}
	cout << endl;
	return alphabet;
}

void Insertar_datos(string suf) {
	vector<edge1>().swap(nfaa);
	int ID = -1;
	stack<int> beginst;
	stack<int> endst;
	for (int i = 0; i < suf.length(); i++) {
		char tmp = suf[i];
		if (tmp >= 97 && tmp <= 122) {
			ID++;
			edge1 e;
			e.trans = tmp;
			e.cod_inicio = ID;
			beginst.push(ID);
			ID++;
			e.cod_fin = ID;
			nfaa.push_back(e);
			endst.push(ID);
		}
		else if (tmp == '|') {
			ID++;
			edge1 e1;
			edge1 e2;
			edge1 e3;
			edge1 e4;
			e1.trans = e2.trans = e3.trans = e4.trans = '=';
			e1.cod_inicio = e2.cod_inicio = ID;
			e2.cod_fin = beginst.top();
			beginst.pop();
			e1.cod_fin = beginst.top();
			beginst.pop();
			e4.cod_inicio = endst.top();
			endst.pop();
			e3.cod_inicio = endst.top();
			endst.pop();
			beginst.push(ID);
			ID++;
			endst.push(ID);
			e3.cod_fin = e4.cod_fin = ID;
			nfaa.push_back(e1);
			nfaa.push_back(e2);
			nfaa.push_back(e3);
			nfaa.push_back(e4);
		}
		else if (tmp == '.') {
			// ID++;
			edge1 e1;
			e1.trans = '=';
			e1.cod_fin = beginst.top();
			beginst.pop();
			int temp = endst.top();
			endst.pop();
			e1.cod_inicio = endst.top();
			endst.pop();
			endst.push(temp);
			nfaa.push_back(e1);
		}
		else if (tmp == '*')
		{
			ID++;
			edge1 e1;
			edge1 e2;
			edge1 e3;
			edge1 e4;
			e1.trans = e2.trans = e3.trans = e4.trans = '=';
			e1.cod_inicio = ID;
			e3.cod_inicio = ID;
			e1.cod_fin = beginst.top();
			e4.cod_fin = beginst.top();
			beginst.pop();
			e2.cod_inicio = endst.top();
			e4.cod_inicio = endst.top();
			endst.pop();
			beginst.push(ID);
			ID++;
			endst.push(ID);
			e2.cod_fin = ID;
			e3.cod_fin = ID;
			nfaa.push_back(e1);
			nfaa.push_back(e2);
			nfaa.push_back(e3);
			nfaa.push_back(e4);
		}
		else {
			cout << "\t\t\tError" << endl;
		}
	}
	sort(nfaa.begin(), nfaa.end(), [](edge1 a, edge1 b)
		{ return a.cod_inicio < b.cod_inicio; });

	cout << "\n\t\t\tAUTOMATA FINITO DETERMINISTA" << endl;
	cout << "\t\t\tcod_inicio\ttransi\tcod_fin" << endl;
	for (int i = 0; i < nfaa.size(); i++)
	{
		cout << "\t\t\t" << nfaa[i].cod_inicio << "\t" << nfaa[i].trans << "\t" << nfaa[i].cod_fin << endl;
	}
}

int finEmpezar() {
	int cod_inicio = -1;
	for (int i = 0; i < nfaa.size(); i++) {
		int flag = 0;
		for (int j = 0; j < nfaa.size(); j++) {
			if (nfaa[i].cod_inicio == nfaa[j].cod_fin) {
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			cod_inicio = nfaa[i].cod_inicio;
			break;
		}
	}
	return cod_inicio;
}

int fin_final() {
	int cod_fin = -1;
	for (int i = 0; i < nfaa.size(); i++) {
		int flag = 0;
		for (int j = 0; j < nfaa.size(); j++) {
			if (nfaa[i].cod_fin == nfaa[j].cod_inicio) {
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			cod_fin = nfaa[i].cod_fin;
			break;
		}
	}
	return cod_fin;
}

int dfaadd(vector<int>& clos, char tran, int a1) {
	for (int i = 0; i < nfaa.size(); i++) {
		if (nfaa[i].cod_inicio == a1 && (nfaa[i].trans == tran || nfaa[i].trans == '=')) {
			clos.push_back(nfaa[i].cod_fin);
			dfaadd(clos, tran, nfaa[i].cod_fin);
		}
	}
	return 0;
}

vector<int> cierre(int a) {
	vector<int> v;
	v.push_back(a);
	for (int i = 0; i < nfaa.size(); i++) {
		if (nfaa[i].cod_inicio == a && nfaa[i].trans == '=') {
			v.push_back(nfaa[i].cod_fin);
		}
	}
	for (int i = 0; i < v.size(); i++) {
		for (int j = 0; j < nfaa.size(); j++) {
			if (nfaa[j].cod_inicio == v[i] && nfaa[j].trans == '=') {
				v.push_back(nfaa[j].cod_fin);
			}
		}
	}
	sort(v.begin(), v.end());
	v.erase(unique(v.begin(), v.end()), v.end());
	return v;
}

vector<int> mover(vector<int> v, char a) {
	vector<int> vec_cierre;
	// vec_cierre.push_back(1000);
	for (int i = 0; i < v.size(); i++) {
		for (int j = 0; j < nfaa.size(); j++) {
			if (v[i] == nfaa[j].cod_inicio && nfaa[j].trans == a) {
				vec_cierre.push_back(nfaa[j].cod_fin);
			}
		}
	}
	if (!vec_cierre.empty()) {
		cout << "\t\t\tmover(";
		for (int i = 0; i < v.size(); i++) {
			cout << v[i] << ",";
		}
		cout << a << ")={";
		for (int i = 0; i < vec_cierre.size() - 1; i++) {
			cout << vec_cierre[i] << ",";
		}
		cout << vec_cierre[vec_cierre.size() - 1] << "}" << endl;
	}
	else {
		cout << "\t\t\tmover(";
		for (int i = 0; i < v.size(); i++) {
			cout << v[i] << ",";
		}
		cout << a << ")={ }" << endl;
	}
	return vec_cierre;
}

vector<int> cierre_mover(vector<int> v) {
	vector<int> v1;
	vector<int> v2;
	for (int i = 0; i < v.size(); i++) {
		v1 = cierre(v[i]);
		v2.insert(v2.end(), v1.begin(), v1.end());
	}
	sort(v2.begin(), v2.end());
	v2.erase(unique(v2.begin(), v2.end()), v2.end());
	cout << "\t\t\tcierre_mover(";
	for (int i = 0; i < v.size() - 1; i++) {
		cout << v[i] << ",";
	}
	cout << v[v.size() - 1];
	cout << ")={";
	for (int i = 0; i < v2.size() - 1; i++) {
		cout << v2[i] << ",";
	}
	cout << v2[v2.size() - 1] << "}" << endl;
	return v2;
}

int inicio(vector<int> v) {
	for (int i = 0; i < dfaa.size(); i++) {
		if (v == dfaa[i].vec_cierre) {
			return i;
		}
	}
	return 0;
}



void Creardfa(vector<edge1> nfaa, string s1) {
	for (int i = 0; i < nfaa.size(); i++) {
		cout << "\t\t\tcierre(" << nfaa[i].cod_inicio << ")={";
		vector<int> v1;
		v1 = cierre(nfaa[i].cod_inicio);
		for (int j = 0; j < v1.size() - 1; j++) {
			cout << v1[j] << ",";
		}
		cout << v1[v1.size() - 1] << "}" << endl;
	}

	vector<char> alphabet = obtener_alfabeto(s1);
	estado1 d1;
	int cod_inicio = finEmpezar();
	d1.ID = 0;
	d1.nuevo_cod = 1;
	d1.trans = 'N';
	d1.vec_cierre.push_back(cod_inicio);
	dfaadd(d1.vec_cierre, '=', cod_inicio);
	std::sort(d1.vec_cierre.begin(), d1.vec_cierre.end());
	dfaa.push_back(d1);
	queue<estado1> q;
	q.push(d1);
	vector<estado1> dumplicate;

	while (!q.empty()) {
		estado1 s = q.front();
		estado1 temp;
		for (int i = 0; i < alphabet.size(); i++) {
			vector<int> v1 = mover(s.vec_cierre, alphabet[i]);
			vector<int> v2 = cierre_mover(v1);
			if (!inicio(v2)) {
				temp.vec_cierre = v2;
				temp.ID = s.nuevo_cod;
				temp.nuevo_cod = dfaa.size() + 1;
				temp.trans = alphabet[i];
				dfaa.push_back(temp);
				q.push(temp);
			}
			else {
				temp.vec_cierre = v2;
				temp.ID = s.nuevo_cod;
				temp.nuevo_cod = inicio(v2) + 1;
				temp.trans = alphabet[i];
				dumplicate.push_back(temp);
			}
		}
		q.pop();
	}
	int cod_fin = fin_final();
	for (int i = 0; i < dfaa.size(); i++) {
		if (count(dfaa[i].vec_cierre.begin(), dfaa[i].vec_cierre.end(), cod_fin)) {
			dfaa[i].terminal = true;
		}
	}

	dfaa.insert(dfaa.end(), dumplicate.begin(), dumplicate.end());
	sort(dfaa.begin(), dfaa.end(), [](estado1 a, estado1 b)
		{ return a.nuevo_cod < b.nuevo_cod; });
	for (int i = 0; i < dfaa.size(); i++) {
		if (count(dfaa[i].vec_cierre.begin(), dfaa[i].vec_cierre.end(), cod_fin)) {
			dfaa[i].terminal = true;
		}
	}

	ofstream file;
	file.open("crear.txt");


	cout << "\n\t\t\tAUTOMATA DETERMINISTA TRANSICIONES" << endl;
	cout << "\t\t\tID\ttrans\tnuevo_cod\t\tAceptacion\t\tconjuntos" << endl;

	for (int i = 0; i < dfaa.size(); i++){
		if (dfaa[i].terminal == 1) {
			cout << "\t\t\t" << dfaa[i].ID << "\t" << dfaa[i].trans << "\t" << dfaa[i].nuevo_cod << "\t\t   " << "   Aceptado" << "\t\t";

		}else {
			cout << "\t\t\t" << dfaa[i].ID << "\t" << dfaa[i].trans << "\t" << dfaa[i].nuevo_cod << "\t\t   " <<"   Rechazado" << "\t\t";
		}
		
		for (int j = 0; j < dfaa[i].vec_cierre.size(); j++){
			cout << dfaa[i].vec_cierre[j] << " ";


		}

		if (dfaa[i].ID != 0) {

		 
			char id1 = Obtener_abc(dfaa[i].ID);
			char id2 = Obtener_abc(dfaa[i].nuevo_cod);
			file << id1 << "." << dfaa[i].trans << "-" << id2 << endl;

		 

		//CrearGrafo(dfaa[i].lastID, dfaa[i].trans, dfaa[i].nuevo_cod);
		}


		cout << endl;
	}
	file.close();
}

void Eliminar_duplicado(int a, int b)
{
	for (int i = 0; i < dfaa.size(); i++){
		if (dfaa[i].ID == b){
			dfaa[i].ID = a;
		}
		if (dfaa[i].nuevo_cod == b){
			dfaa[i].nuevo_cod = a;
		}
	}
}

bool unique(vector<estado1> v, estado1 s)
{
	for (int i = 0; i < v.size(); i++){
		if (v[i].vec_cierre == s.vec_cierre && v[i].terminal == s.terminal && v[i].ID == s.ID && v[i].nuevo_cod == s.nuevo_cod && v[i].trans == s.trans){
			return false;
		}
	}
	return true;
}




void inicio(string direccion_archivo) {
	ifstream fin(direccion_archivo.c_str());
	if (!fin){
		cout << "Error al abrir el archivo :(" << endl;

	}
	int n = 0;
	while (!fin.eof())
	{
		n++;
		string expression;
		fin >> expression;
		if (fin.fail())
			break;
		if (E_permitido(expression))
		{
			string infix = evaluar(expression);
			string suf = evaluar_des(infix);
			Insertar_datos(suf);
			Creardfa(nfaa, expression);

			AbrirArchivo();
		}
	}
}


char Obtener_abc(int id) {
	int numero = id;
	char vec[] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' };
	char salida;

	if (numero > 0 && numero <= 26) {
		//cout << vec[numero - 1];
		salida = vec[numero - 1];
	}
	else {
		cout << "\n ingresar un valor valido";
	}
	return salida;
}

string AbrirArchivo() {
	string linea, retorno;

	ofstream fout("body2.txt", ios::out);

	char vec1[] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' };

	// Obtener línea de archivo, y almacenar contenido en "linea"
	

	fout << "	<section class = \"table__body\">" << endl;
	fout << "<table>" << endl;
	fout << "<thead>\n<tr>" << endl;
	fout << "<th>Contador<span class = \"icon-arrow\">&UpArrow; </span></th>" << endl;
	fout << "<th>Estado <span class = \"icon-arrow\">&UpArrow; </span></th>" << endl;
	fout << "<th>transicion <span class = \"icon - arrow\">&UpArrow; </span></th>" << endl;
	fout << "<th>Estado<span class = \"icon-arrow\">&UpArrow; </span></th>" << endl;
	fout << "</tr>\n</thead><tbody>" << endl;
	int contador = 0, contador1 =0;

	for (int i = 0; i < 26; i++) {
		string nombreArchivo = "crear.txt";

		string terminald, transicion, no_terminal;
		contador++;
		string casa;
		casa.append(1, vec1[i]);
		ifstream archivo(nombreArchivo.c_str());

		while (getline(archivo, linea)) {
			// Lo vamos imprimiendo
			stringstream input_stringstream(linea);
		
			getline(input_stringstream, no_terminal, '.');
			getline(input_stringstream, transicion, '-');
			getline(input_stringstream, terminald, '-');
			if (casa == no_terminal) {
			
  				fout << "<tr>\n<td>"<<contador<<"</td>\n"<<"\n<td>"<<no_terminal<<"</td>"<<"\n<td>"<<transicion<<"</td>"<<"\n<td>"<<terminald<<"</td>" << "\n</tr>" << endl;
			}

		}
		archivo.close();
	}
	fout << "</tbody></table>" << endl;
 

	//CREANDO LA GRAFICA 
	fout << "<br><br><h5>Grafica AFD</h4>";
	fout << " <pre class=\"mermaid\"> \nflowchart LR\n";
	cout << "\n\t\t\t-- TRANSICIONES DFA --" << endl;
	for (int i = 0; i < 26; i++) {
		string nombreArchivo = "crear.txt";

		string terminald, transicion, no_terminal;
		

		string casa;
		casa.append(1, vec1[i]);
		ifstream archivo(nombreArchivo.c_str());
		contador1++;
		while (getline(archivo, linea)) {
			// Lo vamos imprimiendo
			stringstream input_stringstream(linea);
		
			getline(input_stringstream, no_terminal, '.');
			getline(input_stringstream, transicion, '-');
			getline(input_stringstream, terminald, '-');
			if (casa == no_terminal) {
			
				cout << "\t\t\t" << contador1 << ") " << no_terminal << " --> con " << transicion << " --> " << terminald << endl;
				fout << no_terminal << "((" << no_terminal << "))--" << transicion << "-->" << terminald << "((" << terminald << "))" << endl;


			}

		}
		archivo.close();
	}

	fout.close();
	return ".";

}



