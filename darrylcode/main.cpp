#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <fstream>
#include <sstream>

void getFileContent(std::string fileName, std::vector< char > & vec){
    std::ifstream file;
    file.exceptions(
            std::ifstream::badbit
            | std::ifstream::failbit
            | std::ifstream::eofbit);
    file.open(fileName, std::ifstream::in | std::ifstream::binary);
    file.seekg(0, std::ios::end);
    std::streampos length(file.tellg());
    if (length){
        file.seekg(0, std::ios::beg);
        vec.resize(static_cast<std::size_t>(length));
        file.read(&vec.front(), static_cast<std::size_t>(length));
    }
}

int findMax(int a, int b){
    return (a > b) ? a : b;
}

void badCharHeuristic(char *str, int size, int badchar[256]){
    for (int i = 0; i < 256; i++) {
        badchar[i] = -1;
    }
    for (int i = 0; i < size; i++) {
        badchar[(int) str[i]] = i;
    }
}

void search(char *txt, char *pat){
    int patternSize = strlen(pat);
    int textSize = strlen(txt);
    int s = 0;

    int badchar[256];

    badCharHeuristic(pat, patternSize, badchar);

    while (s <= (textSize - patternSize)){
        int j = patternSize - 1;

        while (j >= 0 && pat[j] == txt[s + j]) {
            j--;
        }

        if (j < 0){
            std::cout << "pattern occurs at shift = " << s + 1 << std::endl;
            s += (s + patternSize < textSize) ? patternSize - badchar[txt[s + patternSize]] : 1;
        }
        else {
            s += findMax(1, j - badchar[txt[s + j]]);
        }
    }
}


void rabinKarp(std::string pattern, std::vector< char > text, int q){
    int patternSize = pattern.length();
    int textSize = text.size();
    int i, j;
    int patternHashVal = 0;
    int textHashVal = 0;
    int h = 1;

    for (i = 0; i < patternSize - 1; i++) {
        h = (h * 10) % q;
    }
    for (i = 0; i < patternSize; i++){
        patternHashVal = (10 * patternHashVal + pattern[i]) % q;
        textHashVal = (10 * textHashVal + text[i]) % q;
    }
    for (i = 0; i < textSize - patternSize; i++){
        if (patternHashVal == textHashVal){
            for (j = 0; j < patternSize; j++) {
                if (text[i + j] != pattern[j]) {
                    break;
                }
            }
            if (j == patternSize) {
                std::cout << "Pattern is found at position: " << j + 1 << std::endl;
            }
        }
        if (i < textSize - patternSize){
            textHashVal = (10 * (textHashVal - text[i] * h) + text[i + patternSize]) % q;
            if (textHashVal < 0) {
                textHashVal = (textHashVal + q);
            }
        }
    }
}

int main(){
    std::vector < char > vecOfStr;
    std::vector < int > holdPos;
    getFileContent("text.txt", vecOfStr);



    std::string pattern = "CDD";
    int q = 13;
    rabinKarp(pattern, vecOfStr, q);
    //search(vecOfStr, pattern);
  }
