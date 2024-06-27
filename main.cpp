#include <iostream>
#include <fstream>
#include <sstream>
#include "scanner.h"
#include "parser.h"
#include "htmlGenerator.h"
#include "tree.h"


using namespace std;

//Función para lectura del archivo
string readFile(const string& filePath) {
    ifstream file(filePath);

    if (!file.is_open()) {
        cerr << "Could not open the file!" << endl;
        return "";
    }

    stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

int main() {
  
    string filePath = "archivo.txt"; 
    string fileContent = readFile(filePath);

    //Inicializa la tabla de parsseo
    initTable();
    //Inicializa el scanner
    vector<Token> tokens = scan(fileContent);

    for(int i = 0; i < tokens.size(); i++){
        tokens[i].print();
        cout << " ";
    }
    //Inicializa el parser
    bool parserCheck =  parser(tokens);
    cout << "\n\nPARSER CHECK: " << parserCheck << endl << endl;

    if (parserCheck){
        string html = tokensToHTML(tokens);
        cout << html << endl;
        saveToHTMLFile(html, "output.html");

        // Token Tree D:
        Tree tree(tokens);
        cout << "Tree Succesfully Generated:" << endl;
        tree.printBFS();
    }
    return 0;
}