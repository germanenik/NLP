/*
 * Description: this program asks the user to input the name of a file and then
 * generates random text (of adjustable length) based on the value of N the user provides.
 * The value of N, provided by the user, regulates randomness: the lower the value of N is,
 * the more random the text is.
 *
*/
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cstdlib>
#include <random>

using namespace std;

vector<string> readFile(ifstream &input);
void writeNGrams(unordered_map<queue<string>, vector<string> >& nGrams, int numOfWords);

int main()
{
    cout << "Welcome to Random Writer ('N-Grams')!" << endl
         << "This program generates random text based on a document." << endl
         << "Give me an input file and an 'N' value for groups" << endl
         << "of words, and I'll create random text for you." << endl << endl;

    ifstream input;
    input.open("text.txt");
    int n = 3; //n-number
    int numOfWords = 40;

    vector<string> text;
    unordered_map<queue<string>, vector<string> > nGrams;

    cout << "...";
    writeNGrams(nGrams, numOfWords);
    cout << "..." << endl << endl;

    input.close();
    return 0;
}

vector<string> readFile(ifstream &input){
    string word;
    vector<string> words;

    while (input >> word) {
        words.push_back(word);
    }
}

/*
 * Generates a Map of N-Grams.
 * Accepts the vector of strings that contains the whole text (by reference)
 * and the integer N which dictates the randomness of the future output text.
 * Returns a HashMap of Queues of strings as keys and of Vectors of string as values.
 */
unordered_map<queue<string>, vector<string> > buildHashMap(vector<string> & text, int N){
    unordered_map<queue<string>, vector<string> > nGrams;
    size_t textSize = text.size();

    for(size_t i = 0; i < text.size(); i++){
        queue<string> prefix;
        for(size_t n = 0; n < N - 1; n++){
            prefix.push(text[ (i + n) % textSize ]);
        }

        string suffix = text[ (i + N - 1) % text.size()];

        if(nGrams.count(prefix)){
            nGrams[prefix].push_back(suffix);
        } else {
            nGrams[prefix] = {suffix};
        }

        prefix = {};
    }

    return nGrams;
}

/*
 * Finally generates the random text.
 * Accepts the nGrams HashMap (by reference) and the number of words to print out as an integer.
 * Outputs the random text but is void and doesn't return anything.
 */
void writeText(unordered_map<queue<string>, vector<string> > & nGrams, int numOfWords){

    auto iter = nGrams.begin();
    int randomNum = rand() % nGrams.size();

    //propagate
    for(int i = 0; i < randomNum; i++){
        iter++;
    }

    queue<string> window = (*iter).first;
    string word;
    for (int i = 0; i < window.size(); i++){
        word = window.front();
        window.pop();
        cout << word << " ";
        window.push(word);
    }


    vector<string> values = nGrams[window];
    size_t sizeV = values.size();
    while (numOfWords > 0) {
        string nextWord = values.at(rand() % sizeV );
        cout << nextWord << " ";
        window.pop();
        window.push(nextWord);

        numOfWords--;
    }
}


