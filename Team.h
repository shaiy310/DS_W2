#ifndef TEAM_H_
#define TEAM_H_

#include "AVLTree.h"
#include "Student.h"

using namespace std;

class Team {

private:

	int size;
	AVLtree<Student, Student> tree;
	int victory_num;
	int biggest_power;
	int id_biggest_power;

public:

	explicit Team(int size = 0,
				  AVLtree<Student, Student> tree = AVLtree<Student, Student>(),
				  int victory_num = 0,
				  int biggest_power = -1,
				  int id_biggest_power = -1) :
				  size(size), tree(tree), victory_num(victory_num),
				  biggest_power(biggest_power),
				  id_biggest_power(id_biggest_power) {}

	~Team() = default;

//	Team(const Team& t) = default;
//
//	Team& operator = (const Team& t) = default;

	void incSize() {
		size++;
	}

	void decSize() {
		size--;
	}

	const int getSize() const {
		return size;
	}

	bool addStudent(Student student) {

		if(tree.insert(student, student, student.getPower())) {
			// update size
			size++;

			// update max power
			if(student.getPower() > biggest_power) {
				biggest_power = student.getPower();
				id_biggest_power = student.getID();
			} else if(student.getPower() == biggest_power &&
					  student.getID() < id_biggest_power) {
				id_biggest_power = student.getID();
			}

			return true;
		}

		return false;
	}

	bool studentIsInTheTree(Student student) const {
		try {
			tree.searchInTree(student);
		} catch(exception& e) {
			return false;
		}
		return true;
	}

	void increaseVicNum(int num) {
		victory_num += num;
	}

	int getVicNum() const {
		return victory_num;
	}

	int getIdBiggestPower() const {
		return id_biggest_power;
	}

	// doing nothing if student is missing
	bool deleteStudent(Student student) {

		if(!studentIsInTheTree(student)) {
			return false;
		}

		// delete
		tree.deleteKey(student);

		// update size.
		size--;

		// update max power
		if(student.getPower() == biggest_power &&
		   student.getID() == id_biggest_power) {

			if(tree.getSize() == 0) {
				biggest_power = -1;
				id_biggest_power = -1;
			} else {
				Student biggest_student = tree.getBiggestKeysData();
				biggest_power = biggest_student.getPower();
				id_biggest_power = biggest_student.getID();
			}
		}

		return true;
	}

	const AVLtree<Student, Student> getTree() const {
		return tree;
	}

	AVLtree<Student, Student>* getPointerToTree() {
		return &tree;
	}

	int sumOfBestFighters(int num) const {

		if(num <= 0) return 0;

		if(num >= size) {
			return tree.getAllTreeSumOfPower();
		}

		return tree.getSumOfAllPowersBiggerThenRank(size - num + 1);
	}



};



#endif /* TEAM_H_ */
