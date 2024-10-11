#include <vector>

/************************************************************************************
 *
 *
 * TEXTURES + LIGHTING
 *
 *
 ************************************************************************************/

extern GLfloat quadMat_ambient[];
extern GLfloat quadMat_specular[];
extern GLfloat quadMat_diffuse[];
extern GLfloat quadMat_shininess[];

/************************************************************************************
 *
 *
 * STRUCTS
 *
 *
 ************************************************************************************/

typedef struct Vector3D
{
	GLdouble x, y, z;
} Vector3D;

// Struct prototypes
typedef struct Vertex Vertex;


/************************************************************************************
 *
 *
 * CAMERA CONSTANTS
 *
 *
 ************************************************************************************/

extern GLdouble eyeX, eyeY, eyeZ;
extern GLdouble radius;
extern GLdouble zNear, zFar;
extern GLdouble fov;

/************************************************************************************
 *
 *
 * PLAYER.CPP
 *
 *
 ************************************************************************************/

class Player
{
public:

	/*********************
	 *
	 * SPAWN LOCATION
	 *
	 *********************/

	Vector3D position;
	Vector3D viewVector;

	/*********************
	 *
	 * PLAYER HEALTH
	 *
	 *********************/

	int health = 24;

	/*********************
	 *
	 * PLAYER MODEL CONSTANTS
	 *
	 *********************/

	float gunWidth = 1.0;
	float gunLength = 1.5;
	float gunDepth = 1.0;

	// Control Body Rotation
	float yawAngle = 0.0;
	float pitchAngle = 0.0;

	/*********************
	 *
	 * MESH CONSTANTS
	 *
	 *********************/

	// Indices/Vertices/Normals Array
	float* vertices;
	float* normals;
	int* indices;

	// Array Sizes
	int vSize = 0;
	int nSize = 0;
	int iSize = 0;

	/*********************
	 *
	 * CONSTRUCTOR / DESTRUCTOR
	 *
	 *********************/
	// Robot Constructor
	Player();

	// Robot Deconstructor
	~Player();

	/*********************
	 *
	 * FUNCTION DEFINITIONS
	 *
	 *********************/

	void draw();

	/*********************
	 *
	 * GETTERS / SETTERS POSITION
	 *
	 *********************/

	float getXCoord();
	void setXCoord(float newXCoord);

	float getYCoord();
	void setYCoord(float newYCoord);

	float getZCoord();
	void setZCoord(float newZCoord);

	// Horizontal rotation
	float getYawAngle();
	void setYawAngle(float newYawAngle);

	// Vertical rotation
	float getPitchAngle();
	void setPitchAngle(float newPitchAngle);

	/*********************
	 *
	 * VIEW VECTOR
	 *
	 *********************/

	float getViewX();
	void setViewX(float newXCoord);

	float getViewY();
	void setViewY(float newYCoord);

	float getViewZ();
	void setViewZ(float newZCoord);

	/*********************
	 *
	 * MESH FUNCTIONS
	 *
	 *********************/
	void createMesh();
	void loadMesh();

};


/************************************************************************************
 *
 *
 * MIDDLE ROBOT.CPP
 *
 *
 ************************************************************************************/

extern float robotSpawnBoundary;

class MiddleRobot
{

public:

	/*********************
	 *
	 * SHUFFLE ANIMATION
	 *
	 *********************/

	float shuffleAngle = 0.0f;


	/*********************
	 *
	 * HITBOX CONSTANTS
	 *
	 *********************/

	int health = 3;

	/*********************
	 *
	 * ANIMATION DELAY CONSTANTS
	 *
	 *********************/

	float currTime;
	float oldTime = 0;
	bool startAnimation = false;

	/*********************
	 *
	 * ROBOT RESPAWN BOUNDARY
	 *
	 *********************/

	float worldBoundary = 55.0f;

	/*********************
	 *
	 * ROBOT MODEL CONSTANTS
	 *
	 *********************/

	// Body Constants
	float robotBodyWidth = 4.0;
	float robotBodyLength = 4.0;
	float robotBodyDepth = 4.0;

