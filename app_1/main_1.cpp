#include "../code_1/miniGit.hpp"

using namespace std;

/*
 * Purpose; displays a menu with options
 */
void displayMenu()
{
    cout << "Select a numerical option:" << endl;
    cout << "+=====Main Menu=========+" << endl;
    cout << " 1. init " << endl;
    cout << " 2. add " << endl;
    cout << " 3. rm " << endl;
    cout << " 4. commit " << endl;
    cout << " 5. checkout" << endl;
    cout << " 6. search" << endl;
    cout << " 7. quit " << endl;
    cout << "+-----------------------+" << endl;
    cout << "#> ";
}

int main(int argc, char *argv[])
{
    MiniGit minigit;
    int table_size = 5; // table_size capped at 5 for assignment

    int working_commit = 0;
    int current_commit = 0;

    string input_string;
    int input_int;
    bool initialized = false;
    bool quit = false;
    while (!quit)
    {
        input_string = "";
        input_int = 0;
        bool valid = false;

        displayMenu();
        getline(cin, input_string);
        cout << endl;

        // catch non numerical inputs as invalid inputs
        if (!input_string.empty() && input_string.find_first_not_of("0123456789") == string::npos)
        {
            input_int = stoi(input_string);
        }
        else
        {
            input_int = -1;
        }

        // user must initialize minigit before doing anything else
        if (!initialized && input_int != 1)
        {
            cout << "You have to initialize MiniGit before doing anything else!" << endl;
            input_int = -1;
        }

        // user cannot do anything other than checkout and search if they are not on the working commit
        if (current_commit != working_commit && !(input_int == 5 || input_int == 6))
        {
            cout << "You cannot perform any MiniGit operations on a commit that is not your working commit" << endl;
            cout << "Your working commit has ID: " << working_commit << endl;
            input_int = -1;
        }

        switch (input_int)
        {
        case 1:
            try
            {
                minigit.init(table_size);
                initialized = true;

                cout << "MiniGit initialized" << endl;
            }
            catch (const exception &e)
            {
                // user tries to initialize minigit after it has already been initialized
                cout << e.what() << endl;
            }

            break;

        case 2:
            while (!valid)
            {
                cout << "Please enter the relative path of the file you would like to add" << endl;

                getline(cin, input_string);

                try
                {
                    minigit.add(input_string);
                    valid = true;
                    cout << "File successfully added" << endl;
                }
                catch (const exception &e)
                {
                    // file was in the last commit, or the file couldn't be found
                    cout << e.what() << endl;
                }
            }

            break;

        case 3:
            cout << "Please enter the path of the file you would like to remove" << endl;

            getline(cin, input_string);

            try
            {
                minigit.rm(input_string);
                cout << "File successfully removed" << endl;
            }
            catch (const exception &e)
            {
                // file to remove couldn't be found in working commit
                cout << e.what() << endl;
            }

            break;

        case 4:
            while (!valid)
            {
                cout << "Please enter a commit message (between one and three words separated by spaces)" << endl;

                getline(cin, input_string);

                try
                {
                    string commit_id = minigit.commit(input_string);
                    valid = true;
                    cout << "Commit ID: " << commit_id << " has been committed" << endl;
                    working_commit = stoi(commit_id) + 1;
                    current_commit = working_commit;
                }
                catch (const exception &e)
                {
                    // commit message entered was invalid, or a file was deleted since adding it to the working commit
                    cout << e.what() << endl;
                }
            }

            break;

        case 5:
            cout << "Please enter the id of the commit you would like to check out" << endl;
            cout << "WARNING: Checking out a commit will overwrite the files included in that commit" << endl;
            cout << "Please make sure that everything is backed up or saved in a new commit" << endl;

            getline(cin, input_string);

            try
            {
                minigit.checkout(input_string);
                current_commit = stoi(input_string); // this line will only be hit if the commit ID is valid
            }
            catch (const exception &e)
            {
                // the commit ID entered was either invalid or could not be found
                cout << e.what() << endl;
            }

            break;

        case 6:
            cout << "Please enter a keyword to search" << endl;

            getline(cin, input_string);

            minigit.search(input_string);

            break;

        case 7:
            quit = true;

            break;

        default:
            cout << "Invalid option" << endl;

            break;
        }

        cout << endl;
    }

    return 0;
}