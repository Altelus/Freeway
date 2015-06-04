#ifndef _GAME_H
#define _GAME_H

#include <vector>
#include "Model.h"
#include "Camera3D.h"
#include "ParticleEngine.h"

class Game
{
public:
	Game();
	void init();
	void run();
	void processEvents();
	void update();
	void draw();

private:
	GLboolean light;
	GLboolean model;
	GLint polyMode;

	sf::Clock clock;

	sf::RenderWindow window;
	sf::Event event;

	Camera3D* cam;
	sf::Mouse mouse;
	ParticleEngine* particleEngine;
	ParticleEngine* playerParticle;

	Model* player;
	Model* floor;
	Model trigger;
	std::vector<Model*> objects;

	int score;
	int deaths;
};

bool detectColiding(Model &obj1, Model &obj2);

#endif