	// Gun Constants
	float gunLength = robotBodyWidth * 2.0f;
	float gunWidth = robotBodyWidth * 0.2f;
	float gunDepth = robotBodyWidth * 2.0f;

	float hipDepth = robotBodyDepth * 0.2f;
	float hipWidth = robotBodyWidth * 5.0f;
	float hipLength = robotBodyLength * 5.0f;

	float kneeLength = robotBodyLength * 2.0f;
	float kneeWidth = robotBodyWidth * 0.5f;
	float kneeDepth = robotBodyDepth * 0.5f;

	float footLength = robotBodyLength * 0.15f;
	float footWidth = robotBodyWidth * 5.0f;
	float footDepth = robotBodyDepth * 5.0f;

	float baseWidth = 2.0f * robotBodyWidth;
	float baseLength = 0.25f * robotBodyLength;

	// Control Robot body rotation on base
	float robotAngle = -90.0f;

	// Control Body Rotation
	float bodyAngle = 0.0f;

	// Control Knee Rotation
	float kneeAngle = 0.0f;
	float leftKneeAngle = 0.0f;
	float rightKneeAngle = 0.0f;

	// Control Hip Rotation
	float hipAngle = 0.0f;
	float leftHipAngle = 0.0f;
	float rightHipAngle = 0.0f;

	// Control Foot Rotation
	float footAngle = 0.0f;
	float leftFootAngle = 0.0f;
	float rightFootAngle = 0.0f;

	// Contro Gun Rotation
	float gunAngle = 0.0f;

	// Walking Animation Bools
	bool stopGunBool = false;
	bool walkBool = false;

	/*********************
	 *
	 * LEG ANIMATION VARIABLES
	 *
	 *********************/
	
	// Limits on the leg movement
	const float maxHipAngle = 3.0;
	const float maxKneeAngle = 5.0;
	const float angleSensitivity = 15.0;

	/*********************
	 *
	 * SPAWN LOCATION VARIABLES
	 *
	 *********************/

	// DO NOT CHANGE position.y
	// Robot must be above the ground
	Vector3D spawn;
	Vector3D position;

	/*********************
	 *
	 * CONSTRUCTOR / DESTRUCTOR
	 *
	 *********************/

	// Robot Constructor
	// Robot must be given an x spawn coordinate
	MiddleRobot(float x);

	// Robot Deconstructor
	~MiddleRobot();

	/*********************
	 *
	 * FUNCTION DEFINITIONS
	 *
	 *********************/

	void draw();
	void drawBody();
	void drawGun();
	void drawGunDesign();
	void drawLeftHip();
	void drawRightHip();
	void drawLeftKnee();
	void drawRightKnee();
	void drawLeftFoot();
	void drawRightFoot();
	void animate(int delay);
	void walkAnimation();
	void respawn();
	void gunAnimation();
	void headAnimation();
	void shuffleAnimation();
};

/************************************************************************************
 *
 *
 * LEFTROBOT.CPP
 *
 *
 ************************************************************************************/

class LeftRobot
{

public:

	/*********************
	 *
	 * SHUFFLE ANIMATION
	 *
	 *********************/

	float shuffleAngle = 0.0f;

	/*********************
	 *
	 * HITBOX CONSTANTS
	 *
	 *********************/

	int health = 3;

	/*********************
	 *
	 * ANIMATION DELAY CONSTANTS
	 *
	 *********************/

	float currTime;
	float oldTime = 0;
	bool startAnimation = false;

	/*********************
	 *
	 * ROBOT RESPAWN BOUNDARY
	 *
	 *********************/

	float worldBoundary = 55.0f;

	/*********************
	 *
	 * ROBOT MODEL CONSTANTS
	 *
	 *********************/

	 // Body Constants
	float robotBodyWidth = 4.0;
	float robotBodyLength = 4.0;
	float robotBodyDepth = 4.0;

	// Gun Constants
	float gunLength = robotBodyWidth * 2.0;
	float gunWidth = robotBodyWidth * 0.2;
	float gunDepth = robotBodyWidth * 2.0;

