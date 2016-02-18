#pragma once

static bool init;
static void render();
static void mouseEvent(int, int, int, int);
static void keyboardEvent(unsigned char, int, int);
static void mouseMove(int, int);
int window;
void onRender();
void onMouseEvent(int, int, int, int);
void onKeyboardEvent(unsigned char, int, int);
void onMouseMove(int x, int y);