#include "ofApp.h"

void ofApp::setup() {
	img1.load("grass.jpg");
	img2.load("cobra3.jpg");
	img3.load("apple.png");
	img4.load("cobra4.jpg");
	img5.load("tree.png");
	img6.load("light.png");
	img7.load("wall.png");
	ofSetFrameRate(60);
	glEnable(GL_DEPTH_TEST);
	glPointSize(200);
	glLineWidth(3);
	resX = 25;
	resY = 25;
	lensAngle = 60;
	alpha = 10;
	beta = 1000;
	perfectDist = gh()*0.55/tan(lensAngle*0.5*PI/180.);
	elapsedTime = 0;
	snakeX = floor(resX / 2);
	snakeY = floor(resY / 2);
	snakePos.z = floorHeightPos + snakeDepth * 0.5;
	snakePositions.push_back(ofVec3f(snakeX, snakeY, snakePos.z));
	currentDirection = NONE;
	rotation = 0;
	cameraView = 1;
	destruction = false;
	destructionDuration = 1.0;
	destructionTimer = 0.0;
	destructionIntensity = 0.1;
	colorChange = false;
	colorChangeTimer = 0.0;
	colorChangeDuration = 0.3;
	nitro = false;
	nitroSpeed = 0.05;
	speed = 0.15;
	savespeed = speed;
	score = 0;
	permission = true;
	mode = false;
	level = 0;
	light = false;
	left_lamp = false;
	right_lamp = false;
	follow = false;
	gameOver = false;
	binds = false;
	amb = true;
	dif = true;
	spec = true;
	drawRandomFruit();
	setupSnake();
}

void ofApp::update() {
	rotation -= 3;
	GLfloat time = ofGetLastFrameTime();
	elapsedTime += time;
	// Verificaao para animacao de destruicao quando se perde o jogo
	if (destruction) {
		destructionTimer -= time;
		if (destructionTimer <= 0.0) {
			destruction = false;
			gameOver = true;
			resetGame(); // Reseta o jogo
		}
	}
	// Verificacao para animacao de mudanca de cor quando uma fruta é comida
	if (colorChange) {
		colorChangeTimer -= time;
		if (colorChangeTimer <= 0.0) {
			colorChange = false;
		}
	}
	// Verificacao para garantir que a cobra so anda nos quadrados
	if (elapsedTime >= speed) {
		switch (currentDirection) {
		case LEFT:
			snakeX -= 1;
			break;
		case RIGHT:
			snakeX += 1;
			break;
		case UP:
			snakeY -= 1;
			break;
		case DOWN:
			snakeY += 1;
			break;
		default:
			break;
		}
		// Atualizacao da posicao
		snakePos.x = -floorWidth * 0.5 + (snakeX * floorWidth / GLfloat(resX)) + snakeWidth * 0.5;
		snakePos.y = floorHeight * 0.5 - (snakeY * floorHeight / GLfloat(resY)) - snakeHeight * 0.5;
		snakePos.z = floorHeightPos + snakeDepth * 0.5;
		// Verificacao para colisao com a parede e com a propria cobra
		if (checkCollisionWall(currentDirection) || checkSelfCollision()) {
			destruction = true;
			destructionTimer = destructionDuration;
			currentDirection = NONE;
		}
		// Vericacao para a colisao com uma fruta
		if (checkCollisionFruit()) {
			colorChange = true;
			colorChangeTimer = colorChangeDuration;
			drawRandomFruit();
			snakePositions.push_back(snakePositions.back());
			score += 100; // Incrementa a pontuação
			if (mode) {
				if (score > 0 && score % 200 == 0) {
					speed *= 0.85; // Aumenta a velocidade
				}
			}
			else {
				if (score > 0 && score % 500 == 0) {
					speed *= 0.9; // Aumenta a velocidade
				}
			}
			if (mode) { // Modo Levels
				if (score > 0 && score % 1000 == 0) { // De 1000 em 1000
					if (level < 7) {
						resX -= 2; // Diminui o tamanho do campo
						resY -= 2; // Diminui o tamanho do campo
						// Reseta o jogo
						setupSnake();
						snakeX = floor(resX / 2);
						snakeY = floor(resY / 2);
						snakePositions.clear();				
						snakePositions.push_back(ofVec3f(snakeX, snakeY, snakePos.z));
						drawRandomFruit();
						currentDirection = NONE;
						speed = 0.15;
						level++; // Incrementa o level
					}
				}
			}
		}
		elapsedTime = 0;
		// Atualizacao das posicoes consoante a posicao imediatamente a seguir
		for (int i = snakePositions.size() - 1; i > 0; i--) {
			snakePositions[i] = snakePositions[i - 1];
		}
		// Atualizacao da cabeca da cobra
		snakePositions[0].x = snakeX;
		snakePositions[0].y = snakeY;
		snakePositions[0].z = snakePos.z;
	}
}

