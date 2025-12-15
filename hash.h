#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <cctype>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here
				
				//Getting the length of the string
				int stringLength=k.length(); 
				if (stringLength==0){return 0;} //early return if empty string


				int location = stringLength;
	
				//If not we will first create an initialize an array to store our chunks
				unsigned long long slots[5]= {0,0,0,0,0};

				for (int slot_index=4; slot_index>=0 && 0<location; slot_index--){

					//We are calculating the starting point, which will prevent us from going before the beginning of the string
					int begin;
					if (0<location-6){
						begin=location-6;
					}else{
						begin =0; 
					}

					//Converting this cunk into base 36:
					unsigned long long r=0;
					for (int i=begin; i<location; i++){
						HASH_INDEX_T digit =letterDigitToNumber(k[i]);
						r=r*36 + digit;
					}

					//Storing our value in array
					slots[slot_index]=r;

					//Moving left in the string; 
					location=begin; 
				}

				unsigned long long result=0;
				for (int i=0; i<5; i++){
					result+= (unsigned long long)(rValues[i]*slots[i]);
				}
				return result;


    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it
				
				//If letter is a digit:
				if (letter >='0' && letter<='9'){
					return (letter - '0')+26;
				}

				//Else, lowercase and return
				return tolower(letter)- 'a';
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
