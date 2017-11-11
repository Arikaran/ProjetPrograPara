//#pragma once
#ifndef THREAD_H
#define THREAD_H
#include <iostream>
#include <string>

char* createThread(int i) ;
char* generateHash(char* s) ;
bool compareHash(char* h) ;
bool compareHashes(std::string hash1, std::string hash2);
void manageThread() ;

bool compareHashes(std::string hash1, std::string hash2)
{
	if (hash1.compare(hash2) == 0) return true;
	else return false;
}

#endif
