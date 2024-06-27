#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include "token.h"

using namespace std;

// Clase para representar cada nodo en el árbol
class TreeNode {
public:
	Token token;
	vector<TreeNode*> children;

	TreeNode(const Token& t) : token(t) {}
};

// Clase para representar nuestra estructura de datos árbol
class Tree {
public:
	TreeNode* root;

	Tree(vector<Token> tokens) {
		root = constructTree(tokens);
	}

	TreeNode* constructTree(vector<Token>& tokens) {
		if (tokens.empty()) return nullptr;
	
		stack<TreeNode*> nodeStack;

		// Se agrega \body y { } al inicio y final, respectivamente
		tokens.insert(tokens.begin(), Token("{"));
		tokens.push_back(Token("}"));

		Token body = Token("");
		body.typeToken = "\\body";
		body.content = "";

		TreeNode* currentNode = new TreeNode(body);

		for (int i = 0; i < tokens.size(); i++){
			if (tokens[i].typeToken == "{" || tokens[i].typeToken == "text") {
				currentNode->children.push_back(new TreeNode(tokens[i]));
			} else if (tokens[i].typeToken == "}") { // Subir
				currentNode->children.push_back(new TreeNode(tokens[i]));
				currentNode = nodeStack.top();
				nodeStack.pop();
			} else { // Bajar
				currentNode->children.push_back(new TreeNode(tokens[i]));
				nodeStack.push(currentNode);
				currentNode = currentNode->children.back();
			}
		}

		while (nodeStack.size() != 0) {
			currentNode = nodeStack.top();
			nodeStack.pop();
		}
	
		return currentNode;
	}

	void printBFS() const {
		if (!root) return;
	
		queue<TreeNode*> q;
		q.push(root);
	
		while (!q.empty()) {
			int size = q.size();
			for (int i = 0; i < size; ++i) {
				TreeNode* currentNode = q.front();
				q.pop();
	
				// Imprime el current token
				currentNode->token.print();
				cout << " ";
	
				// Poner en cola a los hijos
				for (TreeNode* child : currentNode->children) {
					q.push(child);
				}
			}
			cout << "\n"; // Se imprime cada nivel del arbol y leugo se salta la linea
		}
	}
};