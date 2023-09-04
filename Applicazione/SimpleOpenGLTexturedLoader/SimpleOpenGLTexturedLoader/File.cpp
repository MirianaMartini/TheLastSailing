#include "File.h"

extern File file;
extern Score score;

void File::CreateFiles() {
	ofstream myfile;

	myfile.open("scores_lv1.txt", ios::app);
	myfile.close();

	myfile.open("scores_lv2.txt", ios::app);
	myfile.close();

	myfile.open("scores_lv3.txt", ios::app);
	myfile.close();
}

bool File::SavePointsInFile(GLint level, GLfloat points) {
	ofstream myfile;

	score.oldScore = std::stof(GetBestScore(level));
	
	if (score.oldScore > points) return false;
	else {
		switch (level) {
		case LEVEL_1:
			myfile.open("scores_lv1.txt", ios::out);
			break;
		case LEVEL_2:
			myfile.open("scores_lv2.txt", ios::out);
			break;
		case LEVEL_3:
			myfile.open("scores_lv3.txt", ios::out);
			break;
		}

		time_t id = time(0);
		myfile << points << "_" << id << "_" << GetDate();
		myfile.close();

		cout << "-------------------- " << score.oldScore << endl;
		return true;
	}
}

 string File::GetBestScore(GLint level) {
	ifstream myfile;
	std::string scr;

	switch (level) {
	case LEVEL_1:
		myfile.open("scores_lv1.txt");
		break;
	case LEVEL_2:
		myfile.open("scores_lv2.txt");
		break;
	case LEVEL_3:
		myfile.open("scores_lv3.txt");
		break;
	}
	myfile >> scr;
	myfile.close();

	if (scr == "") scr = "0";
	else scr = scr.substr(0, scr.find("_"));
	
	return scr;	
}

 char* File::GetDate(void) {
	 time_t now = time(0);
	 char* dt = ctime(&now);
	 return dt;
 }