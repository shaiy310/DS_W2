#include <iostream>

#ifndef AVL_TREE_H
#define AVL_TREE_H

/* Exceptions */
class AVLTreeExcpetion : public std::exception {};
class DataNotFound : public std::exception {};

/* AVL node
this class stores key and value.
K - key, T - value.
height - the height of the current node.
balance - left child height - right child height.
parent - the parent node.
*/
template <class K, class T>
class AVLnode {
	K key;
	T data;
	int balance;
	int height;
	int sub_tree_nodes;
	int power;
	int sub_tree_sum_power;

public:
	// adding more info to the class.
	AVLnode<K, T> *left, *right, *parent;

	// C'tors
	// first C'tor - create AVLnode with the default key and value
	AVLnode<K, T>(AVLnode<K, T> *p, int P) {
		parent = p;
		balance = 0;
		height = 0;
		sub_tree_nodes = 1;
		power = P;
		sub_tree_sum_power = P;
		right = NULL;
		left = NULL;
	}

	// second C'tor - create AVLnode with the given key and value
	AVLnode<K, T>(K key, T data, AVLnode<K, T> *p, int P) :
				  key(key), data(data), balance(0), height(0),
				  sub_tree_nodes(1), power(P), sub_tree_sum_power(P),
				  left(NULL), right(NULL), parent(p) {}

	// D'ctor - default
	~AVLnode() {}

	/* Getters - O(1)*/
	K getKey() {
		return key;
	}

	const K getKey() const {
		return key;
	}

	T& getData() {
		return data;
	}

	const T getData() const {
		return data;
	}

	int getHeight() const {
		return height;
	}

	int getBalance() const {
		return balance;
	}

	const AVLnode<K, T>* getLeft() const {
		return left;
	}

	AVLnode<K, T>* getLeft() {
		return left;
	}

	const AVLnode<K, T>* getRight() const {
		return right;
	}

	AVLnode<K, T>* getRight() {
		return right;
	}

	const AVLnode<K, T>* getParent() const {
		return parent;
	}

	AVLnode<K, T>* getParent() {
		return parent;
	}

	const int getPower() const {
		return power;
	}

	const int getSubTreeNodes() const {
		return sub_tree_nodes;
	}

	const int getSubTreeSum() const {
		return sub_tree_sum_power;
	}

	/* Setters - O(1)*/
	void setKey(K newKey) {
		key = K(newKey);
	}

	void setData(T newData) {
		data = newData;
	}

	void setBalanc(int newBalacne) {
		balance = newBalacne;
	}

	void setHeight(int newHeight) {
		height = newHeight;
	}

	void setPower(int p) {
		power = p;
	}

	void setSum(int s) {
		sub_tree_sum_power = s;
	}

	void setSubTreeNodes(int num) {
		sub_tree_nodes = num;
	}

	void setSubTreeSum(int num) {
		sub_tree_sum_power = num;
	}

	void increaseSubTreeNodes() {
		++sub_tree_nodes;
	}

	void increaseSubTreeSumPower(int p) {
		sub_tree_sum_power += p;
	}

	void decreaseSubTreeNodes() {
		--sub_tree_nodes;
	}

	void setLeft(AVLnode<K, T>* l) {
		left = l;
	}

	void setRight(AVLnode<K, T>* r) {
		right = r;
	}

	void setParent(AVLnode<K, T>* p) {
		parent = p;
	}
};

/*
AVL tree - stores AVL tree
K - the key
T - the value
*/

/*
must functions to K - D'ctor, copy, < , =
must functions to K - D'ctor, copy, =
*/
template <class K, class T>
class AVLtree {
private:
	AVLnode<K, T> *root;
	int size;

	// private functions

	// check if keys are eqaul
	bool keysAreEqual(const K k1, const K k2) const {
		return ((!(k1 < k2) && !(k2 < k1)));
	}

	// first key is less than second
	bool keyIsLessThan(const K k1, const K k2) const {
		return (k1 < k2);
	}

	// first key is less or equal than second
	bool keyIsLessOrEqualThan(const K k1, const K k2) const {
		return keysAreEqual(k1, k2) || keyIsLessThan(k1, k2);
	}

	// get node by key. if no key return NULL. - O(logn).
	AVLnode<K, T>* getNodeByKey(const K key) const {
		AVLnode<K, T>* node = root;
		while (node != NULL) {
			if (keysAreEqual(node->getKey(), key)) {
				return node;
			}

			if(keyIsLessOrEqualThan(node->getKey(), key)) {
				node = node->right;
			} else {
				node = node->left;
			}
		}

		return NULL;
	}