void ofApp::draw() {
	// Tela do game over
	if (gameOver) {
		ofBackground(135, 206, 250);
		std::string gameOverText = "** GAME OVER **";
		std::string scoreText = "SCORE: " + std::to_string(score);
		float textX = gw() / 2 - 100;
		float textY = gh() / 2 - 20;
		ofDrawBitmapString(gameOverText, textX, textY);
		ofDrawBitmapString(scoreText, textX + 20, textY + 20);
		std::string restartText = "PRESS '0' TO RESTART";
		ofDrawBitmapString(restartText, textX - 20, textY + 500);
		return;
	}
	ofBackground(135, 206, 250); // Cor do background
	// Textos do score, mode e level
	glPushMatrix();
	ofSetColor(255, 255, 255);
	std::string showText = "PRESS 'b' TO SHOW THE BINDS";
	float showX = gw() / 2 - 100;
	float showY = gh() / 50;
	ofDrawBitmapString(showText, showX, showY);
	ofSetColor(255, 255, 255);
	std::string scoreText = "SCORE: " + std::to_string(score);
	float scoreX = gw() / 2 - 30;
	float scoreY = gh() / 25;
	ofDrawBitmapString(scoreText, scoreX, scoreY);
	ofSetColor(255, 255, 255); // Branco
	std::string modeText = "MODE: " + std::string(mode ? "LEVELS" : "NORMAL");
	float textX = gw() / 2 - 30;
	float textY = gh() / 16;
	ofDrawBitmapString(modeText, textX, textY);
	if (mode) {
		ofSetColor(255, 255, 255); // Branco
		std::string levelText = "LEVEL: " + std::to_string(level);
		float textX = gw() / 2 - 30;
		float textY = gh() / 12;
		ofDrawBitmapString(levelText, textX, textY);
	}
	// Texto das binds
	if (binds) {
		ofSetColor(255, 255, 255); // Branco
		std::string bindsText = "BINDS:\n1 - First Camera\n2 - Second Camera\n3 - Third Camera\n'7'|'8'|'9' - Change Lights Components\n'm' - Select Mode\n'l' - Turn On The Fruit Light\n'o' - Turn On The Lamp On The Left\n'p' - Turn On The Lamp On The Right\n'k' - The Lamps Follow The Snake";
		float bindsX = gw() / 2 - 950;
		float bindsY = gh() / 1.15;
		ofDrawBitmapString(bindsText, bindsX, bindsY);
	}
	glPopMatrix();
	ofVec3f headPos = snakePositions[0];
	GLfloat camX, camY, camZ;
	switch (cameraView) {
	// Vista ortografica
	case 1:
		glViewport(0, 0, gw(), gh());
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-gw() * 0.5, gw() * 0.5, -gh() * 0.5, gh() * 0.5, -10000, 10000);
		lookat(0, 0, gw()/20, 0, 0, 0, 0, 1, 0);
		glPushMatrix();
		poste_luz(-1,-1);
		lampLight(GL_LIGHT2, 0, -1, -1, 25, 25);
		poste_luz(resX, -1);
		lampLight(GL_LIGHT3, 1, resX, -1, 0, 25);
		glPopMatrix();
		wall();
		drawSnake();
		break;
	// Vista perspetiva principal 
	case 2:
		glViewport(0, 0, gw(), gh());
		perspective(lensAngle, alpha, beta);
		lookat(0, -gh() * 0.65, 0.5 * perfectDist * 1.7, 0, 0, 0, 0, 1, 0);
		drawSnake();
		poste_luz(-1, -1);
		glPushMatrix();
		poste_luz(-1, -1);
		lampLight(GL_LIGHT2, 0, -1, -1, 25, 25);
		poste_luz(resX, -1);
		lampLight(GL_LIGHT3, 1, resX, -1, 0, 25);
		glPopMatrix();
		wall();
		break;
	// Vista terceira pessoa + "minimap"
	case 3:
		glPushMatrix();
		glViewport(0, gh() * 0.75, gw() * 0.25, gh() * 0.25);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-gw() * 0.5, gw() * 0.5, -gh() * 0.5, gh() * 0.5, -10000, 10000);
		lookat(0, 0, 1, 0, 0, 0, 0, 1, 0);
		drawSnake();
		glPopMatrix();
		glPushMatrix();
		glViewport(0, 0, gw(), gh());
		camX  = -floorWidth * 0.5 + (headPos.x * snakeWidth) + snakeWidth * 0.5;
		camY  = floorHeight * 0.5 - (headPos.y * snakeHeight) - snakeHeight * 0.5;
		camZ  = snakePos.z;
		perspective(lensAngle, alpha, beta);
		lookat(camX, camY - snakeHeight * 10.0, camZ + snakeDepth * 10.0, camX, camY, camZ, 0, 0, 1);
		drawSnake();
		poste_luz(-1, -1);
		glPushMatrix();
		poste_luz(-1, -1);
		lampLight(GL_LIGHT2, 0, -1, -1, 25, 25);
		poste_luz(resX, -1);
		lampLight(GL_LIGHT3, 1, resX, -1, 0, 25);
		glPopMatrix();
		wall();
		glPopMatrix();
		break;
	}
	glViewport(0, 0, gw(), gh());
}

