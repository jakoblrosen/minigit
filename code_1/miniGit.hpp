#pragma once

#include <filesystem>
#include <iostream>
#include <fstream>
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

    string minigit_file_name(string file_name, int version);
    filesystem::path minigit_file_path(string file_name, string directory);
    bool check_file(string file_name, int version, string directory);
    int get_next_version(string file_name);
    bool compare_files(string source_file, string minigit_file);

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