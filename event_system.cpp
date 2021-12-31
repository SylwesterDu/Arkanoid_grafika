#pragma once

#include <allegro5/allegro.h>
#include "Engine.h"
#include "arkanoid_objects.h"
#include "event_system.h"

enum GAME_STATE {
	MENU,
	GAME,
	EXIT_APP
};

enum MENU_OPTION {
	DIFFICULTY_LEVEL,
	START_GAME,
	EXIT
};

enum DIFFICULTY {
	EASY,
	MEDIUM,
	HARD
};

enum HIT_FROM {
	LEFT,
	RIGHT,
	TOP,
	BOTTOM,
	NONE
};

inline void game_loop();
inline void menu_loop();
inline void main_loop();

static GAME_STATE current_game_state = MENU;
static MENU_OPTION menu_option = START_GAME;

static DIFFICULTY difficulty_level = MEDIUM;

static Settings settings;


inline void show_menu() {
	int screen_width = Engine::getInstance()->getWindowWidth();
	int screen_height = Engine::getInstance()->getWindowHeight();
	static int image_width = 770;
	PrimitiveRenderer::getInstance()->drawBitmap(
		Point(screen_width / 2 - image_width / 2, screen_height / 15),
		"logo"
	);

	std::string current_difficulty_level = "";
	switch (difficulty_level)
	{
		case EASY:
			current_difficulty_level = "   EASY  >";
			break;
		case MEDIUM:
			current_difficulty_level = "< MEDIUM >";
			break;
		case HARD:
			current_difficulty_level = "<  HARD   ";
			break;
	default:
		break;
	}
	int width = Engine::getInstance()->getWindowWidth();
	int height = Engine::getInstance()->getWindowHeight();

	PrimitiveRenderer::getInstance()->setColor(100, 100, 100);
	if (menu_option == DIFFICULTY_LEVEL) {
		PrimitiveRenderer::getInstance()->setColor(240, 240, 240);
	}
	PrimitiveRenderer::getInstance()->drawText(Point(width / 2, height * 0.5), "DIFFICULTY_LEVEL: " + current_difficulty_level);
	PrimitiveRenderer::getInstance()->setColor(100, 100, 100);

	if (menu_option == START_GAME) {
		PrimitiveRenderer::getInstance()->setColor(240, 240, 240);
	}
	PrimitiveRenderer::getInstance()->drawText(Point(width / 2, height * 0.65), "START_GAME");
	PrimitiveRenderer::getInstance()->setColor(100, 100, 100);

	if (menu_option == EXIT) {
		PrimitiveRenderer::getInstance()->setColor(240, 240, 240);
	}
	PrimitiveRenderer::getInstance()->drawText(Point(width / 2, height * 0.8), "EXIT");

}

inline void show_blocks(Block** blocks, int x, int y) {
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if (!blocks[i][j].exists) continue;
			PrimitiveRenderer::getInstance()->setColor(blocks[i][j].color.r * 255, blocks[i][j].color.g * 255, blocks[i][j].color.b * 255);
			PrimitiveRenderer::getInstance()->drawFilled(
				Rectangle(blocks[i][j].p1, blocks[i][j].p2)
			);
		}
	}
}

inline void show_shelf(Shelf shelf) {
	PrimitiveRenderer::getInstance()->setColor(shelf.color.r * 255, shelf.color.g * 255, shelf.color.b * 255);
	PrimitiveRenderer::getInstance()->drawFilled(Rectangle(shelf.p1, shelf.p2));
}

inline void show_ball(Ball ball) {
	PrimitiveRenderer::getInstance()->setColor(200, 200, 200);
	PrimitiveRenderer::getInstance()->drawFilled(Circle(ball.p, ball.r));
}


inline void set_difficulty_level_settings() {
	switch (difficulty_level) {
	case EASY:
		settings.ball_speed = 2;
		settings.shelf_width = Engine::getInstance()->getWindowWidth() / 6;
		settings.block_cols = 6;
		settings.block_rows = 6;
		settings.shelf_speed = 2;
		settings.points_per_block = 10;
		break;
	case MEDIUM:
		settings.ball_speed = 4;
		settings.shelf_width = Engine::getInstance()->getWindowWidth() / 8;
		settings.block_cols = 7;
		settings.block_rows = 8;
		settings.shelf_speed = 4;
		settings.points_per_block = 20;
		break;
	case HARD:
		settings.ball_speed = 5;
		settings.shelf_width = Engine::getInstance()->getWindowWidth() / 12;
		settings.block_cols = 10;
		settings.block_rows = 9;
		settings.shelf_speed = 6;
		settings.points_per_block = 30;
	}
}

