#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>
#include <set>
using namespace std;

//Declaramos nuestros tokens
set<string> allTokens = {"\\h1", "\\h2", "\\h3", "\\link", "\\pic", "\\bf", "\\it", "\\un", "\\cmt", "\\list", "\\item", "\\tbl", "&&", "||", "{", "}"};

//Verificacion de tokens
bool isToken(const string& token){
	return allTokens.find(token) != allTokens.end();
}

class Token{

public:

//Atributos del objeto token
	string typeToken = "";
	string content = "";

	Token(string word){
		if (isToken(word)) typeToken =  word;
		else{
			typeToken =  "text";
			content = word;
		}
	}

	void print(){
		if (typeToken == "text") cout << "text("<< content<< ")";
		else cout << typeToken;
	}

//Traduccion de tokens a código HTML
	string toHTML() {
		if (typeToken == "text") return content;
		else if (typeToken == "\\h1") return "<h1>";
		else if (typeToken == "\\h2") return "<h2>";
		else if (typeToken == "\\h3") return "<h3>";
		else if (typeToken == "\\link") return "<a href=\"" + content + "\">" + content + "</a>";
		else if (typeToken == "\\pic") return "<img src=\"" + content + "\" alt=\"Image\">";
		else if (typeToken == "\\bf") return "<b>" + content + "</b>";
		else if (typeToken == "\\it") return "<i>" + content + "</i>";
		else if (typeToken == "\\un") return "<u>" + content + "</u>";
		else if (typeToken == "\\cmt") return "<!-- " + content + " -->";
		else if (typeToken == "\\item") return "<li>" + content + "</li>";
		else if (typeToken == "&&") return "</td><td>";
		else if (typeToken == "||") return "</td></tr><tr><td>";
		else if (typeToken == "\\tbl") return "<table>" + content + "</table>";
		else if (typeToken == "\\list") return "<ul>" + content + "</ul>";
		return "";
	}

};


#endif