#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>
#include <set>
#include <algorithm>
#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <list>
#include <map>
#include <queue>
#include <sstream>
#include <utility>
#include "GraficaHTML.h"

#define pb push_back
#define mp make_pair
#define pi acos(-1.0)
#define ff first
#define LL long long
#define ss second
#define rep(i,n) for(i = 0; i < n; i++)
#define REP(i,n) for(i=n;i>=0;i--)
#define FOR(i,a,b) for(int i = a; i <= b; i++)
#define ROF(i,a,b) for(int i = a; i >= b; i--)
#define re return
#define QI queue<int>
#define SI stack<int>
#define pii pair <int,int>
#define MAX
#define MOD
#define INF 1<<30
#define SZ(x) ((int) (x).size())
#define ALL(x) (x).begin(), (x).end()
#define sqr(x) ((x) * (x))
#define memo(a,b) memset((a),(b),sizeof(a))
#define G() getchar()
#define MAX3(a,b,c) max(a,max(b,c))

double const EPS = 3e-8;
using namespace std;
string Obtener_abc(int id);


template< class T > T gcd(T a, T b) { return (b != 0 ? gcd<T>(b, a % b) : a); }
template< class T > T lcm(T a, T b) { return (a / gcd<T>(a, b) * b); }
template <class T> inline T bigmod(T p, T e, T M)
{
	if (e == 0)return 1;
	if (e % 2 == 0) { LL t = bigmod(p, e / 2, M); return (T)((t * t) % M); }
	return (T)((LL)bigmod(p, e - 1, M) * (LL)p) % M;
}

template <class T> inline T modinverse(T a, T M) { return bigmod(a, M - 2, M); }
//Fast Reader
template<class T>inline bool read(T& x) { int c = getchar(); int sgn = 1; while (~c && c < '0' || c>'9') { if (c == '-')sgn = -1; c = getchar(); }for (x = 0; ~c && '0' <= c && c <= '9'; c = getchar())x = x * 10 + c - '0'; x *= sgn; return ~c; }

map<string, int> convertir;
map<string, int> convertir2;

vector<string> pos1;
vector<string> pos2;
vector<string> pos3;
void CrearBod();


int numRegEx;
int leaf; // count number of Alphabet + '#' ;
string inpstring;
set<int>followPos[505];
map < char, int > Highrank;
map < int, char > who;
map < set<int>, bool > visit;
int deg[550];
int TablaSimbolos[130][200];
vector<string> myvector;

struct Nodo
{
	Nodo* izquierdo, * derecho;
	char datos;
	set <int> primeraPos, segundaPos;
	bool nullable;
	Nodo()
	{   // sob kisu 1st e clear ar null dite initail kore rakhchi
		izquierdo = NULL;
		derecho = NULL;
		datos = '\0';
		primeraPos.clear();
		segundaPos.clear();
	}
};
Nodo* root, * nd;
bool alfabeto(char x)
{
	if ((x >= 'A' && x <= 'Z') || (x >= 'a' && x <= 'z')) return 1;
	else return 0;
}
bool operador(char x) {
	return (x == '*' || x == '+' || x == '.');
}

set<int> setUnion(set<int >a, set<int >b) {
	set<int>c;
	c.insert(a.begin(), a.end());
	c.insert(b.begin(), b.end());
	return c;
}


