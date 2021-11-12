// CPP program to implement hashing with chaining
#include <iostream>
#include "hash.hpp"

using namespace std;

HashNode *HashTable::createNode(string key, HashNode *next)
{
    HashNode *nw = nullptr;
    return nw;
}

HashTable::HashTable(int bsize)
{
    // intialize variable for underlying data structure
    table = new HashNode *;
    *table = new HashNode[bsize];
    tableSize = bsize;

    // initialize variables for hash function
    reset(digest, buffer, transforms);
}

// function to calculate hash function
// implementation of SHA-1 hash function
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
    string hash = hashFunction(key);

    return nullptr;
}

// function to insert
bool HashTable::insertItem(string key, int cNum)
{
    // get base hash from key
    string hash = hashFunction(key);
    // TODO convert string hash into int index

    return false;
}

// function to display hash table //
/* assume the table size is 5. For each bucket it will show the
** the string key and the commit number (separated by comma) within parenthesis
** e.g. the key is science and commit numbers are 1 and 4. The key science
** is hashed to position 0. So the print format will be-

0|| science(1,4,)
1||
2||
3||
4|| difficult(3,)-->fun(2,)-->computer(0,)

*/
void HashTable::printTable()
{
}
