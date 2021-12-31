#pragma once
#include <allegro5/color.h>

class Block {
public:
	Point p1;
	Point p2;
	ALLEGRO_COLOR color; //! kolor bloku
	bool exists; //! odpowiedzialne za zbicie bloku
};

class Shelf {
public:
	Point p1;
	Point p2;
	float movement_speed; //! prêdkoœæ poruszania siê
	ALLEGRO_COLOR color;
};

class Ball {
public:
	Point p;
	float r;
	float movement_speed_x;
	float movement_speed_y;
};

class Settings {
public:
	float ball_speed;
	int block_rows;
	int block_cols;
	float shelf_width;
	float shelf_speed;
	int points_per_block;
};
