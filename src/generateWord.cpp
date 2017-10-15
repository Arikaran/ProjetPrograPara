#include <iostream>
#include <string>

using namespace std;

// Create alphabet tables

const char AlphabetUpper[26] =
{
	'A', 'B', 'C', 'D', 'E', 'F', 'G',
	'H', 'I', 'J', 'K', 'L', 'M', 'N',
	'O', 'P', 'Q', 'R', 'S', 'T', 'U',
	'V', 'W', 'X', 'Y', 'Z'
};

const char AlphabetLower[26] =
{
	'a', 'b', 'c', 'd', 'e', 'f', 'g',
	'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u',
	'v', 'w', 'x', 'y', 'z'
};

const char AlphabetNumber[10] = 
{
	'0', '1', '2', '3', '4', 
	'5', '6', '7', '8', '9'
};

const char AlphabetSpace[1] = 
{
	' '
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

	for(unsigned int i = 0; i < 26; i++) // iterate through AlphabetLower
	{
		// Create new string with next character
		// Call generate again until string has reached it's length
		string appended = s + AlphabetLower[i];
		Generate(length-1, appended);
	}

	for(unsigned int i = 0; i < 26; i++) // iterate through AlphabetUpper
	{
		string appended = s + AlphabetUpper[i];
		Generate(length-1, appended);
	}

	for(unsigned int i = 0; i < 10; i++) // iterate through AlphabetNumber
	{
		string appended = s + AlphabetNumber[i];
		Generate(length-1, appended);
	}

	for(unsigned int i = 0; i < 1; i++) // iterate through AlphabetSpace
	{
		string appended = s + AlphabetSpace[i];
		Generate(length-1, appended);
	}
}

void GenerateWords(string prefix, unsigned int stringlength)
{
	Generate(stringlength, prefix);
}

int main()
{	
	string prefix = "a";
	unsigned int stringlength = 2;
	
	cout << "Generating..."<<endl;

	for(int i = 0; i <= stringlength; i++){	
		GenerateWords(prefix, i);
	}
	
	return 0;
}
