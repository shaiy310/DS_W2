#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include <cstdlib>
#include <exception>
#include <stdio.h>

// hash table with dynamic size
// double hashing
// regular hash function - Multiply

typedef int (*hash_func)(int key, int size);

// General hashing functions
// Module
int Module(int key, int size) {
	return (key % size);
}

// Multiply
const int factor = 0.6180339887498949; // result of: (sqrt(5.0) - 1) / 2;
int Multiply(int key, int size) {
	float fraction = key * factor - ((int)(key * factor));
	return (int)(size * fraction);
}

// One - used as second hash function for creating linear hashing
int One(int key, int size) {
	// unused variables
	(void)key;
	(void)size;
	return 1;
}


// Class: HashEntry
// Purpose: represent an hash table entry.
// Template: D - the type of data stored in the entry.
template <class D>
class HashEntry {
public:
	static const int DELETED = -1;
	HashEntry(int k, D d) : key(k), data(d) { }
	
	int GetKey() { return this->key; }
	D GetData() { return this->data; }
	void MarkDelete() { this->key = HashEntry::DELETED; }
	
private:
	int key;
	D data;
};


// Class: HashTable
// Purpose: represent a double-hashing hash table.
// Template: D - the type of data stored in the table.
template <class D>
class HashTable {
public:
	static const int TABLE_SIZE = 79;
	class NotFound : public std::exception {};
	class InvalidArg : public std::exception {};
	class CollidingKeys : public std::exception {};

	// ~~~ C'tor ~~~
	HashTable(int min_size=TABLE_SIZE, hash_func hash1=Multiply, hash_func hash2=Multiply) : 
		minimum_size(min_size), size(min_size), length(0), func1(hash1), func2(hash2) {
		
		this->min_threshold = 0.25;
		this->max_threshold = 0.75;
		
		this->init_table();
	}
	
	// ~~~ D'tor ~~~
	~HashTable() {
		this->clear_table();
	}
	
	// ~~~ properties ~~~
	// get the number of items in the table.
	int Count() const {
		return this->length;
	}
	
	// get the threshold for expanding the table.
	double GetMaxThreshold() const {
		return this->max_threshold;
	}
	
	// set the threshold for expanding the table.
	void SetMaxThreshold(double new_threshold) {
		// the maximum cant be larger than 1 because 1 represent a full table.
		// also, it can't be below the minimum threshold for obvious reasons.
		if ((new_threshold > 1) || (new_threshold <= this->min_threshold)) {
			throw InvalidArg();
		}
		this->max_threshold = new_threshold;
	}
	
	// get the threshold for reducing the table.
	double GetMinThreshold() const {
		return this->min_threshold;
	}
	
	// set the threshold for reducing the table.
	void SetMinThreshold(double new_threshold) {
		// the minimum cant be non-positive number because 0 represent an empty table.
		// also, it can't be over the maximum threshold for obvious reasons.
		if ((new_threshold <= 0) || (new_threshold >= this->max_threshold)) {
			throw InvalidArg();
		}
		this->min_threshold = new_threshold;
	}
	
	#ifndef NDEBUG
	// get the number of max items in the table. (used for testing only
	int Size() { return this->size; }
	#endif
	
	
	D Get(int key) const {
		int hash = this->calculate_hash(key, true);
		
		return this->table[hash]->GetData();
	}
	
	void Add(int key, D data) {
		int hash = this->calculate_hash(key, false);
		// incase we override a key, destroy the old instance
		if (this->table[hash] != NULL) {
			delete this->table[hash];
		}
		
		this->table[hash] = new HashEntry<D>(key, data);
		++(this->length);
		
		if (this->length >= (int)(this->size * this->max_threshold)) {
			this->resize(true);
		}
	}

	void Remove(int key) {
		int hash = this->calculate_hash(key, true);
		
		this->table[hash]->MarkDelete();
		--(this->length);
		
		if ((this->length <= (int)(this->size * this->min_threshold)) &&
			((this->size / 2) >= this->minimum_size)) {
			this->resize(false);
		}
	}
	
	void print_table() {
		for (int i = 0; i < this->size; ++i) {
			if (this->table[i] != NULL) {
				printf("hash: %03d \t key: %03d\n", i, this->table[i]->GetKey());
			} else {
				printf("hash: %03d \t key: NONE\n", i);
			}
		}
	}

private:
	int minimum_size;
	int size;
	int length;
	double min_threshold;
	double max_threshold;
	
	hash_func func1, func2;
	HashEntry<D> ** table;
	
	void clear_table() {
		// clear all allocated entries.
		for (int i = 0; i < this->size; ++i) {
            if (this->table[i] != NULL) {
				delete table[i];
			}
		}
		delete[] this->table;
	}
	
	void resize(bool should_expand) {
		int new_size = (should_expand) ? (2 * this->size + 1) : (this->size / 2);
		
		//create a larger table.
		HashTable<D> temp(new_size, this->func1, this->func2);
		
		// move all entries to the new table. (skip deleted entries)
		for (int i = 0; i < this->size; ++i) {
			if ((this->table[i] != NULL) && 
				(this->table[i]->GetKey() != HashEntry<D>::DELETED)) {
				temp.Add(this->table[i]->GetKey(), this->table[i]->GetData());
			}
		}
		
		// replace the old table with the new one.
		this->clear_table();
		this->size = new_size;
		this->init_table();
		
		// clone the new table.
		for (int i = 0; i < this->size; ++i) {
			if (temp.table[i] != NULL) {
				this->table[i] = new HashEntry<D>(temp.table[i]->GetKey(),
					temp.table[i]->GetData());
			}
		}
	}
	
	// get the correspanding location of key in table
	// run time: O(n)
	int calculate_hash(int key, bool should_exist) const {
		if (key < 0) {
			throw InvalidArg();
		}
		
		int hash = this->func1(key, this->size) % this->size;
		int first_hash = hash;			// use to avoid getting into infinite loop
		
		// passed is used to break the loop after size iteration (avoid infinite
		// loop in case of table full and key not in table).
		while ((this->table[hash] != NULL) && 
			(this->table[hash]->GetKey() != key)) {
			
			if ((!should_exist) &&
				(this->table[hash]->GetKey() == HashEntry<D>::DELETED)) {
				break;
			}
			
			hash = (hash + this->func2(key, this->size)) % this->size;
			if (hash == first_hash) {
				// moved a full cycle...
				break;
			}
		}
		
		if (should_exist) {
			// no such key in table
			if ((this->table[hash] == NULL) || 
				(this->table[hash]->GetKey() == HashEntry<D>::DELETED)) {
				throw NotFound();
			}
		} else {
			if ((this->table[hash] != NULL) && 
				(this->table[hash]->GetKey() != HashEntry<D>::DELETED)) {
				throw CollidingKeys();
			}
		}
		
		return hash;
	}
	
	// allocate and initialize the table
	// run time: O(n)
	void init_table() {
		this->table = new HashEntry<D>*[this->size];
		for (int i = 0; i < this->size; ++i) {
			this->table[i] = NULL;
		}
	}
};

#endif
