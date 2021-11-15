#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#include <filesystem>
namespace fs = std::filesystem;

#include "miniGit.hpp"
#include <vector>

MiniGit::MiniGit()
{
    fs::remove_all(".minigit");
    fs::create_directory(".minigit");
}

MiniGit::~MiniGit()
{
    // Any postprocessing that may be required
}

void MiniGit::init(int table_size)
{
}

void MiniGit::add(string file_name)
{
}

void MiniGit::rm(string file_name)
{
}

void MiniGit::printSearchTable()
{
    hash_table->printTable();
}

void MiniGit::search(string key)
{
}

string MiniGit::commit(string msg)
{
    return " "; // should return the commit_id of the commited DLL node
}

void MiniGit::checkout(string commit_id)
{
}
