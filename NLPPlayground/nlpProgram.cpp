/*
 This program determines similarity between two texts by calculating
 how many function words are used in each text. Helps determine authorship
 of an unknown text if provided texts by other authors.
 */

#include <iostream>
#include <fstream>
#include "hello.h"
#include <vector>
#include <cmath>
#include <numeric>
#include <regex>
using namespace std;

void pickFile(ifstream& file);
vector<string> createFeature(string fileName);
string fileToString(ifstream& input);
double getSimilarity(const string f1, const string f2, const vector<string>& features);
vector<int> createDocVector(const string text, const vector<string>& features);
int count_occurences(const string& feature, /*const */string& text);
double getDotProduct(const vector<int>& docVec1, const vector<int>& docVec2);
double mag(const vector<int> vec);

int main() {

//    create features - vector of stop words
    vector<string> features = createFeature("stopWords.txt");

    ifstream file1, file2;

    cout << "Pick the first file to compare." << endl;
    pickFile(file1);
    cout << "Pick the second file to compare." << endl;
    pickFile(file2);

    //read files into strings
    string f1 = fileToString(file1);
    string f2 = fileToString(file2);


    //compute similary between texts
    cout << "Similarity between text1 <-> text2: " << getSimilarity(f1, f2, features) << "%" << endl;
    return 0;
}

/**
 * This function accepts user's input of a filename and tried to open it. If no file exists,
 * the use is prompted again.
 */
void pickFile(ifstream& file){
    string name;

    while (true) {
       cout << "File name: ";
       cout.flush();
       cin >> name;
       file.open(name);
       if(file){
           cout << endl;
           break;
       } else {
           cerr << "Could not open file. Try again." << endl;
       }
    }
}

/*
 This function creates a vector of function words to be used to calculate
 similarity.
 */
vector<string> createFeature(string fileName){
    ifstream function(fileName);

    vector<string> functionWords {};

    string word;
    while(getline(function, word)){
        functionWords.push_back(word);
    }

    return functionWords;
}

/*
 This function converts the contents of the file into a string.
 */
string fileToString(ifstream& input){
    string ret;
    string line;
    while(getline(input, line)){
        ret += line + " ";
    }
    return ret;
}

/*
 This function calculates the similarity of two vectors by returning
 their dot product divided by their magnitudes.
 */
double getSimilarity(const string f1, const string f2, const vector<string>& features){
    vector<int> docVec1 = createDocVector(f1, features);
    vector<int> docVec2 = createDocVector(f2, features);

    //get cosine similarity
    //cos sum = dot of vec1 and vec2 divided by their magnitudes

    double dotProduct = getDotProduct(docVec1, docVec2);
    double cosOfTheta = dotProduct / mag(docVec1) / mag(docVec2);

    return dotProduct == 0 ? 0 : (cosOfTheta * 100);
}

/*
 This function goes through the string and every time it finds
 a function word contained in the features vector, it increments
 the counter at the corresponding positions in the features vector.
 */
vector<int> createDocVector(const string text, const vector<string>& features){
    vector<int> frequencies;
    string _text;

    for(const string& word : features){
        _text = text;
        frequencies.push_back(count_occurences(word, _text));
    }
    
    return frequencies;
}

/*
 This function calculates the dot product of two vectors.
 */
double getDotProduct(const vector<int>& docVec1, const vector<int>& docVec2){
    if(docVec1.size() != docVec2.size()){
        throw "Mismatched sizes";
    }

    return std::inner_product(docVec1.begin(), docVec1.end(), docVec2.begin(), 0);
}

/*
 This function returns the magnitude of a vector.
 */
double mag(const vector<int> vec){
    return sqrt(getDotProduct(vec, vec));
}

int count_occurences(const string& word, string& text){
    int counter = 0;

    //regex word formatting
    char c = *(word.begin());
    string upperOrLower = string(1, toupper(c)) + string(1, tolower(c));
    regex r("(^|[^[:alnum:]])[" + upperOrLower + "]" + word.substr(1) + "[^[:alnum:]]");

    smatch m;

    while(regex_search(text, m, r)){
        ++counter;
        text = m.suffix().str();
    }

    return counter;
}

/* NOTES
 *
 * every author has a writing style - usually in a consistent style
 * literary fingerprint
 *
 * Idea:
 * need a writer invariant - something that doesn't change with different writing
 * NPL idea: normal words and function words (syntactic glue of the language)
 * find the frequency of appearance of each function word
 * project them as vector and find correlation coefficient: cos(theta) = u•v/(||u|| * ||v||)
 */