inline Block** create_blocks(int x, int y) {
	Engine* engine = Engine::getInstance();
	Block** blocks = new Block*[x];
	for (int i = 0; i < x; i++) {
		blocks[i] = new Block[y];

		for (int j = 0; j < y; j++) {
			blocks[i][j] = Block();
		}
	}

	float distanceX = (float) engine->getWindowWidth() / x;
	float distanceY = (float) engine->getWindowHeight() / (2 * y);


	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			blocks[i][j].p1.x = distanceX * i + distanceX * 0.01;
			blocks[i][j].p1.y = distanceY * j + distanceY * 0.01;
			blocks[i][j].p2.x = blocks[i][j].p1.x + distanceX * 0.98;
			blocks[i][j].p2.y = blocks[i][j].p1.y + distanceY * 0.96;
			blocks[i][j].color = al_map_rgb(255 - i * (250 / x), 0 + i * (250 / x), 255 - j * (250 / y));
			blocks[i][j].exists = true;
		}
	}

	return blocks;
}

inline Ball create_ball(float speed) {
	Engine* engine = Engine::getInstance();
	Ball ball;

	ball.p = Point(engine->getWindowWidth() / 2, engine->getWindowHeight() * 0.7);
	ball.r = 15;
	ball.movement_speed_x = sqrt(0.2) * speed;
	ball.movement_speed_y = -sqrt(0.8) * speed;
	return ball;
}

inline Shelf create_shelf(int width, float speed) {
	Shelf shelf;
	Engine* engine = Engine::getInstance();
	shelf.p1.x = engine->getWindowWidth() / 2 - width / 2;
	shelf.p1.y = engine->getWindowHeight() * 0.95;
	shelf.p2.x = engine->getWindowWidth() / 2 + width / 2;
	shelf.p2.y = shelf.p1.y + engine->getWindowHeight() / 30;
	shelf.movement_speed = speed;
	shelf.color = al_map_rgb(230, 230, 230);
	return shelf;
}

inline void change_ball_direction_if_needed(Ball* ball) {
	int display_width = Engine::getInstance()->getWindowWidth();
	if (ball->p.x + ball->r >= display_width) { //! Odbicie od prawej krawêdzi
		ball->movement_speed_x = -ball->movement_speed_x;
	}
	else if (ball->p.x - ball->r <= 0) { //! Odbicie od lewej krawêdzi
		ball->movement_speed_x = -ball->movement_speed_x;
	}
	else if (ball->p.y - ball->r <= 0) { //! Odbicie od góry
		ball->movement_speed_y = -ball->movement_speed_y;
	}
}

inline bool check_if_hit_on_shelf(Ball ball, Shelf shelf) {
	if (
		ball.p.y + ball.r / 2 >= shelf.p1.y
		&& ball.p.x + ball.r / 2 > shelf.p1.x
		&& ball.p.x - ball.r / 2 < shelf.p2.x
		) {
		return true;
	}
	return false;
}

inline void get_new_movement(Ball* ball, Shelf shelf) {
	int shelf_center = (shelf.p1.x + shelf.p2.x) / 2;
	int shelf_width = shelf.p2.x - shelf.p1.x;
	int ball_distance_from_center = ball->p.x - shelf_center;
	//! Oddalenie od srodka zmieniamy na liczbê z zakresu <0;1> licz¹c u³amek
	float distance = (float)ball_distance_from_center / shelf_width;
	ball->movement_speed_x = atan(distance) * settings.ball_speed;
	//! Z twierdzenia Pitagorasa liczymy szybkoœæ poruszania siê po y, aby szybkoœæ pi³ki by³a zawsze taka sama.
	ball->movement_speed_y = -sqrt(pow(settings.ball_speed, 2) - pow(ball->movement_speed_x, 2));
}