	// rotate left the node - O(1)
	AVLnode<K, T>* rotateLeft(AVLnode<K, T> *b) {


		AVLnode<K, T> *a = b->left;
		AVLnode<K, T> *aR = a->right;
		AVLnode<K, T> *aL = a->left;
		AVLnode<K, T> *bR = b->right;

		int w_Ar = (aR == NULL ? 0 : aR->getSubTreeNodes());
		int w_Br = (bR == NULL ? 0 : bR->getSubTreeNodes());
		int w_Al = (aL == NULL ? 0 : aL->getSubTreeNodes());

		b->setSubTreeNodes(w_Ar + w_Br + 1);
		a->setSubTreeNodes(b->getSubTreeNodes() + w_Al + 1);

		int sum_power_Ar = (aR == NULL ? 0 : aR->getSubTreeSum());
		int sum_power_Br = (bR == NULL ? 0 : bR->getSubTreeSum());
		int sum_power_Al = (aL == NULL ? 0 : aL->getSubTreeSum());

		b->setSubTreeSum(sum_power_Ar + sum_power_Br + b->getPower());
		a->setSubTreeSum(b->getSubTreeSum() + sum_power_Al + a->getPower());

		a->parent = b->parent;
		b->left = a->right;

		if (b->left != NULL)
			b->left->parent = b;

		a->right = b;
		b->parent = a;

		if (a->parent != NULL) {
			if (a->parent->right == b) {
				a->parent->right = a;
			}
			else {
				a->parent->left = a;
			}
		}

		a->setHeight(height(a));
		b->setHeight(height(b));

		setBalance(b);
		setBalance(a);

		return a;
	}

	// rotate right the node - O(1)
	AVLnode<K, T>* rotateRight(AVLnode<K, T> *b) {

		AVLnode<K, T> *a = b->right;
		AVLnode<K, T> *aL = a->left;
		AVLnode<K, T> *aR = a->right;
		AVLnode<K, T> *bL = b->left;

		int w_bL = (bL == NULL ? 0 : bL->getSubTreeNodes());
		int w_aL = (aL == NULL ? 0 : aL->getSubTreeNodes());
		int w_aR = (aR == NULL ? 0 : aR->getSubTreeNodes());

		b->setSubTreeNodes(w_bL + w_aL + 1);
		a->setSubTreeNodes(b->getSubTreeNodes() + w_aR + 1);

		int sum_bL = (bL == NULL ? 0 : bL->getSubTreeSum());
		int sum_aL = (aL == NULL ? 0 : aL->getSubTreeSum());
		int sum_aR = (aR == NULL ? 0 : aR->getSubTreeSum());

		b->setSubTreeSum(sum_bL + sum_aL + b->getPower());
		a->setSubTreeSum(b->getSubTreeSum() + sum_aR + a->getPower());

		a->parent = b->parent;
		b->right = a->left;

		if (b->right != NULL)
			b->right->parent = b;

		a->left = b;
		b->parent = a;


		if (a->parent != NULL) {
			if (a->parent->right == b)
				a->parent->right = a;
			else
				a->parent->left = a;
		}

		b->setHeight(height(b));
		a->setHeight(height(a));

		setBalance(b);
		setBalance(a);


		return a;
	}

	// rotate right and than left the node - O(1)
	AVLnode<K, T>* rotateRL(AVLnode<K, T> *n) {
		n->left = rotateRight(n->left);
		return rotateLeft(n);
	}

	// rotate left and than right the node - O(1)
	AVLnode<K, T>* rotateLR(AVLnode<K, T> *n) {
		n->right = rotateLeft(n->right);
		return rotateRight(n);
	}

	// return next node of the tree - O(logn)
	AVLnode<K, T>* getNextNode(AVLnode<K, T>* n, K key) {
		if (n == NULL) {
			return NULL;
		}

		AVLnode<K, T>* leftRes = getNextNode(n->left, key);
		if (leftRes != NULL) {
			return leftRes;
		}
		if (keyIsLessThan(key, n->getKey())) {
			return n;
		}
		return getNextNode(n->right, key);
	}

