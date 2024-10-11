#include <stdio.h>
#include <windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "main.h"
#include <math.h>
#include <string.h>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

// Constructor
Bullet::Bullet()
{
	position.x = 0.0f;
	position.y = 0.0f;
	position.z = 0.0f;

	worldPosition.x = 0.0f;
	worldPosition.y = 0.0f;
	worldPosition.z = (float)(groundZ - 4.0f);

	worldBoundaryNegZ = -(groundZ + 5);
	worldBoundaryPosZ = groundZ + 5;

	worldBoundaryNegX = -(groundX);
	worldBoundaryPosX = groundX;

	worldBoundaryPosY = 100.0f;

	//worldBoundaryNegY = -(groundY + 40.0f);
	//worldBoundaryPosY = groundY + 40.0f;
}

// Deconstructor
Bullet::~Bullet()
{

}

/************************************************************************************
 *
 *
 * DRAWING BULLETS
 *
 *
 ************************************************************************************/

void Bullet::draw()
{

	// Stops drawing bullet if it collides with something
	if (!collision)
	{
		glPushMatrix();

		glTranslatef(position.x, position.y, position.z);

			glPushMatrix();

			glScalef(bulletWidth, bulletLength, bulletDepth);
			gluCylinder(gluNewQuadric(), 0.1f, 0.1f, 3.0f, 32, 32);

			glPopMatrix();

		glPopMatrix();
	}
}

void Bullet::animate()
{
	// Stops drawing bullet if it collides with something
	if (!collision)
	{
		// make a velocity variable?
		position.z -= 3.0f;
		worldPosition.z += 3.0f;
	}
}

// Convert Bullet Local Coordinates to Gun Global Coordinates
// Necessary for Bullet and Robot Collision Detection
void Bullet::getGunCoordinates(GLfloat modelview[16])
{
	worldPosition.x = modelview[0] * worldPosition.x + modelview[4] * worldPosition.y + modelview[8] * worldPosition.z + modelview[12];
	worldPosition.y = modelview[1] * worldPosition.x + modelview[5] * worldPosition.y + modelview[9] * worldPosition.z + modelview[13];
	worldPosition.z = modelview[2] * worldPosition.x + modelview[6] * worldPosition.y + modelview[10] * worldPosition.z + modelview[14];
}

/*********************
 *
 * GETTERS / SETTERS POSITION
 *
 *********************/

float Bullet::getXCoord()
{
	return position.x;
};

void Bullet::setXCoord(float newXCoord)
{
	position.x = newXCoord;
};

float Bullet::getYCoord()
{
	return position.y;
};

void Bullet::setYCoord(float newYCoord)
{
	position.y = newYCoord;
};

float Bullet::getZCoord()
{
	return position.z;
};

void Bullet::setZCoord(float newZCoord)
{
	position.z = newZCoord;
};


/*********************
 *
 * GETTERS / SETTERS PITCH AND YAW
 *
 *********************/

float Bullet::getYawAngle()
{
	return yawAngle;
};

void Bullet::setYawAngle(float newYawAngle)
{
	yawAngle = newYawAngle;
};

float Bullet::getPitchAngle()
{
	return pitchAngle;
};

void Bullet::setPitchAngle(float newPitchAngle)
{
	pitchAngle = newPitchAngle;
};

/*********************
 *
 * GETTERS / SETTERS WORLD POSITION
 *
 *********************/

float Bullet::getWorldX()
{
	return worldPosition.x;
};

void Bullet::setWorldX(float newXCoord)
{
	worldPosition.x = newXCoord;
};

float Bullet::getWorldY()
{
	return worldPosition.y;
};

void Bullet::setWorldY(float newYCoord)
{
	worldPosition.y = newYCoord;
};

float Bullet::getWorldZ()
{
	return worldPosition.z;
};

void Bullet::setWorldZ(float newZCoord)
{
	worldPosition.z = newZCoord;
};
