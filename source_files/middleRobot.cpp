/*******************************************************************
		   Assignment 1 - Bot 2
********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <gl/glut.h>
#include <utility>
#include <vector>
#include "main.h"
using namespace std;

/************************************************************************************
 *
 *
 * MIDDLE ROBOT CLASS METHODS
 *
 *
 ************************************************************************************/

// MiddleRobot Constructor
// MiddleRobot must be given a (x, y, z) spawn coordinates
MiddleRobot::MiddleRobot(float x)
{
	// position.y never changes
	// MiddleRobot always stays above the ground when initialized
	position.x = x;
	position.y = groundY + robotBodyLength + 2;
	position.z = robotSpawnBoundary;

	spawn.x = x;
	spawn.y = groundY + robotBodyLength + 2;
	spawn.z = robotSpawnBoundary;
}

// Draw the MiddleRobot
void MiddleRobot::draw()
{
	GLfloat modelview[16];

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, texture[4]);

	if (health > 0) {

		glPushMatrix();

		// Translate robot to spawn location
		glTranslatef(position.x, position.y, position.z);

		// The angle the Robot shuffles
		glRotatef(shuffleAngle, 0.0, 1.0, 0.0);

		// Orient robot to face the player
		glRotatef(robotAngle, 0.0, 1.0, 0.0);

		drawBody();
		drawLeftHip();
		drawRightHip();

		// Get ModelView
		glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
		robotBulletToWorldCoordinates(modelview);

		glPopMatrix();
	}
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
}

void MiddleRobot::drawBody()
{
	GLfloat modelview[16];

	glPushMatrix();

	// Rotate Body
	glRotatef(bodyAngle, 0.0, 0.0, 1.0);
	// Create Body
	glPushMatrix();
	glScalef(robotBodyWidth, robotBodyLength, robotBodyDepth);
	gluSphere(gluNewQuadric(), 1.0f, 20, 20);
	glPopMatrix();

	// Hierarchy for Gun
	drawGun();

	// Get ModelView
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	robotBulletToWorldCoordinates(modelview);

	glPopMatrix();
}

void MiddleRobot::drawGun()
{
	GLfloat modelview[16];

	glPushMatrix();

	// Rotate Main Gun Cylinder
	glTranslatef(robotBodyWidth * 0.5 + 1.9, 0.0, 0.0);
	glRotatef(gunAngle, 1.0, 0.0, 0.0);
	glTranslatef(-(robotBodyWidth * 0.5 + 1.9), 0.0, 0.0);

	glPushMatrix();

	// translate gun to position
	glTranslatef(robotBodyWidth * 0.5 + 1.9, 0.0, 0.0);

	// build the gun
	glPushMatrix();
	glScalef(gunWidth, gunLength, gunDepth);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	gluCylinder(gluNewQuadric(), 0.1f, 0.1f, 3.0f, 32, 32);
	glPopMatrix();

	// Shoot Bullets From Robot
	glPushMatrix();

	glRotatef(-90.0f, 0, 1, 0);
	drawRobotBullets();

	// Get ModelView
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	robotBulletToWorldCoordinates(modelview);

	glPopMatrix();

	// Get ModelView
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	robotBulletToWorldCoordinates(modelview);

	glPopMatrix();

	// Hierarchy for Gun Design
	drawGunDesign();

	// Get ModelView
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	robotBulletToWorldCoordinates(modelview);

	glPopMatrix();
}

void MiddleRobot::drawGunDesign()
{
	// Gun Top Design
	glPushMatrix();

	// Rotate Top Gun Cylinder
	glTranslatef(gunLength * 0.6, 0.0, 0.0);
	glRotatef(gunAngle, 1.0, 0.0, 0.0);
	glTranslatef(-gunLength * 0.6, 0.0, 0.0);

	// Position Top Gun Cylinder
	glTranslatef(gunLength * 0.6, gunWidth, 0.0);

	// Build Top Gun Cylinder
	glScalef(gunLength * 0.3, gunWidth * 0.2, gunDepth * 0.3);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(gluNewQuadric(), 0.1f, 0.1f, 3.0f, 32, 32);

	glPopMatrix();

	// Gun Bottom Design
	glPushMatrix();

	// Rotate Bottom Gun Cylinder
	glTranslatef(gunLength * 0.6, 0.0, 0.0);
	glRotatef(gunAngle, 1.0, 0.0, 0.0);
	glTranslatef(-gunLength * 0.6, 0.0, 0.0);

	// Build Bottom Gun Cylinder
	glTranslatef(gunLength * 0.6, -gunWidth, 0.0);
	glScalef(gunLength * 0.3, gunWidth * 0.2, gunDepth * 0.3);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(gluNewQuadric(), 0.1f, 0.1f, 3.0f, 32, 32);

	glPopMatrix();

}

