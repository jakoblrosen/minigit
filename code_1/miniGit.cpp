#include "miniGit.hpp"

using namespace std;

MiniGit::MiniGit()
{
    filesystem::remove_all(".minigit");
    filesystem::create_directory(".minigit");
}

MiniGit::~MiniGit()
{
    // any postprocessing that may be required
}

void MiniGit::init(int table_size)
{
    commit_head = new BranchNode;
    commits = 0;
    hash_table = new HashTable(table_size);
}

void MiniGit::add(string file_name)
{
    if (filesystem::exists(file_name))
    {
        bool found = false;
        FileNode *curr_node = commit_head->file_head;

        while (!found && curr_node != nullptr)
        {
            if (curr_node->name == file_name)
            {
                found = true;
            }
            else
            {
                curr_node = curr_node->next;
            }
        }

        if (!found)
        {
            FileNode *node = new FileNode;
            node->name = file_name;
            node->next = commit_head->file_head;

            int version = 0;
            bool next_version_found = false;

            while (!next_version_found)
            {
                next_version_found = !check_file(file_name, version, ".minigit/");

                if (!next_version_found)
                {
                    version++;
                }
            }

            node->version = version;

            commit_head->file_head = node;
        }
        else
        {
            exception e;
            throw e;
        }
    }
    else
    {
        exception e;
        throw e;
    }
}

void MiniGit::rm(string file_name)
{
    bool found = false;
    FileNode *curr_node = commit_head->file_head;
    FileNode *prev_node = nullptr;

    while (!found && curr_node != nullptr)
    {
        if (curr_node->name == file_name)
        {
            found = true;
        }
        else
        {
            prev_node = curr_node;
            curr_node = curr_node->next;
        }
    }

    if (found)
    {
        if (prev_node == nullptr)
        {
            commit_head->file_head = curr_node->next;
            delete curr_node;
        }
        else
        {
            prev_node->next = curr_node->next;
            delete curr_node;
        }
    }
    else
    {
        exception e;
        throw e;
    }
}

void MiniGit::printSearchTable()
{
    hash_table->printTable();
}

void MiniGit::search(string key)
{
    try
    {
        HashNode *node = hash_table->searchItem(key);
        cout << "Commits with the word \"" << key << "\": ";
        for (int i = 0; i < node->commit_nums.size(); i++)
        {
            cout << node->commit_nums[i] << ", ";
        }
        cout << endl;
    }
    catch (const exception &e)
    {
        cout << "No commits were found with the keyword \"" << key << "\"" << endl;
    }
}

string MiniGit::commit(string msg)
{
    // visit all file nodes for current commit to copy to .minigit/
    FileNode *crawler = commit_head->file_head;
    while (crawler != nullptr)
    {
        filesystem::path in_path = crawler->name;
        string file_name = minigit_file_name(crawler->name, crawler->version);
        filesystem::path out_path = minigit_file_path(file_name, ".minigit/");

        filesystem::copy_file(in_path, out_path);

        crawler = crawler->next;
    }

    // store words in message to hash table
    string word = "";
    stringstream ss(msg);
    while (!ss.eof())
    {
        getline(ss, word, ' ');
        hash_table->insertItem(word, commits);
    }

    // data processing
    commit_head->commit_id = commits;
    commit_head->commit_message = msg;

    BranchNode *node = new BranchNode;
    node->commit_id = commits + 1;
    commit_head->next = node;
    node->previous = commit_head;
    commit_head = node;

    string commit_id = to_string(commits);

    commits++;

    return commit_id;
}

void MiniGit::checkout(string commit_id)
{
    BranchNode *crawler = commit_head;
    bool found = crawler->commit_id == stoi(commit_id);

    while (!found && crawler->previous != nullptr)
    {
        crawler = crawler->previous;
        found = crawler->commit_id == stoi(commit_id);
    }

    if (found)
    {
        FileNode *file_crawler = crawler->file_head;
        while (file_crawler != nullptr)
        {
            filesystem::path out_path = file_crawler->name;
            string file_name = minigit_file_name(file_crawler->name, file_crawler->version);
            filesystem::path in_path = minigit_file_path(file_name, ".minigit/");

            filesystem::copy_file(in_path, out_path, filesystem::copy_options::overwrite_existing);

            file_crawler = file_crawler->next;
        }
    }
    else
    {
        exception e;
        throw e;
    }
}

string MiniGit::minigit_file_name(string file_name, int version_num)
{
    // create string for file in .minigit
    // example.txt -> example00.txt
    string out_file = "";
    int dot_index = file_name.find('.');
    int slash_index = file_name.find('/');
    string version = (version_num < 10) ? "0" + to_string(version_num) : to_string(version_num);
    out_file.append(file_name.substr(slash_index + 1, dot_index - (slash_index + 1)));
    out_file.append(version);
    out_file.append(file_name.substr(dot_index));

    return out_file;
}

filesystem::path MiniGit::minigit_file_path(string file_name, string directory)
{
    string path = "";
    path.append(directory);
    path.append(file_name);

    return path;
}

bool MiniGit::check_file(string file_name, int version, string directory)
{
    string path = minigit_file_path(minigit_file_name(file_name, version), directory);
    return filesystem::exists(path);
}