	// give the nodes the correct balance
	void rebalance(AVLnode<K, T> *n, bool calcHeight = false) {
		if (calcHeight) {
			if (n != NULL) {
				n->setHeight(height(n));
			}
		}

		setBalance(n);

		if (n->getBalance() >= 2) {
			// Left

			if (n->left->getBalance() >= 0) {
				n = rotateLeft(n);
			} else {
				n = rotateRL(n);
			}
		}
		else if (n->getBalance() <= -2) {
			// Right

			if (n->right->getBalance() <= 0)
				n = rotateRight(n);
			else
				n = rotateLR(n);
		}

		if (n->parent != NULL) {
			rebalance(n->parent);
		}
		else {
			root = n;
		}
	}

	// get the height of a node
	int height(AVLnode<K, T> *n) const {
		if (n == NULL)
			return -1;

		int left = height(n->left);
		int right = height(n->right);
		if (left > right)
			return 1 + left;
		return 1 + right;
	}

	// give node a balance
	void setBalance(AVLnode<K, T> *n) {
		if (n == NULL)
			return;

		int lHeight = n->left != NULL ? n->left->getHeight() + 1 : 0;
		int lRight = n->right != NULL ? n->right->getHeight() + 1 : 0;
		if (lHeight > lRight)
			n->setHeight(lHeight);
		else
			n->setHeight(lRight);

		n->setBalanc(lHeight - lRight);
	}

	// swap the key and data of the two given nodes - O(1)
	void swapNodeKeyAndData(AVLnode<K, T>* w, AVLnode<K, T>* t) {
		T temp = w->getData();
		w->setData(t->getData());
		t->setData(temp);

		K tempKey = w->getKey();
		w->setKey(t->getKey());
		t->setKey(tempKey);

		int temp_power = w->getPower();
		w->setPower(t->getPower());
		t->setPower(temp_power);
	}

	/*
	the given node has one or no childer. the function removes the node.
	if it has one son the this one will be the son of the parent node.
	returns true if removes succesfuly - O(1)
	*/
	bool removeLeftOrMaxOneSon(AVLnode<K, T>* node) {
		bool leftExists = node->left != NULL;
		bool rightExists = node->right != NULL;

		int power = node->getPower();

		if (!leftExists && !rightExists) {
			goUpAndUpdateTree(node, power);
			removeLeaf(node);
			return true;
		}
		if ((leftExists && !rightExists) || (!leftExists && rightExists)) {
			goUpAndUpdateTree(node, power);
			removeNodeWithOnlyOneSon(node);
			return true;
		}

		return false;
	}

	/*
	removes the node if it is a leaf. otherwise removes the left leaf.
	otherwise removes the right leaf - O(1)
	*/
	void removeLeaf(AVLnode<K, T> *node) {
		AVLnode<K, T>* parent = node->parent;
		if (parent == NULL) {
			// In case of the node is the root
			delete node;
			root = NULL;
			return;
		}
		if (parent->left == node) {
			delete parent->left;
			parent->left = NULL;
		}
		else {
			delete parent->right;
			parent->right = NULL;
		}
	}

	// the function remove node with one children from the tree - O(1).
	void removeNodeWithOnlyOneSon(AVLnode<K, T> *node) {
		AVLnode<K, T>* t = node->parent;
		if (t == NULL) {
			if (node->left != NULL)
				root = node->left;
			else
				root = node->right;

			root->parent = NULL;
			delete node;
			return;
		}

		bool isTLeft = t->left == node;
		AVLnode<K, T>* son = (node->left == NULL && node->right != NULL) ?
							  node->right : node->left;
		if (son == NULL)
			return;

		if (isTLeft)
			t->left = son;
		else
			t->right = son;
		son->parent = t;

		node->left = NULL;
		node->right = NULL;
		node->parent = NULL;
		delete node;

		return;
	}

	// remove the number given of times the leafs from the tree - O(n)
	void removeNumOfRightLeafs(AVLnode<K, T>* node, int* num) {
		if (*num <= 0) {
			return;
		}
		if (node->right != NULL) {
			removeNumOfRightLeafs(node->right, num);
		} else {
			removeLeaf(node);
			(*(num))--;
			return;
		}

		removeNumOfRightLeafs(node->left, num);
	}

