#include "Settings.h"
#include "Mouse.h"

extern Mouse mouse;
//extern Ship ship;

Mouse::Mouse() {}
Mouse::~Mouse() {}

void Mouse::GetMouseState(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        if (button == GLUT_LEFT_BUTTON) {
            mouse.left_down = true;
            printf("LD -> x = %f - y = %f\n", mouse.pixel_x, mouse.pixel_y);
        }
        else if (button == GLUT_RIGHT_BUTTON) {
            mouse.right_down = true;
            printf("RD -> x = %f - y = %f\n", mouse.pixel_x, mouse.pixel_y);
        }
    }
    else if (state == GLUT_UP) {
        if (button == GLUT_LEFT_BUTTON) {
            mouse.left_down = false;
            printf("LU -> x = %f - y = %f\n", mouse.pixel_x, mouse.pixel_y);
        }
        else if (button == GLUT_RIGHT_BUTTON) {
            mouse.right_down = false;
            printf("RU -> x = %f - y = %f\n", mouse.pixel_x, mouse.pixel_y);
        }
    }
}

void Mouse::SetMousePixelPosition(int x, int y) {
    mouse.pixel_x = static_cast<float>(x);
    mouse.pixel_y = static_cast<float>(y);
}


/* ACTIONS */
//Menu
bool Mouse::StartPressed() {
    float _width = glutGet(GLUT_WINDOW_WIDTH);
    float _height = glutGet(GLUT_WINDOW_HEIGHT);
    float x_ratio = _width / SW;
    float y_ratio = _height / SH;

    if ((pixel_x >= x_ratio * 1155 && pixel_x <= x_ratio * 1580 && pixel_y >= y_ratio * 200 && pixel_y <= y_ratio * 330)
        && mouse.left_down)
        return true;
    else return false;
}

bool Mouse::StartHover() {
    float _width = glutGet(GLUT_WINDOW_WIDTH);
    float _height = glutGet(GLUT_WINDOW_HEIGHT);
    float x_ratio = _width / SW;
    float y_ratio = _height / SH;

    if ((pixel_x >= x_ratio * 1155 && pixel_x <= x_ratio * 1550 && pixel_y >= y_ratio * 200 && pixel_y <= y_ratio * 330)
        && !mouse.left_down)
        return true;
    else return false;
}

bool Mouse::RulesPressed() {
    float _width = glutGet(GLUT_WINDOW_WIDTH);
    float _height = glutGet(GLUT_WINDOW_HEIGHT);
    float x_ratio = _width / SW;
    float y_ratio = _height / SH;

    if ((pixel_x >= x_ratio * 1145 && pixel_x <= x_ratio * 1550 && pixel_y >= y_ratio * 435 && pixel_y <= y_ratio * 560)
        && mouse.left_down)
        return true;
    else return false;
}

bool Mouse::RulesHover() {
    float _width = glutGet(GLUT_WINDOW_WIDTH);
    float _height = glutGet(GLUT_WINDOW_HEIGHT);
    float x_ratio = _width / SW;
    float y_ratio = _height / SH;

    if ((pixel_x >= x_ratio * 1145 && pixel_x <= x_ratio * 1550 && pixel_y >= y_ratio * 435 && pixel_y <= y_ratio * 560)
        && !mouse.left_down)
        return true;
    else return false;
}

bool Mouse::CommandPressed() {
    float _width = glutGet(GLUT_WINDOW_WIDTH);
    float _height = glutGet(GLUT_WINDOW_HEIGHT);
    float x_ratio = _width / SW;
    float y_ratio = _height / SH;

    if ((pixel_x >= x_ratio * 1000 && pixel_x <= x_ratio * 1690 && pixel_y >= y_ratio * 690 && pixel_y <= y_ratio * 820)
        && mouse.left_down)
        return true;
    else return false;
}

bool Mouse::CommandHover() {
    float _width = glutGet(GLUT_WINDOW_WIDTH);
    float _height = glutGet(GLUT_WINDOW_HEIGHT);
    float x_ratio = _width / SW;
    float y_ratio = _height / SH;

    if ((pixel_x >= x_ratio * 1000 && pixel_x <= x_ratio * 1690 && pixel_y >= y_ratio * 690 && pixel_y <= y_ratio * 820)
        && !mouse.left_down)
        return true;
    else return false;
}

//Rules
bool Mouse::BackToMenuRulesPressed() {
    float _width = glutGet(GLUT_WINDOW_WIDTH);
    float _height = glutGet(GLUT_WINDOW_HEIGHT);
    float x_ratio = _width / SW;
    float y_ratio = _height / SH;

    if ((pixel_x >= x_ratio * 1320 && pixel_x <= x_ratio * 1885 && pixel_y >= y_ratio * 940 && pixel_y <= y_ratio * 1025)
        && mouse.left_down)
        return true;
    else return false;
}

bool Mouse::BackToMenuRulesHover() {
    float _width = glutGet(GLUT_WINDOW_WIDTH);
    float _height = glutGet(GLUT_WINDOW_HEIGHT);
    float x_ratio = _width / SW;
    float y_ratio = _height / SH;

    if ((pixel_x >= x_ratio * 1320 && pixel_x <= x_ratio * 1885 && pixel_y >= y_ratio * 940 && pixel_y <= y_ratio * 1025)
        && !mouse.left_down)
        return true;
    else return false;
}

