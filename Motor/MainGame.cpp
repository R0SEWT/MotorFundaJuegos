#include "MainGame.h"
#include "Sprite.h"

MainGame::MainGame()
{
	window = NULL;
	width = 800;
	height = 600;
	gameState = GameState::PLAY;

}

void MainGame::init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	window = new Window();
	window->create("Motor WD2M", width, height, 0);
	initShaders();

}



MainGame::~MainGame()
{


}

void MainGame::run()
{
	init();

	sprite.init(0, 0, 1, 1, "images/si.png");
	//sprite1.init(-1, 0, 1, 1);
	//sprite2.init(0, -1, 1, 1);
	//sprite3.init(-1, -1, 1, 1);

	update();
}

void MainGame::draw()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	program.use();
	GLuint timeLocation = program.getUniformLocation("time");  // que es una variable uniforme
	glUniform1f(timeLocation, time); // da error no usar la variable uniform
	time += 0.312;
	
	cont = 0;

	sprite.draw();

	if (time > ++cont) {
		//sprite1.draw();
	}
	if (time > ++cont) {
		//sprite2.draw();
	}

	if (time > ++cont) {
		//sprite3.draw();
	}

	program.unuse();
	//sprites.at(0).draw();
	window->swapWindow();
	
}

void MainGame::update()
{
	while (gameState != GameState::EXIT) {
		draw();
		processInput();
	}
}

void MainGame::processInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)){
		switch (event.type) {
		case SDL_QUIT:
			gameState = GameState::EXIT;
			break;
		/*case SDL_MOUSEMOTION:
			cout << " Posicion en X " << event.motion.x 
				<< " Posicion en Y " << event.motion.y << endl;
			break;*/
		default:
			break;
		}
	}
}

void MainGame::initShaders()
{
	program.compileShaders("Shaders/colorShaderVert.txt", "Shaders/colorShaderFrag.txt");
	program.addAtribute("vertexPosition");
	program.addAtribute("vertexColor");
	program.addAtribute("vertexUV");
	program.linkShader();
}