string EntradaCadena(string str) {

	string st = "";
	string aff = "";
	st += str[0];

	for (int i = 1; i < str.size(); i++) {

		if ((isalpha(str[i]) || str[i] == '(') && isalpha(str[i - 1])) {
			st += '.', st += str[i];

		}
		else if (str[i - 1] == '*' && (str[i] == '(' || alfabeto(str[i]))) {
			st += '.';
			st += str[i];
		}
		else st += str[i];

	}
	// cout << "es std " << aff << endl;
	cout << "str " << st << endl;
	return st;
}
string infixtopostfix(string str)
{
	stack<char>st;
	int len = str.length(), i;
	string postfix = "";
	Highrank['|'] = 0;
	Highrank['.'] = 1;
	Highrank['*'] = 2;

	Highrank['('] = -1; // lowest
	rep(i, len) {

		if (alfabeto(str[i]) || str[i] == '*') postfix += str[i];
		else if (str[i] == '(') st.push(str[i]);
		else if (str[i] == ')') {
			while (!st.empty() && st.top() != '(') {
				char ch = st.top();
				st.pop();
				postfix += ch;
				cout << ch << endl;
			}
			if (!st.empty())
				st.pop();
		}
		else if (st.empty() || Highrank[str[i]] > Highrank[st.top()]) st.push(str[i]);
		else if (!st.empty() && Highrank[st.top()] >= Highrank[str[i]]) {
			while (!st.empty() && (Highrank[st.top()] >= Highrank[str[i]])) {
				postfix += st.top();
				st.pop();
			}
			st.push(str[i]);

		}
		else {
			cout << ("Error en la cadena de entrada\n");

		}

	}
	while (!st.empty()) {
		char ch = st.top();
		if (ch != '(')
			postfix += ch;

		st.pop();
	}
	postfix += "#.";
	cout << "PostFix expresion: " << postfix << endl;
	return postfix;
}
void Syntex_Tree(string PostFix)
{
	int i, sz = PostFix.size();
	set <int> ::iterator it;
	stack <Nodo*> stk;
	rep(i, sz) {
		nd = new Nodo();
		nd->datos = PostFix[i];
		if (alfabeto(PostFix[i]) || PostFix[i] == '#') {
			nd->izquierdo = NULL;
			nd->derecho = NULL;
			stk.push(nd);
		}
		else {

			if (stk.size() >= 2) {
				nd->derecho = stk.top();
				stk.pop();
				nd->izquierdo = stk.top();
				stk.pop();
			}
			else if (stk.size() == 1) {
				nd->izquierdo = stk.top();
				nd->derecho = NULL;
				stk.pop();
			}
			stk.push(nd);

		}

	}
	root = stk.top();
}
int cnt = 0;
void firstsegundaPosCal(Nodo* N)
{
	if (N->datos == NULL) return;
	cnt++;
	cout << cnt << " " << N->datos << endl;
	set <int> ::iterator it1, it2;
	if (N->izquierdo != NULL) firstsegundaPosCal(N->izquierdo);

	if (N->derecho != NULL) firstsegundaPosCal(N->derecho);

	if ((N->datos == '+' || N->datos == '|') && N->izquierdo != NULL) {

		N->nullable = N->izquierdo->nullable || N->derecho->nullable;
		N->primeraPos = setUnion(N->izquierdo->primeraPos, N->derecho->primeraPos);
		N->segundaPos = setUnion(N->derecho->segundaPos, N->derecho->segundaPos);
	}
	else if ((N->datos == '.') && (N->izquierdo != NULL && N->derecho != NULL)) {
		N->nullable = N->izquierdo->nullable && N->derecho->nullable;
		if (N->izquierdo->nullable)
			N->primeraPos = setUnion(N->izquierdo->primeraPos, N->derecho->primeraPos);
		else
			N->primeraPos = N->izquierdo->primeraPos;
		if (N->derecho->nullable)
			N->segundaPos = setUnion(N->izquierdo->segundaPos, N->derecho->segundaPos);
		else
			N->segundaPos = N->derecho->segundaPos;
		// Follow Position calculation
		for (it1 = N->izquierdo->segundaPos.begin(); it1 != N->izquierdo->segundaPos.end(); it1++)
		{
			for (it2 = N->derecho->primeraPos.begin(); it2 != N->derecho->primeraPos.end(); it2++)
			{
				followPos[*it1].insert(*it2);
			}
		}
	}
	else if ((N->datos == '*') && (N->izquierdo != NULL))
	{

		N->nullable = true;
		N->primeraPos = N->izquierdo->primeraPos;
		N->segundaPos = N->izquierdo->segundaPos;
		for (it1 = N->izquierdo->segundaPos.begin(); it1 != N->izquierdo->segundaPos.end(); it1++)
		{
			for (it2 = N->izquierdo->primeraPos.begin(); it2 != N->izquierdo->primeraPos.end(); it2++)
			{
				followPos[*it1].insert(*it2);
			}
		}

	}
	else {
		N->nullable = false;
		leaf++;
		N->primeraPos.insert(leaf);
		N->segundaPos.insert(leaf);
		who[leaf] = N->datos;
		TablaSimbolos[N->datos][deg[N->datos]++] = leaf;


	}


}
void printTablaSimbolos() {
	int i, j;
	printf("\n\n\t Tabla de simbolos\n\n");
	printf("     simbolo     posicion\n_______________________________\n");
	for (i = 'a'; i <= 'z'; i++)
	{
		if (deg[i])
		{
			printf("\t%c\t", (char)i);
			for (j = 0; j < deg[i]; j++)
			{
				printf(" %d", TablaSimbolos[i][j]);
			}
			printf("\n_______________________________\n");
		}
	}
}
void printFollowTable()
{
	printf("\n\n\t Tabla siguiente\n\n");
	printf("\tNodo      posicion\n_______________________________\n");
	set <int> ::iterator it;
	for (it = root->primeraPos.begin(); it != root->primeraPos.end(); it++)
	{
		followPos[1].insert(*it);

	}
	int i;
	for (i = 1; i <= 200; i++)
	{
		if (followPos[i].size())
		{
			// printf("\t%d\t ", i);

			cout << "\t" << i << "\t";
			for (it = followPos[i].begin(); it != followPos[i].end(); it++)
			{
				cout << " ," << (*it);
			}

			cout << "\n_______________________________\n";
		}
	}
}
void DFD()
{
	printf("\n\n\t\t DFA Construction\n\n");

	int i, j;
	printf("\n____________________________________________________________\n");
	printf("\t  Nodo ");
	set <int> u, v, add, x;
	vector <int> sym;
	for (i = 'a'; i <= 'z'; i++) if (deg[i]) {
		printf("\t\t  %c ", i);
		//	cout <<"\t\t "<< to_char(i);
		sym.pb(i);
	}
	printf("\n______________________________________________________________\n");
	queue < set<int> > q;

	set <int> ::iterator it;
	q.push(root->primeraPos);
	visit[root->primeraPos] = true;
	bool space = true;
	int contador = 0, contador2 = 0;
	string lleva, lleva2;
	while (!q.empty())
	{
		contador++;
		u = q.front();
		q.pop();
		map < int, bool > freq;
		printf("\t");
		for (it = u.begin(); it != u.end(); it++)
		{

			cout << " ." << (*it);
			//lleva = "";

			freq[(*it)] = true;
			lleva += to_string(*it);

		}
		convertir[lleva] = contador;


		if (space)
			printf("\t");
		space = false;
		rep(i, sym.size())
		{
			vector <int > a;
			for (j = 0; j < deg[sym[i]]; j++)
			{
				if (freq[TablaSimbolos[sym[i]][j]])
				{
					a.pb(TablaSimbolos[sym[i]][j]);
				}
			}
			x.clear();
			for (j = 0; j < a.size(); j++)
			{
				x = setUnion(x, followPos[a[j]]);
			}
			if (x.size())
			{
				contador2++;
				printf("\t");
				lleva2 = "";
				for (it = x.begin(); it != x.end(); it++)
				{
					cout << " " << (*it);
					lleva2 += to_string(*it);
				}

				convertir2[lleva2] = contador2;

				myvector.push_back(lleva2);

				if (visit[x] == false)
				{
					q.push(x);
					visit[x] = true;
				}
			}
			else
			{
				printf("\tno");
			}

			lleva = "";

		}
		lleva2 = "";
		printf("\n_______________________________________________________________\n");


	}
}