//Commands
bool Mouse::BackToMenuCommandsPressed() {
    float _width = glutGet(GLUT_WINDOW_WIDTH);
    float _height = glutGet(GLUT_WINDOW_HEIGHT);
    float x_ratio = _width / SW;
    float y_ratio = _height / SH;

    if ((pixel_x >= x_ratio * 1450 && pixel_x <= x_ratio * 1835 && pixel_y >= y_ratio * 985 && pixel_y <= y_ratio * 1045)
        && mouse.left_down)
        return true;
    else return false;
}

bool Mouse::BackToMenuCommandsHover() {
    float _width = glutGet(GLUT_WINDOW_WIDTH);
    float _height = glutGet(GLUT_WINDOW_HEIGHT);
    float x_ratio = _width / SW;
    float y_ratio = _height / SH;

    if ((pixel_x >= x_ratio * 1450 && pixel_x <= x_ratio * 1835 && pixel_y >= y_ratio * 985 && pixel_y <= y_ratio * 1045)
        && !mouse.left_down)
        return true;
    else return false;
}

//Victory
bool Mouse::NextLevelPressed() {
    float _width = glutGet(GLUT_WINDOW_WIDTH);
    float _height = glutGet(GLUT_WINDOW_HEIGHT);
    float x_ratio = _width / SW;
    float y_ratio = _height / SH;

    if ((pixel_x >= x_ratio * 1400 && pixel_x <= x_ratio * 1790 && pixel_y >= y_ratio * 940 && pixel_y <= y_ratio * 1030)
        && mouse.left_down)
        return true;
    else return false;
}

bool Mouse::NextLevelHover() {
    float _width = glutGet(GLUT_WINDOW_WIDTH);
    float _height = glutGet(GLUT_WINDOW_HEIGHT);
    float x_ratio = _width / SW;
    float y_ratio = _height / SH;

    if ((pixel_x >= x_ratio * 1400 && pixel_x <= x_ratio * 1790 && pixel_y >= y_ratio * 940 && pixel_y <= y_ratio * 1030)
        && !mouse.left_down)
        return true;
    else return false;
}

bool Mouse::BackToMenuVictoryPressed() {
    float _width = glutGet(GLUT_WINDOW_WIDTH);
    float _height = glutGet(GLUT_WINDOW_HEIGHT);
    float x_ratio = _width / SW;
    float y_ratio = _height / SH;

    if ((pixel_x >= x_ratio * 123 && pixel_x <= x_ratio * 770 && pixel_y >= y_ratio * 940 && pixel_y <= y_ratio * 1030)
        && mouse.left_down)
        return true;
    else return false;
}

bool Mouse::BackToMenuVictoryHover() {
    float _width = glutGet(GLUT_WINDOW_WIDTH);
    float _height = glutGet(GLUT_WINDOW_HEIGHT);
    float x_ratio = _width / SW;
    float y_ratio = _height / SH;

    if ((pixel_x >= x_ratio * 123 && pixel_x <= x_ratio * 770 && pixel_y >= y_ratio * 940 && pixel_y <= y_ratio * 1030)
        && !mouse.left_down)
        return true;
    else return false;
}

//Gameover
bool Mouse::TryAgainPressed() {
    float _width = glutGet(GLUT_WINDOW_WIDTH);
    float _height = glutGet(GLUT_WINDOW_HEIGHT);
    float x_ratio = _width / SW;
    float y_ratio = _height / SH;

    if ((pixel_x >= x_ratio * 1320 && pixel_x <= x_ratio * 1790 && pixel_y >= y_ratio * 940 && pixel_y <= y_ratio * 1030)
        && mouse.left_down)
        return true;
    else return false;
}

bool Mouse::TryAgainHover() {
    float _width = glutGet(GLUT_WINDOW_WIDTH);
    float _height = glutGet(GLUT_WINDOW_HEIGHT);
    float x_ratio = _width / SW;
    float y_ratio = _height / SH;

    if ((pixel_x >= x_ratio * 1320 && pixel_x <= x_ratio * 1790 && pixel_y >= y_ratio * 940 && pixel_y <= y_ratio * 1030)
        && !mouse.left_down)
        return true;
    else return false;
}

bool Mouse::BackToMenuGameoverPressed() {
    float _width = glutGet(GLUT_WINDOW_WIDTH);
    float _height = glutGet(GLUT_WINDOW_HEIGHT);
    float x_ratio = _width / SW;
    float y_ratio = _height / SH;

    if ((pixel_x >= x_ratio * 123 && pixel_x <= x_ratio * 770 && pixel_y >= y_ratio * 940 && pixel_y <= y_ratio * 1030)
        && mouse.left_down)
        return true;
    else return false;
}

bool Mouse::BackToMenuGameoverHover() {
    float _width = glutGet(GLUT_WINDOW_WIDTH);
    float _height = glutGet(GLUT_WINDOW_HEIGHT);
    float x_ratio = _width / SW;
    float y_ratio = _height / SH;

    if ((pixel_x >= x_ratio * 123 && pixel_x <= x_ratio * 770 && pixel_y >= y_ratio * 940 && pixel_y <= y_ratio * 1030)
        && !mouse.left_down)
        return true;
    else return false;
}