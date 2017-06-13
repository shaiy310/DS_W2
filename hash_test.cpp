#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream> 
#include <stdbool.h>

#include "HashTable.h"

std::string location; // Used for finding the origin of an exception

#define FAIL(message) do { \
		std::ostringstream os; \
		os << __LINE__; \
		location = std::string(__FILE__) + ":" + os.str() + " " + message; \
		return false; \
	} while (0) 

#define ASSERT_NOT_EQUALS(expected,actual) do { \
	if ((expected) == (actual)) { \
		FAIL("values are equal"); \
	} \
	} while (0)

#define ASSERT_EQUALS(expected,actual) do { \
	if (!((expected) == (actual))) { \
		FAIL("values not equal"); \
	} \
	} while (0)

#define ASSERT_NO_THROW(command) do {\
	try { \
	(command); \
} \
catch (std::exception& e) { \
	FAIL("caught " + e.what()); \
} \
catch (...) { \
	FAIL("caught unknown object"); \
} \
} while (0)

#define ASSERT_THROW(exception_type,command) do {\
	try { \
	(command); \
	FAIL("missing exception"); \
	} \
	catch (exception_type&) { \
	} \
	catch (std::exception& e) { \
		FAIL("caught " + e.what()); \
	} \
	catch (...) { \
		FAIL("caught unknown object"); \
	} \
} while (0)
	
#define RUN_TEST(b) do {\
	try { \
	std::cerr << #b << ":"; \
		if (b()) std::cerr << "OK" << std::endl; \
		else std::cerr << "Failed (" << location << ")" << std::endl; \
		} \
		catch (...) { \
			std::cerr << "Failed (" << location << ")" << std::endl; \
		} \
	} while (0) 
	
bool EmptyTableTest() {
	HashTable<int> * t = new HashTable<int>();
	
	ASSERT_EQUALS(t->Count(), 0);
	ASSERT_NO_THROW(delete t);
	
	return true;
}

bool AddRemoveTest() {
	HashTable<int> t(10, Module, One);
	
	// nothing in table
	ASSERT_THROW(HashTable<int>::NotFound, t.Get(1));
	ASSERT_THROW(HashTable<int>::NotFound, t.Remove(1));
	ASSERT_EQUALS(t.Count(), 0);
	
	// add a single element
	ASSERT_NO_THROW(t.Add(1, 5));
	ASSERT_EQUALS(t.Count(), 1);
	ASSERT_EQUALS(t.Get(1), 5);
	
	// Remove the element
	ASSERT_NO_THROW(t.Remove(1));
	ASSERT_EQUALS(t.Count(), 0);
	ASSERT_THROW(HashTable<int>::NotFound, t.Get(1));
	
	return true;
}

bool AddRemoveMultipleTest() {
	HashTable<int> t(HashTable<int>::TABLE_SIZE, Module, One);
	
	// add a elements
	ASSERT_NO_THROW(t.Add(1, 1));
	ASSERT_NO_THROW(t.Add(2, 2));
	ASSERT_NO_THROW(t.Add(3, 3));
	ASSERT_NO_THROW(t.Add(1 + HashTable<int>::TABLE_SIZE, 11)); // should be indexed as 4
	ASSERT_NO_THROW(t.Add(5, 5));
	ASSERT_NO_THROW(t.Add(4, 4));
	ASSERT_EQUALS(t.Count(), 6);
	ASSERT_EQUALS(t.Get(1), 1);
	ASSERT_EQUALS(t.Get(2), 2);
	ASSERT_EQUALS(t.Get(3), 3);
	ASSERT_EQUALS(t.Get(4), 4);
	ASSERT_EQUALS(t.Get(5), 5);
	ASSERT_EQUALS(t.Get(1 + HashTable<int>::TABLE_SIZE), 11);
	
	// Remove element - check skipping deleted
	ASSERT_NO_THROW(t.Remove(1));
	ASSERT_EQUALS(t.Count(), 5);
	ASSERT_EQUALS(t.Get(1 + HashTable<int>::TABLE_SIZE), 11);
	ASSERT_NO_THROW(t.Remove(1 + HashTable<int>::TABLE_SIZE));
	ASSERT_EQUALS(t.Count(), 4);
	
	return true;
}

bool FullTableTest() {
	HashTable<int> t(5, Multiply, One);
	
	// should create 5 resizes (	2*(2*(2*(2*(2*(5)+1)+1)+1)+1)+1 = 191	)
	for (int i = 3000; i < 3150; ++i) {
		ASSERT_NO_THROW(t.Add(i, 11 * i));
		ASSERT_EQUALS(t.Get(i), 11 * i);
	}
	
	//std::cout << "after adding 150: " << std::endl;
	//t.print_table();
	
	#ifndef NDEBUG
	std::cout << ".";
	ASSERT_EQUALS(t.Size(), 191);
	#endif
	
	ASSERT_EQUALS(t.Count(), 150);
	
	for (int i = 0; i < 50; ++i) {
		ASSERT_NO_THROW(t.Remove(3000 + i));
		ASSERT_THROW(HashTable<int>::NotFound, t.Get(i));
	}
	
	//std::cout << "after removing 50: " << std::endl;
	//t.print_table();
	
	ASSERT_EQUALS(t.Count(), 100);
	
	// should not create another resize
	for (int i = 0; i < 90; ++i) {
		ASSERT_NO_THROW(t.Add(i, 11 * i));
		ASSERT_EQUALS(t.Get(i), 11 * i);
	}
	
	//std::cout << "after adding 90: " << std::endl;
	//t.print_table();
	
	ASSERT_EQUALS(t.Count(), 190);
	#ifndef NDEBUG
	std::cout << ".";
	ASSERT_EQUALS(t.Size(), 191);
	#endif
	
	return true;
}
	
int main() {
	RUN_TEST(EmptyTableTest);
	RUN_TEST(AddRemoveTest);
	RUN_TEST(AddRemoveMultipleTest);
	RUN_TEST(FullTableTest);
	
	return 0;
	
}