void MiddleRobot::drawLeftHip()
{
	// Begin Left Hip Cylinder
	glPushMatrix();

	// Rotate Hip
	glTranslatef(0.0, 0.0, robotBodyDepth);
	glRotatef(hipAngle, 0.0, 0.0, 1.0);
	glRotatef(leftHipAngle, 0.0, 0.0, 1.0);
	glTranslatef(0.0, 0.0, -robotBodyDepth);

	glPushMatrix();

	// Position hip with respect to parent body
	glTranslatef(0.0, 0.0, robotBodyDepth);

	// Build Left Hip Cylinder 
	glPushMatrix();
	glScalef(hipWidth, hipLength, hipDepth);
	gluCylinder(gluNewQuadric(), 0.1f, 0.1f, 3.0f, 32, 32);
	glPopMatrix();

	glPopMatrix();

	// Begin Outside Disk
	glPushMatrix();

	// Position the outside disk with respect to the parent body
	glTranslatef(0.0, 0.0, robotBodyWidth + hipDepth * 3);

	// Build Outside Disk
	glPushMatrix();
	glScalef(hipWidth, hipLength, 0.0);
	gluDisk(gluNewQuadric(), 0, 0.1, 30, 1);
	glPopMatrix();

	glPopMatrix();


	// Begin Inside Disk
	glPushMatrix();

	// Position the inside disk with respect to the parent body
	glTranslatef(0.0, 0.0, robotBodyWidth);

	// Build Inside Disk
	glPushMatrix();
	glScalef(hipWidth, hipLength, 0.0);
	gluDisk(gluNewQuadric(), 0, 0.1, 30, 1);
	glPopMatrix();

	glPopMatrix();

	// Hierachy
	drawLeftKnee();

	glPopMatrix();
}

void MiddleRobot::drawRightHip()
{
	// Begin Left Arm Cylinder
	glPushMatrix();

	// Rotate Hip
	glRotatef(hipAngle, 0.0, 0.0, 1.0);
	glRotatef(rightHipAngle, 0.0, 0.0, 1.0);

	glPushMatrix();

	// Position arm with respect to parent body
	glTranslatef(0.0, 0.0, -robotBodyDepth);

	// Build Left Arm Cylinder 
	glPushMatrix();
	glScalef(hipWidth, hipLength, hipDepth);
	glRotatef(180.0f, 0.0, 1.0f, 0.0);
	gluCylinder(gluNewQuadric(), 0.1f, 0.1f, 3.0f, 32, 32);
	glPopMatrix();

	glPopMatrix();

	// Begin Outside Disk
	glPushMatrix();

	// Position the outside disk with respect to the parent body
	glTranslatef(0.0, 0.0, -(robotBodyWidth + hipDepth * 3));

	// Build Outside Disk
	glPushMatrix();
	glScalef(hipWidth, hipLength, 0.0);
	gluDisk(gluNewQuadric(), 0, 0.1, 30, 1);
	glPopMatrix();

	glPopMatrix();

	// Begin Inside Disk
	glPushMatrix();

	// Position the inside disk with respect to the parent body
	glTranslatef(0.0, 0.0, -robotBodyWidth);

	// Build Inside Disk
	glPushMatrix();
	glScalef(hipWidth, hipLength, 0.0);
	gluDisk(gluNewQuadric(), 0, 0.1, 30, 1);
	glPopMatrix();

	glPopMatrix();

	// Draw Right Knee
	drawRightKnee();

	glPopMatrix();

}

void MiddleRobot::drawLeftKnee()
{
	glPushMatrix();

	// Rotate Knee
	glRotatef(kneeAngle, 0.0, 0.0, 1.0);
	glRotatef(leftKneeAngle, 0.0, 0.0, 1.0);

	// Begin Upper Knee
	glPushMatrix();

	// Move it relative to left hip joint
	glTranslatef(-hipDepth + 0.5, -(hipDepth + 0.4), robotBodyDepth + hipDepth + 0.4);
	glRotatef(-45.0f, 0.0, 0.0, 1.0);

	// Create Upper Knee
	glPushMatrix();
	glScalef(kneeWidth, kneeLength * 0.4, kneeDepth);
	glutSolidCube(1.0f);
	glPopMatrix();

	glPopMatrix();

	// Begin Lower Knee
	glPushMatrix();

	// Move it relative to left hip joint
	glTranslatef(0.0, -(robotBodyDepth + hipDepth + 0.4), robotBodyDepth + hipDepth + 0.4);
	glRotatef(45.0f, 0.0, 0.0, 1.0);

	// Create Left Knee
	glPushMatrix();
	glScalef(kneeWidth, kneeLength, kneeDepth);
	glutSolidCube(1.0f);
	glPopMatrix();

	glPopMatrix();

	// Hierarchy for Left Foot
	drawLeftFoot();

	glPopMatrix();
}

