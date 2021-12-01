#ifndef TEST_H__
#define TEST_H__
#include <string>
#include <iostream>
#include <fstream>
#include <array>
#include "../code_1/hash.hpp"
#include "../code_1/miniGit.hpp"

using namespace std;

// TODO add detailed explanation on what the function should do
string exec(const char *cmd);
string readFileIntoString(string file_name);
string test_hash(string key);
string test_insert(string arr[], int length, int table_size);
HashNode test_search(string arr[], int length, int table_size, string search_key);
string test_add(string arr[], int length, int table_size, string commit_message);
string test_rm(string add[], int add_length, string rm[], int rm_length, int table_size, string commit_message);

#endif // TEST_H__