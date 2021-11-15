#pragma once

#include <iostream>
#include <vector>
#include "hash.hpp"

using namespace std;

enum GITOP
{
    init,
    add,
    rm,
    commit,
    checkout,
    status
};

struct FileNode
{
    string name;
    int version;
    FileNode *next;
};

struct BranchNode
{
    int commit_id;
    string commit_message;
    BranchNode *next;
    BranchNode *previous;
    FileNode *file_head;
};

class MiniGit
{
private:
    BranchNode *commit_head;
    int commits;
    HashTable *hash_table;

public:
    MiniGit();
    ~MiniGit();

    void init(int table_size);
    void add(string file_name);
    void rm(string file_name);
    void search(string key);
    string commit(string msg);
    void checkout(string commit_id);
    void printSearchTable();
};