void MiddleRobot::drawRightKnee()
{
	glPushMatrix();

	// Rotate Knee
	glRotatef(kneeAngle, 0.0, 0.0, 1.0);
	glRotatef(rightKneeAngle, 0.0, 0.0, 1.0);

	// Begin Upper Knee
	glPushMatrix();

	// Move it relative to left hip joint
	glTranslatef(-hipDepth + 0.5, -(hipDepth + 0.4), -(robotBodyDepth + hipDepth + 0.4));
	glRotatef(-45.0f, 0.0, 0.0, 1.0);

	// Create Upper Knee
	glPushMatrix();

	glScalef(kneeWidth, kneeLength * 0.4, kneeDepth);
	glutSolidCube(1.0f);

	glPopMatrix();

	glPopMatrix();

	// Begin Lower Knee
	glPushMatrix();

	// Move it relative to left hip joint
	glTranslatef(0.0, -(robotBodyDepth + hipDepth + 0.4), -(robotBodyDepth + hipDepth + 0.4));
	glRotatef(45.0f, 0.0, 0.0, 1.0);

	// Create Lower Knee
	glPushMatrix();
	glScalef(kneeWidth, kneeLength, kneeDepth);
	glutSolidCube(1.0f);
	glPopMatrix();

	glPopMatrix();

	// Hierarchy for Right Foot
	drawRightFoot();

	glPopMatrix();

}

void MiddleRobot::drawLeftFoot()
{
	glPushMatrix();

	// Rotation
	glRotatef(footAngle, 0.0, 0.0, 1.0);
	glRotatef(leftFootAngle, 0.0, 0.0, 1.0);

	// Cylinder and Sphere
	glPushMatrix();

	// Move Cylinder AND Sphere
	glTranslatef(kneeWidth + kneeWidth / 2, -kneeLength * 1.4, kneeDepth * 2 + 1.2f);

	// Create foot cylinder
	glPushMatrix();
	glScalef(footWidth, footLength, footDepth);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(gluNewQuadric(), 0.1f, 0.1f, 3.0f, 32, 32);
	glPopMatrix();

	// Create foot sphere
	glPushMatrix();
	glScalef(footWidth * 0.08, footWidth * 0.08, footWidth * 0.08);
	gluSphere(gluNewQuadric(), 1.0f, 20, 20);
	glPopMatrix();

	glPopMatrix();

	// Ankle
	glPushMatrix();

	// Move Ankle
	glTranslatef(kneeWidth + kneeWidth / 2, -kneeLength - 1.0f, kneeDepth * 2 + 1.2f);

	// Create ankle
	glPushMatrix();
	glScalef(footWidth / 15, footWidth / 6, footWidth / 15);
	glutSolidCube(1.0f);
	glPopMatrix();

	glPopMatrix();

	// Top Disk
	glPushMatrix();
	glTranslatef(kneeWidth + kneeWidth / 2, -kneeLength * 1.4, kneeDepth * 2 + 1.2f);
	glRotatef(90.0f, 1.0, 0.0, 0.0);

	glPushMatrix();
	glScalef(footWidth, footDepth, 0.0);
	gluDisk(gluNewQuadric(), 0, 0.1, 30, 1);
	glPopMatrix();

	glPopMatrix();

	// Bottom Disk
	glPushMatrix();

	glTranslatef(kneeWidth + kneeWidth / 2, -kneeLength * 1.6, kneeDepth * 2 + 1.2f);
	glRotatef(90.0f, 1.0, 0.0, 0.0);

	glPushMatrix();
	glScalef(footWidth, footDepth, 0.0);
	gluDisk(gluNewQuadric(), 0, 0.1, 30, 1);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}

