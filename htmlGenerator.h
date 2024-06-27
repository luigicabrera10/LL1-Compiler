#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <stack>
#include <fstream>
#include "token.h"

using namespace std;

void insertStringAt(string& original, const string& toInsert, size_t index) {
	if (index > original.length()) {
		cerr << "Index out of bounds!" << endl;
		return;
	}
	original.insert(index, toInsert);
}

string tokensToHTML(const vector<Token>& tokens) {
	string html;
	stack<string> tagStack;

	int hrefLink = 0;
	vector< pair< int, string > > hrefs;

	html += "<!DOCTYPE html>\n";
	html += "<html>\n";
	html += "<head>\n";
	html += "<meta charset=\"UTF-8\">\n";
	html += "<title>Generated HTML</title>\n";
	html += "</head>\n";
	html += "<body>\n";

	for (const auto& token : tokens) {
		if (token.typeToken == "text") {
			if (tagStack.empty()) {
				html += "<p>" + token.content + "</p>\n";
			} 
			else if (hrefLink == 1){
				hrefs.back().second = token.content;
				html += token.content;
				hrefLink = 0;
			}
			else if (hrefLink == 2){
				hrefs.back().second = token.content;
				hrefLink = 0;
			}
			else {
				html += token.content;
			}
		} else if (token.typeToken == "{") {
			continue;
		} else if (token.typeToken == "}") {
			if (!tagStack.empty()) {
				if (tagStack.top() == "img"){
					tagStack.pop();
					continue;
				}
				html += "</" + tagStack.top() + ">\n";
				tagStack.pop();
			}
		} else if (token.typeToken == "&&") {
			html += "</td><td style=\"border: 1px solid black;\">";
		} else if (token.typeToken == "||") {
			html += "</td></tr><tr><td style=\"border: 1px solid black;\">";
		} else {
			if (token.typeToken == "\\h1") {
				html += "<h1>";
				tagStack.push("h1");
			} else if (token.typeToken == "\\h2") {
				html += "<h2>";
				tagStack.push("h2");
			} else if (token.typeToken == "\\h3") {
				html += "<h3>";
				tagStack.push("h3");
			} else if (token.typeToken == "\\link") {
				html += "<a href=\"\">";
				tagStack.push("a");
				hrefLink = 1;
				hrefs.push_back({html.size()-2, ""});
			} else if (token.typeToken == "\\pic") {
				html += "<img src=\"\">\n";
				hrefLink = 2;
				hrefs.push_back({html.size()-3, ""});
				tagStack.push("img");
			} else if (token.typeToken == "\\bf") {
				html += "<b>";
				tagStack.push("b");
			} else if (token.typeToken == "\\it") {
				html += "<i>";
				tagStack.push("i");
			} else if (token.typeToken == "\\un") {
				html += "<u>";
				tagStack.push("u");
			} else if (token.typeToken == "\\cmt") {
				html += "<!-- ";
				tagStack.push("!--");
			} else if (token.typeToken == "\\list") {
				html += "<ul>";
				tagStack.push("ul");
			} else if (token.typeToken == "\\item") {
				html += "<li>";
				tagStack.push("li");
			} else if (token.typeToken == "\\tbl") {
				html += "<table style=\"border: 1px solid black; border-collapse: collapse;\"><tr><td style=\"border: 1px solid black;\">";
				tagStack.push("table");
			}
		}
	}

	// Close any remaining open tags
	while (!tagStack.empty()) {
		html += "</" + tagStack.top() + ">";
		tagStack.pop();
	}

	html += "\n</body>\n";
	html += "</html>\n";

	int count = 0;
	for(int i = 0; i < hrefs.size(); ++i){
		insertStringAt(html, hrefs[i].second, hrefs[i].first + count);
		count += hrefs[i].second.size();
	}

	return html;
}

void saveToHTMLFile(const string& content, const string& filename) {
	ofstream outFile(filename);

	if (outFile.is_open()) {
		outFile << content;
		outFile.close();
		cout << "File saved successfully: " << filename << endl;
	} else {
		cerr << "Error: Could not open file " << filename << " for writing." << endl;
	}
}