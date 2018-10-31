#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

int main(int argc, char **argv) {

	string modelName = argv[1];
	string filename = argv[2];
	filename = filename.c_str();
	string line;

	ifstream offFile;
	offFile.open(filename);

	if (offFile.is_open()) {

		while (getline (offFile,line)) {
			cout << line << '\n';
		}


		offFile.close();
	} else {
		cout << "Error: unable to open file" << endl;
	}
  
    return 0;
}
