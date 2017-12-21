#include <iostream>
#include <queue>
#include <mutex>
#include <thread>
#include <string>
#include <condition_variable>
#include "thread.h"
#include "SHA256.h"

using namespace std ;


class ProducerConsumer
{
	public :
		using Queue = std::queue<string> ; 									// Queue shared by producers and consumers						
		std::mutex mutex ; 													// Mutex for critical section
		Queue queue ;
		condition_variable cond_var;

		// Create alphabet table
		const string Alphabets[63] =
		{
			"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
			"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
			"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", " "
		} ;

		bool done = false ;
		bool notified = false;

		ProducerConsumer(unsigned int nbThreads, std::string hashFromUser)
		{
			if(nbThreads % 2 != 0)
			{
				cout << "The number of threads is not valid. Please choose a pair number and greater than 0." << endl ;
				exit(0) ;
			}

			thread t[nbThreads] ;

			if(nbThreads > 126)
			{
				nbThreads = 126 ;
			}

			unsigned int threadBegin = 0 ;
			unsigned int threadEnd = 0 ;
			unsigned int firstGap = (63 / (nbThreads / 2)) + (63 % (nbThreads / 2)) ;

			for(unsigned int i = 0 ; i < (nbThreads / 2) ; i++)
			{
				if(i != 0)
				{
					threadEnd += (63 / (nbThreads / 2)) ;
				}

				else
				{
					threadEnd += firstGap ;
				}

			    t[i] = std::thread(&ProducerConsumer::Produce, this, Alphabets[i], threadBegin, threadEnd, (i + 1)) ;

			    	//done = true;
        			//cond_var.notify_one();

			    t[(nbThreads / 2) + i] = std::thread(&ProducerConsumer::Consume, this, hashFromUser, (i + 1)) ;

				threadBegin = threadEnd ;
			}

			for(unsigned int i = 0 ; i < (nbThreads / 2) ; i++)
			{
			    	t[i].join() ;
			    	t[(nbThreads / 2) + i].join() ;
			}
		}

		~ProducerConsumer()
		{
		
		}

	private :
		
		
	    string hashFromUser = "" ;

		void generateWord(string word, unsigned int wordLength, unsigned int wordLimit, unsigned int tid)
		{
			if(wordLength < wordLimit)
			{
				for(unsigned int i = 0 ; i < 63 ; i++) // Iterate through Alphabets
				{
					// Create new string with next character
					// Call generate again until string has reached it's length
					string newWord = word + Alphabets[i] ;
					wordLength++ ;
					generateWord(newWord, wordLength, wordLimit, tid) ;
				}

			}

			else
			{
				cout << "Thread n°" << tid << " +++ produce +++ : " << word << "\n" ; // Print text out
				{
					lock_guard<std::mutex> lock(this->mutex) ; // Lock mutex for this scope duration
					queue.push(word) ;
					//notified = true;
					//cond_var.notify_one();
				}

				std::this_thread::sleep_for(std::chrono::milliseconds(1)) ; // Sleep for 0,001 second

				return ;
			}
		}

		// Recursive function, keeps clocking characters until length is reached
		void Produce(string s, unsigned int threadBegin, unsigned int threadEnd, unsigned int tid)
		{
			cout << "Produce by thread n°" << tid << endl ;
	
			cout << "Produce()..."<< endl ;

			unsigned int size = 1 ;
			unsigned int i = threadBegin ;

			while(i < threadEnd) // Iterate through Alphabets
			{
				generateWord(Alphabets[i], 1, size, tid) ;

				if(i == (threadEnd - 1)) // For do a new loop
				{
					i = threadBegin ;
					size++ ;
				}

				else
				{
					i++ ;
				}
			}

			cout << "Produce() done !" << endl ;
		}

		void Consume(string hashFromUser, int tid)
		{
			cout << "Consume by thread n°" << tid << endl ;

			cout << "Consume()..." << endl ;

			std::this_thread::sleep_for(std::chrono::milliseconds(2)) ; // Sleep for 0,002 second

			while(!queue.empty())
			{
				{
					lock_guard<std::mutex> lock(mutex) ; 				// Lock mutex for this scope duration

					string text = queue.front() ; 						// Get the text from the global queue
					queue.pop() ; 										// Remove the text in the global queue

			    		cout << "Thread n°"<< tid << " --- consume --- : " << text << endl ;

					char data[text.size() + 1] ;
					strcpy(data, text.c_str()) ;

					string sha256 = SHA256(data) ; 						// Hash the text

					bool compare = compareHashes(hashFromUser, sha256) ; // Hashes comparison

					if(compare == 1)
					{
						cout << "The hash "<< sha256 << " matches with the text : " << text << endl ;
						cout << text << endl ;

						exit(0) ;		
					}
				}

				std::this_thread::sleep_for(std::chrono::milliseconds(2)) ; // Sleep for 0,002 second
			}

			cout << "Consume() done !" << endl ;
		}
} ;


int main(int argc, char* argv[])
{	
	/* To compile : 
		cd src
		cmake .. && make
	*/
	// To run : ./main 4 e061428097b79aab3d587dbb523e19f4e58b67346699208c81f8b5a9449ffe6f
	
	unsigned int nbThreadsEnterred = 0 ;
	string hashFromUser = "" ;
	
	switch (argc)
	{
		case 1 :
			cout << "No arguments find found. Try again with exactly two arguments\n" ;
	        	break ;
		case 2 :
	        	cout << "No arguments find found. Try again with exactly two arguments\n" ;
	        	break ;
	    	case 3 :
	    		nbThreadsEnterred = atoi(argv[1]) ;

			for(int i = 2 ; i < argc ; i++)
			{
				hashFromUser += argv[i] ; 												// Get the hash passed in argument
			}

	    		if(hashFromUser.length() == 64)
			{
	    			ProducerConsumer producerConsumer(nbThreadsEnterred, hashFromUser) ; // Proceed
	    		}

			else
			{
	    			cout << "Hash received : " << hashFromUser << endl ;
	    			cout << "Incorrect hash. Check that the number of chars are equal to 64\n" ;
	    		}
	    		break ;
	    	default :
	        	cout << "Too many things in the command line." ;
	        	exit(-1) ;
	        	break ;
	} ;

	return 0 ;
}