	float hipDepth = robotBodyDepth * 0.2;
	float hipWidth = robotBodyWidth * 5;
	float hipLength = robotBodyLength * 5;

	float kneeLength = robotBodyLength * 2;
	float kneeWidth = robotBodyWidth * 0.5;
	float kneeDepth = robotBodyDepth * 0.5;

	float footLength = robotBodyLength * 0.15;
	float footWidth = robotBodyWidth * 5;
	float footDepth = robotBodyDepth * 5;

	float baseWidth = 2 * robotBodyWidth;
	float baseLength = 0.25 * robotBodyLength;

	// Control Robot body rotation on base
	float robotAngle = -90.0;

	// Control Body Rotation
	float bodyAngle = 0.0;

	// Control Knee Rotation
	float kneeAngle = 0.0;
	float leftKneeAngle = 0.0;
	float rightKneeAngle = 0.0;

	// Control Hip Rotation
	float hipAngle = 0.0;
	float leftHipAngle = 0.0;
	float rightHipAngle = 0.0;

	// Control Foot Rotation
	float footAngle = 0.0;
	float leftFootAngle = 0.0;
	float rightFootAngle = 0.0;

	// Contro Gun Rotation
	float gunAngle = 0.0;

	// Walking Animation Bools
	bool stopGunBool = false;
	bool walkBool = false;

	/*********************
	 *
	 * LEG ANIMATION VARIABLES
	 *
	 *********************/

	 // Limits on the leg movement
	const float maxHipAngle = 3.0;
	const float maxKneeAngle = 5.0;
	const float angleSensitivity = 15.0;

	/*********************
	 *
	 * SPAWN LOCATION VARIABLES
	 *
	 *********************/

	 // DO NOT CHANGE position.y
	 // Robot must be above the ground
	Vector3D spawn;
	Vector3D position;

	/*********************
	 *
	 * CONSTRUCTOR / DESTRUCTOR
	 *
	 *********************/

	 // Robot Constructor
	 // Robot must be given an x spawn coordinate
	LeftRobot(float x);

	// Robot Deconstructor
	~LeftRobot();

	/*********************
	 *
	 * FUNCTION DEFINITIONS
	 *
	 *********************/

	void draw();
	void drawBody();
	void drawGun();
	void drawGunDesign();
	void drawLeftHip();
	void drawRightHip();
	void drawLeftKnee();
	void drawRightKnee();
	void drawLeftFoot();
	void drawRightFoot();
	void animate(int delay);
	void walkAnimation();
	void respawn();
	void gunAnimation();
	void headAnimation();
	void shuffleAnimation();
};

/************************************************************************************
 *
 *
 * RIGHTROBOT.CPP
 *
 *
 ************************************************************************************/

class RightRobot
{

public:

	/*********************
	 *
	 * SHUFFLE ANIMATION
	 *
	 *********************/

	float shuffleAngle = 0.0f;

	/*********************
	 *
	 * HITBOX CONSTANTS
	 *
	 *********************/

	int health = 3;

	/*********************
	 *
	 * ANIMATION DELAY CONSTANTS
	 *
	 *********************/

	float currTime;
	float oldTime = 0;
	bool startAnimation = false;

	/*********************
	 *
	 * ROBOT RESPAWN BOUNDARY
	 *
	 *********************/

	// When Robots walk past Player Cannon, then make them respawn
	float worldBoundary = 55.0f;

	/*********************
	 *
	 * ROBOT MODEL CONSTANTS
	 *
	 *********************/

	 // Body Constants
	float robotBodyWidth = 4.0;
	float robotBodyLength = 4.0;
	float robotBodyDepth = 4.0;

	// Gun Constants
	float gunLength = robotBodyWidth * 2.0;
	float gunWidth = robotBodyWidth * 0.2;
	float gunDepth = robotBodyWidth * 2.0;

	float hipDepth = robotBodyDepth * 0.2;
	float hipWidth = robotBodyWidth * 5;
	float hipLength = robotBodyLength * 5;

