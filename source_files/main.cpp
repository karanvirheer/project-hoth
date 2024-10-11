#include <stdio.h>
#include <windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "main.h"
#include <math.h>
#include <string.h>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

/************************************************************************************
 *
 *
 * CONSTANTS
 *
 *
 ************************************************************************************/


 /*********************
 *
 * WINDOW VARIABLES
 *
 *********************/

GLint glutWindowWidth = 800;
GLint glutWindowHeight = 600;
GLint viewportWidth = glutWindowWidth;
GLint viewportHeight = glutWindowHeight;

// screen window identifiers
int window3D;

int window3DSizeX = 800, window3DSizeY = 600;
GLdouble aspect = (GLdouble)window3DSizeX / window3DSizeY;

/*********************
*
* GROUND MESH VARIABELS
*
*********************/

float groundX = 80.0f;
float groundY = 4.0f;
float groundZ = 50.0f;

int currentButton;

/*********************
*
* ROBOT OBJECTS
*
*********************/

// Z position where Robots Spawn
float robotSpawnBoundary = -10.0;

LeftRobot leftRobot(20);
MiddleRobot middleRobot(0);
RightRobot rightRobot(-20);


/*********************
*
* PLAYER OBJECT
*
*********************/

Player player;

/*********************
*
* TREE GENERATION VARIABLES
*
*********************/

int numTrees = 50;
Tree tree[50];
bool spawnedTerrain = true;

/*********************
*
* BULLET VARIABLES
*
*********************/

vector<Bullet> bulletArray;
vector<Bullet> robotBulletsArray;

bool startBulletAnimation = false;

/*********************
 *
 * RESTART GAME
 *
 *********************/

bool restart = false;
int enemiesKilled = 0;

/*********************
 *
 * TEXTURE MAPPING
 *
 *********************/

GLuint texture[6];

/*********************
*
* RANDOM BULLSHIT
*
*********************/

int pitchAngle;

/************************************************************************************
 *
 *
 * MAIN FUNCTION
 *
 *
 ************************************************************************************/

