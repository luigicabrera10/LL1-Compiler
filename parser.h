#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include "token.h"

using namespace std;

map<pair<string, string>, vector<string>> table;
map<string, vector<string>> follows;

// inicializa la tabla de parseo y la tabla de follows segun nuestra gramatica
void initTable() {

	// TABLA DE PARSEO
	table[{"bloques", "}"}] = {""};
	table[{"bloques", "\\h1"}] = {"bloque", "bloques"};
	table[{"bloques", "\\h2"}] = {"bloque", "bloques"};
	table[{"bloques", "\\h3"}] = {"bloque", "bloques"};
	table[{"bloques", "text"}] = {"bloque", "bloques"};
	table[{"bloques", "\\bf"}] = {"bloque", "bloques"};
	table[{"bloques", "\\it"}] = {"bloque", "bloques"};
	table[{"bloques", "\\un"}] = {"bloque", "bloques"};
	table[{"bloques", "\\link"}] = {"bloque", "bloques"};
	table[{"bloques", "\\cmt"}] = {"bloque", "bloques"};
	table[{"bloques", "\\list"}] = {"bloque", "bloques"};
	table[{"bloques", "\\tbl"}] = {"bloque", "bloques"};
	table[{"bloques", "\\pic"}] = {"bloque", "bloques"};
	
	table[{"bloque", "\\h1"}] = {"title"};
	table[{"bloque", "\\h2"}] = {"title"};
	table[{"bloque", "\\h3"}] = {"title"};
	table[{"bloque", "text"}] = {"textstyle"};
	table[{"bloque", "\\bf"}] = {"textstyle"};
	table[{"bloque", "\\it"}] = {"textstyle"};
	table[{"bloque", "\\un"}] = {"textstyle"};
	table[{"bloque", "\\link"}] = {"url"};
	table[{"bloque", "\\cmt"}] = {"comentario"};
	table[{"bloque", "\\list"}] = {"enumerate"};
	table[{"bloque", "\\tbl"}] = {"table"};
	table[{"bloque", "\\pic"}] = {"img"};
	
	table[{"title", "\\h1"}] = {"header", "{", "textstyle", "}"};
	table[{"title", "\\h2"}] = {"header", "{", "textstyle", "}"};
	table[{"title", "\\h3"}] = {"header", "{", "textstyle", "}"};
	
	table[{"header", "\\h1"}] = {"\\h1"};
	table[{"header", "\\h2"}] = {"\\h2"};
	table[{"header", "\\h3"}] = {"\\h3"};
	
	table[{"textstyle", "text"}] = {"text"};
	table[{"textstyle", "\\bf"}] = {"\\bf", "{", "bloques", "}"};
	table[{"textstyle", "\\it"}] = {"\\it", "{", "bloques", "}"};
	table[{"textstyle", "\\un"}] = {"\\un", "{", "bloques", "}"};
	
	table[{"url", "\\link"}] = {"\\link", "{", "textstyle", "}"};
	
	table[{"comentario", "\\cmt"}] = {"\\cmt", "{", "text", "}"};
	
	table[{"enumerate", "\\list"}] = {"\\list", "{", "elements", "}"};
	
	table[{"elements", "}"}] = {""};
	table[{"elements", "text"}] = {"textstyle", "elements"};
	table[{"elements", "\\bf"}] = {"textstyle", "elements"};
	table[{"elements", "\\it"}] = {"textstyle", "elements"};
	table[{"elements", "\\un"}] = {"textstyle", "elements"};
	table[{"elements", "\\cmt"}] = {"comentario", "elements"};
	table[{"elements", "\\item"}] = {"\\item", "{", "bloques", "}", "elements"};
	
	table[{"table", "\\tbl"}] = {"\\tbl", "{", "cell", "}"};
	
	table[{"cell", "}"}] = {""};
	table[{"cell", "text"}] = {"textstyle", "cell'"};
	table[{"cell", "\\bf"}] = {"textstyle", "cell'"};
	table[{"cell", "\\it"}] = {"textstyle", "cell'"};
	table[{"cell", "\\un"}] = {"textstyle", "cell'"};
	
	table[{"cell'", "}"}] = {""};
	table[{"cell'", "&&"}] = {"&&", "cell"};
	table[{"cell'", "||"}] = {"||", "cell"};
	
	table[{"img", "\\pic"}] = {"\\pic", "{", "text", "}"};
	
	// FOLLOWS
	follows["bloques"] = {"}"};
	
	follows["bloque"] = {"text",  "\\bf",  "\\it",  "\\un",   "\\link",
					   "\\tbl", "\\pic", "\\cmt", "\\list", "\\h1",
					   "\\h2",  "\\h3",  "}"};
	
	follows["title"] = {"text",  "\\bf",  "\\it",  "\\un",   "\\link",
					  "\\tbl", "\\pic", "\\cmt", "\\list", "\\h1",
					  "\\h2",  "\\h3",  "}"};
	
	follows["header"] = {"{"};
	
	follows["textstyle"] = {"text",  "\\bf",  "\\it",  "\\un",   "\\link",
						  "\\tbl", "\\pic", "\\cmt", "\\list", "\\h1",
						  "\\h2",  "\\h3",  "}",     "\\item", "&&",
						  "||"};
	
	follows["url"] = {"text",  "\\bf",  "\\it",  "\\un",   "\\link",
					"\\tbl", "\\pic", "\\cmt", "\\list", "\\h1",
					"\\h2",  "\\h3",  "}"};
	
	follows["comentario"] = {"text",  "\\bf",  "\\it",  "\\un",   "\\link",
						   "\\tbl", "\\pic", "\\cmt", "\\list", "\\h1",
						   "\\h2",  "\\h3",  "}",     "\\item"};
	
	follows["enumerate"] = {"text",  "\\bf",  "\\it",  "\\un",   "\\link",
						  "\\tbl", "\\pic", "\\cmt", "\\list", "\\h1",
						  "\\h2",  "\\h3",  "}"};
	
	follows["elements"] = {"}"};
	
	follows["table"] = {"text",  "\\bf",  "\\it",  "\\un",   "\\link",
					  "\\tbl", "\\pic", "\\cmt", "\\list", "\\h1",
					  "\\h2",  "\\h3",  "}"};
	
	follows["cell"] = {"}"};
	
	follows["cell'"] = {"}"};
	
	follows["img"] = {"text",  "\\bf",  "\\it",  "\\un",   "\\link",
					"\\tbl", "\\pic", "\\cmt", "\\list", "\\h1",
					"\\h2",  "\\h3",  "}"};
}