	float kneeLength = robotBodyLength * 2;
	float kneeWidth = robotBodyWidth * 0.5;
	float kneeDepth = robotBodyDepth * 0.5;

	float footLength = robotBodyLength * 0.15;
	float footWidth = robotBodyWidth * 5;
	float footDepth = robotBodyDepth * 5;

	float baseWidth = 2 * robotBodyWidth;
	float baseLength = 0.25 * robotBodyLength;

	// Control Robot body rotation on base
	float robotAngle = -90.0;

	// Control Body Rotation
	float bodyAngle = 0.0;

	// Control Knee Rotation
	float kneeAngle = 0.0;
	float leftKneeAngle = 0.0;
	float rightKneeAngle = 0.0;

	// Control Hip Rotation
	float hipAngle = 0.0;
	float leftHipAngle = 0.0;
	float rightHipAngle = 0.0;

	// Control Foot Rotation
	float footAngle = 0.0;
	float leftFootAngle = 0.0;
	float rightFootAngle = 0.0;

	// Contro Gun Rotation
	float gunAngle = 0.0;

	// Walking Animation Bools
	bool stopGunBool = false;
	bool walkBool = false;

	/*********************
	 *
	 * LEG ANIMATION VARIABLES
	 *
	 *********************/

	 // Limits on the leg movement
	const float maxHipAngle = 3.0;
	const float maxKneeAngle = 5.0;
	const float angleSensitivity = 15.0;

	/*********************
	 *
	 * SPAWN LOCATION VARIABLES
	 *
	 *********************/

	 // DO NOT CHANGE position.y
	 // Robot must be above the ground
	Vector3D spawn;
	Vector3D position;

	/*********************
	 *
	 * CONSTRUCTOR / DESTRUCTOR
	 *
	 *********************/

	 // Robot Constructor
	 // Robot must be given an x spawn coordinate
	RightRobot(float x);

	// Robot Deconstructor
	~RightRobot();

	/*********************
	 *
	 * FUNCTION DEFINITIONS
	 *
	 *********************/

	void draw();
	void drawBody();
	void drawGun();
	void drawGunDesign();
	void drawLeftHip();
	void drawRightHip();
	void drawLeftKnee();
	void drawRightKnee();
	void drawLeftFoot();
	void drawRightFoot();
	void animate(int delay);
	void walkAnimation();
	void respawn();
	void gunAnimation();
	void headAnimation();
	void shuffleAnimation();
};

/************************************************************************************
 *
 *
 * BULLET.CPP
 *
 *
 ************************************************************************************/

class Bullet
{
public:

	/*********************
	*
	* BULLET VARIABLES
	*
	*********************/

	float bulletWidth = 2.0f;
	float bulletLength = 2.0f; // move this around
	float bulletDepth = 1.0f;

	/*********************
	*
	* SPAWN VARIABLES
	*
	*********************/

	Vector3D position;
	Vector3D worldPosition;

	/*********************
	*
	* DRAWING VARIABLES
	*
	*********************/

	// Control Body Rotation
	float yawAngle = 0.0;
	float pitchAngle = 0.0;

	/*********************
	 *
	 * BULLET COLLISION DETECTION
	 *
	 *********************/

	 // Bounday where Bullet collides with World
	float worldBoundaryNegZ;
	float worldBoundaryPosZ;

	float worldBoundaryNegX;
	float worldBoundaryPosX;

	float worldBoundaryNegY;
	float worldBoundaryPosY;

	// True if collision 
	// False if no collision
	bool collision = false;
	void checkCollision(LeftRobot leftRobot, MiddleRobot middleRobot, RightRobot rightRobot);

	/*********************
	 *
	 * GETTERS / SETTERS POSITION
	 *
	 *********************/

	float getXCoord();
	void setXCoord(float newXCoord);

	float getYCoord();
	void setYCoord(float newYCoord);

	float getZCoord();
	void setZCoord(float newZCoord);

	/*********************
	 *
	 * GETTERS / SETTERS WORLD POSITION
	 *
	 *********************/

	float getWorldX();
	void setWorldX(float newXCoord);

	float getWorldY();
	void setWorldY(float newYCoord);

