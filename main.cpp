#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include "divsufsort64.h"

using namespace std;
// cmake -DCMAKE_BUILD_TYPE="Release" -DBUILD_DIVSUFSORT64=1 ..

typedef long long LL;

vector<int64_t> get_suffix_array(string& s){
    int64_t n = s.size();
    vector<int64_t> v(n);
    for(int64_t i = 0; i < n; i++) v[i] = (int64_t) s[i];
    for(int64_t L = 1; L <= n; L *= 2){
        vector<pair<pair<int64_t,int64_t>, int64_t> > pairs(n);
        for(int64_t i = 0; i < n; i++){
            pairs[i] = {{v[i], (i + L < n) ? v[i+L] : -1}, i};
        }
        sort(pairs.begin(), pairs.end());
        int64_t rank = 0;
        for(int64_t i = 0; i < n; i++){
            if(i > 0 && pairs[i].first != pairs[i-1].first) rank++;
            v[pairs[i].second] = rank;
        }
    }
    vector<int64_t> SA(n);
    for(int64_t i = 0; i < n; i++) SA[v[i]] = i;
    return SA;
}

int main(int argc, char** argv){
    string S = "mississippi";
    string T = "asdaissipasda";
    string ST = S + "$" + T + "#";

    int64_t n = ST.size();

    // Compute the suffix array
    vector<int64_t> SA(n);
    divsufsort64((sauchar_t*)(&ST[0]), SA.data(), ST.size());

    vector<int64_t> ISA(n);
    for(int64_t i = 0; i < n; i++){
        ISA[SA[i]] = i;
    }

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

    cout << longest_match_length << endl;
    cout << longest_match_S_location << endl;
    cout << longest_match_T_location << endl;
    cout << S.substr(longest_match_S_location, longest_match_length) << endl;

}