// Parsear los tokens btenidos por el scanner
bool parser(vector<Token> tokens) {

  bool flag = true;
  stack<string> stck;
  pair<string, string> pairIndex;
  string currentToken, stackTop;

  stck.push("bloques");

  for (int i = 0; i < tokens.size(); ++i) {

	// Obtener el primer token del stack
	stackTop = stck.top();

	// Obtener tipo de token (text, bf, it, un, link, tbl, pic, cmt, list, h1, h2)
	currentToken = tokens[i].typeToken;

	// Significa epsilum en el stack. Pasar a la siguiente iteracion
	if (stackTop == "") {
	  stck.pop();
	  --i;
	  continue;
	}

	// Si el top del stack es terminal
	if (stackTop == currentToken) { // TERMINAL
	  stck.pop();
	  continue;
	}

	// Hacer par con top del stack y el token actual
	pairIndex = make_pair(stackTop, currentToken);


	// Revisa si hay una regla con dicho par en la tabla de parseo
	if (table.find(pairIndex) != table.end()) { // NO TERMINAL

	  stck.pop();

	  // Adiciona las producciones al stack
	  for (int r = table[pairIndex].size() - 1; r >= 0; --r) {
		stck.push(table[pairIndex][r]);
	  }
	  --i;
		
	} else { // Manejo de errores
	  flag = false;

	  bool errorType = 1; // IS TERMINAL
	  for (auto it = table.begin(); it != table.end(); ++it) {
		if (it->first.first == stackTop) {
		  errorType = 0; // IS NON TERMINAL
		  break;
		}
	  }

	  if (errorType) {
		cout << "\nError: Parser expected ( " << stackTop << " ) but found ( "
			 << currentToken << " )" << endl;
		  stck.pop();

	  } else { // STACK TOP IS NON TERMINAL
		cout << "\nError: Unexpected symbol found ( " << currentToken
			 << " ) with rule ( " << stackTop << " )" << endl;

		// Error handler
		bool skipInput = 1;
		for (int j = 0; j < follows[stackTop].size(); ++j) {
		  if (follows[stackTop][j] == currentToken) {
			// Current token is a follow of stackTop. (Pop stack)
			skipInput = 0;
			break;
		  }
		}

		if (!skipInput) {
		  stck.pop();
		  cout << "El token " << currentToken << " es un follow, se popea stack"
			   << endl;
		} else {
			cout << "El token " << currentToken << " NO es follow, se pasa al siguiente input stack"
				   << endl;
		}
		// Else skip input (ignore)
	  }
	}
  }

  if ( stck.size() == 0 || (stck.size() == 1 && stck.top() == "bloques")) return flag;

  cout<< "Still tokens in stack:" << endl;
  while(stck.size() > 1){
	  cout << stck.top() << endl;
	  stck.pop();
  }
  return 0;
	
}
