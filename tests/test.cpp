// If you change anything in this file, your changes will be ignored
// in your homework submission.
// Chekout TEST_F functions bellow to learn what is being tested.
#include <gtest/gtest.h>
// #include "../code_1/<HEADER FILE>.hpp"

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
	// This function runs only once before any TEST_F function
	static void SetUpTestCase()
	{
		std::ofstream outgrade("./total_grade.txt");
		if (outgrade.is_open())
		{
			outgrade.clear();
			outgrade << (int)0;
			outgrade.close();
		}
	}

	// This function runs after all TEST_F functions have been executed
	static void TearDownTestCase()
	{
		std::ofstream outgrade("./total_grade.txt");
		if (outgrade.is_open())
		{
			outgrade.clear();
			outgrade << (int)std::ceil(100 * total_grade / max_grade);
			outgrade.close();
			std::cout << "Total Grade is : " << (int)std::ceil(100 * total_grade / max_grade) << std::endl;
		}
	}

	void add_points_to_grade(double points)
	{
		if (!::testing::Test::HasFailure())
		{
			total_grade += points;
		}
	}

	// this function runs before every TEST_F function
	void SetUp() override {}

	// this function runs after every TEST_F function
	void TearDown() override
	{
		std::ofstream outgrade("./total_grade.txt");
		if (outgrade.is_open())
		{
			outgrade.clear();
			outgrade << (int)std::ceil(100 * total_grade / max_grade);
			outgrade.close();
		}
	}

	static double total_grade;
	static double max_grade;
};

double test_x::total_grade = 0;
double test_x::max_grade = 100;

/////////////////////////////////////////
// Tests start here
/////////////////////////////////////////

TEST_F(test_x, TestHash)
{
	string key;
	string result;
	string expected;


	key = "abc";

	result = test_hash(key);

	expected = "a9993e364706816aba3e25717850c26c9cd0d89d";

	ASSERT_EQ(expected, result);
	add_points_to_grade(1);


	key = "abcdef";

	result = test_hash(key);

	expected = "1f8ac10f23c5b5bc1167bda84b833e5c057a77d2";

	ASSERT_EQ(expected, result);
	add_points_to_grade(1);


	key = "abcdefghijklmnopqrstuvwxyzZYXWVUTSRQPONMLKJIHGFEDCBA";

	result = test_hash(key);

	expected = "77852a3e9b97c65c9bafe5e688ea1d19d69ea25d";

	ASSERT_EQ(expected, result);
	add_points_to_grade(1);


	key = "The quick brown fox jumps over the lazy dog";

	result = test_hash(key);

	expected = "2fd4e1c67a2d28fced849ee1bb76e7391b93eb12";

	ASSERT_EQ(expected, result);
	add_points_to_grade(1);

	
	key = "The quick brown fox jumps over the lazy cog";

	result = test_hash(key);

	expected = "de9f2c7fd25e1b3afad3e85a0bd17d9b100db4b3";

	ASSERT_EQ(expected, result);
	add_points_to_grade(1);


	key = "";

	result = test_hash(key);

	expected = "da39a3ee5e6b4b0d3255bfef95601890afd80709";

	ASSERT_EQ(expected, result);
	add_points_to_grade(1);
}

TEST_F(test_x, TestInsert)
{
	string result;
	string expected;
	int len;
	int tableSize;


	string commits_1[] = {"computer", "science", "fun", "difficult", "science"};
	len = 5;
	tableSize = 5;

	result = test_insert(commits_1, len, tableSize);

	expected = "0|| \n1|| \n2|| \n3|| difficult(3,)-->fun(2,)-->science(1,4,)\n4|| computer(0,)\n";

	ASSERT_EQ(expected, result);
	add_points_to_grade(2);


	string commits_2[] = {"git", "hash", "data", "data", "coding"};
	len = 5;
	tableSize = 10;

	result = test_insert(commits_2, len, tableSize);

	expected = "0|| \n1|| hash(1,)\n2|| \n3|| coding(4,)\n4|| \n5|| \n6|| \n7|| git(0,)\n8|| \n9|| data(2,3,)\n";

	ASSERT_EQ(expected, result);
	add_points_to_grade(2);


	string commits_3[] = {"computer", "science", "fun", "difficult", "science", "git", "hash", "data", "data", "coding"};
	len = 10;
	tableSize = 15;

	result = test_insert(commits_3, len, tableSize);

	expected = "0|| \n1|| hash(6,)\n2|| \n3|| difficult(3,)\n4|| computer(0,)\n5|| \n6|| \n7|| \n8|| science(1,4,)\n9|| data(7,8,)\n10|| \n11|| \n12|| git(5,)\n13|| coding(9,)-->fun(2,)\n14|| \n";

	ASSERT_EQ(expected, result);
	add_points_to_grade(2);
}

// TODO add more tests for insert. Design tests for search from hashTable.