	/*
	using left order algorithm the function puts the keys and values in the
	tree
	node - current node of the tree.
	keys_array - the array of sorted keys.
	values_array - the values array.
	p_index - the current place of the arrays to be placed in the node.
	size - the amount of the nodes.
	O(n)
	*/
	void putKeysAndValuesAndPowerInTree(AVLnode<K, T>* node, K keys_array[],
							  T values_arr[], int* powers_array,
							  int* p_index, int size) {
		if (node == NULL || p_index == NULL || *p_index == size)
			return;
		if (node->left != NULL) {
			putKeysAndValuesAndPowerInTree(node->left, keys_array, values_arr,
								   	   	   powers_array, p_index, size);
			node->left->parent = node;
		}

		node->setKey(keys_array[*p_index]);
		node->setData(values_arr[*p_index]);
		node->setPower(powers_array[*p_index]);
		(*p_index)++;

		if (node->right != NULL) {
			putKeysAndValuesAndPowerInTree(node->right, keys_array, values_arr,
										   powers_array, p_index, size);
			node->right->parent = node;
		}
	}

	/*
	getting to the given keys_array the keys from the tree by there order
	O(n)
	*/
	void getKeysInorder(AVLnode<K, T>* node, K* keys_array, int* p_index) const {
		if (node == NULL)
			return;

		getKeysInorder(node->left, keys_array, p_index);
		keys_array[(*p_index)++] = K(node->getKey());
		getKeysInorder(node->right, keys_array, p_index);
	}

	/*
	getting to the given values_arr the value from the tree by there order
	O(n)
	*/
	void getDataInOrderInternal(AVLnode<K, T>* node, T* values_arr,
								int* p_index) {
		if (node == NULL)
			return;

		getDataInOrderInternal(node->left, values_arr, p_index);
		values_arr[(*p_index)++] = node->getData();
		getDataInOrderInternal(node->right, values_arr, p_index);
	}

	void updateSumPowerAndNumOfSubNodes(const K key, int power) {
		AVLnode<K, T>* node = root;
		while (node != NULL) {
			node->increaseSubTreeSumPower((-1) * power);
			node->decreaseSubTreeNodes();
			if (keysAreEqual(node->getKey(), key)) {
				return;
			}

			if(keyIsLessOrEqualThan(node->getKey(), key)) {
				node = node->right;
			} else {
				node = node->left;
			}
		}
	}

	void updateRanksOfNode(AVLnode<K, T>* node, int power) {
		if(node == NULL) return;
		node->decreaseSubTreeNodes();
		node->increaseSubTreeSumPower((-1) * power);
	}

	void goUpAndUpdateTree(AVLnode<K, T>* node, int power) {

		if(node == NULL) return;

		node = node->parent;
		while(node != NULL) {
			updateRanksOfNode(node, power);
			node = node->parent;
		}

	}

	void releaseAllNodes(AVLnode<K, T>* node) {
		if (node == NULL) {
			return;
		}

		releaseAllNodes(node->left);
		releaseAllNodes(node->right);
		delete node;
	}

	void cpyTree(AVLnode<K, T>* new_node, const AVLnode<K, T>* old_node) {

		if(old_node == NULL) return;

		if(old_node->getLeft() != NULL) {
			new_node->setLeft(new AVLnode<K, T>(*(old_node->getLeft())));
			AVLnode<K, T>* left = new_node->getLeft();
			left->setParent(new_node);
		}

		if(old_node->getRight() != NULL) {
			new_node->setRight(new AVLnode<K, T>(*(old_node->getRight())));
			AVLnode<K, T>* right = new_node->getRight();
			right->setParent(new_node);
		}

		cpyTree(new_node->getLeft() ,old_node->getLeft());
		cpyTree(new_node->getRight() ,old_node->getRight());

	}

public:
	//  C'tors

	// create a tree without nodes
	AVLtree() : root(NULL), size(0) {};

	// create a tree that points to a given node
	AVLtree(AVLnode<K, T>* n) : root(n), size(0) {};

