# CSCI 2275 – Data Structures - Final Project #

## Jakob Rosen ##

## MiniGit ##

### Overview ###

The MiniGit class is meant to be a smaller version of the popular tool Git. The functionality included allows users to initialize the vcs for a new repository, stage changes for a commit, remove staged changes from a commit, commit the staged changes to save the changes to the files, checkout a previosly made commit, and search for commits using words from the commit message.

### Usage ###

To start using MiniGit, navigate to the "```build/```" directory by entering "```cd build/```" in your terminal. Next setup CMake in the build directory with the command, "```cmake ..```". Finally, complile the project with the command, "```make```" and run it with "```./run_app_1```". You can now use MiniGit! For further detail and instruction, keep reading about the MiniGit functions.

### Init ###

When initializing MiniGit, you will be asked to provide a size for the hash table. A smaller hash table will be less intensive on memory, however a larger hash table will reduce collisions and get you closer to a perfect O(1) search time. It is also recommended that you choose a prime number for your table size as these have been shown to reduce collisions.

### Add ###

The *add* function is used to add files to a commit. Files added in previous commits will also be included in the current commit unless they are explicitly removed.

**NOTE:** *Files from previous commits will have their changes automatically added. If you want to update these files in the current commit, you will need to add them as well.*

### Remove ###

The *rm* function will remove a file from your current commit so the changes will not be stored in .minigit.

**NOTE:** *This function will not remove the source file from your filesystem, it will only stop the changes from being stored in .minigit.*

### Commit ###

The *commit* function is what will store your changes in the .minigit/ directory. You will also be asked to include a commit message. This should be a short and concise set of words that describe your commit. These words will also be used to store the commit num in the hash table for searching later.

### Checkout ###

The *checkout* function will restore files to the version that were part of the commit ID provided. This is a great way to look over your previous versions.

**WARNING:** **This function will overwrite the files included in the commit. If you don't want to lose any work please save all of your files with a new commit and then checkout the older commit.**

### Search ###

The *search* function will print the *commit_nums* associated a certain *key* word from a commit message. This is accomplished through the hash table *search* function.

## Hash Table ##

### Overview ###

This hash table implementation uses the SHA-1 hashing algorithm which will turn a key value (in this case a string) into a 20 byte or 40 digit hexadecimal digest. This hash table will resolve collisions with chaining, allowing for a relatively small table size. 

**NOTE:** *The table should not be too small, otherwise you lose the O(1) time complexity*

### Constructor ###

The constructor has an int parameter *bsize* which refers to the number of buckets in the hash table, or in other words the number of indexes. The underlying data structure used is a primitive array of *HashNode* pointers

### Hash Function ###

The *hash* function is the actual implementation of the SHA-1 hashing algorithm. The helper functions/operations for the hashing algorithm are defined in the header file to maintain readability and cleanliness. The input for this function is a string *key* and the output is a 40 digit hexadecimal string that is used by other functions to calculate indexes.

### Insert ###

The *insert* function will take in a string *key* as well as an int *commit_num*. The *key* value will be hashed to get the index that will be used to store *commit_num* in the table. This implementation of a hash table will resolve collisions through chaining, which allows for more data in a relatively small table. Each index in the array actually contains a linked list of the stored *keys* and *commit_nums*.

### Search ###

The *search* function will return a *HashNode* struct that can be used to retrieve the *key* value as well as associated *commit_nums*. The hashing process used to search for a *HashNode* is the same process used to insert one, so if a *HashNode* with the specified *key* exists, it will be found. If there is no *HashNode* with the specified *key*, an exception is thrown and handled accordingly.

## Tests ##

For running tests, navigate into the build directory and execute the command ```./run_tests```.

### MiniGit Tests ###

The MiniGit class has tests written for the *add* and *rm* functions. The tests test for expected outputs and exceptions given various inputs.

### Hash Table Tests ###

The hash table class has tests written to test the *hash* function, the *insert* function, and the *search* function. A write up for the derivation of the *insert* test cases can be found in the tests folder, entitled *test_insert_cases.txt*. The process starts with the SHA-1 hash of the key, followed by 4 bytes of the hash being selected for use in getting the index. These 4 bytes are converted to an unsigned int and then modded by the table size to get a usable index.