string Obtener_abc(int id) {
	int numero = id;
	char vec[] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' };
	string salida;

	if (numero > 0 && numero <= 26) {
		//cout << vec[numero - 1];
		salida = vec[numero - 1];
	}
	else {
		cout << "\n ingresar un valor valido";
	}
	return salida;
}


void uno() {
	int i, cs;

	string direccion_archivo, inpstring;
	cout << "Ingrese la direccion del archivo: ";
	getline(cin, direccion_archivo);
	ifstream fin(direccion_archivo.c_str());
	while (getline(fin, inpstring)) {
		// Lo vamos imprimiendo
	//	cout << "Una línea: ";*/
		cout <<" 1 "<< inpstring << endl;
	}

	

	cout << "Inicio cadena: " << inpstring << endl;
	string infix = EntradaCadena(inpstring);
	cout << "soy infix: " << infix << endl;
	string postfix = infixtopostfix(infix);
	cout << "soy posfix: " << postfix << endl;
	// creating syntext tree
	Syntex_Tree(postfix);
	// follow position calculation
	firstsegundaPosCal(root);
	printFollowTable();
	printTablaSimbolos();

	DFD();

	for (auto it2 = convertir.rbegin(); it2 != convertir.rend(); it2++) {
		string abce = Obtener_abc(it2->second);
		cout << "{" << abce << " ";
		pos1.push_back(abce);
	}

	cout << endl << endl;
	for (int a = 0; a < myvector.size(); a++) {
		for (auto it = convertir.rbegin(); it != convertir.rend(); it++) {
			string abce = Obtener_abc(it->second);

			if (myvector[a] == it->first) {

				if (a % 2 == 0) {

					pos2.push_back(abce);
				}
				else {

					pos3.push_back(abce);
				}
			}
		}
	}




	cout << endl << endl;

	cout << "TABLA DE TRANSICIONES " << endl;

	cout << endl;
	for (int i = 0; i < pos1.size(); i++) {
		cout << "\t\t\t" << i << ") " << pos1[i] << " --> con " << "a" << " --> " << pos2[i] << endl;
		cout << "\t\t\t" << i << ") " << pos1[i] << " --> con " << "b" << " --> " << pos3[i] << endl;
		//cout << pos1[i] << " " << pos2[i] << " " << pos3[i] << endl;
	}

	CrearBod();
}