	AVLtree(const AVLtree<K, T>& tree) {

		size = tree.size;

		if(size == 0) {
			root = NULL;
			return;
		}

		AVLnode<T, K>* new_root = new AVLnode<T, K>(*(tree.root));
		cpyTree(new_root, tree.root);

		root = new_root;

//		// update size
//		size = tree.getSize();
//
//		// create array of key, data, power
//		T* dataArr1 = new T[size];
//		K* keyArr1 = new K[size];
//		int* powerArr1 = new int[size];
//
//		// copy tree to arrays
//		int j = 0;
//		fillArraysFromTree(tree.getRoot(), keyArr1, dataArr1, powerArr1, &j);
//
//		// create empty tree
//		int counter = 0;
//		int n = size;
//
//		while (n > 0) {
//			n /= 2;
//			counter++;
//		}
//
//		AVLnode<K, T>* node = new AVLnode<K, T>(NULL, 0);
//		buildTree(node, counter - 1);
//		node->setHeight(counter - 1);
//
//		root = node;
//		int powOfTwo = 1;
//		while (counter-- > 0) {
//			powOfTwo *= 2;
//		}
//
//		removeRightLeafs(powOfTwo - size - 1);
//
//		// put to tree
//		int i = 0;
//		putKeysAndValues(keyArr1, dataArr1, powerArr1, &i, size);
//
//		// update sub nodes and sum of sub power
//		AuxUpdateSubNodesAndSubPowersOfWholeTree(root);

	}

	// D'ctor
	~AVLtree() {
		releaseAllNodes(root);
	}

	AVLtree<K, T>& operator = (const AVLtree<K, T>& tree) = delete;

//	{
//
//		if(this == &tree) return *this;
//
//		AVLtree<K, T>* temp_tree = new AVLtree<K, T>();
//
//		mergeTrees(&temp_tree, &tree, NULL, tree.getSize(), 0);
//
//		this->destroyTree();
//
//		this = temp_tree;
//
//		return *this;
//	}


	// release all the nodes and put null to the root and update the size
	void destroyTree() {
		releaseAllNodes(root);
		root = NULL;
		size = 0;
	}

	// set the root
	void setRoot(AVLnode<K, T>* node) {
		root = node;
	}

	/* puts the given key and value in the correct place in the true.
	 * if success return true otherwise return false.
	 * O(logn)
	 */
	bool insert(K key, T value, int power) {
		if (root == NULL) {
			root = new AVLnode<K, T>(K(key), value, NULL, power);
			size++;
			return true;
		}

		AVLnode<K, T> *node = root, *parent;

		while (true) {
			if (keysAreEqual(node->getKey(), key))
				return false;

			parent = node;
			parent->increaseSubTreeNodes();
			parent->increaseSubTreeSumPower(power);

			bool goLeft = keyIsLessThan(key, node->getKey());
			node = goLeft ? node->left : node->right;


			if (node == NULL) {
				if (goLeft) {
					parent->left = new AVLnode<K, T>(K(key), value, parent, power);
				}
				else {
					parent->right = new AVLnode<K, T>(K(key), value, parent, power);
				}

				parent->setHeight(1);

				rebalance(parent, true);
				break;
			}
		}

		size++;
		return true;
	}

	// deletes the given keys node from the tree - O(logn)
	void deleteKey(const K key) {
		if (root == NULL)
			return;

		AVLnode<K, T>* t = getNodeByKey(key);

		// Key wasn't found
		if (t == NULL)
			return;

		size--;
		AVLnode<K, T>* parent = t->parent;
		if (removeLeftOrMaxOneSon(t)) {
			if (parent != NULL) {
				parent->setHeight(height(parent));
				rebalance(parent, true);
			}
			else {
				// The node is the root
				if (root != NULL) {
					root->setBalanc(0);
					root->setHeight(0);
				}
			}
			return;
		}

		AVLnode<K, T>* w = getNextNode(root, key);

		// Swap data
		swapNodeKeyAndData(w, t);

		AVLnode<K, T>* wParent = w->parent;
		removeLeftOrMaxOneSon(w);

		wParent->setHeight(height(wParent));
		rebalance(wParent, true);
	}

	/* the given key is the max key. the function returns the previus value.
	 * O(logn)
	*/
	T getPreviousData(const K key) {
		AVLnode<K, T>* n = getNodeByKey(key);
		if (n->left != NULL)
			return n->getLeft()->getData();
		if (n->parent != NULL)
			return n->parent->getData();
		throw DataNotFound();
	}

	// check if the key is the max in the tree
	bool checkIfMax(const K key) const {

		// get the node of the key
		AVLnode<K, T>* n = getNodeByKey(key);

		AVLnode<K, T>* cur_node = root;
		AVLnode<K, T>* last_node = root;

		// get in last node the max node
		while(cur_node != NULL) {
			last_node = cur_node;
			cur_node = cur_node->right;
		}

		if(n == last_node) {
			return true;
		} else {
			return false;
		}
	}

