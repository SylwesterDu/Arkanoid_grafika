#include "Engine.h"
#include "PrimitiveRenderer.h"

//! Metoda zwracaj¹ca zawsze t¹ sam¹ instancjê klasy Engine
Engine* Engine::getInstance() {
	static Engine* _engine;
	if (_engine == nullptr) {
		_engine = new Engine();
	}
	return _engine;
}


//! Metoda inicjuj¹ca silnik. Tworzy kolejkê zdarzeñ, timer oraz inicjuje rysowanie prymitywów
bool Engine::init() {
	al_init();
	queue = al_create_event_queue();
	timer = al_create_timer(1 / 60.0f);
	al_start_timer(timer);
	al_register_event_source(queue, al_get_timer_event_source(timer));
	return al_init_primitives_addon();
}

//! MEtoda zwracaj¹ca instancjê obietu klasy PrimitiveRenderer
PrimitiveRenderer* Engine::getRenderer() {
	return PrimitiveRenderer::getInstance();
}


//! Metoda tworz¹ca oraz wyœwietlaj¹ca okno
//! \param enum okreœlaj¹cy rodzaj okna: ALLEGRO_MAXIMIZED, ALLEGRO_FULLSCREEN, ALLEGRO_WINDOWED
void Engine::createWindow(int flags) {
	this->running = true;
	ALLEGRO_MONITOR_INFO monitor;
	al_get_monitor_info(0, &monitor);
	const int SCREEN_W = monitor.x2 - monitor.x1;
	const int SCREEN_H = monitor.y2 - monitor.y1;

	al_set_new_display_flags(flags);
	display = al_create_display(SCREEN_W, SCREEN_H);
	al_register_event_source(queue, al_get_display_event_source(display));
}

//! Metoda zwracaj¹ca szerokoœæ okna
int Engine::getWindowWidth() {
	return al_get_display_width(display);
}

//! Metoda zwracaj¹ca wysokoœæ okna
int Engine::getWindowHeight()
{
	return al_get_display_height(display);
}


//! W³¹cza obs³ugê klawiatury do programu
void Engine::installKeyboard() {
	al_install_keyboard();
	al_register_event_source(queue, al_get_keyboard_event_source());
}

//! W³¹cza obs³ugê myszy do programu
void Engine::installMouse() {
	al_install_mouse();
	al_register_event_source(queue, al_get_mouse_event_source());
}


//! Pozwala na u¿ycie czcionek 
void Engine::installFont() {
	al_init_font_addon();
	al_init_ttf_addon();
}

//! Zwraca Point okreœlaj¹cy obecne wspó³rzêdne myszy
Point Engine::getMousePosition() {
	ALLEGRO_MOUSE_STATE state;
	al_get_mouse_state(&state);
	return Point(state.x, state.y);
}

//! Zwraca kolejkê zdarzeñ, jednoczeœnie oczekuj¹c na timer
ALLEGRO_EVENT Engine::getEvents() {
	ALLEGRO_EVENT event;
	al_get_next_event(queue, &event);
	return event;
}

ALLEGRO_KEYBOARD_STATE Engine::getKeyboardState()
{
	ALLEGRO_KEYBOARD_STATE state;
	al_get_keyboard_state(&state);
	return state;
}

//! Zamyka okno i zwlania pamiêæ
void Engine::close() {
	delete PrimitiveRenderer::getInstance();
	this->running = false;
}

//! Zwraca true, jeœli program dzia³a
inline bool Engine::isRunning()
{
	return this->running;
}

//! Usuwa z pamiêci kolejkê zdarzeñ, ekran oraz timer
Engine::~Engine() {
	al_destroy_display(display);
	al_destroy_event_queue(queue);
	al_destroy_timer(timer);
}