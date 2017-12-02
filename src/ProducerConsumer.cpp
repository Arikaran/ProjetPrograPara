#include <iostream>
#include <queue>
#include <mutex>
#include <thread>
#include <string>
//#include <cstring>
#include "thread.h"
#include "SHA256.h"

using namespace std;


class ProducerConsumer {
public:
	using Queue = std::queue<string>;						// Queue shared by producers and consumers						

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

	ProducerConsumer(uint8_t nbThreads, std::string hashFromUser)
	{
		string prefix = "a";
		unsigned int stringlength = 2;
		
		cout << "produce()..."<<endl;
		
		for(int i = 0; i <= stringlength; i++){	
			Produce(i, prefix);								// Produce words
		}
		cout << "produce() done !" << endl;

		Consume(hashFromUser);								// Consume words			
	}

	~ProducerConsumer()
	{
		
	}

private:
	Queue queue;
	std::mutex mutex;										// mutex for critical section
	const uint8_t nbProducer=1;
    const uint8_t nbConsumer=1;
	//std::deque<std::thread> producers;
	//std::deque<std::thread> consumers;
	const uint8_t nbThreads=1;
    const std::string hashFromUser="";

	// Recursive function, keeps clocking characters
	// until length is reached
	void Produce(unsigned int length, string s)
	{
		
		if(length == 0) // when length has been reached
		{
			//cout << s << "\n"; // print it out
			{
				lock_guard<std::mutex> lock(this->mutex);					// lock mutex for this scope duration
				queue.push(s);
				//std::this_thread::sleep_for(std::chrono::milliseconds(10));		// Sleep for 1 second
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

	void Consume(string hashFromUser){
		cout << "consume()..." << endl;

		while (!queue.empty())
		{
			{
				lock_guard<std::mutex> lock(this->mutex);				// lock mutex for this scope duration

				string text = queue.front();					// get the text from the global queue
				queue.pop();							// remove the text in the global queue

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

};


int main(int argc, char* argv[])
{	
	// To compile : g++ ProducerConsumer.cpp -o ProducerConsumer -std=c++11
	// To run : ./ProducerConsumer 2 af1c08098cf119bb4981729721714a4b9948dbcb6b5fff21cb0f45f06ad1f7ea
	uint8_t nbThreadsEnterred = 0;
	std::string hashFromUser = "";
	
	switch (argc) {
	    case 1:
	        cout << "No arguments findf ound. Try again !\n";
	        break;
	    case 2:
	        cout << "Not enough arguments. Try again with exactly two arguments\n";
	        break;
	    case 3:
	    	nbThreadsEnterred = *argv[1];
	    	
			for(int i = 2; i < argc; i++)
			{
				hashFromUser += argv[i];													//Get the hash passed in argument
			}

	    	if(hashFromUser.length() == 64){
	    		ProducerConsumer producerConsumer(nbThreadsEnterred, hashFromUser);			// Proceed
	    	}else{
	    		std::cout<<"Size : "<<hashFromUser.length()<<endl;
	    		std::cout<<"Hash : "<<hashFromUser<<endl;
	    		cout << "Incorrect hash. Check that the number of chars are equal to 64\n";
	    	}
	    	break;
	    default:
	        cout << "Too many things in the command line.";
	        exit(-1);
	        break;
	};

	return 0;
}