	float getWorldZ();
	void setWorldZ(float newZCoord);

	/*********************
	 *
	 * GETTERS / SETTERS PITCH AND YAW
	 *
	 *********************/

	 // Horizontal rotation
	float getYawAngle();
	void setYawAngle(float newYawAngle);

	// Vertical rotation
	float getPitchAngle();
	void setPitchAngle(float newPitchAngle);


	/*********************
	*
	* CONSTRUCTOR / DESTRUCTOR
	*
	*********************/

	//Bullet(float x, float y, float z);
	Bullet();
	~Bullet();

	/*********************
	*
	* FUNCTION DEFINITIONS
	*
	*********************/

	void draw();
	void animate();
	void getGunCoordinates(GLfloat modelview[16]);
	//void getWorldCoordinates(GLfloat modelview[16]);
};

/************************************************************************************
 *
 *
 * TREE.CPP
 *
 *
 ************************************************************************************/

class Tree
{
public:
	
	/*********************
	 *
	 * TREE MODEL CONSTANTS
	 *
	 *********************/

	float treeLength = (rand() % 10) + 10.0; // y
	float treeDepth = 10.0; // z
	float treeWidth = 10.0; // x

	float leafLength = treeDepth;
	float leafDepth = treeDepth/2;
	float leafWidth = treeDepth/2;


	/*********************
	 *
	 * SPAWN LOCATION VARIABLES
	 *
	 *********************/

	Vector3D position;

	/*********************
	*
	* CONSTRUCTOR / DESTRUCTOR
	*
	*********************/

	Tree();
	~Tree();

	/*********************
	 *
	 * FUNCTION DEFINITIONS
	 *
	 *********************/

	void drawStump();
	void drawLeaves();
	void draw();
};

/************************************************************************************
 *
 *
 * MAIN.CPP
 *
 *
 ************************************************************************************/


/*********************
*
* GROUND MESH CONSTANTS
*
*********************/

 // Ground Mesh Depth Value
extern float groundX;
extern float groundY;
extern float groundZ;

/*********************
 *
 * TERRAIN CONSTANTS
 *
 *********************/

// Terrain Bool
extern bool spawnedTerrain;

/*********************
*
* BULLET VARIABLES
*
*********************/

extern bool startBulletAnimation;
extern std::vector<Bullet> bulletArray;
extern std::vector<Bullet> robotBulletsArray;

// Robot Functions
void robotShoot();
void drawRobotBullets();
void robotBulletToWorldCoordinates(GLfloat modelview[16]);
void animateRobotBullets();

// Player Functions
void shoot();
void drawBullets(GLfloat modelview[16]);
void deleteBullets();
void animateBullets();

void setBulletCoordinates(GLfloat modelview[16]);

/*********************
 *
 * RESTART GAME
 *
 *********************/

extern bool restart;
extern int enemiesKilled;


/*********************
 *
 * TEXTURE MAPPING
 *
 *********************/

extern GLuint texture[6];
void generateTexture();

/*********************
*
* SCENE SETUP
*
*********************/
void createTerrain();
void spawnTerrain();
void drawGround();
void setup();
void gameStateHandler();

/*********************
*
* DISPLAY FUNCTIONS
*
*********************/

void init3DSurfaceWindow();
void display3D();
void reshape3D(int w, int h);
void mouseButtonHandler3D(int button, int state, int x, int y);
void mouseMotionHandler3D(int xMouse, int yMouse);
void passiveMouseMotionHandler(int xMouse, int yMouse);
void keyboardHandler3D(unsigned char key, int x, int y);

/*********************
*
* ANIMATION FUNCTIONS
*
*********************/

void animationTimer(int param);
int* generateAnimationDelay(int setMax);

/*********************
*
* UTILITY FUNCTIONS
*
*********************/

Vector3D crossProduct(Vector3D a, Vector3D b);
float euclideanDistance(Vector3D a, Vector3D b);

/*********************
*
* COLLISION DETECTION
*
*********************/

void robotDamage(int id);
void checkPlayerBulletCollision();
void checkRobotBulletCollision();