void MiddleRobot::drawRightFoot()
{
	// Rotation
	glPushMatrix();

	glRotatef(footAngle, 0.0, 0.0, 1.0);
	glRotatef(rightFootAngle, 0.0, 0.0, 1.0);

	// Cylinder and Sphere
	glPushMatrix();

	// Move Cylinder AND Sphere
	glTranslatef(kneeWidth + kneeWidth / 2, -kneeLength * 1.4, -(kneeDepth * 2 + 1.2f));

	// Create foot cylinder
	glPushMatrix();
	glScalef(footWidth, footLength, footDepth);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(gluNewQuadric(), 0.1f, 0.1f, 3.0f, 32, 32);
	glPopMatrix();

	// Create foot sphere
	glPushMatrix();
	glScalef(footWidth * 0.08, footWidth * 0.08, footWidth * 0.08);
	gluSphere(gluNewQuadric(), 1.0f, 20, 20);
	glPopMatrix();

	glPopMatrix();

	// Ankle
	glPushMatrix();

	// Move Ankle
	glTranslatef(kneeWidth + kneeWidth / 2, -kneeLength - 1.0f, -(kneeDepth * 2 + 1.2f));

	// Create ankle
	glPushMatrix();
	glScalef(footWidth / 15, footWidth / 6, footWidth / 15);
	glutSolidCube(1.0f);
	glPopMatrix();

	glPopMatrix();

	// Top Disk
	glPushMatrix();
	glTranslatef(kneeWidth + kneeWidth / 2, -kneeLength * 1.4, -(kneeDepth * 2 + 1.2f));
	glRotatef(90.0f, 1.0, 0.0, 0.0);

	glPushMatrix();
	glScalef(footWidth, footDepth, 0.0);
	gluDisk(gluNewQuadric(), 0, 0.1, 30, 1);
	glPopMatrix();

	glPopMatrix();

	// Bottom Disk
	glPushMatrix();
	glTranslatef(kneeWidth + kneeWidth / 2, -kneeLength * 1.6, -(kneeDepth * 2 + 1.2f));
	glRotatef(90.0f, 1.0, 0.0, 0.0);

	glPushMatrix();
	glScalef(footWidth, footDepth, 0.0);
	gluDisk(gluNewQuadric(), 0, 0.1, 30, 1);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}

// All MiddleRobot Animations Here
void MiddleRobot::animate(int delay)
{
	// Random Delay to Offset Animations
	currTime = glutGet(GLUT_ELAPSED_TIME);
	
	float deltaTime = currTime - oldTime;
	
	if (deltaTime > delay)
	{
		startAnimation = true;
	}

	if (startAnimation) {
		// Everything that happens when MiddleRobot walks
		walkAnimation();
		headAnimation();
		gunAnimation();
		shuffleAnimation();
	}

	// Respawning Conditionals
	respawn();
}

// MiddleRobot Walking Animation
void MiddleRobot::walkAnimation()
{
	static float hipIncr = 3.0 / angleSensitivity;
	static float kneeIncr = 5.0 / angleSensitivity;

	leftHipAngle += hipIncr;
	leftKneeAngle += kneeIncr;

	rightHipAngle -= hipIncr;
	rightKneeAngle -= kneeIncr;

	// When done taking left step forward
	// Begin taking a right step
	if (leftHipAngle > maxHipAngle && leftKneeAngle > maxKneeAngle)
	{
		hipIncr = -(3.0 / angleSensitivity);
		kneeIncr = -(5.0 / angleSensitivity);
	}

	// When done taking right step forward
	// Begin taking a left step
	if (leftHipAngle < -maxHipAngle && leftKneeAngle < -maxKneeAngle)
	{
		hipIncr = 3.0 / angleSensitivity;
		kneeIncr = 5.0 / angleSensitivity;
	}

	position.z += 0.05;
}

// MiddleRobot should never go past the player
// Respawn if health = 0
void MiddleRobot::respawn()
{
	int* delaysPtr = generateAnimationDelay(50);
	float deltaTime = currTime - oldTime;

	// MiddleRobot has passed the Player
	// Delay the respawn
	if ((position.z > worldBoundary) && (deltaTime > *delaysPtr + 2))
	{
		position.x = spawn.x;
		position.y = spawn.y;
		position.z = spawn.z;
		this->health = 3;
	}

	if ((health == 0) && (deltaTime > *delaysPtr + 2))
	{
		position.x = spawn.x;
		position.y = spawn.y;
		position.z = spawn.z;
		this->health = 3;

		// Number of enemies killed increases
		enemiesKilled += 1;
	}
}

// Gun spinning animation
// Guns should shoot projectiles at canon position
void MiddleRobot::gunAnimation()
{

	if (!stopGunBool)
	{
		gunAngle += 2.0;
	}

}

// Bobs MiddleRobot head up and down
void MiddleRobot::headAnimation()
{
	int maxHeadAngle = 20.0;
	static float increment = 0.1;

	bodyAngle += increment;

	if (bodyAngle > maxHeadAngle) {
		increment = -0.1;
	}
	if (bodyAngle < -maxHeadAngle) {
		increment = 0.1;
	}

}

void MiddleRobot::shuffleAnimation()
{
	int maxRobotAngle = 35.0;
	static float increment = 0.1;

	shuffleAngle += increment;

	if (shuffleAngle > maxRobotAngle) {
		increment = -0.1;
	}
	if (shuffleAngle < -maxRobotAngle) {
		increment = 0.1;
	}
}

// MiddleRobot Deconstructor
MiddleRobot::~MiddleRobot()
{
}