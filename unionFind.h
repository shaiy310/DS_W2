#ifndef UNIONFIND_H_
#define UNIONFIND_H_

#include <exception>
using namespace std;

#define EMPTY -1

class UnionFind {

private:

	int size;
	int* sizes_array;
	int* parents_array;

public:

	class BadRoot : public exception {};
	class BadGroup : public exception {};

	// C'tor
	UnionFind(int n) : size(n), sizes_array(new int[size]),
								parents_array(new int[size]) {

		// initilize the sizes array to 1, and parents_array to EMPTY
		for(int i = 0; i < size; i++) {
			sizes_array[i] = 1;
			parents_array[i] = EMPTY;
		}
	}

	// D'tor
	~UnionFind() {

		delete[] sizes_array;
		delete[] parents_array;

	}

	// Copy C'tor
	UnionFind(const UnionFind& u) : size(u.size),
									sizes_array(new int[size]),
									parents_array(new int[size]) {

		for(int i = 0; i < size; i++) {
			sizes_array[i] = u.sizes_array[i];
			parents_array[i] = u.parents_array[i];
		}
	}

	UnionFind& operator = (const UnionFind& u) {

		if(&u == this) return *this;

		// allocate new arrays
		int* temp_sizes_array = new int[u.size];
		int* temp_parents_array = new int[u.size];

		size = u.size;

		// delete arrays
		delete[] sizes_array;
		delete[] parents_array;

		for(int i = 0; i < size; i++) {
			temp_sizes_array[i] = u.sizes_array[i];
			temp_parents_array[i] = u.parents_array[i];
		}

		sizes_array = temp_sizes_array;
		parents_array = temp_parents_array;

		return *this;
	}


	int Union(int a, int b) {

		if(a < 1 || b < 1 || a > size || b > size) throw BadGroup();

		if(parents_array[a - 1] != EMPTY || parents_array[b - 1] != EMPTY) {
			throw BadRoot();
		}

		if(sizes_array[a - 1] <= sizes_array[b - 1]) {
			parents_array[a - 1] = b;
			sizes_array[b - 1] += sizes_array[a - 1];
			sizes_array[a - 1] = EMPTY;
			return b;
		}

		parents_array[b - 1] = a;
		sizes_array[a - 1] += sizes_array[b - 1];
		sizes_array[b - 1] = EMPTY;
		return a;
	}

	int find(int x) const {

		if(x < 1 || x > size) throw BadGroup();

		// if x is the root
		if(parents_array[x - 1] == EMPTY) {
			return x;
		}

		// find the root of x
		int root = x;
		while(parents_array[root - 1] != EMPTY) {
			root = parents_array[root - 1];
		}

		// collapse tracks
		while(parents_array[x - 1] != root) {
			int temp = parents_array[x - 1];
			parents_array[x - 1] = root;
			x = temp;
		}

		return root;
	}

};



#endif /* UNIONFIND_H_ */