int main(int argc, char* argv[])
{
	glutInit(&argc, (char**)argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(glutWindowWidth, glutWindowHeight);
	glutInitWindowPosition(50, 100);

	// The 3D Window
	window3D = glutCreateWindow("Star Wars Battlefront: Budget Edition");
	glutPositionWindow(900, 100);
	glutDisplayFunc(display3D);
	glutReshapeFunc(reshape3D);
	glutMouseFunc(mouseButtonHandler3D);
	glutMotionFunc(mouseMotionHandler3D);
	glutPassiveMotionFunc(passiveMouseMotionHandler);
	glutKeyboardFunc(keyboardHandler3D);
	init3DSurfaceWindow();

	// GLEW Stuff
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	// Annnd... ACTION!!
	glutMainLoop();

	return 0;
}

/************************************************************************************
 *
 *
 * BASIC OPENGL SHIT
 *
 * 
 ************************************************************************************/
 // Ground Mesh material
GLfloat groundMat_ambient[] = { 0.4, 0.4, 0.4, 1.0 };
GLfloat groundMat_specular[] = { 0.01, 0.01, 0.01, 1.0 };
GLfloat groundMat_diffuse[] = { 0.4, 0.4, 0.7, 1.0 };
GLfloat groundMat_shininess[] = { 1.0 };

GLfloat light_position_main0[] = { 4.0, 8.0, 8.0, 1.0 };
GLfloat light_diffuse0[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat light_position_main1[] = { -4.0, 8.0, 8.0, 1.0 };
GLfloat light_diffuse1[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat light_specular_main[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat model_ambient[] = { 0.5, 0.5, 0.5, 1.0 };

// Set up lighting/shading and material properties for surface of revolution
GLfloat quadMat_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat quadMat_specular[] = { 0.45, 0.55, 0.45, 1.0 };
GLfloat quadMat_diffuse[] = { 0.1, 0.35, 0.1, 1.0 };
GLfloat quadMat_shininess[] = { 10.0 };


/*********************
*
* MOUSE MOVEMENT VARIABLES
*
*********************/

int lastMouseX;
int lastMouseY;

/*********************
*
* CAMERA ROTATION CONSTANTS
*
*********************/

int gunYawAngle = 0;
int gunPitchAngle = 0;

int cameraYawAngle = 0;
int mouseYawAngle = 0;


/*********************
*
* PERSPECTIVE/ORTHO CAMERA CONSTANTS
*
*********************/

GLdouble eyeX = 0.0, eyeY = 5.0, eyeZ = groundZ;
GLdouble radius = eyeZ;
GLdouble zNear = 0.1, zFar = 100.0;
GLdouble fov = 60.0;

void init3DSurfaceWindow()
{
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position_main0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular_main);
	glLightfv(GL_LIGHT0, GL_AMBIENT, model_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular_main);
	glLightfv(GL_LIGHT1, GL_AMBIENT, model_ambient);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position_main1);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);    // Renormalize normal vectors 
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glClearColor(0.4F, 0.4F, 0.4F, 0.0F);  // Color and depth for glClear

	glViewport(0, 0, (GLsizei)window3DSizeX, (GLsizei)window3DSizeY);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, zNear, zFar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}


void reshape3D(int w, int h)
{
	glutWindowWidth = (GLsizei)w;
	glutWindowHeight = (GLsizei)h;
	glViewport(0, 0, glutWindowWidth, glutWindowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, zNear, zFar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void display3D()
{
	// Added this myself
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);

	// ---------------

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	// Set up the Viewing Transformation (V matrix)	
	gluLookAt(eyeX, eyeY, eyeZ, player.getViewX() * 20, player.getViewY(), player.getViewZ() * 20, 0.0, 1.0, 0.0);

	glPushMatrix();

	gameStateHandler();

	glPopMatrix();

	glutSwapBuffers();
}

void gameStateHandler()
{
	glEnable(GL_TEXTURE_2D);

	// DO THIS ONE TIME ONLY
	if (spawnedTerrain)
	{

		// Spawn The Trees
		spawnTerrain();

		// Create Player Mesh
		player.createMesh();

		// Generate all textures
		generateTexture();

		// Start the animation timer
		glutTimerFunc(1000 / 60, animationTimer, 0);

		// Never run this IF statement again
		spawnedTerrain = false;
	}

	drawGround();
	spawnTerrain();
	player.draw();

	// If Restart Condition Has NOT Been Met
	if (!(enemiesKilled >= 10 || player.health <= 0))
	{
		// Robots are drawn
		setup();
	}

	// Restart Condition HAS Been Met
	else 
	{
		restart = true;

		leftRobot.health = 0;
		rightRobot.health = 0;
		middleRobot.health = 0;
		player.health = 0;
	}

	glDisable(GL_TEXTURE_2D);
}

/************************************************************************************
 *
 *
 * TEXTURE GENERATION
 *
 *
 ************************************************************************************/

void generateTexture()
{
	int width, height, nrChannels;

	const char* texturePaths[6] =
	{
		"./Textures/Bullet_Texture/gun.jpg", // 0
		"./Textures/Ground_Texture/ground.png", // 1
		"./Textures/Gun_Texture/gun.png",		// 2
		"./Textures/Leaves_Texture/leaves.jpg", // 3
		"./Textures/Robot_Texture/gold.jpg",	// 4
		"./Textures/Tree_Texture/tree.jpg"		// 5
	};

	glGenTextures(6, texture);

	for (int i = 0; i < 6; i++)
	{
		glBindTexture(GL_TEXTURE_2D, texture[i]);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		unsigned char* data = stbi_load(texturePaths[i], &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			printf("TEXTURE FAILED TO LOAD\n");
		}

		stbi_image_free(data);
	}
}

/************************************************************************************
 *
 *
 * TERRAIN + ROBOT + PLAYER GENERATION
 *
 *
 ************************************************************************************/

void drawGround()
{
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glPushMatrix();
	
	glBegin(GL_QUADS);

		glNormal3f(0, 1, 0);
		glTexCoord3f(1.0, 0.0, 1.0);
		glVertex3f(-groundX, -groundY, -groundZ);

		glNormal3f(0, 1, 0);
		glTexCoord3f(1.0, 0.0, 0.0);
		glVertex3f(-groundX, -groundY, groundZ);

		glNormal3f(0, 1, 0);
		glTexCoord3f(0.0, 0.0, 0.0);
		glVertex3f(groundX, -groundY, groundZ);

		glNormal3f(0, 1, 0);
		glTexCoord3f(0.0, 0.0, 1.0);
		glVertex3f(groundX, -groundY, -groundZ);

	glEnd();
	
	glPopMatrix();

}

// Robots Spawned
void setup()
{
	checkPlayerBulletCollision();
	checkRobotBulletCollision();

	robotShoot();

	leftRobot.draw();
	middleRobot.draw();
	rightRobot.draw();

	deleteBullets();
}


/************************************************************************************
 *
 *
 * TERRAIN GENERATION
 *
 *
 ************************************************************************************/

// Draws the Tree Objects
void spawnTerrain()
{
	for (int i = 0; i < numTrees; i++)
	{
		tree[i].draw();
	}
}

// Creates the Tree Objects
void createTerrain()
{
	for (int i = 0; i < numTrees; i++)
	{
		tree[i] = Tree();
	}
}

/************************************************************************************
 *
 *
 * ROBOT BULLET GENERATION + DELETION + ANIMATION + COLLISION DETECTION
 *
 *
 ************************************************************************************/

float currTime;
float oldTime = 0.0f;

void robotShoot()
{
	int ptrNum = ((rand() % (2 - 0 + 1)) + 0);
	int delayNum = ((rand() % (5 - 1 + 1)) + 1);

	// Random Delay to Offset Animations
	currTime = glutGet(GLUT_ELAPSED_TIME);

	// Random Delay to Offset Animations
	int* delaysPtr = generateAnimationDelay(delayNum);
	float deltaTime = currTime - oldTime;

	if (deltaTime > *delaysPtr + ptrNum) {
		// Create a Bullet and store it
		robotBulletsArray.push_back(Bullet());
		oldTime = currTime;
	}
}

void drawRobotBullets() 
{

	for (int i = 0; i < size(robotBulletsArray); i++)
	{

		robotBulletsArray[i].draw();

	}
}

void robotBulletToWorldCoordinates(GLfloat modelview[16])
{
	for (int i = 0; i < size(robotBulletsArray); i++)
	{

		robotBulletsArray[i].getGunCoordinates(modelview);

	}
}

void animateRobotBullets()
{
	for (int i = 0; i < size(robotBulletsArray); i++)
	{
		robotBulletsArray[i].animate();
	}
}

/*********************
 *
 * ROBOT BULLET COLLISION WITH PLAYER
 *
 *********************/

void checkRobotBulletCollision()
{
	for (int i = 0; i < size(robotBulletsArray); i++)
	{

		if (robotBulletsArray[i].getWorldZ() < robotBulletsArray[i].worldBoundaryNegZ ||
			robotBulletsArray[i].getWorldZ() > robotBulletsArray[i].worldBoundaryPosZ ||
			robotBulletsArray[i].getWorldX() < robotBulletsArray[i].worldBoundaryNegX ||
			robotBulletsArray[i].getWorldX() > robotBulletsArray[i].worldBoundaryPosX)
		{
			robotBulletsArray[i].collision = true;
		}

		if (euclideanDistance(player.position, robotBulletsArray[i].worldPosition) <= 10.0f)
		{
			player.health -= 1;
			robotBulletsArray[i].collision = true;
		}
	}
}

/************************************************************************************
 *
 *
 * PLAYER BULLET GENERATION + DELETION + ANIMATION + COLLISION DETECTION
 *
 *
 ************************************************************************************/

/*********************
 *
 * CREATE A BULLET OBJECT EVERYTIME YOU SHOOT
 *
 *********************/

void shoot()
{
	// Create a Bullet and store it
	bulletArray.push_back(Bullet());

	// Give it the correct pitch and angle
	bulletArray.back().setPitchAngle(player.getPitchAngle());
	bulletArray.back().setYawAngle(player.getYawAngle());

	// start animating the bullet
	startBulletAnimation = true;

}

/*********************
 *
 * DRAW ALL BULLETS SHOT
 *
 *********************/

void drawBullets(GLfloat modelview[16])
{

	for (int i = 0; i < size(bulletArray); i++)
	{
		
		//bulletArray[i].checkCollision(leftRobot, middleRobot, rightRobot);
		bulletArray[i].draw();
		bulletArray[i].getGunCoordinates(modelview);

	}
}

/*********************
 *
 * DELETE BULLETS IF THEY COLLIDED WITH ANYTHING
 *
 *********************/

void deleteBullets()
{
	for (int i = 0; i < size(bulletArray); i++)
	{
		if (bulletArray[i].collision)
		{
			bulletArray.erase(bulletArray.begin() + i);
		}
	}
}

/*********************
 *
 * ANIMATE ALL BULLETS
 *
 *********************/

void animateBullets()
{
	for (int i = 0; i < size(bulletArray); i++)
	{
		bulletArray[i].animate();
	}
}

/*********************
 *
 * PLAYER BULLET COLLISION WITH ROBOT
 *
 *********************/

void checkPlayerBulletCollision()
{
	for (int i = 0; i < size(bulletArray); i++)
	{

		if (bulletArray[i].getWorldZ() < bulletArray[i].worldBoundaryNegZ ||
			bulletArray[i].getWorldZ() > bulletArray[i].worldBoundaryPosZ ||
			bulletArray[i].getWorldX() < bulletArray[i].worldBoundaryNegX ||
			bulletArray[i].getWorldX() > bulletArray[i].worldBoundaryPosX)
		{
			bulletArray[i].collision = true;
		}

		if (euclideanDistance(leftRobot.position, bulletArray[i].worldPosition) <= 10.0f)
		{
			leftRobot.health -= 1;
			bulletArray[i].collision = true;
		}

		if (euclideanDistance(middleRobot.position, bulletArray[i].worldPosition) <= 10.0f)
		{
			middleRobot.health -= 1;
			bulletArray[i].collision = true;
		}

		if (euclideanDistance(rightRobot.position, bulletArray[i].worldPosition) <= 10.0f)
		{
			rightRobot.health -= 1;
			bulletArray[i].collision = true;
		}
	}
}


/************************************************************************************
 *
 *
 * ANIMATION
 *
 *
 ************************************************************************************/

void animationTimer(int param)
{
	// 50 * 1000 ms is the Max Delay
	int *delaysPtr = generateAnimationDelay(50);

	// Animate the Robots with Delay
	leftRobot.animate(*delaysPtr);
	middleRobot.animate(*(delaysPtr + 1));
	rightRobot.animate(*(delaysPtr + 2));

	// Player Bullet Animation
	if (startBulletAnimation)
	{
		animateBullets();
	}

	animateRobotBullets();

	glutPostRedisplay();
	glutTimerFunc(1000 / 60, animationTimer, 0);
}

// Generates a list of delays for the Robot animations
// setMax is the upperbound for the delay
int *generateAnimationDelay(int setMax)
{
	unsigned seed;
	seed = time(0);
	srand(seed);

	int delays[3];
	int max = setMax;
	int min = 1;

	delays[0] = ((rand() % (max - min + 1)) + min) * 1000;
	delays[1] = ((rand() % (max - min + 1)) + min) * 1000;
	delays[2] = ((rand() % (max - min + 1)) + min) * 1000;

	return delays;
}

/************************************************************************************
 *
 *
 * MOUSE AND CAMERA HANDLING
 *
 *
 ************************************************************************************/

void mouseButtonHandler3D(int button, int state, int x, int y)
{
	currentButton = button;
	lastMouseX = x;
	lastMouseY = y;

	switch (button)
	{
	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_UP)
		{
		}
		break;
	default:
		break;
	}
}

// Handles Gun Movement with Mouse
void passiveMouseMotionHandler(int x, int y)
{
	float dx = lastMouseX - x; // xoffset
	float dy = lastMouseY - y; // yoffset

	dy *= 3;

	float yawSensitivity = 0.2f;
	float pitchSensitivity = 0.2f;

	gunYawAngle += (float)dx * yawSensitivity;
	gunPitchAngle += (float)dy * pitchSensitivity;

	// Horizontal Rotation
	player.setYawAngle(gunYawAngle);

	if (player.getYawAngle() > 60) {
		player.setYawAngle(60.0f);
	}
	else if (player.getYawAngle() < -60) {
		player.setYawAngle(-60.0f);
	}

	// Vertical Rotation
	player.setPitchAngle(gunPitchAngle);

	if (player.getPitchAngle() > 90) {
		player.setPitchAngle(90.0f);
	}
	else if (player.getPitchAngle() < 0) {
		player.setPitchAngle(0.0f);
	}

	lastMouseX = x;
	lastMouseY = y;
	glutPostRedisplay();
}

void mouseMotionHandler3D(int x, int y)
{
	int dx = x - lastMouseX; // xoffset
	int dy = y - lastMouseY; // yoffset

	mouseYawAngle += (float)dx * 0.5f;
	pitchAngle += (float)dy * 0.5f;

	if (currentButton == GLUT_LEFT_BUTTON)
	{

		eyeX = cos(mouseYawAngle * 0.01f) * radius;
		eyeZ = sin(mouseYawAngle * 0.01f) * radius;

	}
	if (currentButton == GLUT_RIGHT_BUTTON)
	{

		eyeY = sin(pitchAngle * 0.02f) * radius;

		// Constraints for pitch
		if (eyeY > 60.0f)
			eyeY = 60.0f;
		if (eyeY < 0.0f)
			eyeY = 0.0f;
	}
	else if (currentButton == GLUT_MIDDLE_BUTTON)
	{

		fov -= (GLdouble)dy;

		// Setting FOV limits
		if (fov < 1.0f)
			fov = 1.0f;
		if (fov > 100.0f)
			fov = 100.0f;
	}

	lastMouseX = x;
	lastMouseY = y;
	glutPostRedisplay();
}

void keyboardHandler3D(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
	case 'Q':
	case 27:
		// Esc, q, or Q key = Quit 
		free(player.vertices);
		free(player.normals);
		free(player.indices);
		exit(0);
		break;

	case 'r':
	case 'R':
		enemiesKilled = 0;
		player.health = 24;
		break;

	// Shoot
	// Spacebar
	case 32:
		shoot();
		break;

	default:
		break;
	}
	glutPostRedisplay();
}

Vector3D crossProduct(Vector3D a, Vector3D b)
{
	Vector3D cross;

	cross.x = a.y * b.z - b.y * a.z;
	cross.y = a.x * b.z - b.x * a.z;
	cross.z = a.x * b.y - b.x * a.y;
	return cross;
}

float euclideanDistance(Vector3D a, Vector3D b) 
{
	b.z = -b.z;

	float c_x = (a.x - b.x) * (a.x - b.x);
	float c_y = (a.y - b.y) * (a.y - b.y);
	float c_z = (a.z - b.z) * (a.z - b.z);

	return sqrt(c_x + c_y + c_z);
	
}
