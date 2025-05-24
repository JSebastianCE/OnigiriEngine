#pragma once
#include "Prerequisites.h"
#include <Window.h>

class 
BaseApp{
public:
	BaseApp() = default;
	~BaseApp();

	//Funcion encargada de ejecutar la aplicaci[on en main
	int
	run();


	//Funcion de inicializacion
	bool
	init();

	//Funcion que actualiza por Frame
	void
	update();

	//Funcion renderizado
	void
	render();

	void
	destroy();

private:
	Window* m_window;
	//sf::RenderWindow* m_window;
	sf::CircleShape* m_circle;
};
