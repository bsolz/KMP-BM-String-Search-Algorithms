#include <iostream>
#include <vector>
#include <utility>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <string>


int KMP(std::string key, std::string Text);
int main(){
    // Choose Key
    std::string key = "TATTLETALE";
    // Choose text file to work on and place contents in one string
    std::ifstream file("250.txt");
    std::string Text;
    std::string str;

    while (getline(file, str))
    {
        Text += str;
    }

    // Run algorithm and have it output how many times the word appears in the text file

    std::cout << KMP(key, Text) << '\n';
    // Output run time
    return 0;
}

int KMP(std::string key, std::string Text){

    // Get length of searched string and key
    int keyLength = key.size();
    int textLength =  Text.size();


    // Create the LPS array
    int lpsArray[keyLength];
    lpsArray[0] = 0;
    int i = 1;
    int counter = 0;
    while(i < keyLength){
        if(key[i] == key[counter]){
            counter++;
            lpsArray[i] = counter;
            i++;
        }
        if(key[i] != key[counter]){
            if(counter > 0){
                counter = lpsArray[counter - 1];
            }
            else{
                lpsArray[i] = 0;
                i++;
            }
        }
    }

    std::cout << "LPS Array: ";
    for(int i = 0; i < keyLength; i++){
        std::cout <<  lpsArray[i] << ' ';
    }
    std::cout << '\n';
    // Use a while loop to traverse the searched string
    // Can't do for loop because the amount of i increments by is dynamic

    int Q, W;
    Q = 0;
    W = 0;

    int FinalCounter = 0;
    while(Q < textLength){
        if(key[W] == Text[Q]){
            W++;
            Q++;
        }

        if(W == keyLength){
            FinalCounter++;
            W = lpsArray[W - 1];
        }

        else if(Q < textLength && key[W] != Text[Q]){
            if(W == 0){
                Q++;
            }
            else{
                W = lpsArray[W - 1];
            }
        }
    }
    return FinalCounter;
}



/*
TATTLETALE
*/
