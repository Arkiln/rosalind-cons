#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

unsigned int read_DNA(string ** input, unsigned int & size) {
	// Opens data file, passes data within in format
	//	name sequence
	//	name sequence
	//	etc.
	// and also passes/returns the number of pairs

	//	input of size[2][10]
	//		size[0][x] contains names, size[1][x] contains sequences

	size = 0;

	ifstream data("dataset.txt");
	string line;
	if (data.is_open()) {
		while (data) {
			getline(data, line);
			if (line[0] == '>') {
				size++;
				input[0][size - 1] = line;
			}
			else {
				input[1][size - 1] += line;
			}
		}
		return size;
	}
	else {
		cout << "Could not open file." << endl;
		return 0;
	}
	return size;
}

unsigned int ** get_profile(string ** storage, unsigned int & size) {
	// profile is of size 4, length depends on inputted strings

	// Initializes profile, our return value, in the following format:
	// 5 1 0 0 5 5 0 0
	// 0 0 1 4 2 0 6 1
	// 1 1 6 3 0 1 0 0
	// 1 5 0 0 0 1 1 6
	// where row 0 is A, 1 is C, 2 is G, 3 is T

	// Initialization of profile, our return value
	// Declared as static so our address pointer actually points to something
	static unsigned int * profile[4];
	for (unsigned int i = 0; i < 4; i++) {
		profile[i] = new unsigned int[storage[1][0].size()];
		for (unsigned int j = 0; j < storage[1][0].size(); j++) {
			profile[i][j] = 0;
		}
	}

	// Reads in from our dataset (all the sequences) and counts
	// each instance of an amino at a position
	char temp;
	unsigned int length = storage[1][0].size();
	for (unsigned int i = 0; i < size; i++) {
		for (unsigned int j = 0; j < length; j++) {
			temp = storage[1][i][j];

			if (temp == 'A')
				profile[0][j]++;
			if (temp == 'C')
				profile[1][j]++;
			if (temp == 'G')
				profile[2][j]++;
			if (temp == 'T')
				profile[3][j]++;
		}
	}
	return profile;
}

char get_char(int * input) {
	// Finds the position that has the largest amount of hits
	// Input should always be of size 4, for ACGT
	
	int * temp;
	temp = &input[0];
	for (unsigned int i = 0; i < 4; i++) {
		if (input[i] > *temp) {
			temp = &input[i];
		}
	}
	if (temp == &input[0])
		return 'A';
	if (temp == &input[1])
		return 'C';
	if (temp == &input[2])
		return 'G';
	if (temp == &input[3])
		return 'T';

	return 'Z';
}

string get_consensus(unsigned int ** profile, unsigned int length) {
	// Takes in profile and profile length, returns
	// a consensus string (most likely amino at each position)

	string output = "";
	int temp[4];

	for (unsigned int i = 0; i < length; i++) {
		temp[0] = profile[0][i];
		// cout << temp[0] << profile[0][i] << endl;
		temp[1] = profile[1][i];
		// cout << temp[1] << profile[1][i] << endl;
		temp[2] = profile[2][i];
		// cout << temp[2] << profile[2][i] << endl;
		temp[3] = profile[3][i];
		// cout << temp[3] << profile[3][i] << endl << endl;

		output += get_char(temp);
	}
	return output;
}

int main() {

	string * storage[2];
	storage[0] = new string[10];
	storage[1] = new string[10];
	unsigned int size;
	read_DNA(storage, size);

	// profile stores # of instances of each amino at each position
	unsigned int ** profile;
	profile = get_profile(storage, size);

	unsigned int length = storage[1][0].size();
	string answer = get_consensus(profile, length);
	cout << answer << endl;

	ofstream output("output.txt");
	output << answer << endl;
	output << "A: ";
	for (unsigned int i = 0; i < length; i++) {
		output << profile[0][i] << " ";
	}
	output << endl << "C: ";
	for (unsigned int i = 0; i < length; i++) {
		output << profile[1][i] << " ";
	}
	output << endl << "G: ";
	for (unsigned int i = 0; i < length; i++) {
		output << profile[2][i] << " ";
	}
	output << endl << "T: ";
	for (unsigned int i = 0; i < length; i++) {
		output << profile[3][i] << " ";
	}
	output << endl;

	delete storage[0];
	delete storage[1];

	return 0;
}