// CPP program to implement hashing with chaining
#include <iostream>
#include "hash.hpp"

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
    tableSize = bsize;

    // initialize variables for hash function
    reset(digest, buffer, transforms);
}

HashTable::~HashTable()
{
    for (int i = 0; i < tableSize; i++)
    {
        if (table[i] != nullptr)
        {
            HashNode *currNode = table[i];
            while (currNode != nullptr)
            {
                HashNode *toDelete = currNode;
                currNode = currNode->next;
                delete toDelete;
            }
        }
    }

    delete [] table;
}

// function to calculate hash function
// implementation of SHA-1 hash function from https://github.com/vog/sha1
string HashTable::hashFunction(string s)
{
    istringstream is(s);

    while (true)
    {
        char sbuf[BLOCK_BYTES];
        is.read(sbuf, BLOCK_BYTES - buffer.size());
        buffer.append(sbuf, (size_t)is.gcount());
        if (buffer.size() != BLOCK_BYTES)
        {
            break;
        }
        uint32_t block[BLOCK_INTS];
        buffer_to_block(buffer, block);
        transform(digest, block, transforms);
        buffer.clear();
    }

    // total number of hashed bits
    uint64_t total_bits = (transforms * BLOCK_BYTES + buffer.size()) * 8;

    // padding
    buffer += (char)0x80;
    size_t orig_size = buffer.size();
    while (buffer.size() < BLOCK_BYTES)
    {
        buffer += (char)0x00;
    }

    uint32_t block[BLOCK_INTS];
    buffer_to_block(buffer, block);

    if (orig_size > BLOCK_BYTES - 8)
    {
        transform(digest, block, transforms);
        for (size_t i = 0; i < BLOCK_INTS - 2; i++)
        {
            block[i] = 0;
        }
    }

    // append total_bits, split this uint64_t into two uint32_t
    block[BLOCK_INTS - 1] = (uint32_t)total_bits;
    block[BLOCK_INTS - 2] = (uint32_t)(total_bits >> 32);
    transform(digest, block, transforms);

    ostringstream result;
    for (size_t i = 0; i < sizeof(digest) / sizeof(digest[0]); i++)
    {
        result << hex << setfill('0') << setw(8);
        result << digest[i];
    }

    // reset for next run
    reset(digest, buffer, transforms);

    return result.str();
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
    unsigned int index = stoul(final_hash, nullptr, 16) % tableSize;

    HashNode *currNode = table[index];
    while (currNode != nullptr && currNode->key != key)
    {
        currNode = currNode->next;
    }

    return currNode;
}

// function to insert
bool HashTable::insertItem(string key, int cNum)
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
    unsigned int index = stoul(final_hash, nullptr, 16) % tableSize;

    // check if key already exists in table or not
    if (searchItem(key) == nullptr)
    {
        table[index] = createNode(key, table[index]);
        table[index]->commitNums.push_back(cNum);

        return true;
    }
    else
    {
        searchItem(key)->commitNums.push_back(cNum);

        return false;
    }
}

// function to display hash table
void HashTable::printTable()
{
    HashNode *currNode = nullptr;
    for (int i = 0; i < tableSize; i++)
    {
        cout << i << "|| ";

        currNode = table[i];
        while (currNode != nullptr)
        {
            cout << currNode->key << "(";

            for (unsigned int j = 0; j < currNode->commitNums.size(); j++)
            {
                cout << currNode->commitNums[j] << ",";
            }

            cout << ")";

            if (currNode->next != nullptr)
            {
                cout << "-->";
            }

            currNode = currNode->next;
        }

        cout << endl;
    }
}
