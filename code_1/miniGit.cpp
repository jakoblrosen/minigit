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
    commit_head->commit_id = -1;
    commit_head->commit_message = "";
    commit_head->file_head = nullptr;
    commit_head->next = nullptr;
    commit_head->previous = nullptr;
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
            // case of adding a new file
            FileNode *node = new FileNode;
            node->name = file_name;
            node->next = commit_head->file_head;

            node->version = get_next_version(node->name);

            commit_head->file_head = node;
        }
        else
        {
            // file has already been updated
            throw runtime_error("The file path provided has already been added");
        }
    }
    else
    {
        // file does not exist
        throw runtime_error("The file path provided could not be found");
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
        throw runtime_error("The file provided does not exist in the current working commit");
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
        cout << e.what() << endl;
    }
}

string MiniGit::commit(string msg)
{
    // check that commit message is not empty, unique, and at most three words
    int word_count = 0;
    string placeholder;
    stringstream message_stream(msg);

    if (msg == "")
    {
        throw runtime_error("The commit message cannot be empty");
    }
    else
    {
        while (!message_stream.eof())
        {
            getline(message_stream, placeholder, ' ');
            word_count++;
        }

        if (word_count < 1 || word_count > 3)
        {
            throw runtime_error("The commit message provided is invalid");
        }
    }

    {
        BranchNode *message_crawler = commit_head->previous;
        while (message_crawler != nullptr && message_crawler->commit_message != msg)
        {
            message_crawler = message_crawler->previous;
        }

        if (message_crawler != nullptr)
        {
            throw runtime_error("The commit message provided has already been used before");
        }
    }

    FileNode *version_crawler = commit_head->file_head;
    while (version_crawler != nullptr)
    {
        // check if file has changed
        if (!compare_files(version_crawler->name, minigit_file_path(minigit_file_name(version_crawler->name, version_crawler->version), ".minigit/")))
        {
            // see if source file still exists
            if (!filesystem::exists(version_crawler->name))
            {
                throw runtime_error("File \"" + version_crawler->name + "\" could not be found");
            }
            else
            {
                version_crawler->version = get_next_version(version_crawler->name);
            }
        }

        version_crawler = version_crawler->next;
    }

    // visit all file nodes for current commit to copy to .minigit/
    FileNode *file_crawler = commit_head->file_head;
    while (file_crawler != nullptr)
    {
        filesystem::path in_path = file_crawler->name;
        string file_name = minigit_file_name(file_crawler->name, file_crawler->version);
        filesystem::path out_path = minigit_file_path(file_name, ".minigit/");

        // only store new or updated files
        if (!filesystem::exists(out_path))
        {
            filesystem::copy_file(in_path, out_path);
        }

        file_crawler = file_crawler->next;
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

    // prepare a new working commit
    BranchNode *node = new BranchNode;
    node->commit_id = -1; // set commit_id to -1 to disallow the user from checking out the working commit
    // copy over the SLL from the previous commit
    file_crawler = commit_head->file_head;
    while (file_crawler != nullptr)
    {
        FileNode *new_file = new FileNode;
        *new_file = *file_crawler;
        new_file->next = node->file_head;
        node->file_head = new_file;

        file_crawler = file_crawler->next;
    }
    commit_head->next = node;
    node->previous = commit_head;
    commit_head = node;

    string commit_id = to_string(commits);

    commits++;

    return commit_id;
}

void MiniGit::checkout(string commit_id)
{
    BranchNode *commit_crawler = commit_head;
    bool found = commit_crawler->commit_id == stoi(commit_id);

    if (commit_id.find_first_not_of("0123456789") != string::npos)
    {
        throw runtime_error("The commit ID provided is not a valid commit ID");
    }

    // check that commit_id >= 0
    if (stoi(commit_id) < 0)
    {
        throw runtime_error("The commit ID provided was invalid");
    }

    while (!found && commit_crawler->previous != nullptr)
    {
        commit_crawler = commit_crawler->previous;
        found = commit_crawler->commit_id == stoi(commit_id);
    }

    if (found)
    {
        FileNode *file_crawler = commit_crawler->file_head;
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
        throw runtime_error("The commit ID provided could not be found");
    }
}

string MiniGit::minigit_file_name(string file_name, int version_num)
{
    // create string for file in .minigit
    // example.txt -> example00.txt
    string out_file = "";
    int dot_index = file_name.find_last_of('.');
    int slash_index = file_name.find_last_of('/');
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

int MiniGit::get_next_version(string file_name)
{
    // get next available version number
    int version = 0;
    bool next_version_found = false;

    while (!next_version_found)
    {
        next_version_found = !check_file(file_name, version, ".minigit/");

        if (next_version_found)
        {
            break;
        }
        else
        {
            version++;
        }
    }

    return version;
}

// file comparison function from https://stackoverflow.com/questions/6163611/compare-two-files/37575457
bool MiniGit::compare_files(string source_file, string minigit_file)
{
    ifstream source_ifs(source_file, ifstream::binary | ifstream::ate);
    ifstream minigit_ifs(minigit_file, ifstream::binary | ifstream::ate);

    if (source_ifs.fail() || minigit_ifs.fail())
    {
        return false; // problem with files
    }

    if (source_ifs.tellg() != minigit_ifs.tellg())
    {
        return false; // sizes are not equal
    }

    // compare contents from beginning with equal()
    source_ifs.seekg(0, ifstream::beg);
    minigit_ifs.seekg(0, ifstream::beg);
    return equal(istreambuf_iterator<char>(source_ifs.rdbuf()),
                 istreambuf_iterator<char>(),
                 istreambuf_iterator<char>(minigit_ifs.rdbuf()));
}