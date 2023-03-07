#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <fstream>
#include "divsufsort64.h"

using namespace std;

typedef long long LL;

// https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
string read_file(string filename){
    std::ifstream t(filename);
    if(!t.good()){
        cerr << "Error opening file " << filename << endl;
        exit(1);
    }
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string buffer(size, ' ');
    t.seekg(0);
    t.read(&buffer[0], size);
    return buffer;
}

char FIRST_SEPARATOR = '$';
char SECOND_SEPARATOR = '#';

bool alphabet_contains_separators(const string& S){
    for(char c : S){
        if(c == FIRST_SEPARATOR || c == SECOND_SEPARATOR){
            return true;
        }
    }
    return false;
}

int main(int argc, char** argv){

    if(argc == 1){
        cerr << "Usage: " << argv[0] << " example/file1.txt example/file2.txt" << endl << endl;
        cerr << "Prints two lines. On the first line, three space-separated integers: the length of the longest match, starting position in file1.txt, starting position in file2.txt" << endl;
        cerr << "On the next line, prints the longest match as a string" << endl;
        return 1;
    }

    string S = read_file(argv[1]);
    string T = read_file(argv[2]);

    if(alphabet_contains_separators(S)){
        cerr << "Error: first input file contains a separator character '$' or '#'" << endl;
        return 1;
    }

    if(alphabet_contains_separators(T)){
        cerr << "Error: second input file contains a separator character '$' or '#'" << endl;
        return 1;
    }

    string ST = S + "$" + T + "#";

    int64_t n = ST.size();

    // Compute the suffix array
    vector<int64_t> SA(n);
    divsufsort64((sauchar_t*)(&ST[0]), SA.data(), ST.size());

    // Compute the inverse suffix array
    vector<int64_t> ISA(n);
    for(int64_t i = 0; i < n; i++){
        ISA[SA[i]] = i;
    }

    // Build the LCP array
    vector<int64_t> LCP(n);

    int64_t match_length = 0;
    for(int64_t i = 0; i < n; i++){
        match_length = max((int64_t) 0, match_length - 1); // Drop one character
        int64_t lex = ISA[i];
        if(lex > 0){
            int64_t j = SA[lex-1];
            while(i + match_length < n && j + match_length < n){
                if(ST[i + match_length] == ST[j + match_length]){
                    match_length++;
                } else break; // Mismatch
            }
        } else{
            match_length = 0;
        }
        LCP[lex] = match_length;
    }

    // Find the longest common match
    int64_t longest_match_length = -1;
    int64_t longest_match_S_location = -1;
    int64_t longest_match_T_location = -1;
    for(int64_t i = 1; i < n; i++){
        bool in_S = SA[i] < S.size() || SA[i-1] < S.size();
        bool in_T = SA[i] > S.size() || SA[i-1] > S.size();
        if(in_S && in_T && LCP[i] > longest_match_length){
            longest_match_length = LCP[i];
            longest_match_S_location = min(SA[i], SA[i-1]);
            longest_match_T_location = max(SA[i], SA[i-1]) - S.size() - 1;
        }
    }

    if(longest_match_length <= 0){
        // -1 happens very rarely in the case where there are characters smaller than '$' and '#'
        // in the inputs, and there is no match, and no two lexicographically adjacent suffixes start
        // from different strings.
        longest_match_length = 0;
        longest_match_S_location = 0;
        longest_match_T_location = 0;
    }
    // Print the answer
    cout << longest_match_length << " " << longest_match_S_location << " " << longest_match_T_location << endl;
    cout << S.substr(longest_match_S_location, longest_match_length) << endl;

}