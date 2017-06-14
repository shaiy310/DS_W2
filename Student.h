#ifndef STUDENT_H_
#define STUDENT_H_

#include "AVLTree.h"

class Student {

private:

	int id;
	int power;

public:

	Student(int i = -1, int p = -1) :
					id(i), power(p) {}

	~Student() {}

	int getID() const {
		return id;
	}

	int getPower() const {
		return power;
	}

	void increasePower(int p) {
		power += p;
	}

	friend bool operator < (const Student& student1, const Student& student2) {

		if(student1.power < student2.power) return true;
		if(student1.power == student2.power) {
			if(student1.id > student2.id) {
				return true;
			}
		}
		return false;
	}

};



#endif /* STUDENT_H_ */