void ofApp::setupSnake() {
	floorWidth = gw() * 0.5;
	floorHeight = gw() * 0.5;
	floorHeightPos = 0.0;
	snakeWidth = floorWidth / GLfloat(resX);
	snakeHeight = floorHeight / GLfloat(resY);
	snakeDepth = snakeWidth;
	snakeX = floor(resX / 2);
	snakeY = floor(resY / 2);
	snakePos.z = floorHeightPos + snakeDepth * 0.5;
}

void ofApp::drawSnake() {
	glPushMatrix();
	glScalef(floorWidth, floorHeight, 1.0);
	img1.bind();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	malha_unit(resX, resY, 512);
	img1.unbind();
	glPopMatrix();
	// Percorrer as posicoes dos cubos da cobra
	for (ofVec3f& pos : snakePositions) {
		glPushMatrix();
		// Verificacao se ocorreu a animacao de destruicao
		if (destruction) {
			// Randoms para as posicoes apos destruicao com valores entre -0.1 e 0.1
			GLfloat destructionX = ofRandom(-destructionIntensity, destructionIntensity);
			GLfloat destructionY = ofRandom(-destructionIntensity, destructionIntensity);
			GLfloat destructionZ = ofRandom(-destructionIntensity, destructionIntensity);
			pos += ofVec3f(destructionX, destructionY, destructionZ);
		}
		// Translacao da cobra
		glTranslatef(-floorWidth * 0.5 + (pos.x * floorWidth / GLfloat(resX)) + snakeWidth * 0.5, floorHeight * 0.5 - (pos.y * floorHeight / GLfloat(resY)) - snakeHeight * 0.5, pos.z);
		// Scale "default" da cobra
		glScalef(snakeWidth, snakeHeight, snakeDepth);
		img4.bind();
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Verifica se uma fruta foi comida
		if (colorChange) {
			glPushMatrix();
			glScalef(1.0, 1.0, 1.0);
			cube_unit(100);
			glPopMatrix();
		}
		img4.unbind();
		img2.bind();
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		cube_unit(100);
		// Verica se o nitro esta ativo
		if (nitro) {
			glPushMatrix();
			// Scale que torna a cobra maior
			glScalef(1.2, 1.2, 1.2);
			cube_unit(100);
			glPopMatrix();
		}
		img2.unbind();
		glPopMatrix();
	}
	glPushMatrix();
	// Translacao da fruta
	glTranslatef(-floorWidth * 0.5 + (fruitX * floorWidth / GLfloat(resX)) + snakeWidth * 0.5, floorHeight * 0.5 - (fruitY * floorHeight / GLfloat(resY)) - snakeHeight * 0.5, floorHeightPos + snakeDepth * 0.5);
	// Rotacao da fruta
	glRotatef(rotation, 0, 0, 1);
	// Scale "default" da fruta
	glScalef(snakeWidth * 0.8, snakeHeight * 0.8, snakeDepth);
	img3.bind();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	cube_unit(208);
	img3.unbind();
	glPopMatrix();
	glPushMatrix();
	fruitLight();
	glPopMatrix();
}

