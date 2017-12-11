#include <iostream>
#include <queue>
#include <mutex>
#include <thread>
#include <string>
#include <functional>
#include "thread.h"
#include "SHA256.h"

using namespace std;


class ProducerConsumer {
public:
	using Queue = std::queue<string>;						// Queue shared by producers and consumers						

	// Create alphabet table
	const char Alphabets[63] =
	{
		'a', 'b', 'c', 'd',	'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ' '
	};

	const char Prefixes[63] =
	{
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 
		'k', 'l', 'm', 'n',	'o', 'p', 'q', 'r', 's', 't', 
		'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D',
		'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
		'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
		'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', 
		'8', '9', ' '
	};

	ProducerConsumer(int nbThreads, std::string hashFromUser)
	{
		thread t[nbThreads];

		//int nbProducersThreads = nbThreads/2;

		string prefix0 = " "; string prefix1 = "a";string prefix2 = "b";string prefix3 = "c";string prefix4 = "d";string prefix5 = "e";string prefix6 = "f";string prefix7 = "g";
		string prefix8 = "h";string prefix9 = "i";string prefix10 = "j";string prefix11 = "k";string prefix12 = "l";string prefix13 = "m";string prefix14 = "n";
		string prefix15 = "o";string prefix16 = "p";string prefix17 = "q";string prefix18 = "r";string prefix19 = "s";string prefix20 = "t";
		string prefix21 = "u";string prefix22 = "v";string prefix23 = "w";string prefix24 = "x";string prefix25 = "y";string prefix26 = "z";
/*
		for (int i = 0; i < nbProducersThreads; ++i) {
			t[i] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, i, &Prefixes[i]);
		}

		for (int i = 0; i < nbConsumersThreads; ++i) {
			t[i] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, i, hashFromUser);
		}
*/
		if(nbThreads%2 != 0){
			cout<<"The number of threads is not valid. Please choose a pair number and greater than 0."<< endl;
			exit(0);
		}
		// Function : Producer_call_from_thread(threadID, prefix)
		// Function : Consumer_call_from_thread(threadID, hash)
		switch (nbThreads) {
			case 0 : 
					cout<<"The number of threads is not valid. Please choose a pair number and greater than 0."<< endl;
					exit(0);
					break;
			case 2 : 
				t[0] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 0, prefix1);
				t[1] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 1, hashFromUser);
				
