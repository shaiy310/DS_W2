#include "allTeams.h"
#include <iostream>
#include "Team.h"

#define N 5

using namespace std;


int main() {

//	AVLtree<Student, Student> tree;
//	Student s1(1,1);
//	Student s2(2,2);
//	Student s3(3,3);
//	Student s4(4,4);
//
//	tree.insert(s2,s2,2);
//	tree.insert(s1,s1,1);
//	tree.insert(s3,s3,3);
//	tree.insert(s4,s4,4);
//
//	AVLtree<Student, Student> tree1 = tree;
//	AVLtree<Student, Student> tree2 = tree1;
//	cout << "anc" << endl;
//	cout << "anc" << endl;



	allTeams all_teams(N);

	all_teams.addStudent(Student(2,2), 2);
	all_teams.addStudent(Student(1,1), 1);
	all_teams.addStudent(Student(3,3), 3);
	all_teams.addStudent(Student(4,4), 4);


	/* team 1 - 1
	 * team 2 - 2
	 * team 3 - 3
	 * team 4 - 4
	 * team 5 -
	 */

	all_teams.joinTeams(3,2);
	all_teams.joinTeams(1,4);
	all_teams.addStudent(Student(5,5), 5);
	all_teams.addStudent(Student(6,6), 2);
	all_teams.joinTeams(5,1);

	/* team
	 * team 3, 2 -  2, 3, 6
	 * team
	 * team 4, 1, 5 - 1, 4, 5
	 * team
	 */

	all_teams.teamFight(1, 3, 2);

//	Team** teams_data = all_teams.getArray();
//
//	for(int i = 0; i < N; i++) {
//		Team* team = teams_data[i];
//		if(team != NULL) {
//			AVLtree<Student, Student> tree = team->getTree();
//		}
//	}

	int Leader;
	cout << all_teams.getTeamLeader(7, &Leader) << endl;
	cout << "Leader " << Leader << endl;


	return 0;
}







