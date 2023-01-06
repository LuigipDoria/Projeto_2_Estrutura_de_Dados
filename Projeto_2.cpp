#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <fstream>
#include <string>
#include <tuple>
#include "bits/stdc++.h"
using namespace std;
 
// Trie Node
struct TrieNode {
 
    // Using map to store the pointers
    // of children nodes for dynamic
    // implementation, for making the
    // program space efficient
    map<char, TrieNode*> children;
 
    // If isEndOfWord is true, then
    // node represents end of word
    bool isEndOfWord;
 
    // num represents number of times
    // a character has appeared during
    // insertion of the words in the
    // trie
    map<char, int> num;
    
    int begin_line;
    int num_character;
    bool criada;
};
 
// Declare root node
struct TrieNode* root;
 
// Function to create New Trie Node
struct TrieNode* getNewTrieNode() {
    struct TrieNode* pNode = new TrieNode;
    pNode->isEndOfWord = false;
    pNode->begin_line = 0;
    pNode->num_character = 0;
    pNode->criada = false;
    return pNode;
}
 
// Function to insert a string in trie
void insertWord(string word, int num_character, int begin_line) {
    
    // To hold the value of root
    struct TrieNode* current = root;
 
    // To hold letters of the word
    char s;
 
    // Traverse through strings in std::list<T> ;
    for (int i = 0; i < word.length(); i++) {
        s = word[i];
 
        // If s is not present in the
        // character field of current node
        if (current->children.find(s) == current->children.end()) {
            // Get new node
            struct TrieNode* p = getNewTrieNode();
 
            // Insert s in character
            // field of current node
            // with reference to node p
            (current->children)[s] = p;
 
            // Insert s in num field
            // of current node with
            // value 1
            (current->num)[s] = 1;
        } else {
            // Increment the count
            // corresponding to the
            // character s
            current->num[s] = (current->num)[s] + 1;
        }
        
        
        
        // Go to next node
        current = (current->children)[s];
    }
    current->criada = true;
    current->num_character = num_character;
    current->begin_line = begin_line;
    current->isEndOfWord = true;
}
 
// Function to count the number of
// words in trie with given prefix
tuple<int,int,int, bool> search(TrieNode* current, string prefix) {
    char s;
    bool palavra_completa;
    // Initialize the wordCount = 0
    int wordCount = 0;
    int begin;
    int num_character;
 
    for (int i = 0; prefix[i]; i++) {
        s = prefix[i];
        
        // If the complete prefix is
        // not present in the trie
        if (current->children.find(s) == current->children.end()) {
 
            // Make wordCount 0 and
            // break out of loop
            wordCount = 0;
            break;
        }
 
        // Update the wordCount
        wordCount = (current->num)[s];
 
        // Go to next node
        current = (current->children)[s];
        palavra_completa = current->isEndOfWord;
        begin = current->begin_line;
        num_character = current->num_character;
    }
    return make_tuple(wordCount, begin, num_character, palavra_completa);
}

int main() {
    string filename;
    string word;
    int number_of_words = 0;
    cin >> filename;  // entrada
    vector<string> words;
    while (1) {  // leitura das palavras até encontrar "0"
        cin >> word;
        if (word.compare("0") == 0) {
            break;
        } else {
            words.push_back(word);
            number_of_words++;
        }
    }
    vector<string> words_in_dict;
    ifstream archive (filename);
    if (archive.is_open())
    {
        string linha;
        while (getline(archive,linha))
        {
            string word_to_insert = "";
            bool new_insertion = false;
            for(int i=0;i<linha.length();++i)
            {
                if (new_insertion && linha[i] != ']')
                {
                    word_to_insert.push_back(linha[i]);
                }
                if (linha[i] == '[')
                {
                    new_insertion = true;
                } else if (linha[i] == ']') {
                    new_insertion = false;
                }
            }
            if (word_to_insert != "")
            {
                words_in_dict.push_back(word_to_insert);
            }
        }
    }
    archive.close();
    // output
    
    // Criação da arvore
    root = getNewTrieNode();
    
    // Size of list of string
    int n = words_in_dict.size();
    
    int num_palavras = 0;
    int num_character = 0;
    int begin_line = 0;
    // Abrindo o arquivo
    ifstream file (filename);
    if (file.is_open()) {
        int begin = -1;
        int n_linha = 0;
        string linha;
        while (getline(file,linha)) {
            size_t found = linha.find("["+words_in_dict[num_palavras]+"]");
            n_linha = n_linha + 1;
            if (found != string::npos) {
                num_character = linha.length();
                begin_line = begin + n_linha;
            }
            begin = begin + linha.length();
            // Construct trie containing
            // all the words
            insertWord(words_in_dict[num_palavras], num_character, begin_line);
            num_palavras++;
        }
    }    
    file.close();    
    struct TrieNode* current = root;
    
    bool palavra_completa;
    
    for(int i=0;i<number_of_words;++i) {
        // Parte 1
        int param, begin, num_character;
        tie(param, begin, num_character, palavra_completa) = search(current, words[i]);
        if (param == 0) {
            cout << words[i] << " is not prefix" << endl;
        } else {
            if (palavra_completa) {
                cout << words[i] << " is prefix of " << param << " words" << endl;
                // Parte 2
                cout << words[i] << " is " << "at (" << begin << "," << num_character << ")"<< endl;
            } else {
               if (param > 1) {
                cout << words[i] << " is prefix of " << param << " words" << endl;
                } 
            }
            
        }
    }
    return 0;
}