void CrearBod() {

	ofstream fout("body.txt", ios::out);


	fout << "<section class = \"table__body\">" << endl;
	fout << "<table>" << endl;
	fout << "<tnd>\n<tr>" << endl;
	fout << "<th>Contador<span class = \"icon-arrow\">&UpArrow; </span></th>" << endl;
	fout << "<th>Estado <span class = \"icon-arrow\">&UpArrow; </span></th>" << endl;
	fout << "<th>transicion <span class = \"icon - arrow\">&UpArrow; </span></th>" << endl;
	fout << "<th>Estado<span class = \"icon-arrow\">&UpArrow; </span></th>" << endl;
	fout << "</tr>\n</tnd><tbody>" << endl;
	for (int i = 0; i < pos1.size(); i++) {
		cout << "\t\t\t" << i << ") " << pos1[i] << " --> con " << "a" << " --> " << pos2[i] << endl;
		cout << "\t\t\t" << i << ") " << pos1[i] << " --> con " << "b" << " --> " << pos3[i] << endl;
		fout << "<tr>\n<td>" << i << "</td>\n" << "\n<td>" << pos1[i] << "</td>" << "\n<td>" << "a" << "</td>" << "\n<td>" << pos2[i] << "</td>" << "\n</tr>" << endl;
		fout << "<tr>\n<td>" << i << "</td>\n" << "\n<td>" << pos1[i] << "</td>" << "\n<td>" << "b" << "</td>" << "\n<td>" << pos3[i] << "</td>" << "\n</tr>" << endl;

		//cout << pos1[i] << " " << pos2[i] << " " << pos3[i] << endl;
	}

	fout << "</tbody></table>" << endl;
	//CREANDO LA GRAFICA 
	fout << "<br><br><h5>Grafica AFD</h4>";
	fout << " <pre class=\"mermaid\"> \nflowchart LR\n";
	cout << "\n\t\t\t-- TRANSICIONES DFA --" << endl;

	for (int i = 0; i < pos1.size(); i++) {
		fout << pos1[i] << "((" << pos1[i] << "))--" << "a" << "-->" << pos2[i] << "((" << pos2[i] << "))" << endl;
		fout << pos1[i] << "((" << pos1[i] << "))--" << "b" << "-->" << pos3[i] << "((" << pos3[i] << "))" << endl;
		//cout << pos1[i] << " " << pos2[i] << " " << pos3[i] << endl;
	}
	fout.close();

	Operaciones_archivos op;
	int retorno = op.InsertarHTML("AFD.html", "header.txt", "body.txt", "footer.txt");
	if (retorno == 1) {
		cout << "\n\t\t\t" << "GRAFICA AFND CREADO CORRECTAMENTE" << endl;
	}

}
