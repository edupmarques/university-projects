#pragma once

#include "ofMain.h"
#include "cg_extras.h"
#include "cg_drawing_extras.h"
#include "cg_cam_extras.h"
#include <random>
#include <cstdlib>
#include <ctime>

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void drawRandomFruit();
		bool checkCollisionFruit();
		bool checkSelfCollision();
		void resetGame();
		void poste_luz(GLint x, GLint y);
		void wallLine(GLint x, GLint y, GLint counter);
		void wallColumn(GLint x, GLint y, GLint counter);
		void wall();
		void fruitLight();
		void lampLight(GLenum lightID, GLint lamp, GLint posX, GLint posY, GLint dirX, GLint dirY);

		int view;
		GLfloat lensAngle;
		GLfloat alpha, beta;
		GLfloat perfectDist;
		
		void setupSnake();
		void drawSnake();

		GLint resX, resY;
		GLfloat floorWidth, floorHeight, floorHeightPos;

		GLfloat snakeWidth, snakeDepth, snakeHeight;
		GLfloat snakeX, snakeY;
		ofVec3f snakePos;

		enum Direction { NONE, LEFT, RIGHT, UP, DOWN };
		Direction currentDirection;
		bool checkCollisionWall(Direction direction);

		std::vector<ofVec3f> snakePositions;

		GLfloat rotation;
		GLint cameraView;
		GLfloat elapsedTime;

		GLfloat fruitX, fruitY;

		GLfloat speed;
		GLfloat nitroSpeed;
		bool nitro;
		GLfloat savespeed;

		GLfloat destructionDuration;
		GLfloat destructionTimer;
		GLfloat destructionIntensity;
		bool destruction;

		GLfloat colorChangeTimer;
		GLfloat colorChangeDuration;
		bool colorChange;

		GLint score;

		ofImage img1;
		ofImage img2;
		ofImage img3;
		ofImage img4;
		ofImage img5;
		ofImage img6;
		ofImage img7;

		GLfloat lightPos[4];
		GLfloat lightAmb[4];
		GLfloat lightDif[4];
		GLfloat lightSpec[4];
		GLfloat lampPos[4];
		GLfloat lampDir[4];
		GLfloat lampAmb[4];
		GLfloat lampDif[4];
		GLfloat lampSpec[4];

		bool permission;
		bool mode;
		GLint level;

		bool light;
		bool left_lamp;
		bool right_lamp;
		bool follow;

		bool amb;
		bool dif;
		bool spec;

		bool binds;
		bool gameOver;
};
