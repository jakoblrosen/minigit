#include <gtest/gtest.h>

#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <array>
#include <vector>
#include "test_helpers.hpp"

using namespace std;

class test_x : public ::testing::Test
{
protected:
	// this function runs only once before any TEST_F function
	static void SetUpTestCase() {}

	// this function runs after all TEST_F functions have been executed
	static void TearDownTestCase() {}

	// this function runs before every TEST_F function
	void SetUp() override {}

	// this function runs after every TEST_F function
	void TearDown() override {}
};

/////////////////////////////////////////
// Tests start here
/////////////////////////////////////////

TEST_F(test_x, TestHash)
{
	string key;
	string result;
	string expected;

	// case 1
	key = "abc";

	result = test_hash(key);

	expected = "a9993e364706816aba3e25717850c26c9cd0d89d";

	ASSERT_EQ(expected, result);

	// case 2
	key = "abcdef";

	result = test_hash(key);

	expected = "1f8ac10f23c5b5bc1167bda84b833e5c057a77d2";

	ASSERT_EQ(expected, result);

	// case 3
	key = "abcdefghijklmnopqrstuvwxyzZYXWVUTSRQPONMLKJIHGFEDCBA";

	result = test_hash(key);

	expected = "77852a3e9b97c65c9bafe5e688ea1d19d69ea25d";

	ASSERT_EQ(expected, result);

	// case 4
	key = "The quick brown fox jumps over the lazy dog";

	result = test_hash(key);

	expected = "2fd4e1c67a2d28fced849ee1bb76e7391b93eb12";

	ASSERT_EQ(expected, result);

	// case 5
	key = "The quick brown fox jumps over the lazy cog";

	result = test_hash(key);

	expected = "de9f2c7fd25e1b3afad3e85a0bd17d9b100db4b3";

	ASSERT_EQ(expected, result);

	// case 6
	key = "";

	result = test_hash(key);

	expected = "da39a3ee5e6b4b0d3255bfef95601890afd80709";

	ASSERT_EQ(expected, result);
}

TEST_F(test_x, TestInsert)
{
	string result;
	string expected;
	int length;
	int table_size;

	// case 1
	string commits_1[] = {"computer", "science", "fun", "difficult", "science"};
	length = 5;
	table_size = 5;

	result = test_insert(commits_1, length, table_size);

	expected = "0|| difficult(3,)\n1|| fun(2,)\n2|| science(1,4,)\n3|| computer(0,)\n4|| \n";

	ASSERT_EQ(expected, result);

	// case 2
	string commits_2[] = {"git", "hash", "data", "data", "coding"};
	length = 5;
	table_size = 10;

	result = test_insert(commits_2, length, table_size);

	expected = "0|| \n1|| \n2|| coding(4,)\n3|| \n4|| hash(1,)-->git(0,)\n5|| \n6|| \n7|| data(2,3,)\n8|| \n9|| \n";

	ASSERT_EQ(expected, result);

	// case 3
	string commits_3[] = {"computer", "science", "fun", "difficult", "science", "git", "hash", "data", "data", "coding"};
	length = 10;
	table_size = 15;

	result = test_insert(commits_3, length, table_size);

	expected = "0|| \n1|| \n2|| data(7,8,)\n3|| computer(0,)\n4|| \n5|| difficult(3,)\n6|| fun(2,)\n7|| coding(9,)-->science(1,4,)\n8|| \n9|| \n10|| \n11|| \n12|| \n13|| \n14|| hash(6,)-->git(5,)\n";

	ASSERT_EQ(expected, result);

	// case 4
	string commits_4[] = {};
	length = 0;
	table_size = 5;

	result = test_insert(commits_4, length, table_size);

	expected = "0|| \n1|| \n2|| \n3|| \n4|| \n";

	ASSERT_EQ(expected, result);
}

TEST_F(test_x, TestSearch)
{
	HashNode result;
	HashNode expected;
	string search_key;

	string commits[] = {"computer", "science", "fun", "difficult", "science", "git", "hash", "data", "data", "coding"};
	int length = 10;
	int table_size = 15;

	// case 1
	search_key = "science";

	result = test_search(commits, length, table_size, search_key);

	expected.key = search_key;
	expected.commit_nums = {1, 4};

	ASSERT_EQ(expected.key, result.key);
	ASSERT_EQ(expected.commit_nums, result.commit_nums);

	// case 2
	search_key = "data";

	result = test_search(commits, length, table_size, search_key);

	expected.key = search_key;
	expected.commit_nums = {7, 8};

	ASSERT_EQ(expected.key, result.key);
	ASSERT_EQ(expected.commit_nums, result.commit_nums);

	// case 3
	search_key = "fun";

	result = test_search(commits, length, table_size, search_key);

	expected.key = search_key;
	expected.commit_nums = {2};

	ASSERT_EQ(expected.key, result.key);
	ASSERT_EQ(expected.commit_nums, result.commit_nums);

	// case 4
	search_key = "error";

	ASSERT_THROW(test_search(commits, length, table_size, search_key), exception);
}

