#ifndef RECCHECK
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <random>
#include <iomanip>
#include <fstream>
#include <exception>
#endif

#include "boggle.h"

std::vector<std::vector<char> > genBoard(unsigned int n, int seed)
{
	//random number generator
	std::mt19937 r(seed);

	//scrabble letter frequencies
	//A-9, B-2, C-2, D-4, E-12, F-2, G-3, H-2, I-9, J-1, K-1, L-4, M-2, 
	//N-6, O-8, P-2, Q-1, R-6, S-4, T-6, U-4, V-2, W-2, X-1, Y-2, Z-1
	int freq[26] = {9,2,2,4,12,2,3,2,9,1,1,4,2,6,8,2,1,6,4,6,4,2,2,1,2,1};
	std::vector<char> letters;
	for(char c='A'; c<='Z';c++)
	{
		for(int i=0;i<freq[c-'A'];i++)
		{
			letters.push_back(c);
		}
	}
	std::vector<std::vector<char> > board(n);
	for(unsigned int i=0;i<n;i++)
	{
		board[i].resize(n);
		for(unsigned  int j=0;j<n;j++)
		{
			board[i][j] = letters[(r() % letters.size())];
		}
	}
	return board;
}

void printBoard(const std::vector<std::vector<char> >& board)
{
	unsigned int n = board.size();
	for(unsigned int i=0;i<n;i++)
	{
		for(unsigned int j=0;j<n;j++)
		{
			std::cout << std::setw(2) << board[i][j];
		}
		std::cout << std::endl;
	}
}

std::pair<std::set<std::string>, std::set<std::string> > parseDict(std::string fname)
{
	std::ifstream dictfs(fname.c_str());
	if(dictfs.fail())
	{
		throw std::invalid_argument("unable to open dictionary file");
	} 
	std::set<std::string> dict;
	std::set<std::string> prefix;
	std::string word;
	while(dictfs >> word)
	{
		dict.insert(word);
		for(unsigned int i=word.size()-1;i>=1;i--)
		{
			prefix.insert(word.substr(0,i));
		}
	}
	prefix.insert("");
	return make_pair(dict, prefix);
}

std::set<std::string> boggle(const std::set<std::string>& dict, const std::set<std::string>& prefix, const std::vector<std::vector<char> >& board)
{
	std::set<std::string> result;
	for(unsigned int i=0;i<board.size();i++)
	{
		for(unsigned int j=0;j<board.size();j++)
		{
			boggleHelper(dict, prefix, board, "", result, i, j, 0, 1);
			boggleHelper(dict, prefix, board, "", result, i, j, 1, 0);
			boggleHelper(dict, prefix, board, "", result, i, j, 1, 1);
		}
	}
	
	return result;
}

bool boggleHelper(const std::set<std::string>& dict, const std::set<std::string>& prefix, const std::vector<std::vector<char> >& board, 
								   std::string word, std::set<std::string>& result, unsigned int r, unsigned int c, int dr, int dc)
{
//add your solution here!

		//BASECASE: Let's get the board size and check for bounds: #rows= #columns
		unsigned int bSize= board.size();
		if (r>=bSize || c>= bSize){ //PARAMETERS OUT OF BOUNDS, return early
			return false;
		}

		//STEP 1: Let's build a word by adding chars
		std::string wordNew= word+ board[r][c]; //This will add the current position, will use in recursion


		//STEP 2: We can check if the word generated is in the dictionary
		bool inDict= dict.find(wordNew)!=dict.end();



		bool longerExists=false; //Variable to tracck whether a longer word exists beyond the current, initialized to false

		//STEP 3: BACKTRACKING: If this word is not a valid prefix and not in the dictionary, we can return early
		if (prefix.find(wordNew)==prefix.end()){
			if (!inDict){
				return false; //Dead end, stop recursion on this path
				}
		}

		//STEP 4: We can recurse since we know this word is a valid prefix
		else{
			//STEP 5: Setting coordinates for recursion 
			int nc=dc+c;
			int nr=dr+r;

			//STEP 6: Recurse and see if there is a longer word
			longerExists=boggleHelper(dict, prefix, board, wordNew, result, nr, nc, dr, dc);
		}

		//STEP 7: If there is no longer word and is a valid word, insert into results and return true
		if (!longerExists && (inDict)){
			result.insert(wordNew);
		}

		//STEP 8: If the word forms a longer word or itself is a word, return true 
		return (inDict || longerExists);
}