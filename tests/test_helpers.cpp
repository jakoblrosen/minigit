#include <gtest/gtest.h>
#include "test_helpers.hpp"

string exec(const char *cmd)
{
    array<char, 128> buffer;
    string result;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe)
    {
        throw runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }
    return result;
}

string readFileIntoString(string file_name)
{
    string line, out;
    ifstream ifs;
    ifs.open(file_name);
    if (!ifs)
    {
        throw runtime_error("could not open file: " + file_name);
    }
    while (getline(ifs, line))
    {
        out = out + line + "\n";
    }
    return out;
}

string test_hash(string key)
{
    HashTable table(0);

    return table.hashFunction(key);
}

string test_insert(string arr[], int length, int table_size)
{
    HashTable table(table_size);

    for (int i = 0; i < length; i++)
    {
        table.insertItem(arr[i], i);
    }

    testing::internal::CaptureStdout();
    table.printTable();
    string output = testing::internal::GetCapturedStdout();
    return output;
}

HashNode test_search(string arr[], int length, int table_size, string search_key)
{
    HashTable table(table_size);

    for (int i = 0; i < length; i++)
    {
        table.insertItem(arr[i], i);
    }

    HashNode *node = table.searchItem(search_key);
    return *node;
}

string test_add(string arr[], int length, int table_size, string commit_message)
{
    MiniGit minigit;

    minigit.init(table_size);

    for (int i = 0; i < length; i++)
    {
        minigit.add(arr[i]);
    }

    minigit.commit(commit_message);

    testing::internal::CaptureStdout();
    minigit.printSearchTable();
    string output = testing::internal::GetCapturedStdout();
    return output;
}

string test_rm(string add[], int add_length, string rm[], int rm_length, int table_size, string commit_message)
{
    MiniGit minigit;

    minigit.init(table_size);

    for (int i = 0; i < add_length; i++)
    {
        minigit.add(add[i]);
    }

    for (int i = 0; i < rm_length; i++)
    {
        minigit.rm(rm[i]);
    }

    minigit.commit(commit_message);

    testing::internal::CaptureStdout();
    minigit.printSearchTable();
    string output = testing::internal::GetCapturedStdout();
    return output;
}