void ofApp::keyPressed(int key) {
	// "Binds"
	switch (key) {
	case '0':
		if (gameOver) {
			gameOver = false;
			resetGame();
		}
		break;
	case '1':
		cameraView = 1;
		break;
	case '2':
		cameraView = 2;
		break;
	case '3':
		cameraView = 3;
		break;
	case '7':
		amb = !amb;
		break;
	case '8':
		dif = !dif;
		break;
	case '9':
		spec = !spec;
		break;
	case 'g':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'f':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case ' ':
		if (!nitro) {
			savespeed = speed;
		}
		nitro = true;
		speed = nitroSpeed;
		break;
	case 'm':
		if (permission) {
			mode = !mode;
		}
		break;
	case 'l':
		light = !light;
		break;
	case 'o':
		left_lamp = !left_lamp;
		break;
	case 'p':
		right_lamp = !right_lamp;
		break;
	case 'k':
		follow = !follow;
		break;
	case 'b':
		binds = !binds;
		break;
	}
	if (destruction) {
		return;
	}
	// Atualizacao das posicoes consoante as setas premidas
	if (snakePositions.size() > 1) {
		switch (key) {
		case OF_KEY_LEFT:
			if (currentDirection != RIGHT) {
				currentDirection = LEFT;
			}
			permission = false;
			break;
		case OF_KEY_RIGHT:
			if (currentDirection != LEFT) {
				currentDirection = RIGHT;
			}
			permission = false;
			break;
		case OF_KEY_UP:
			if (currentDirection != DOWN) {
				currentDirection = UP;
			}
			permission = false;
			break;
		case OF_KEY_DOWN:
			if (currentDirection != UP) {
				currentDirection = DOWN;
			}
			permission = false;
			break;
		}
	}
	else {
		switch (key) {
		case OF_KEY_LEFT:
			currentDirection = LEFT;
			permission = false;
			break;
		case OF_KEY_RIGHT:
			currentDirection = RIGHT;
			permission = false;
			break;
		case OF_KEY_UP:
			currentDirection = UP;
			permission = false;
			break;
		case OF_KEY_DOWN:
			currentDirection = DOWN;
			permission = false;
			break;
		}
	}
}

void ofApp::drawRandomFruit() {
	bool positionOccupied;
	// Encontra uma posicao para a fruta nao ocupada pelas posicoes da cobra
	do {
		positionOccupied = false;
		fruitX = rand() % resX;
		fruitY = rand() % resY;
		for (ofVec3f& pos : snakePositions) {
			if (fruitX == pos.x &&  fruitY == pos.y) {
				positionOccupied = true;
				break;
			}
		}
	} while (positionOccupied);
}

// Funcao para verificar colisao com a fruta
bool ofApp::checkCollisionFruit() {
	return (snakeX == fruitX && snakeY == fruitY);
}

// Funcao para verificar colisao com a propria cobra
bool ofApp::checkSelfCollision() {
	ofVec3f headPos = snakePositions[0];
	for (int i = 1; i < snakePositions.size(); i++) {
		if (headPos.x == snakePositions[i].x && headPos.y == snakePositions[i].y) {
			return true;
		}
	}
	return false;
}

// Funcao para verificar colisao com a parede
bool ofApp::checkCollisionWall(Direction direction) {
	switch (direction) {
		case LEFT:
			return snakeX < 0;
		case RIGHT:
			return snakeX >= resX;
		case UP:
			return snakeY < 0;
		case DOWN:
			return snakeY >= resY;
		default:
			return false;
		}
}

// Funcao para dar "reset" ao jogo
void ofApp::resetGame() {
	resX = 25;
	resY = 25;
	currentDirection = NONE;
	snakeX = floor(resX / 2);
	snakeY = floor(resY / 2);
	snakePos.z = floorHeightPos + snakeDepth * 0.5;
	snakePositions.clear();
	snakePositions.push_back(ofVec3f(snakeX, snakeY, snakePos.z));
	drawRandomFruit();
	speed = 0.15;
	permission = true;
	level = 0;
	setupSnake();
	if (!gameOver) {
		score = 0;
	}
}