				t[0].join();t[1].join();
				break;
			case 4 : 
				t[0] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 0, prefix1);
				t[1] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 1, prefix2);
				t[2] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 2, hashFromUser);
				t[3] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 3, hashFromUser);
				
				t[0].join();t[1].join();t[2].join();t[3].join();
				break;
			case 6:
				t[0] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 0, prefix1);
				t[1] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 1, prefix2);
				t[2] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 2, prefix3);
				t[3] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 3, hashFromUser);
				t[4] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 4, hashFromUser);
				t[5] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 5, hashFromUser);
				
				t[0].join();t[1].join();t[2].join();
				t[3].join();t[4].join();t[5].join();
				break;
			case 8:
				t[0] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 0, prefix1);
				t[1] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 1, prefix2);
				t[2] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 2, prefix3);
				t[3] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 3, prefix4);
				t[4] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 4, hashFromUser);
				t[5] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 5, hashFromUser);
				t[6] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 6, hashFromUser);
				t[7] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 7, hashFromUser);
				
				t[0].join();t[1].join();t[2].join();t[3].join();
				t[4].join();t[5].join();t[6].join();t[7].join();
				break;
			case 10:
				t[0] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 0, prefix1);
				t[1] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 1, prefix2);
				t[2] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 2, prefix3);
				t[3] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 3, prefix4);
				t[4] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 4, prefix5);
				t[5] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 5, hashFromUser);
				t[6] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 6, hashFromUser);
				t[7] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 7, hashFromUser);
				t[8] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 8, hashFromUser);
				t[9] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 9, hashFromUser);
				
				t[0].join();t[1].join();t[2].join();t[3].join();t[4].join();
				t[5].join();t[6].join();t[7].join();t[8].join();t[9].join();
				break;
			case 12:
				t[0] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 0, prefix1);
				t[1] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 1, prefix2);
				t[2] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 2, prefix3);
				t[3] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 3, prefix4);
				t[4] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 4, prefix5);
				t[5] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 5, prefix6);
				t[6] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 6, hashFromUser);
				t[7] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 7, hashFromUser);
				t[8] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 8, hashFromUser);
				t[9] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 9, hashFromUser);
				t[10] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 10, hashFromUser);
				t[11] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 11, hashFromUser);

				t[0].join();t[1].join();t[2].join();t[3].join();t[4].join();t[5].join();
				t[6].join();t[7].join();t[8].join();t[9].join();t[10].join();t[11].join();
				break;
			case 14:
				t[0] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 0, prefix1);
				t[1] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 1, prefix2);
				t[2] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 2, prefix3);
				t[3] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 3, prefix4);
				t[4] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 4, prefix5);
				t[5] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 5, prefix6);
				t[6] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 6, prefix7);
				t[7] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 7, hashFromUser);
				t[8] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 8, hashFromUser);
				t[9] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 9, hashFromUser);
				t[10] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 10, hashFromUser);
				t[11] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 11, hashFromUser);
				t[12] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 12, hashFromUser);
				t[13] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 13, hashFromUser);

				t[0].join();t[1].join();t[2].join();t[3].join();t[4].join();t[5].join();t[6].join();t[7].join();
				t[8].join();t[9].join();t[10].join();t[11].join();t[12].join();t[13].join();
				break;
			case 16:
				t[0] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 0, prefix1);
				t[1] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 1, prefix2);
				t[2] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 2, prefix3);
				t[3] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 3, prefix4);
				t[4] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 4, prefix5);
				t[5] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 5, prefix6);
				t[6] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 6, prefix7);
				t[7] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 7, prefix8);
				t[8] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 8, hashFromUser);
				t[9] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 9, hashFromUser);
				t[10] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 10, hashFromUser);
				t[11] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 11, hashFromUser);
				t[12] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 12, hashFromUser);
				t[13] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 13, hashFromUser);
				t[14] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 14, hashFromUser);
				t[15] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 15, hashFromUser);

				t[0].join();t[1].join();t[2].join();t[3].join();t[4].join();t[5].join();t[6].join();t[7].join();t[8].join();t[9].join();
				t[10].join();t[11].join();t[12].join();t[13].join();t[14].join();t[15].join();
				break;
			case 18:
				t[0] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 0, prefix1);
				t[1] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 1, prefix2);
				t[2] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 2, prefix3);
				t[3] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 3, prefix4);
				t[4] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 4, prefix5);
				t[5] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 5, prefix6);
				t[6] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 6, prefix7);
				t[7] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 7, prefix8);
				t[8] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 8, prefix9);
				t[9] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 9, hashFromUser);
				t[10] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 10, hashFromUser);
				t[11] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 11, hashFromUser);
				t[12] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 12, hashFromUser);
				t[13] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 13, hashFromUser);
				t[14] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 14, hashFromUser);
				t[15] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 15, hashFromUser);
				t[16] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 16, hashFromUser);
				t[17] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 17, hashFromUser);

				t[0].join();t[1].join();t[2].join();t[3].join();t[4].join();t[5].join();t[6].join();t[7].join();t[8].join();t[9].join();
				t[10].join();t[11].join();t[12].join();t[13].join();t[14].join();t[15].join();t[16].join();t[17].join();
				break;
			case 20:
				t[0] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 0, prefix1);
				t[1] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 1, prefix2);
				t[2] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 2, prefix3);
				t[3] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 3, prefix4);
				t[4] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 4, prefix5);
				t[5] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 5, prefix6);
				t[6] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 6, prefix7);
				t[7] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 7, prefix8);
				t[8] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 8, prefix9);
				t[9] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 9, prefix10);
				t[10] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 10, hashFromUser);
				t[11] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 11, hashFromUser);
				t[12] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 12, hashFromUser);
				t[13] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 13, hashFromUser);
				t[14] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 14, hashFromUser);
				t[15] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 15, hashFromUser);
				t[16] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 16, hashFromUser);
				t[17] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 17, hashFromUser);
				t[18] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 18, hashFromUser);
				t[19] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 19, hashFromUser);

				t[0].join();t[1].join();t[2].join();t[3].join();t[4].join();t[5].join();t[6].join();t[7].join();t[8].join();t[9].join();
				t[10].join();t[11].join();t[12].join();t[13].join();t[14].join();t[15].join();t[16].join();t[17].join();t[18].join();t[19].join();
				break;
			case 22:
				t[0] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 0, prefix1);
				t[1] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 1, prefix2);
				t[2] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 2, prefix3);
				t[3] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 3, prefix4);
				t[4] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 4, prefix5);
				t[5] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 5, prefix6);
				t[6] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 6, prefix7);
				t[7] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 7, prefix8);
				t[8] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 8, prefix9);
				t[9] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 9, prefix10);
				t[10] = std::thread(&ProducerConsumer::Producer_call_from_thread, this, 10, prefix0);
				t[11] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 11, hashFromUser);
				t[12] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 12, hashFromUser);
				t[13] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 13, hashFromUser);
				t[14] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 14, hashFromUser);
				t[15] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 15, hashFromUser);
				t[16] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 16, hashFromUser);
				t[17] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 17, hashFromUser);
				t[18] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 18, hashFromUser);
				t[19] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 19, hashFromUser);
				t[20] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 20, hashFromUser);
				t[21] = std::thread(&ProducerConsumer::Consumer_call_from_thread, this, 21, hashFromUser);

				t[0].join();t[1].join();t[2].join();t[3].join();t[4].join();t[5].join();t[6].join();t[7].join();t[8].join();
				t[9].join();t[10].join();t[11].join();t[12].join();t[13].join();t[14].join();t[15].join();t[16].join();
				t[17].join();t[18].join();t[19].join();t[20].join();t[21].join();
				break;
			default:
					cout<<"The number of threads is not valid. Please choose a pair number and greater than 0."<< endl;
					exit(0);
					break;
		}

