#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <cctype>
#include "token.h"

using namespace std;

// Quitar todos los saltos de lineas, espascios en blanco
string fixWord(const string& word) {
	size_t start = word.find_first_not_of(" \n");
	if (start == string::npos) {
		return ""; // The string is all whitespace
	}
	size_t end = word.find_last_not_of(" \n");
	return word.substr(start, end - start + 1);
}

//Agrega el texto al vector de tokens
void addText(vector<Token> &tokens, string &word){
	word = fixWord(word);
	if (word != ""){
		tokens.push_back(Token(word)); // Texto
		word = "";
	}
}

//Returna el vector de tokens
vector<Token> scan(string input){

	string word = "";
	vector<Token> tokens;

	for (int i = 0; i < input.size(); i++){

		if (word != "" && isToken(word)){
			if (word == "\\it" && input[i] == 'e' && input[i+1] == 'm'){
				word += input[i];
				continue;
			}
			tokens.push_back(Token(word));
			word = "";
		}
		
		if (input[i] == '\\' || input[i] == '|' || input[i] == '&' || input[i] == '{' || input[i] == '}'){
			if (word.size() > 0 && word.back() == '\\'){ // Caracter especial
				word.pop_back();
			}
			else if (input[i] == '|' || input[i] == '&'){
				if (i+1 < input.size() && input[i] == input[i+1]){
					addText(tokens, word);
					word += input[i];
					word += input[i+1];
					tokens.push_back(Token(word));
					word = "";
					i+=1;
					continue;
				}
			}
			else if (word.size() > 0){
				addText(tokens, word);
			}
		}
		
		word += input[i];
	}

	word = fixWord(word);
	tokens.push_back(Token(word));

	return tokens;
	
}