// Funcao para desenhar as lampadas
void ofApp::poste_luz(GLint x, GLint y) {
	for (int i = 0; i < 4; i++) {
		glPushMatrix();
		glTranslatef(-floorWidth * 0.5 + (x * floorWidth / GLfloat(resX)) + snakeWidth * 0.5, floorHeight * 0.5 - (y * floorHeight / GLfloat(resY)) - snakeHeight * 0.5, floorHeightPos + snakeDepth * 0.5 + i * snakeDepth);
		glScalef(snakeWidth, snakeHeight, snakeDepth);
		img5.bind();
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		cube_unit(256);
		img5.unbind();
		glPopMatrix();
	}
	glPushMatrix();
	glTranslatef(-floorWidth * 0.5 + (x * floorWidth / GLfloat(resX)) + snakeWidth * 0.5, floorHeight * 0.5 - (y * floorHeight / GLfloat(resY)) - snakeHeight * 0.5, floorHeightPos + snakeDepth * 0.5 + 4 * snakeDepth);
	glScalef(snakeWidth, snakeHeight, snakeDepth);
	img6.bind();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	cube_unit(650);
	img6.unbind();
	glPopMatrix();
}

// Funcao para desenhar uma linha da parede
void ofApp::wallLine(GLint x, GLint y, GLint counter) {
	for (int i = 0; i < counter; i++) {
		glPushMatrix();
		glTranslatef(-floorWidth * 0.5 + (x * floorWidth / GLfloat(resX)) + snakeWidth * 0.5 + i * snakeWidth, floorHeight * 0.5 - (y * floorHeight / GLfloat(resY)) - snakeHeight * 0.5, floorHeightPos + (snakeDepth * 0.3) * 0.5);
		glScalef(snakeWidth, snakeHeight, snakeDepth * 0.3);
		img7.bind();
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		cube_unit(356);
		img7.unbind();
		glPopMatrix();
	}
}

// Funcao para desenhar uma coluna da parede
void ofApp::wallColumn(GLint x, GLint y, GLint counter) {
	for (int i = 0; i < counter; i++) {
		glPushMatrix();
		glTranslatef(-floorWidth * 0.5 + (x * floorWidth / GLfloat(resX)) + snakeWidth * 0.5, floorHeight * 0.5 - (y * floorHeight / GLfloat(resY)) - snakeHeight * 0.5 - i * snakeHeight, floorHeightPos + (snakeDepth * 0.3) * 0.5);
		glScalef(snakeWidth, snakeHeight, snakeDepth * 0.3);
		img7.bind();
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		cube_unit(356);
		img7.unbind();
		glPopMatrix();
	}
}

// Funcao para fazer a parede
void ofApp::wall() {
	wallLine(0, -1, resX);
	wallLine(-1, resY, resX + 2);
	wallColumn(-1, 0, resY);
	wallColumn(resX, 0, resY);
}

// Funcao para colocar luz pontual na fruta
void ofApp::fruitLight() {
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	lightPos[0] = -floorWidth * 0.5 + (fruitX * floorWidth / GLfloat(resX)) + snakeWidth * 0.5;  // Posição X da luz
	lightPos[1] = floorHeight * 0.5 - (fruitY * floorHeight / GLfloat(resY)) - snakeHeight * 0.5; // Posição Y da luz
	lightPos[2] = floorHeightPos + snakeDepth * 0.5; // Posição Z da luz
	lightPos[3] = 1.0; // Constante
	if (amb) {
		lightAmb[0] = 0.1;  // R
		lightAmb[1] = 0.1;  // G
		lightAmb[2] = 0.1;  // B
		lightAmb[3] = 1.0;  // Constante
	}
	else {
		lightAmb[0] = 0.0;  // R
		lightAmb[1] = 0.0;  // G
		lightAmb[2] = 0.0;  // B
		lightAmb[3] = 1.0;  // Constante
	}
	if (dif) {
		lightDif[0] = 0.75; // R
		lightDif[1] = 0.0;  // G
		lightDif[2] = 0.0;  // B
		lightDif[3] = 1.0;  // Constante
	}
	else {
		lightDif[0] = 0.0;  // R
		lightDif[1] = 0.0;  // G
		lightDif[2] = 0.0;  // B
		lightDif[3] = 1.0;  // Constante
	}
	if (spec || !spec) {
		lightSpec[0] = 0.0; // R
		lightSpec[1] = 0.0; // G
		lightSpec[2] = 0.0; // B
		lightSpec[3] = 1.0; // Constante
	}
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpec);
	// Ligar e desligar as luzes
	if (light) {
		glEnable(GL_LIGHT1);
	}
	else {
		glDisable(GL_LIGHT1);
	}
}