	/* get the given value of the key - O(logn)
	 * if not found throw DataNotFound.
	*/
	const T searchInTree(const K key) const {
		AVLnode<K, T> * node = getNodeByKey(key);
		if (node == NULL)
			throw DataNotFound();
		return node->getData();
	}

	/* get the given value of the key - O(logn)
	 * if not found throw DataNotFound.
	*/
	T	searchInTree(const K key) {
		AVLnode<K, T> * res = getNodeByKey(key);
		if (res == NULL)
			throw DataNotFound();
		return res->getData();
	}

	// get the size of the tree
	int getSize() const {
		return size;
	}

	// remove the number given of times the leafs from the tree - O(n)
	void removeRightLeafs(int num) {
		if (num <= 0)
			return;
		removeNumOfRightLeafs(root, &num);
	}

	/*
	using left order algorithm the function puts the keys and values int the
	tree
	node - current node of the tree.
	keys_array - the array of sorted keys.
	values_array - the values array.
	p_index - the current place of the arrays to be placed in the node.
	size - the amount of the nodes.
	O(n)
	*/
	void putKeysAndValues(K* keys_array, T* values_array, int* powers_array,
						  int* p_index, int array_size) {
		putKeysAndValuesAndPowerInTree(root, keys_array, values_array, powers_array,
							   p_index, array_size);
		size = array_size;
	}

	/*
	getting to the given keys_array the keys from the tree by there order
	O(n)
	*/
	void getKeysInorder(K* keys_array) const {
		int i = 0;
		getKeysInorder(root, keys_array, &i);
	}

	/*
	getting to the given values_arr the value from the tree by there order
	O(n)
	*/
	void getDataInOrder(T* values_array) {
		int index = 0;
		getDataInOrderInternal(root, values_array, &index);
	}


	/*
	 * recursive function that gets a rank and return a key with this rank.
	 * O(logn). pay attantion that this function assumes that key with this
	 * rank exists
	 */
	K SelectRec(AVLnode<K, T> * node, int rank) {

		AVLnode<K, T> * left = node->left;

		int w_left;
		if(left == NULL) {
			w_left = 0;
		} else {
			w_left = left->getSubTreeNodes();
		}

		if(w_left == (rank - 1)) {
			return node->getKey();
		} else if(w_left > (rank - 1)) {
			return SelectRec(left, rank);
		} else {
			return SelectRec(node->right, rank - w_left - 1);
		}

		return node->getKey();
	}

	/*
	 * return a key from the tree that has the given rank
	 * O(logn). pay attantion that this function assumes that key with this
	 * rank exists
	 */
	K Select(int rank) {
		return SelectRec(root, rank);
	}

	// return the sum of power of all students in the tree - O(1)
	int getAllTreeSumOfPower() const {
		if(root == NULL) return 0;
		return root->getSubTreeSum();
	}

	/*
	 * returns the sum of power of those students that has bigger or the same
	 * rank as the given rank - O(logn)
	 */
	int getSumOfAllPowersBiggerThenRank(int rank) const {

		return getSumRecursive(root, rank, 0);

	}

	/*
	 * returns the sum of power of those students that has bigger or the same
	 * rank as the given rank - O(logn)
	 */
	int getSumRecursive(AVLnode<K, T> * node, int rank, int sum) const {

		if(node == NULL) return 0;
		if(rank <= 0) return 0;

		AVLnode<K, T> * left = node->left;
		AVLnode<K, T> * right = node->right;

		int w_left;
		int sum_right;

		if(left == NULL) {
			w_left = 0;
		} else {
			w_left = left->getSubTreeNodes();
		}

		if(right == NULL) {
			sum_right = 0;
		} else {
			sum_right = right->getSubTreeSum();
		}

		if(w_left == (rank - 1)) {
			return sum + sum_right + node->getPower();
		} else if(w_left > (rank - 1)) {
			return getSumRecursive(left, rank,
								   sum + sum_right + node->getPower());
		} else {
			return getSumRecursive(node->right, rank - w_left - 1, sum);
		}

		return sum;
	}

	AVLnode<K, T>* getRoot() {
		return root;
	}

	const AVLnode<K, T>* getRoot() const {
		return root;
	}

	const T getBiggestKeysData() const {

		AVLnode<K, T>* node = root;
		if(node == NULL) throw DataNotFound();
		while(node->right != NULL) {
			node = node->right;
		}

		return node->getData();
	}

};