inline HIT_FROM check_if_hit_on_block(Ball ball, Block** blocks, int x, int y) {

	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if (!blocks[i][j].exists) continue;
			if (
				ball.p.x + ball.r >= blocks[i][j].p1.x
				&& ball.p.x - ball.r <= blocks[i][j].p2.x
				&& ball.p.y + ball.r >= blocks[i][j].p1.y
				&& ball.p.y - ball.r <= blocks[i][j].p2.y
				) { //! Sprawdzenie, czy dosz³o do uderzenia.

				blocks[i][j].exists = false; //! Usuniêcie bloku

				if (
					ball.p.x + ball.r + 4 >= blocks[i][j].p1.x
					&& ball.p.x + ball.r - 4 <= blocks[i][j].p1.x
					) { //! Uderzenie z lewej
					return LEFT;
				}
				if (
					ball.p.x - ball.r - 4 <= blocks[i][j].p2.x
					&& ball.p.x - ball.r + 4 >= blocks[i][j].p2.x
					) { //! Uderzenie z prawej
					return RIGHT;
				}
				if (
					ball.p.y + ball.r + 4 >= blocks[i][j].p1.y
					&& ball.p.y + ball.r - 4 <= blocks[i][j].p1.y
					) { //! Uderzenie z góry
					return TOP;
				}
				else { //! Uderzenie z do³u. Tutaj jest pewnoœæ, ¿e do uderzenia dosz³o. Pozosta³ tylko jeden kierunek.
					return BOTTOM;
				}
			}
		}
	}
	return NONE; //! Zwracane jest 0, kiedy nie dosz³o do uderzenia.
}

inline void main_loop() {
	set_difficulty_level_settings();
	PrimitiveRenderer::getInstance()->loadBitmap("Arkanoid_Logo.png", "logo");

	while (true) {
		switch (current_game_state)
		{
		case MENU:
			menu_loop();
			break;
		case GAME:
			game_loop();
			break;
		case EXIT:
			return;
		default:
			break;
		}
	}
}

inline void menu_loop() {
	Engine* engine = Engine::getInstance();
	PrimitiveRenderer* renderer = PrimitiveRenderer::getInstance();
	renderer->setBackgroundColor(0, 0, 64);

	ALLEGRO_EVENT event;
	while (true) {
		event = engine->getEvents();
		if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
			if (key == ALLEGRO_KEY_DOWN) {
				switch (menu_option)
				{
				case DIFFICULTY_LEVEL:
					menu_option = START_GAME;
					break;
				case START_GAME:
					menu_option = EXIT;
					break;
				}
			}	
			else if (key == ALLEGRO_KEY_UP) {
				switch (menu_option)
				{
				case EXIT:
					menu_option = START_GAME;
					break;
				case DIFFICULTY_LEVEL:
					break;
				case START_GAME:
					menu_option = DIFFICULTY_LEVEL;
					break;
				}
			}
			else if (key == ALLEGRO_KEY_LEFT) {
				switch (menu_option)
				{
				case DIFFICULTY_LEVEL:
					if (difficulty_level == MEDIUM) difficulty_level = EASY;
					if (difficulty_level == HARD) difficulty_level = MEDIUM;
					set_difficulty_level_settings();
					break;
				}
			}
			else if (key == ALLEGRO_KEY_RIGHT) {
				switch (menu_option)
				{
				case DIFFICULTY_LEVEL:
					if (difficulty_level == EASY) {
						difficulty_level = MEDIUM;
						break;
					}
					if (difficulty_level == MEDIUM) difficulty_level = HARD;
					set_difficulty_level_settings();
					break;
				}
			}
			else if (key == ALLEGRO_KEY_ENTER) {
				switch (menu_option)
				{
				case START_GAME:
					current_game_state = GAME;
					return;
				case EXIT:
					current_game_state = EXIT_APP;
					engine->close();
					return;
				}
			}
	
		}

		if (event.type == ALLEGRO_EVENT_TIMER) {
			renderer->clear();
			show_menu();
			renderer->update();
		}

		if (event.type == CLOSE_WINDOW) {
			engine->close();
			current_game_state = EXIT_APP;
			return;
		}
	}
}

inline void show_score(bool win, int score, int destroyed_blocks) {
	PrimitiveRenderer* renderer = PrimitiveRenderer::getInstance();
	int height = Engine::getInstance()->getWindowHeight();
	int width = Engine::getInstance()->getWindowWidth();

	renderer->setColor(50, 50, 50);
	renderer->drawFilled(
		Rectangle(
			Point(width / 3, height * 0.25),
			Point(width / 3 * 2, height * 0.6)
		)
	);
	if (win) {
		renderer->setColor(100, 200, 100);
	}
	else {
		renderer->setColor(200, 100, 100);
	}
	renderer->drawText(
		Point(width / 2, height * 0.25 + height / 30),
		win ? " You won!" : "You lost!"
	);

	renderer->setColor(230, 230, 230);
	renderer->drawText(
		Point(width / 2, height * 0.25 + height / 10),
		"Your score is: "
	);

	renderer->setColor(230, 230, 160);
	renderer->drawText(
		Point(width / 2, height * 0.25 + height / 6.5),
		std::to_string(score)
	);

	renderer->setColor(230, 230, 230);
	renderer->drawText(
		Point(width / 2, height * 0.37 + height / 10),
		"destroyed blocks: "
	);

	renderer->setColor(230, 230, 160);
	renderer->drawText(
		Point(width / 2, height * 0.37 + height / 6.5),
		std::to_string(destroyed_blocks)
	);

}