// Funcao para colocar luz foco nas lampadas
void ofApp::lampLight(GLenum lightID, GLint lamp, GLint posX, GLint posY, GLint dirX, GLint dirY) {
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	GLfloat angulo = 25.0;
	lampPos[0] = -floorWidth * 0.5 + (posX * floorWidth / GLfloat(resX)) + snakeWidth * 0.5; // Posição X da luz
	lampPos[1] = floorHeight * 0.5 - (posY * floorHeight / GLfloat(resY)) - snakeHeight * 0.5; // Posição Y da luz
	lampPos[2] = floorHeightPos + snakeDepth * 0.5 + snakeDepth; // Posição Z da luz
	lampPos[3] = 1.0; // Constante
	ofVec3f headPos = snakePositions[0];
	if (follow) { // Seguir a cabeça da cobra
		lampDir[0] = -floorWidth * 0.5 + (headPos.x * floorWidth / GLfloat(resX)) + snakeWidth * 0.5; // Direção X da luz
		lampDir[1] = floorHeight * 0.5 - (headPos.y * floorHeight / GLfloat(resY)) - snakeHeight * 0.5; // Direção Y da luz
		lampDir[2] = floorHeightPos + snakeDepth * 0.5 + snakeDepth; // Direção Z da luz
		lampDir[3] = 1.0;
	}
	else { // Luz fixa
		lampDir[0] = -floorWidth * 0.5 + (dirX * floorWidth / GLfloat(resX)) + snakeWidth * 0.5; // Direção X da luz
		lampDir[1] = floorHeight * 0.5 - (dirY * floorHeight / GLfloat(resY)) - snakeHeight * 0.5; // Direção Y da luz
		lampDir[2] = floorHeightPos + snakeDepth * 0.5 + snakeDepth; // Direção Z da luz
		lampDir[3] = 1.0f;
	}
	if (amb || !amb) {
		lampAmb[0] = 0.0;  // R
		lampAmb[1] = 0.0;  // G
		lampAmb[2] = 0.0;  // B
		lampAmb[3] = 0.0;  // Constante
	}
	if (dif) {
		lampDif[0] = 0.25;  // R
		lampDif[1] = 0.50;  // G
		lampDif[2] = 0.25;  // B
		lampDif[3] = 0.25;  // Constante
	}
	else {
		lampDif[0] = 0.0;  // R
		lampDif[1] = 0.0;  // G
		lampDif[2] = 0.0;  // B
		lampDif[3] = 0.0;  // Constante
	}
	if (spec) {
		lampSpec[0] = 0.10;  // R
		lampSpec[1] = 0.10;  // G
		lampSpec[2] = 0.10;  // B
		lampSpec[3] = 0.30;  // Constante
	}
	else {
		lampSpec[0] = 0.0;  // R
		lampSpec[1] = 0.0;  // G
		lampSpec[2] = 0.0;  // B
		lampSpec[3] = 0.0;  // Constante
	}
	glLightf(lightID, GL_SPOT_CUTOFF, angulo);
	glLightfv(lightID, GL_POSITION, lampPos);
	glLightfv(lightID, GL_SPOT_DIRECTION, lampDir);
	glLightfv(lightID, GL_AMBIENT, lampAmb);
	glLightfv(lightID, GL_DIFFUSE, lampDif);
	glLightfv(lightID, GL_SPECULAR, lampSpec);
	// Ligar e desligar as luzes
	if (lamp == 0) {
		if (left_lamp) {
			glEnable(lightID);
		}
		else {
			glDisable(lightID);
		}
	}
	if (lamp == 1) {
		if (right_lamp) {
			glEnable(lightID);
		}
		else {
			glDisable(lightID);
		}
	}
}

void ofApp::keyReleased(int key) {
	if (key == ' ') {
		nitro = false;
		speed = savespeed;
	}
}

void ofApp::mouseMoved(int x, int y) {

}

void ofApp::mouseDragged(int x, int y, int button) {

}

void ofApp::mousePressed(int x, int y, int button) {
	cout << endl << x << " " << y;
}

void ofApp::mouseReleased(int x, int y, int button) {

}

void ofApp::mouseEntered(int x, int y) {

}

void ofApp::mouseExited(int x, int y) {

}

void ofApp::windowResized(int w, int h) {
	setup();
}

void ofApp::gotMessage(ofMessage msg) {

}

void ofApp::dragEvent(ofDragInfo dragInfo) {

}