/* the function merges the two given key array and value array in right order
 * O(max(m,n))
 * m - the first arrays size
 * n - the second arrays size
*/
template <class K, class T>
void merge(K keys_array1[], K keys_array2[],
		   T values_array1[], T values_array2[],
		   int powers_array1[], int powers_array2[],
		   int m, int n,
		   K keyMergesArr[], T dataMergedArr[], int powersMergedArr[]) {

	int i = 0, j = 0, k = 0;
	while (i < m && j < n) {
		if (keys_array1[i] < keys_array2[j]) {
			keyMergesArr[k] = K(keys_array1[i]);
			dataMergedArr[k] = values_array1[i];
			powersMergedArr[k] = powers_array1[i];
			i++;
		}
		else {
			keyMergesArr[k] = K(keys_array2[j]);
			dataMergedArr[k] = values_array2[j];
			powersMergedArr[k] = powers_array2[j];
			j++;
		}
		k++;
	}

	while (i < m)
	{
		keyMergesArr[k] = K(keys_array1[i]);
		dataMergedArr[k] = values_array1[i];
		powersMergedArr[k] = powers_array1[i];
		i++;
		k++;
	}

	while (j < n)
	{
		keyMergesArr[k] = K(keys_array2[j]);
		dataMergedArr[k] = values_array2[j];
		powersMergedArr[k] = powers_array2[j];
		j++;
		k++;
	}
}


// make a empty new tree in the given height - O(n)
template <class K, class T>
AVLtree<K, T>* makeEmptyTree(AVLtree<K, T>* tree, int height) {
	if (height <= 0)
		return NULL;

	AVLnode<K, T>* node = new AVLnode<K, T>(NULL, 0);
	buildTree(node, height - 1);
	node->setHeight(height - 1);

	tree->setRoot(node);
	return tree;
}


/* using the array the function makes a new tree with nodes containing the
 * keys and values from the array. - o(n)
*/
template <class K, class T>
AVLtree<K, T>* makeTreeFromArray(AVLtree<K, T>* tree,
								 K* keys_array, T* data_array,
								 int* powers_array, int size) {
	if (size <= 0)
		return NULL;

	int counter = 0;
	int n = size;

	while (n > 0) {
		n /= 2;
		counter++;
	}

	makeEmptyTree<K, T>(tree, counter);
	int powOfTwo = 1;
	while (counter-- > 0) {
		powOfTwo *= 2;
	}

	// size of tree begin from 0
	tree->removeRightLeafs(powOfTwo - size - 1);
	int i = 0;
	tree->putKeysAndValues(keys_array, data_array, powers_array, &i, size);

	return tree;
}
/* the function makes a new tree under the given node, in the required
 * height without freeing the old nodes - O(n)
*/
template <class K, class T>
void buildTree(AVLnode<K, T>* node, int height) {
	if (height <= 0 || node == NULL)
		return;

	node->left = new AVLnode<K, T>(node, 0);
	node->right = new AVLnode<K, T>(node, 0);

	node->setHeight(height -1);

	buildTree(node->left, height - 1);
	buildTree(node->right, height - 1);
}


/* fill arrays of key and data in order from a tree
 * O(n)
 */
template <class K, class T>
void fillArraysFromTree(const AVLnode<K, T>* node, K keys_array[],
				  	    T datas_array[], int powers_array[], int *index) {

	if (node == NULL) {
		return;
	}

	fillArraysFromTree(node->getLeft(), keys_array, datas_array,
					   powers_array, index);

	datas_array[(*index)] = T(node->getData());
	keys_array[(*index)] = K(node->getKey());
	powers_array[(*index)++] = int(node->getPower());

	fillArraysFromTree(node->getRight(), keys_array, datas_array,
					   powers_array, index);
}

template <class K, class T>
void fillNodesArrayFromTree(const AVLnode<K, T>* node,
							AVLnode<K, T> nodes[],
							int* index) {

	if (node == NULL) {
		return;
	}

	fillArraysFromTree(node->getLeft(), nodes, index);

	nodes[(*index)] = *node;

	fillArraysFromTree(node->getRight(), nodes, index);
}

/* go through all the tree and update in each node the sub nodes and
 * sum power of sub nodes
 * O(logn)
 */
