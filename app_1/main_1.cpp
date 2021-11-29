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
    bool quit = false;
    while (!quit)
    {
        input_string = "";
        input_int = 0;

        displayMenu();
        getline(cin, input_string);
        cout << endl;

        input_int = stoi(input_string);

        switch (input_int)
        {
        case 1:
            minigit.init(table_size);

            break;

        case 2:
            cout << "Please enter the  path of the file you would like to add" << endl;

            getline(cin, input_string);

            try
            {
                minigit.add(input_string);
            }
            catch (const exception &e)
            {
                cout << "File path entered does not exist" << endl;
            }

            break;

        case 3:
            cout << "Please enter the path of the file you would like to remove" << endl;

            getline(cin, input_string);

            try
            {
                minigit.rm(input_string);
            }
            catch (const exception &e)
            {
                cout << "File not found in current commit" << endl;
            }

            break;

        case 4:
            cout << "Please enter a commit message (about three words separated by spaces)" << endl;

            getline(cin, input_string);

            minigit.commit(input_string);

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
                cout << "A commit with id \"" << input_string << "\" could not be found" << endl;
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

        cout << endl << endl;
    }

    return 0;
}