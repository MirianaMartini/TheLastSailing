#include <stdio.h>
#include <gl/glut.h>

class Clicking {
public:
	Clicking();
	Clicking(int sw, int sh);
	//~Clicking();

	void draw();
	void init(int w, int h);
	void select(int x, int y);
	void selectAll(GLint hits, GLuint* buff);
	void mouseClick(int button, int state, int x, int y);
	void mouseDw(int x, int y, int but);
	void drawBlock(float x, float y, float z);
	void listHits(GLint hits, GLuint* names);

	//Getters
	int getSelected() const;

private:
	int _selected = 0;
	int _sw = 0;
	int _sh = 0;

};

