#include "Engine.h"
#include <sstream>

//FOR TESTING
GLfloat xRot = 0.0f;
GLfloat yRot = 0.0f;
GLfloat zRot = 0.0f;

sf::Vector3f obj1Pos;
GLuint playerTexture;
GLuint partTexture;

bool showHud = false;
///
sf::Font font;
Vec3 col (0,0,0);
Game::Game()
{
	light = TRUE;
	model = TRUE;
	polyMode = GL_FILL;

	sf::ContextSettings contextSettings;
	contextSettings.depthBits = SFML_DEPTH_BITS;
	contextSettings.antialiasingLevel = SFML_ANTI_ALIAS;

	window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), GAME_TITLE, DEFAULT_WINDOW_MODE, contextSettings);

	window.setVerticalSyncEnabled(SFML_VERTICAL_SYNC);
	window.setMouseCursorVisible(SFML_CURSOR_VISIBLE);

	cam = new Camera3D(&window);
	particleEngine = new ParticleEngine(cam);
	playerParticle = new ParticleEngine(cam);
}

void Game::run()
{
	init();

	while (window.isOpen())
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processEvents();
		update();
		draw();

		window.display();
	}
}

void Game::init()
{
	score = 0;
	deaths = 0;

	printf("Press 1 to show HUD");
	// Set color and depth clear value
	glClearDepth(1.f);
	glClearColor(CLEAR_COLOR);

	// Enable Z-buffer read and write (for hidden surface removal)
	glEnable(GL_DEPTH_TEST);

	// Setup a perspective projection
	glViewport(VIEW_BORDER_SIZE, VIEW_BORDER_SIZE, event.size.width - VIEW_BORDER_SIZE, event.size.height - VIEW_BORDER_SIZE);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.f, WINDOW_WIDTH/WINDOW_HEIGHT, 1.f, FOV_FAR);

	Model* obj1;

	floor = new Model("resources/plane.obj", MODEL_TYPE_VTN);
	floor->SetScale(Vec3(5000,1.f,5000));
	floor->SetPos(0, -10, 0);
	floor->loadTexture("resources/Backgroung1600x900.png");

	player = new Model("resources/monkey_textured_low.obj", MODEL_TYPE_VTN);
	player->SetScale(Vec3(200,100,200));
	player->loadTexture("resources/monkey.jpg");

	srand(time(NULL));
	for (int i = 0; i < 7; i++)
	{
		if (i ==3)i++;
		obj1 = new Model("resources/cube.obj", MODEL_TYPE_VTN);
		obj1->SetScale(Vec3(250,100,100));

		int randomTexture = rand() % 4;
		if (randomTexture == 0)
			obj1->loadTexture("resources/Blue Sports Car 100x150.png");
		else if (randomTexture == 1)
			obj1->loadTexture("resources/Red Sports Car 100x150.png");
		else if (randomTexture == 2)
			obj1->loadTexture("resources/Purple Truck 100 x 200.png");
		else
			obj1->loadTexture("resources/Red Truck 100 x 200.png");

		//int xDir = ( rand()%2 < 1 ) ? 1 : -1;
		int xDir = (i < 4)? 1 : -1;

		float x = xDir*2500.f;
		float y = 50.f;
		float z = -1650 + (i* 550);

		obj1->SetP0(x, y, z);
		obj1->SetP1(-x, y, z);
		obj1->SetPos(x, y, z);
		obj1->speed = xDir*Random(10,30);

		objects.push_back(obj1);
	}

	trigger.SetModelType(MODEL_TYPE_VTN);
	trigger.loadObj("resources/plane.obj");
	trigger.SetScale(Vec3(5000, 1, 1100));
	trigger.SetPos(0,0,-2500);


	//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	cam->pos = Vec3(0.f, 500.f, 2500.f);

	playerTexture = playerParticle->loadTexture("resources/Cat Protagonist 100 x 50.png");
	partTexture = particleEngine->loadTexture("resources/crash.jpg");
	
	//particleEngine->generateRandom(particleEngine->loadTexture("resources/part1.jpg"), 1000,1000,1000);
	//glDisable(GL_LIGHTING);

	GLfloat ambientColor[] = LIGHT_AMBIENT_COL;
	GLfloat diffuseColor[] = LIGHT_DEFUSE_COL;
	GLfloat specularColor[] = LIGHT_SPECULAR_COL;
	//GLfloat lightColor[] = {1.f, 1.f, 0.f, 1.f};
	GLfloat lightPos1[] = {0, 1.f, 1.f, 1};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos1);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientColor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularColor);

	glEnable(GL_LIGHT0);

	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	//FONT
	if (!font.loadFromFile("resources/font/DroidSerif.ttf"))
		printf("ERROR LOADING FONT");

}