TEST_F(test_x, TestAdd)
{
	string result;
	string expected;
	int length;
	int table_size;
	string commit_message;

	// case 1
	string file_paths_1[] = {"../tests/folder/text.txt"};
	length = 1;
	table_size = 5;
	commit_message = "insert text file";

	result = test_add(file_paths_1, length, table_size, commit_message);

	expected = "0|| text(0,)-->insert(0,)\n1|| file(0,)\n2|| \n3|| \n4|| \n";

	ASSERT_EQ(expected, result);

	// case 2
	string file_paths_2[] = {"../tests/folder/cplusplus.cpp"};
	length = 1;
	table_size = 5;
	commit_message = "insert cpp file";

	result = test_add(file_paths_2, length, table_size, commit_message);

	expected = "0|| insert(0,)\n1|| file(0,)\n2|| \n3|| \n4|| cpp(0,)\n";

	ASSERT_EQ(expected, result);

	// case 3
	string file_paths_3[] = {"../tests/folder/text.txt", "../tests/folder/cplusplus.cpp"};
	length = 2;
	table_size = 5;
	commit_message = "insert both";

	result = test_add(file_paths_3, length, table_size, commit_message);

	expected = "0|| insert(0,)\n1|| both(0,)\n2|| \n3|| \n4|| \n";

	ASSERT_EQ(expected, result);

	// case 4
	string file_paths_4[] = {"../tests/folder/text.txt", "../tests/folder/cplusplus.cpp", "../folder/text.txt"};
	length = 3;
	table_size = 5;
	commit_message = "testing exception updated";

	ASSERT_THROW(test_add(file_paths_4, length, table_size, commit_message), exception);

	// case 5
	string file_paths_5[] = {"../tests/folder/fake_file.txt"};
	length = 1;
	table_size = 5;
	commit_message = "testing exception non-existent";

	ASSERT_THROW(test_add(file_paths_5, length, table_size, commit_message), exception);
}

TEST_F(test_x, TestRm)
{
	string result;
	string expected;
	int add_length;
	int rm_length;
	int table_size;
	string commit_message;

	// case 1
	string add_1[] = {"../tests/folder/text.txt", "../tests/folder/cplusplus.cpp"};
	add_length = 2;
	string rm_1[] = {"../tests/folder/text.txt"};
	rm_length = 1;
	table_size = 5;
	commit_message = "remove text file";

	result = test_rm(add_1, add_length, rm_1, rm_length, table_size, commit_message);

	expected = "0|| text(0,)-->remove(0,)\n1|| file(0,)\n2|| \n3|| \n4|| \n";

	ASSERT_EQ(expected, result);

	// case 2
	string add_2[] = {"../tests/folder/text.txt", "../tests/folder/cplusplus.cpp"};
	add_length = 2;
	string rm_2[] = {"../tests/folder/cplusplus.cpp"};
	rm_length = 1;
	table_size = 5;
	commit_message = "remove cpp file";

	result = test_rm(add_2, add_length, rm_2, rm_length, table_size, commit_message);

	expected = "0|| remove(0,)\n1|| file(0,)\n2|| \n3|| \n4|| cpp(0,)\n";

	ASSERT_EQ(expected, result);

	// case 3
	string add_3[] = {"../tests/folder/text.txt", "../tests/folder/cplusplus.cpp"};
	add_length = 2;
	string rm_3[] = {"../tests/folder/text.txt", "../tests/folder/cplusplus.cpp"};
	rm_length = 2;
	table_size = 5;
	commit_message = "remove both";

	result = test_rm(add_3, add_length, rm_3, rm_length, table_size, commit_message);

	expected = "0|| remove(0,)\n1|| both(0,)\n2|| \n3|| \n4|| \n";

	ASSERT_EQ(expected, result);

	// case 4
	string add_4[] = {"../tests/folder/text.txt", "../tests/folder/cplusplus.cpp"};
	add_length = 2;
	string rm_4[] = {"../tests/folder/text.txt", "../tests/folder/text.txt"};
	rm_length = 2;
	table_size = 5;
	commit_message = "testing exception pre-removed";

	ASSERT_THROW(test_rm(add_4, add_length, rm_4, rm_length, table_size, commit_message), exception);

	// case 5
	string add_5[] = {};
	add_length = 0;
	string rm_5[] = {"../tests/folder/fake_file.txt"};
	rm_length = 1;
	table_size = 5;
	commit_message = "testing exception non-existent";

	ASSERT_THROW(test_rm(add_5, add_length, rm_5, rm_length, table_size, commit_message), exception);
}
