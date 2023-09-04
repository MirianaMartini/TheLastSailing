#pragma once
#include "Settings.h"

/* TIPI */
typedef struct {
	time_t name;
	GLint score;
} scoreRecord;

class File {
public:

	void CreateFiles();
	bool SavePointsInFile(GLint level, GLfloat points);
	std::string GetBestScore(GLint level);

private:
	char* GetDate();

};