void Game::processEvents()
{
	Vec3 oldPos = cam->pos;
	// Camera Movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		cam->pos +=cam->forward*PLAYER_SPEED;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		cam->pos -=cam->forward*PLAYER_SPEED;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		cam->pos -=cam->strafe*PLAYER_SPEED;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		cam->pos +=cam->strafe*PLAYER_SPEED;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		cam->pos +=cam->up*PLAYER_SPEED;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		cam->pos -=cam->up*PLAYER_SPEED;



	//LIGHT TESTING
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
	{
		static bool colFlag = false;

		static float col = 0;

		col += colFlag ? -0.01 : 0.01;
		
		if (col > 1)
			col = 0;

		GLfloat diffuseColor[] = {col,col,0,1};

		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseColor);
	}

	while (window.pollEvent(event))
	{
		// Close window : exit
		if (event.type == sf::Event::Closed)
			window.close();

		// Escape key : exit
		if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
			window.close();

		// Example05 - toggle wireframe mode ON/OFF
		if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::K)) {
			if (polyMode == GL_FILL)
				polyMode = GL_LINE; 
			else polyMode = GL_FILL;

			glPolygonMode( GL_FRONT_AND_BACK, polyMode );
		}

		// Resize event : adjust viewport
		if (event.type == sf::Event::Resized) {
			//glViewport(VIEW_BORDER_SIZE, VIEW_BORDER_SIZE, event.size.width - VIEW_BORDER_SIZE, event.size.height - VIEW_BORDER_SIZE);
			//glMatrixMode(GL_PROJECTION);
			//glLoadIdentity();
			//gluPerspective(90.f, event.size.width/event.size.height, 1.f, FOV_FAR);
			//glMatrixMode(GL_MODELVIEW);
		}

		//// L key: switch Light ON/OFF
		//if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::L)) 
		//	if (light) {
		//		glDisable(GL_LIGHTING);
		//		light = FALSE;
		//	}
		//	else {
		//		glEnable(GL_LIGHTING);
		//		light = TRUE;
		//	}

		// M key: switch shading model (smooth | flat)
		if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::M)) 
			if (model) {
				glShadeModel(GL_FLAT);
				model = 0;
			}
			else {
				glShadeModel(GL_SMOOTH);
				model = 1;
				}

		if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Num1)) 
			showHud = !showHud;
	}

	//Mouse control
	sf::Vector2i center;
	center = sf::Vector2i(window.getSize().x/2 +window.getPosition().x, 
		window.getSize().y/2+window.getPosition().y);

	sf::Vector2i mousePos;
	mousePos = sf::Vector2i(mouse.getPosition().x, 
		mouse.getPosition().y);

	sf::Vector2i diff = mousePos-center;

	mouse.setPosition(sf::Vector2i(window.getSize().x/2 +window.getPosition().x, 
		window.getSize().y/2+window.getPosition().y));

	cam->azemuth += float(diff.x)*0.1f;
	cam->elevation += float(diff.y)*0.1f;

	//lock camera rotation
	if (cam->azemuth > 45) cam->azemuth = 45;
	else if (cam->azemuth< -45) cam->azemuth = -45;

	if (cam->elevation < 40) cam->elevation = 40;
	else if (cam->elevation > 65) cam->elevation = 65;

	// restrict player movement to level
	if (cam->pos.x > 2500.f)
		cam->pos.x = 2500.f;
	else if (cam->pos.x < -2500.f)
		cam->pos.x = -2500.f;

	if (cam->pos.z > 2500.f)
		cam->pos.z = 2500.f;


	window.setActive();
}

