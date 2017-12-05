#include <iostream>
#include <queue>
#include <mutex>
#include <thread>
#include <string>
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
		thread t[nbThreads];
		string prefix = "a";								
/*
		//Launch a group of producers threads
		for (int i = 0; i < nbThreads; ++i) {
			t[i] = std::thread(Producer_call_from_thread, i, prefix);
		}

		//Launch a group of producers threads
		for (int i = 0; i < nbThreads; ++i) {
			t[i] = std::thread(Consumer_call_from_thread, i);
		}
*/
		// Function : Producer_call_from_thread(threadID, prefix)
		// Consumer_call_from_thread(threadID, hash)
		t[1] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 1, prefix);
		t[2] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 2, hashFromUser);

		//Join the threads with the main thread
		t[1].join();
		t[2].join();
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
	//uint8_t nbThreads;
    string hashFromUser="";

    void Producer_call_from_thread(int tid, string prefix)
    {	
		cout << "Produce by thread n°" << tid << "\n" << endl;
		unsigned int stringlength = 1;
		
		cout << "produce()..."<<endl;
		
		for(uint8_t i = 0; i <= stringlength; i++){
			Produce(i, prefix);								// Produce words
		}
		cout << "produce() done !" << endl;
    }

    void Consumer_call_from_thread(int tid, string hashFromUser)
    {	
    	cout << "Consume by thread n°" << tid << "\n" << endl;
    	Consume(hashFromUser);								// Consume words
    }

	// Recursive function, keeps clocking characters
	// until length is reached
	void Produce(unsigned int length, string s)
	{
		if(length == 0) // when length has been reached
		{
			cout << "produce : " << s << "\n"; 										// Print text out
			{
				lock_guard<std::mutex> lock(this->mutex);							// Lock mutex for this scope duration
				queue.push(s);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(100));				// Sleep for 1 second
			return;
		}

		for(unsigned int i = 0; i < 63; i++) 										// Iterate through Alphabets
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
				lock_guard<std::mutex> lock(this->mutex);							// Lock mutex for this scope duration

				string text = queue.front();										// get the text from the global queue
				queue.pop();														// remove the text in the global queue

		    	cout << "consume() pop() : " << text << endl;

				char data[text.size()+1];
				strcpy(data, text.c_str());

				string sha256 = SHA256(data);										// hash the text

				bool compare = compareHashes(hashFromUser, sha256);					// hashes comparison

				if(compare == 1){
					cout << "The hash "<< sha256 << "matches with the text : " << text << endl;
					exit(0);		
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));		// Sleep for 1 second
		}
		cout << "consume() done !" << endl;
	}

};


int main(int argc, char* argv[])
{	
	// To compile : g++ ProducerConsumer.cpp -std=c++11 -lpthread -o ProducerConsumer
	// To run : ./ProducerConsumer 2 af1c08098cf119bb4981729721714a4b9948dbcb6b5fff21cb0f45f06ad1f7ea
	uint8_t nbThreadsEnterred = 0;
	std::string hashFromUser = "";
	
	switch (argc) {
	    case 1:
	        cout << "No arguments find found. Try again with exactly two arguments\n";
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


