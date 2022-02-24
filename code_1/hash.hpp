#ifndef HASH_HPP
#define HASH_HPP

#include "sha1.hpp"
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

struct HashNode // hash node datastructure
{
    string key;
    vector<int> commit_nums;
    HashNode *next;
};

class HashTable
{
private:
    int table_size; // No. of buckets (linked lists)

    // Pointer to an array containing buckets
    HashNode **table;

    HashNode *createNode(string key, HashNode *next);

public:
    HashTable(int bsize); // Constructor
    ~HashTable();

    // hash function to map values to key
    string hashFunction(string key);

    // inserts a key into hash table
    bool insertItem(string key, int commit_num);

    void printTable();

    HashNode *searchItem(string key);
};

#endif
