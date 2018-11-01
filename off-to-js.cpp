#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

struct coord {
	double x;
	double y;
	double z;
};

vector<string> ReadLines(ifstream& offFile);
int GetValue(string line, int index);
void MakeTriangles(vector<string>& lines, unsigned vertices);
void MakeBC(vector<vector<coord>> triangles);
vector<coord> FindPoints(vector<string> lines);
vector<vector<coord>> FindPolygons(vector<coord> points, vector<string> lines, unsigned vertices);
vector<vector<coord>> PolygonsToTriangles(vector<vector<coord>> polygons);
vector<vector<coord>> ScaleTriangles(vector<vector<coord>> triangles);

int main(int argc, char **argv) {

	if (argc == 3) {
		string modelName = argv[1];
		modelName += "Model";
		string filename = argv[2];
		filename = filename.c_str();
		string line;
		vector<string> lines;

		ifstream offFile;
		offFile.open(filename);

		getline (offFile,line);
		if (offFile.is_open() && line == "OFF") {
			lines = ReadLines(offFile);
			unsigned vertices = GetValue(lines[0], 0);
			/* unsigned faces = GetValue(lines[0], 1); */

			cout << "function " << modelName << "() {" << endl;
			cout << '\t'; 
			MakeTriangles(lines, vertices);
			cout << endl;

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
void MakeTriangles(vector<string>& lines, unsigned vertices) {
	vector<string> triLines;
	vector<coord> points;
	vector<vector<coord>> polygons;
	vector<vector<coord>> triangles;
	unsigned triPoints = 3;

	for (unsigned i=1; i<=vertices; i++) {
		triLines.push_back(lines[i]);
	}
	points = FindPoints(triLines);
	polygons = FindPolygons(points, lines, vertices);
	triangles = PolygonsToTriangles(polygons);
	triangles = ScaleTriangles(triangles);

	cout << "this.triangles = [";
	for (unsigned j=0; j<triangles.size(); j++) {
		for (unsigned k=0; k<triPoints; k++) {
			cout << '[';
			cout << triangles[j][k].x;
			cout << ", ";
			cout << triangles[j][k].y;
			cout << ", ";
			cout << triangles[j][k].z;
			cout << "], ";
		}
	}
	cout << "];" << endl;

	MakeBC(triangles);
	
	/* for (unsigned i=0; i<triLines.size(); i++) */
	/* 	cout << triLines[i] << endl; */
}

// Generate and return barycentric coordinates
void MakeBC(vector<vector<coord>> triangles) {
	cout << '\t' ;
	cout << "this.BC = [";
	for (unsigned i=0; i<triangles.size(); i++) {
		cout << "[1, 0, 0], [0, 1, 0], [0, 0, 1], ";
	}
	cout << "];" << endl;
	cout << "};" << endl;
}

// Convert points from strings to float coordinates
vector<coord> FindPoints(vector<string> lines) {
	vector<coord> points;
	int xyz = 3;
	coord newCoord;
	string word;

	for(unsigned i=0; i<lines.size(); i++) {
		stringstream ss(lines[i]);
		for (int i=1; i<=xyz; i++) {
			ss >> word;
			switch (i) {
				case 1:
					newCoord.x = stod(word);
					break;
				case 2:
					newCoord.y = stod(word);
					break;
				case 3:
					newCoord.z = stod(word);
					break;
			}
		}
		points.push_back(newCoord);
	}
	/* for (unsigned i=0; i<points.size(); i++) { */
	/* 	cout << "x: " << points[i].x; */
	/* 	cout << " y: " << points[i].y; */
	/* 	cout << " z: " << points[i].z; */
	/* 	cout << endl; */
	/* } */
	return points;
}

// Returns the coordinates to make polygons given by input
vector<vector<coord>> FindPolygons(vector<coord> points, vector<string> lines, unsigned vertices) {
	vector<vector<coord>> polygons;
	vertices++;
	string word;
	int faces;
	vector<int> verts;
	vector<coord> newPoly;

	for (unsigned i=vertices; i<lines.size(); i++) {
		stringstream ss(lines[i]);
		ss >> word;
		faces = stoi(word);
		for (int j=0; j<faces; j++) {
			ss >> word;
			verts.push_back(stoi(word));			
		}
		for (unsigned k=0; k<verts.size(); k++) {
			newPoly.push_back(points[verts[k]]);
		}
		verts.clear();
		polygons.push_back(newPoly);
		newPoly.clear();
	}

	/* for (unsigned i=0; i<polygons.size(); i++) { */
	/* 	cout << "p" << i << " verts: " << polygons[i].size() << endl; */
	/* 	for (unsigned j=0; j<polygons[i].size(); j++) { */
	/* 		cout << "p" << i << ',' << j << " x: " << polygons[i][j].x; */
	/* 		cout << " y: " << polygons[i][j].y; */
	/* 		cout << " z: " << polygons[i][j].z << endl; */
	/* 	} */
	/* } */

	return polygons;
}

// Performs the fan algorithm on polygons to create triangles
vector<vector<coord>> PolygonsToTriangles(vector<vector<coord>> polygons) {
	vector<vector<coord>> triangles;
	vector<coord> newTriangle;

	for (unsigned i=0; i<polygons.size(); i++) {
		if (polygons[i].size() == 3) {
			triangles.push_back(polygons[i]);
		} else {
			for (unsigned j=0; j<polygons[i].size()-2; j++) {
				newTriangle.push_back(polygons[i][0]);
				newTriangle.push_back(polygons[i][j+1]);	
				newTriangle.push_back(polygons[i][j+2]);	
				triangles.push_back(newTriangle);
				newTriangle.clear();
			}
		}
	}
	return triangles;
}

// Scales the triangles to fit within the bounding box
vector<vector<coord>> ScaleTriangles(vector<vector<coord>> triangles) {
	float minX = triangles[0][0].x;
	float minY = triangles[0][0].y;
	float minZ = triangles[0][0].z;
	float maxAll = triangles[0][0].x;	
	unsigned triPoints = 3;

	for (unsigned i=0; i<triangles.size(); i++) {
		for (unsigned j=0; j<triPoints; j++) {
			if(triangles[i][j].x < minX)
				minX = triangles[i][j].x;
			if(triangles[i][j].y < minY)
				minY = triangles[i][j].y;
			if(triangles[i][j].z < minZ)
				minZ = triangles[i][j].z;
		}
	}

	minX = -1 * minX;
	minY = -1 * minY;
	minZ = -1 * minZ;

	for (unsigned k=0; k<triangles.size(); k++) {
		for (unsigned l=0; l<triPoints; l++) {
			triangles[k][l].x += minX;
			triangles[k][l].y += minZ;
			triangles[k][l].z += minY;
		}
	}

	for (unsigned m=0; m<triangles.size(); m++) {
		for (unsigned n=0; n<triPoints; n++) {
			if (triangles[m][n].x > maxAll)
				maxAll = triangles[m][n].x;
			if (triangles[m][n].y > maxAll)
				maxAll = triangles[m][n].y;
			if (triangles[m][n].z > maxAll)
				maxAll = triangles[m][n].z;
		}
	}

	for (unsigned o=0; o<triangles.size(); o++) {
		for (unsigned p=0; p<triPoints; p++) {
			triangles[o][p].x *= 1/maxAll;
			triangles[o][p].y *= 1/maxAll;
			triangles[o][p].z *= 1/maxAll;

			triangles[o][p].x -= .5;
			triangles[o][p].y -= .5;
			triangles[o][p].z -= .5;
		}
	}

	return triangles;
}
