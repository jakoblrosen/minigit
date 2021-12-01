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
    int table_size = 10; // table_size may be entered by user

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

        input_string.find_first_not_of("0123456789") || input_string == "" ? input_int = stoi(input_string) : input_int = -1;

        if (!initialized && input_int != 1)
        {
            cout << "You have to initialize MiniGit before doing anything else!" << endl;
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
                cout << e.what() << endl;
            }

            break;

        case 2:
            while (!valid)
            {
                cout << "Please enter the  path of the file you would like to add" << endl;

                getline(cin, input_string);

                try
                {
                    minigit.add(input_string);
                    valid = true;
                    cout << "File successfully added" << endl;
                }
                catch (const exception &e)
                {
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
                }
                catch (const exception &e)
                {
                    cout << e.what() << endl;
                }
            }

            break;

        case 5:
            cout << "Please enter the id of the commit you would like to check out" << endl;

            getline(cin, input_string);

            try
            {
                minigit.checkout(input_string);
            }
            catch (const exception &e)
            {
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