/*
		for (int i = 0; i < nbThreads; ++i) {
			t[i].join();
		}
*/
	}

	~ProducerConsumer()
	{
		
	}

private:
	Queue queue;
	std::mutex mutex;																// Mutex for critical section
	const uint8_t nbProducer=1;
    const uint8_t nbConsumer=1;
	//std::deque<std::thread> producers;
	//std::deque<std::thread> consumers;
	//uint8_t nbThreads;
    string hashFromUser="";

    void Producer_call_from_thread(int tid, string prefix)
    {	
		cout << "Produce by thread n°" << tid << endl;
		unsigned int stringlength = 63;
		
		cout << "produce()..."<<endl;
		
		for(uint8_t i = 0; i <= stringlength; i++){
			Produce(i, prefix, tid);												// Produce words
		}
		cout << "produce() done !" << endl;
    }

    void Consumer_call_from_thread(int tid, string hashFromUser)
    {	
    	cout << "Consume by thread n°" << tid << endl;
    	Consume(hashFromUser, tid);													// Consume words
    }

	// Recursive function, keeps clocking characters
	// until length is reached
	void Produce(unsigned int length, string s, int tid)
	{
		if(length == 0) // when length has been reached
		{
			cout << "thread n°"<< tid << " produce : " << s << "\n"; 				// Print text out
			{
				lock_guard<std::mutex> lock(this->mutex);							// Lock mutex for this scope duration
				queue.push(s);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(500));			// Sleep for 1 second
			return;
		}

		for(unsigned int i = 0; i < 63; i++) 										// Iterate through Alphabets
		{
			// Create new string with next character
			// Call generate again until string has reached it's length
			string appended = s + Alphabets[i];
			Produce(length-1, appended, tid);
		}
	}

	void Consume(string hashFromUser, int tid){
		cout << "consume()..." << endl;

		while (!queue.empty())
		{
			{
				lock_guard<std::mutex> lock(this->mutex);							// Lock mutex for this scope duration

				string text = queue.front();										// get the text from the global queue
				queue.pop();														// remove the text in the global queue

		    	cout << "thread n°"<< tid << " ***consume*** : " << text << endl;

				char data[text.size()+1];
				strcpy(data, text.c_str());

				string sha256 = SHA256(data);										// hash the text

				bool compare = compareHashes(hashFromUser, sha256);					// hashes comparison

				if(compare == 1){
					cout << "The hash "<< sha256 << " matches with the text : " << text << endl;
					exit(0);		
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(600));			// Sleep for 1 second
		}
		cout << "consume() done !" << endl;
	}

};


int main(int argc, char* argv[])
{	
	/* To compile : 
		cd src
		cmake ..
		make
	*/
	// To run : ./main 4 e061428097b79aab3d587dbb523e19f4e58b67346699208c81f8b5a9449ffe6f
	
	int nbThreadsEnterred = 0;
	std::string hashFromUser = "";
	
	switch (argc) {
	    case 1:
	        cout << "No arguments find found. Try again with exactly two arguments\n";
	        break;
	    case 2:
	        cout << "Not enough arguments. Try again with exactly two arguments\n";
	        break;
	    case 3:
	    	nbThreadsEnterred = atoi(argv[1]);

			for(int i = 2; i < argc; i++)
			{
				hashFromUser += argv[i];													//Get the hash passed in argument
			}

	    	if(hashFromUser.length() == 64){
	    		ProducerConsumer producerConsumer(nbThreadsEnterred, hashFromUser);			// Proceed
	    	}else{
	    		cout<<"Hash received : "<<hashFromUser<<endl;
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


