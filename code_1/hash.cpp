// CPP program to implement hashing with chaining
#include "hash.hpp"
#include <iostream>

using namespace std;

HashNode *HashTable::createNode(string key, HashNode *next)
{
    HashNode *node = new HashNode;
    node->key = key;
    node->next = next;
    return node;
}

HashTable::HashTable(int bsize)
{
    // intialize variable for underlying data structure
    table = new HashNode *[bsize];
    memset(table, 0, sizeof(HashNode *) * bsize);
    table_size = bsize;
}

HashTable::~HashTable()
{
    for (int i = 0; i < table_size; i++)
    {
        if (table[i] != nullptr)
        {
            HashNode *curr_node = table[i];
            while (curr_node != nullptr)
            {
                HashNode *toDelete = curr_node;
                curr_node = curr_node->next;
                delete toDelete;
            }
        }
    }

    delete[] table;
}

// function to calculate hash function
// implementation of SHA-1 hash function from https://github.com/vog/sha1
string HashTable::hashFunction(string key)
{
    SHA1 sha1;
    return sha1.final(key);
}

// function to search
HashNode *HashTable::searchItem(string key)
{
    // get base hash from key
    string hash = hashFunction(key);

    // split 20 byte hash into 5 4 byte sub hashes
    string sub_hash[5];
    sub_hash[0] = hash.substr(0, 8);
    sub_hash[1] = hash.substr(8, 8);
    sub_hash[2] = hash.substr(16, 8);
    sub_hash[3] = hash.substr(24, 8);
    sub_hash[4] = hash.substr(32, 8);

    // choose which block to use by modding key length by five
    string final_hash = sub_hash[key.length() % 5];

    // convert hexadecimal hash into decimal int
    unsigned int index = stoul(final_hash, nullptr, 16) % table_size;

    HashNode *curr_node = table[index];
    while (curr_node != nullptr && curr_node->key != key)
    {
        curr_node = curr_node->next;
    }

    if (curr_node != nullptr)
    {
        return curr_node;
    }
    else
    {
        throw runtime_error("No commits with key \"" + key + "\" could be found");
    }
}

// function to insert
bool HashTable::insertItem(string key, int commit_num)
{
    // get base hash from key
    string hash = hashFunction(key);

    // split 20 byte hash into 5 8 byte sub hashes
    string sub_hash[5];
    sub_hash[0] = hash.substr(0, 8);
    sub_hash[1] = hash.substr(8, 8);
    sub_hash[2] = hash.substr(16, 8);
    sub_hash[3] = hash.substr(24, 8);
    sub_hash[4] = hash.substr(32, 8);

    // choose which block to use by modding key length by five
    string final_hash = sub_hash[key.length() % 5];

    // convert hexadecimal hash into decimal int
    unsigned int index = stoul(final_hash, nullptr, 16) % table_size;

    // check if key already exists in table or not
    try
    {
        searchItem(key)->commit_nums.push_back(commit_num);

        return false;
    }
    catch (const exception &e)
    {
        table[index] = createNode(key, table[index]);
        table[index]->commit_nums.push_back(commit_num);

        return true;
    }
}

// function to display hash table
void HashTable::printTable()
{
    HashNode *curr_node = nullptr;
    for (int i = 0; i < table_size; i++)
    {
        cout << i << "|| ";

        curr_node = table[i];
        while (curr_node != nullptr)
        {
            cout << curr_node->key << "(";

            for (unsigned int j = 0; j < curr_node->commit_nums.size(); j++)
            {
                cout << curr_node->commit_nums[j] << ",";
            }

            cout << ")";

            if (curr_node->next != nullptr)
            {
                cout << "-->";
            }

            curr_node = curr_node->next;
        }

        cout << endl;
    }
}