void Game::update()
{
	static float t;
	static bool lerpForward = true;

	//clear smack particle
	float secondsPassed = clock.getElapsedTime().asSeconds();
	if (t > .5f) 
	{
		particleEngine->clear();
		t = 0;
	}

	t+=  secondsPassed;

	// move cars
	for (int i = 0; i < objects.size(); i++)
	{
		Vec3 newPos = objects[i]->pos + Vec3(objects[i]->speed, 0, 0);

		if (newPos.x > 2500.f && objects[i]->p1.x == -2500.f)
			newPos.x = -2500.f;
		else if (newPos.x < -2500.f && objects[i]->p1.x == 2500.f)
			newPos.x = 2500.f;

		objects[i]->SetPos(newPos.x, newPos.y, newPos.z);
	}


	player->SetPos(cam->pos.x,50.f,cam->pos.z-100.f);

	//player - car collision
	for (int i = 0; i < objects.size(); i ++)
	{
		if (detectColiding(*player, *objects[i]))
		{
			deaths++;

			cam->pos = Vec3(0.f, 500.f, 2500.f);
			cam->forward = Vec3(0,0,0);
			cam->strafe = Vec3(0,0,0);
			cam->up = Vec3(0,1,0);

			particleEngine->clear();
			particleEngine->addParticle(partTexture, cam->pos.x, 900.f, cam->pos.z-20.f);
			//particleEngine->generateRandom(partTexture, cam->pos.x, cam->pos.y, cam->pos.z );
			break;

		}
	}

	//end reached
	if (detectColiding(*player, trigger))
	{
		score++;
		cam->pos = Vec3(0.f, 500.f, 2500.f);
		cam->forward = Vec3(0,0,0);
		cam->strafe = Vec3(0,0,0);
		cam->up = Vec3(0,1,0);
		printf("%d\n", score);
	}

	playerParticle->clear();
	playerParticle->addParticle(playerTexture, cam->pos.x, 50, cam->pos.z-100);

	clock.restart();

	glEnable(GL_COLOR_MATERIAL);
}

void Game::draw()
{
	glColor4f(1.f, 1.f, 1.f, 1.f);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glColor3f(1.0f, 1.0f, 1.0f);

	cam->pos.y = 1000.f;
	cam->Activate();

	//////////////////////////////////////
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	//////////////////////////////////////
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->draw();
	}

	floor->draw();
	player->draw();
	//trigger.draw();

	glPopMatrix();

	//playerParticle->draw();
	particleEngine->draw();


///////HUD//////
	glDisable(GL_LIGHTING);
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);

	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	//glClear(GL_DEPTH_BUFFER_BIT);
	//glEnable (GL_BLEND);
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(10);
	glBegin(GL_LINE_LOOP);
	glVertex2d(0, 0);
	glVertex2d(WINDOW_WIDTH, 0);
	glVertex2d(WINDOW_WIDTH, WINDOW_HEIGHT);
	glVertex2d(0, WINDOW_HEIGHT);
	glEnd();

	glPopMatrix();
	glLineWidth(2);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	//text
	if (showHud)
	{
		window.pushGLStates();

		std::stringstream ss1 (std::stringstream::in | std::stringstream::out);
		ss1 << score;

		sf::Text txtScore;
		txtScore.setFont(font);
		txtScore.setCharacterSize(32);
		txtScore.setColor(sf::Color::White);
		txtScore.setPosition(20,0);
		txtScore.setString(ss1.str());

		window.draw(txtScore);

		std::stringstream ss2 (std::stringstream::in | std::stringstream::out);
		ss2 << deaths;

		sf::Text txtDeaths;
		txtDeaths.setFont(font);
		txtDeaths.setCharacterSize(32);
		txtDeaths.setColor(sf::Color::Red);
		txtDeaths.setPosition(WINDOW_WIDTH - 60,0);
		txtDeaths.setString(ss2.str());

		window.draw(txtDeaths);

		window.popGLStates();
	}
}

bool detectColiding(Model &obj1, Model &obj2)
{
	// check for distance between the two on X axis
	float distanceX = abs(obj1.pos.x - obj2.pos.x);

	if (distanceX > obj1.scale.x/2 + obj2.scale.x/2)
	{
		return false;
	}

	// check for distance between the two on X axis
	float distanceZ = abs(obj1.pos.z - obj2.pos.z);

	if (distanceZ > obj1.scale.z/2 + obj2.scale.z/2)
	{
		return false;
	}

	return true;
}
