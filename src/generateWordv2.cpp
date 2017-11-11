#include <iostream>
#include <queue>
#include <mutex>
#include <thread>
#include <string>
#include "thread.h"
#include "SHA256.h"

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

queue<string> globalQueue;			// Queue shared by producers and consumers
mutex globalMutex;				// mutex for critical section

// Recursive function, keeps clocking characters
// until length is reached
void Produce(unsigned int length, string s)
{
	
	if(length == 0) // when length has been reached
	{
		//cout << s << "\n"; // print it out
		{
			lock_guard<mutex> lock(globalMutex);					// lock mutex for this scope duration
			globalQueue.push(s);
			//std::this_thread::sleep_for(std::chrono::milliseconds(1000));		// Sleep for 1 second
		}
		
		return;
	}

	for(unsigned int i = 0; i < 63; i++) // iterate through Alphabets
	{
		// Create new string with next character
		// Call generate again until string has reached it's length
		string appended = s + Alphabets[i];
		Produce(length-1, appended);
	}
}

void Consume(queue <string> myqueue, string hashFromUser){
	cout << "consume()..." << endl;

	while (!myqueue.empty())
	{
		{
			lock_guard<mutex> lock(globalMutex);				// lock mutex for this scope duration

			string text = myqueue.front();					// get the text from the global queue
			myqueue.pop();							// remove the text in the global queue

	    		cout << "consume() pop() : " << text << endl;

			char data[text.size()+1];
			strcpy(data, text.c_str());

			string sha256 = SHA256(data);					// hash the text

			cout << "The hash is : "<< sha256 << endl;
	
			bool compare = compareHashes(hashFromUser, sha256);		// hashes comparison

			if(compare == 1){
				cout << "Hash found !" << endl;
				exit(0);		
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));	// Sleep for 1 second	
		}
	}
	cout << "consume() done !" << endl;
}

void GenerateWords(string hashFromUser)
{
	string prefix = "a";
	unsigned int stringlength = 2;
	
	cout << "produce()..."<<endl;
	
	for(int i = 0; i <= stringlength; i++){	
		Produce(i, prefix);			// Produce words
	}
	cout << "produce() done !" << endl;

	Consume(globalQueue, hashFromUser);				// Consume words							
	
}

int main()
{	
	// To compile : g++ generateWordv2.cpp -o generateWordv2 -std=c++11
	// To run : ./generateWordv2

	string hashFromUser = "08389ec21e094342a0d7aaeb2ea21e184531a12f399b99f1efc29a1bea2143da";

	GenerateWords(hashFromUser);

	return 0;
}


