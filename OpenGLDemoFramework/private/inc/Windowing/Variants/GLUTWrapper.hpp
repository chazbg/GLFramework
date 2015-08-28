#pragma once

static bool init;
static void render();
static void mouseEvent(int, int, int, int);
static void keyboardEvent(unsigned char, int, int);
int window;
void onRender();
void onMouseEvent(int, int, int, int);
void onKeyboardEvent(unsigned char, int, int);