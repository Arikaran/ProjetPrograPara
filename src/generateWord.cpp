#include <iostream>
#include <string>

using namespace std;

// Create alphabet table
const char Alphabets[63] =
{
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
	'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
	'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',
	'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 
	'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', 
	'8', '9', ' '
};

// Recursive function, keeps clocking characters
// until length is reached
void Generate(unsigned int length, string s)
{

	if(length == 0) // when length has been reached
	{
		cout << s << "\n"; // print it out
		return;
	}

	for(unsigned int i = 0; i < 63; i++) // iterate through Alphabets
	{
		// Create new string with next character
		// Call generate again until string has reached it's length
		string appended = s + Alphabets[i];
		Generate(length-1, appended);
	}
}


void GenerateWords(string prefix, unsigned int stringlength)
{
	Generate(stringlength, prefix);
}


int main()
{	
	string prefix = "";
	unsigned int stringlength = 1;
	
	cout << "Generating..."<<endl;

	for(int i = 0; i <= stringlength; i++){	
		GenerateWords(prefix, i);
	}
	
	return 0;
}