inline void game_loop() {
	Engine* engine = Engine::getInstance();
	PrimitiveRenderer* renderer = PrimitiveRenderer::getInstance();
	renderer->setBackgroundColor(0, 0, 0);

	Block** blocks = create_blocks(settings.block_cols, settings.block_rows);
	Shelf shelf = create_shelf(settings.shelf_width, settings.shelf_speed);
	Ball ball = create_ball(settings.ball_speed);



	ALLEGRO_KEYBOARD_STATE keyboard_state;
	ALLEGRO_EVENT event;

	bool started = false;
	bool win = false;
	int destroyed_blocks = 0;
	int score = 0;
	//! pêtla gry
	while (true) {
		if (!started) {
			event = engine->getEvents();
			if (event.type == ALLEGRO_EVENT_TIMER) {
				keyboard_state = engine->getKeyboardState();
				if (al_key_down(&keyboard_state, ALLEGRO_KEY_SPACE)) {
					started = true;
				}
				else if (al_key_down(&keyboard_state, ALLEGRO_KEY_ESCAPE)) {
					delete blocks;
					current_game_state = MENU;
					return;
				}
			}
			renderer->clear();
			show_blocks(blocks, settings.block_cols, settings.block_rows);
			show_shelf(shelf);
			show_ball(ball);
			renderer->update();
			continue;
		}

		event = engine->getEvents();

		if (event.type == CLOSE_WINDOW) {
			engine->close();
			current_game_state = EXIT_APP;
			return;
		}

		if (event.type != ALLEGRO_EVENT_TIMER) continue;

		keyboard_state = engine->getKeyboardState();

		if (al_key_down(&keyboard_state, ALLEGRO_KEY_LEFT)) {
			if (shelf.p1.x > 0) {
				shelf.p1.x -= shelf.movement_speed;
				shelf.p2.x -= shelf.movement_speed;
			}
		}
		else if (al_key_down(&keyboard_state, ALLEGRO_KEY_RIGHT)) {
			if (shelf.p2.x < engine->getWindowWidth()) {
				shelf.p1.x += shelf.movement_speed;
				shelf.p2.x += shelf.movement_speed;
			}
		}

		else if (al_key_down(&keyboard_state, ALLEGRO_KEY_ESCAPE)) {
			current_game_state = MENU;
			return;
		}
		

		if (check_if_hit_on_shelf(ball, shelf)) { //! Dosz³o do uderzenia
			get_new_movement(&ball, shelf);
		}

		if (ball.p.y + ball.r - 20 > shelf.p1.y) { //! Odbicie od do³u, przegrana
			win = false;
			break;
		}

		HIT_FROM hit = check_if_hit_on_block(ball, blocks, settings.block_cols, settings.block_rows);
		if (hit != NONE) {
			destroyed_blocks++;
			score += settings.points_per_block;
			if (destroyed_blocks == settings.block_cols * settings.block_rows) {
				win = true;
				break;
			}
			if (hit == BOTTOM) {
				ball.movement_speed_y = -ball.movement_speed_y;
			}
			else if (hit == TOP) {
				ball.movement_speed_y = -ball.movement_speed_y;
			}
			else if (hit == LEFT) {
				ball.movement_speed_x = -ball.movement_speed_x;
			}
			else if (hit == RIGHT) {
				ball.movement_speed_x = -ball.movement_speed_x;
			}
		}


		//! przesuniêcie pi³ki
		ball.p.x += ball.movement_speed_x;
		ball.p.y += ball.movement_speed_y;

		change_ball_direction_if_needed(&ball);

		renderer->clear();
		show_blocks(blocks, settings.block_cols, settings.block_rows);
		show_shelf(shelf);
		show_ball(ball);
		renderer->update();
	}

	while (true) {
		event = engine->getEvents();
		if (event.type == CLOSE_WINDOW) {
			engine->close();
			current_game_state = EXIT_APP;
			return;
		}
		if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
			delete blocks;
			current_game_state = MENU;
			return;
		}


		renderer->clear();
		show_blocks(blocks, settings.block_cols, settings.block_rows);
		show_shelf(shelf);
		show_score(win, score, destroyed_blocks);
		al_flip_display();
	}
}