#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

vector<string> ReadLines(ifstream& offFile);
int GetValue(string line, int index);
string MakeTriangles(vector<string>& lines, int vertices);
string MakeBC(vector<string>& lines, int faces);

int main(int argc, char **argv) {

	if (argc == 3) {
		string modelName = argv[1];
		string filename = argv[2];
		filename = filename.c_str();
		string line;
		vector<string> lines;

		ifstream offFile;
		offFile.open(filename);

		getline (offFile,line);
		if (offFile.is_open() && line == "OFF") {
			lines = ReadLines(offFile);
			int vertices = GetValue(lines[0], 0);
			int faces = GetValue(lines[0], 1);

			cout << "function " << modelName << "() {" << endl;
			cout << '\t' << MakeTriangles(lines, vertices) << endl;
			cout << '\t' << MakeBC(lines, faces) << endl;
			cout << "};" << endl;

			offFile.close();
		} else {
			cout << "Error: unable to open file" << endl;
		}
	} else {
		cout << "Error: invalid number of arguments" << endl;
	}
  
    return 0;
}

// Reads in all lines of a file except for comments into a vector of strings
vector<string> ReadLines(ifstream& offFile) {
	vector<string> lines;
	string line;
	while(getline(offFile, line)) {
		if (line.at(0) != '#') {
			lines.push_back(line);
		}
	}
	return lines;
}

// Get the value within a line at word index
// Used to find vertices, faces, and edges
int GetValue(string line, int index) {
	string word;
	stringstream ss(line);
	for (int i=0; i<=index; i++) {
		ss >> word;
	}
	return stoi(word);
}

// Generate and return triangles
string MakeTriangles(vector<string>& lines, int vertices) {
	string triangles = "test";

	return triangles;
}

// Generate and return barycentric coordinates
string MakeBC(vector<string>& lines, int faces) {
	string bc = "test";

	return bc;
}
