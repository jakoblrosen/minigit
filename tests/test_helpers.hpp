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
std::string exec(const char *cmd);
std::string readFileIntoString(std::string file_name);
string test_hash(string key);
string test_insert(string arr[], int length, int table_size);
HashNode test_search(string arr[], int length, int table_size, string search_key);
#endif // TEST_H__