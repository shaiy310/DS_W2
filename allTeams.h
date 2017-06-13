#ifndef ALLTEAMS_H_
#define ALLTEAMS_H_

#include "Team.h"
#include "unionFind.h"

class allTeams {

private:

	int size;
	UnionFind teams_union;
	Team** teams_data;

public:

	allTeams(int n) : size(n), teams_union(UnionFind(n)),
							   teams_data(new Team*[n]) {

		for(int i = 0; i < n; i++) {
			teams_data[i] = new Team();
		}

	}

	~allTeams() {
		for(int i = 0; i < size; i++) {
			delete teams_data[i];
		}
		delete[] teams_data;
	}

	allTeams(const allTeams& all_teams) = delete;

	allTeams& operator = (const allTeams& all_teams) = delete;

	bool addStudent(Student student, int team_num) {
		try {
			int real_team_num = teams_union.find(team_num);
			return teams_data[real_team_num - 1]->addStudent(student);

		} catch(UnionFind::BadGroup& e) {
			return false;
		}
	}

	bool removeStudent(Student student, int team_num) {
		try {
			int real_team_num = teams_union.find(team_num);
			return teams_data[real_team_num - 1]->deleteStudent(student);

		} catch(UnionFind::BadGroup& e) {
			return false;
		}
	}

	bool joinTeams(int team1, int team2) {

		try {

			// check if same teams
			if(team1 == team2) return true;

			// check if they are under same group
			int real_team1 = teams_union.find(team1);
			int real_team2 = teams_union.find(team2);
			if(real_team1 == real_team2) return true;

			// make the union
			int joined_team_num = teams_union.Union(real_team1, real_team2);

			int other_team_num = joined_team_num == real_team1 ? real_team2 :
																 real_team1;

			// get the two teams
			Team bigger_team = *teams_data[joined_team_num - 1];
			Team smaller_team = *teams_data[other_team_num - 1];

			AVLtree<Student, Student> merged_tree;

			AVLtree<Student, Student> bigger_team_tree = bigger_team.getTree();

			AVLtree<Student, Student> smaller_team_tree = smaller_team.getTree();

			mergeTrees(&merged_tree,
					   &bigger_team_tree,
					   &smaller_team_tree,
					   bigger_team.getSize(),
					   smaller_team.getSize());

			Student best_student = merged_tree.getBiggestKeysData();

			// delete the teams
			delete teams_data[other_team_num - 1];
			teams_data[other_team_num - 1] = NULL;
			delete teams_data[joined_team_num - 1];
			teams_data[joined_team_num - 1] = NULL;

			// put the new joined team
			teams_data[joined_team_num - 1] = new Team
					(bigger_team.getSize() + smaller_team.getSize(),
					 merged_tree,
					 bigger_team.getVicNum() + smaller_team.getVicNum(),
					 best_student.getPower(),
					 best_student.getID());

		} catch(exception& e) {
			return false;
		}

		return true;
	}

	void teamFight(int team1, int team2, int num_of_fighters) {

		if(team1 == team2) return;

		// check if they are under same group
		try {
			int real_team1 = teams_union.find(team1);
			int real_team2 = teams_union.find(team2);
			if(real_team1 == real_team2) return;

			int sum1, sum2;
			sum1 = teams_data[real_team1 - 1]->sumOfBestFighters(num_of_fighters);
			sum2 = teams_data[real_team2 - 1]->sumOfBestFighters(num_of_fighters);

			if(sum1 > sum2) {
				teams_data[real_team1 - 1]->increaseVicNum(1);
			} else if(sum1 < sum2) {
				teams_data[real_team2 - 1]->increaseVicNum(1);
			}
		} catch(UnionFind::BadGroup& e) {
			return;
		}
	}

	bool getNumOfWins(int team, int* Wins) const {

		if(team <= 0 || Wins == NULL) return false;
		try {
			int real_team = teams_union.find(team);
			*Wins = teams_data[real_team - 1]->getVicNum();
			return true;
		} catch(UnionFind::BadGroup& e) {
			return false;
		}
	}

	bool getTeamLeader(int team, int* Leader) const {

		if(team <= 0 || Leader == NULL) return false;
		try {
			int real_team = teams_union.find(team);
			*Leader = teams_data[real_team - 1]->getIdBiggestPower();
			return true;
		} catch(UnionFind::BadGroup& e) {
			return false;
		}
	}



	// delete
//	Team** getArray() const {
//
//		return teams_data;
//
//	}

};


#endif /* ALLTEAMS_H_ */
