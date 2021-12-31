#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "PrimitiveRenderer.h"

#define CLOSE_WINDOW ALLEGRO_EVENT_DISPLAY_CLOSE
#define KEY_DOWN ALLEGRO_EVENT_KEY_DOWN
#define key event.keyboard.keycode


//! Klasa zawierająca metody tworzące okno, timer, kolejkę zdarzeń. Pozwala na włączenie klawiatury oraz myszy jako źródeł zdarzeń.
class Engine
{
private:
	Engine() {};
	ALLEGRO_DISPLAY* display = nullptr;
	ALLEGRO_EVENT_QUEUE* queue = nullptr;
	ALLEGRO_TIMER* timer = nullptr;
	bool running = false;


public:
	static Engine* getInstance();
	bool init();
	PrimitiveRenderer* getRenderer();
	void createWindow(int);
	int getWindowWidth();
	int getWindowHeight();
	void installKeyboard();
	void installMouse();
	void installFont();
	Point getMousePosition();
	ALLEGRO_EVENT getEvents();
	ALLEGRO_KEYBOARD_STATE getKeyboardState();
	void close();
	bool isRunning();
	~Engine();
};