template <class K, class T>
void AuxUpdateSubNodesAndSubPowersOfWholeTree(AVLnode<K, T>* node) {

	if(node == NULL) return;

	AuxUpdateSubNodesAndSubPowersOfWholeTree(node->left);
	AuxUpdateSubNodesAndSubPowersOfWholeTree(node->right);

	AVLnode<K, T>* left = node->left;
	AVLnode<K, T>* right = node->right;

	if(left == NULL && right == NULL) {
		node->setSubTreeNodes(1);
		node->setSubTreeSum(node->getPower());

	} else if(left == NULL && right != NULL) {
		node->setSubTreeNodes(right->getSubTreeNodes() + 1);
		node->setSubTreeSum(right->getSubTreeSum() + node->getPower());

	} else if(left != NULL && right == NULL) {
		node->setSubTreeNodes(left->getSubTreeNodes() + 1);
		node->setSubTreeSum(left->getSubTreeSum() + node->getPower());

	} else {
		node->setSubTreeNodes(left->getSubTreeNodes() +
							  right->getSubTreeNodes() + 1);
		node->setSubTreeSum(left->getSubTreeSum() +
							right->getSubTreeSum() +
							node->getPower());
	}


}

/* go through all the tree and update in each node the sub nodes and
 * sum power of sub nodes
 * O(logn)
 */
template <class K, class T>
void updateSubNodesAndSubPowersOfWholeTree(AVLtree<K, T>* tree) {

		if(tree == NULL) return;
		AuxUpdateSubNodesAndSubPowersOfWholeTree(tree->getRoot());

}

/* merges two AVL trees into one AVL tree
 * O(first tree size + second tree size)
 */
template <class K, class T>
void mergeTrees(AVLtree<K, T>* tree, AVLtree<K, T>* tree1, AVLtree<K, T>* tree2,
				int firstTreeSize, int secondTreeSize) {
	T* dataMergeArr, *dataArr2;
	K* keyMergeArr, *keyArr2;
	int* powersMergeArr, *powersArr2;

	int size = firstTreeSize + secondTreeSize;

	int i = 0;
	bool isFirstTreeEmpty = tree1 == NULL || tree1->getRoot() == NULL,
		isSecondTreeEmpty = tree2 == NULL || tree2->getRoot() == NULL;
	if (isFirstTreeEmpty && isSecondTreeEmpty)
		return;

	if (isFirstTreeEmpty || isSecondTreeEmpty || (tree1 == tree2)) {
		if (isFirstTreeEmpty)
			size = secondTreeSize;
		else
			size = firstTreeSize;

		dataMergeArr = new T[!isFirstTreeEmpty ? firstTreeSize : secondTreeSize];
		keyMergeArr = new K[!isFirstTreeEmpty ? firstTreeSize : secondTreeSize];
		powersMergeArr = new int[!isFirstTreeEmpty ? firstTreeSize : secondTreeSize];
		fillArraysFromTree(isFirstTreeEmpty ? tree2->getRoot() :
						   tree1->getRoot(), keyMergeArr, dataMergeArr,
						   powersMergeArr, &i);
	}
	else {
		T* dataArr1 = new T[firstTreeSize];
		K* keyArr1 = new K[firstTreeSize];
		int* powerArr1 = new int[firstTreeSize];
		fillArraysFromTree<K, T>(tree1->getRoot(), keyArr1, dataArr1, powerArr1, &i);

		i = 0;
		dataArr2 = new T[secondTreeSize];
		keyArr2 = new K[secondTreeSize];
		powersArr2 = new int[secondTreeSize];
		fillArraysFromTree(tree2->getRoot(), keyArr2, dataArr2, powersArr2, &i);

		dataMergeArr = new T[firstTreeSize + secondTreeSize];
		keyMergeArr = new K[firstTreeSize + secondTreeSize];
		powersMergeArr = new int[firstTreeSize + secondTreeSize];

		merge<K, T>(keyArr1, keyArr2, dataArr1, dataArr2, powerArr1, powersArr2,
					firstTreeSize, secondTreeSize,
					keyMergeArr, dataMergeArr, powersMergeArr);

		delete[] dataArr1;
		delete[] keyArr1;
		delete[] dataArr2;
		delete[] keyArr2;
		delete[] powerArr1;
		delete[] powersArr2;
	}

	makeTreeFromArray(tree, keyMergeArr, dataMergeArr, powersMergeArr, size);

	delete[] dataMergeArr;
	delete[] keyMergeArr;
	delete[] powersMergeArr;

	updateSubNodesAndSubPowersOfWholeTree(tree);
}


#endif // AVL_TREE_H
