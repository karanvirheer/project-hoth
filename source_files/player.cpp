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

// Lighting/shading and material properties for robot - upcoming lecture - just copy for now
// Robot RGBA material properties (NOTE: we will learn about this later in the semester)
GLfloat playerGun_mat_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat playerGun_mat_specular[] = { 0.45f,0.55f,0.45f,1.0f };
GLfloat playerGun_mat_diffuse[] = { 0.1f,0.35f,0.1f,1.0f };
GLfloat playerGun_mat_shininess[] = { 32.0F };


/************************************************************************************
 *
 *
 * PLAYER CLASS METHODS
 *
 *
 ************************************************************************************/

void Player::draw()
{
	GLfloat modelview[16];

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	glPushMatrix();

	// TRANSLATE GUN
	glTranslatef(0.0, 0.0, groundZ - 4);

	// Vertical Rotation
	glRotatef(pitchAngle, 1, 0, 0);

	// Horizontal Rotation
	glRotatef(yawAngle, 0, 1, 0);

	if (health > 0)
	{
		// DRAW AND ORIENT GUN
		glPushMatrix();

		// Make gun face robots
		glRotatef(-90, 1, 0, 0);

		glScalef(gunWidth, gunLength, gunDepth);

		// Draw Gun Mesh From Arrays
		for (int i = 0; i < iSize; i += 4)
		{
			int one = indices[i] * 3;
			int two = indices[i + 1] * 3;
			int three = indices[i + 2] * 3;
			int four = indices[i + 3] * 3;


			glBegin(GL_QUADS);
			glNormal3f(normals[one], normals[one + 1], normals[one + 2]);
			glVertex3f(vertices[one], vertices[one + 1], vertices[one + 2]);

			glNormal3f(normals[two], normals[two + 1], normals[two + 2]);
			glVertex3f(vertices[two], vertices[two + 1], vertices[two + 2]);

			glNormal3f(normals[three], normals[three + 1], normals[three + 2]);
			glVertex3f(vertices[three], vertices[three + 1], vertices[three + 2]);

			glNormal3f(normals[four], normals[four + 1], normals[four + 2]);
			glVertex3f(vertices[four], vertices[four + 1], vertices[four + 2]);

			glEnd();
		}

		glPopMatrix();
	}
	else {

		// DRAW AND ORIENT GUN
		glPushMatrix();

		glScalef(gunWidth, gunLength, gunDepth);

		// Draw Gun Mesh From Arrays
		for (int i = 0; i < iSize; i += 4)
		{
			int one = indices[i] * 3;
			int two = indices[i + 1] * 3;
			int three = indices[i + 2] * 3;
			int four = indices[i + 3] * 3;


			glBegin(GL_QUADS);
			glNormal3f(normals[one], normals[one + 1], normals[one + 2]);
			glVertex3f(vertices[one], vertices[one + 1], vertices[one + 2]);

			glNormal3f(normals[two], normals[two + 1], normals[two + 2]);
			glVertex3f(vertices[two], vertices[two + 1], vertices[two + 2]);

			glNormal3f(normals[three], normals[three + 1], normals[three + 2]);
			glVertex3f(vertices[three], vertices[three + 1], vertices[three + 2]);

			glNormal3f(normals[four], normals[four + 1], normals[four + 2]);
			glVertex3f(vertices[four], vertices[four + 1], vertices[four + 2]);

			glEnd();
		}

		glPopMatrix();
	}

	// Move Camera By Getting View Vector of Cylinder
	// Convert Bullet local coordinate system to Gun/Robot coordinate system for collision detection
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

	drawBullets(modelview);

	setViewX(modelview[2]);
	setViewY(modelview[6]);
	setViewZ(modelview[10]);

	glPopMatrix();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
}

/*
	Class Constructor and Deconstructor
*/

// Constructor
Player::Player()
{
	position.x = 0.0f;
	position.y = 0.0f;
	position.z = (float)(groundZ - 4);

}

// Deconstructor
Player::~Player()
{

}

/*********************
 *
 * VIEW VECTOR
 *
 *********************/

float Player::getViewX()
{
	return viewVector.x;
}

void Player::setViewX(float newXCoord)
{
	viewVector.x = newXCoord;
}

float Player::getViewY()
{
	return viewVector.y;
}

void Player::setViewY(float newYCoord)
{
	viewVector.y = newYCoord;
}

float Player::getViewZ()
{
	return viewVector.z;
}

void Player::setViewZ(float newZCoord)
{
	viewVector.z = newZCoord;
}

/*********************
 *
 * GETTERS / SETTERS
 *
 *********************/

float Player::getXCoord()
{
	return position.x;
};

void Player::setXCoord(float newXCoord)
{
	position.x = newXCoord;
};

float Player::getYCoord()
{
	return position.y;
};

void Player::setYCoord(float newYCoord)
{
	position.y = newYCoord;
};

float Player::getZCoord()
{
	return position.z;
};

void Player::setZCoord(float newZCoord)
{
	position.z = newZCoord;
};

float Player::getYawAngle()
{
	return yawAngle;
};

void Player::setYawAngle(float newYawAngle)
{
	yawAngle = newYawAngle;
};

float Player::getPitchAngle()
{
	return pitchAngle;
};

void Player::setPitchAngle(float newPitchAngle)
{
	pitchAngle = newPitchAngle;
};

/*********************
 *
 * MESH FUNCTIONS
 *
 *********************/

// Allocates space for Vertice, Indices and Normals Arrays
void Player::createMesh()
{
	//--------------------------------------------------- 
	// ------------- FINDING ARRAY SIZES ---------------
	//--------------------------------------------------- 

	fstream myFile;

	// opening and reading the OBJ file
	myFile.open("mesh.obj", ios::in);


	if (!myFile)
	{
		exit(1);
	}

	if (myFile.is_open())
	{

		// Get Sizes of Arrays
		string line;
		while (getline(myFile, line))
		{
			// vertices
			if (line.substr(0, 2) == "v ")
			{
				vSize += 1;
				vSize += 1;
				vSize += 1;
			}

			// normals
			else if (line.substr(0, 3) == "vn ")
			{
				nSize += 1;
				nSize += 1;
				nSize += 1;
			}

			// indices
			else if (line.substr(0, 2) == "f ")
			{
				iSize += 1;
				iSize += 1;
				iSize += 1;
				iSize += 1;
			}

		}

		myFile.close();
	}


	//--------------------------------------------------- 
	// ------------- MALLOC ARRAYS ---------------
	//--------------------------------------------------- 

	vertices = (float*)malloc(sizeof(float) * vSize);
	normals = (float*)malloc(sizeof(float) * nSize);
	indices = (int*)malloc(sizeof(int) * iSize);

	//--------------------------------------------------- 
	// ------------- READING OBJ FILE ---------------
	//--------------------------------------------------- 

	loadMesh();
}

/************************************************************************************
 *
 *
 * Mesh Loader Functions
 *
 *
 ************************************************************************************/

 // Fills the Vertice, Indices and Normals Arrays
void Player::loadMesh()
{
	fstream otherFile;

	int vIndex = 0;
	int nIndex = 0;
	int iIndex = 0;

	string type, v1, v2, v3, v4;

	// opening reading the mesh.obj file
	otherFile.open("mesh.obj", ios::in);

	if (!otherFile)
	{
		exit(1);
	}

	if (otherFile.is_open())
	{
		string line;
		while (getline(otherFile, line))
		{
			stringstream linestream(line);

			// vertices
			if (line.substr(0, 2) == "v ")
			{
				while (linestream >> type >> v1 >> v2 >> v3)
				{
					vertices[vIndex] = stof(v1);
					vertices[vIndex + 1] = stof(v2);
					vertices[vIndex + 2] = stof(v3);
					vIndex += 3;
				}
			}

			// normals
			else if (line.substr(0, 3) == "vn ")
			{
				while (linestream >> type >> v1 >> v2 >> v3)
				{
					normals[nIndex] = stof(v1);
					normals[nIndex + 1] = stof(v2);
					normals[nIndex + 2] = stof(v3);
					nIndex += 3;
				}
			}

			// indices
			else if (line.substr(0, 2) == "f ")
			{
				while (linestream >> type >> v1 >> v2 >> v3 >> v4)
				{
					indices[iIndex] = stoi(v1);
					indices[iIndex + 1] = stoi(v2);
					indices[iIndex + 2] = stoi(v3);
					indices[iIndex + 3] = stoi(v4);
					iIndex += 4;
				}
			}
		}

		otherFile.close();
	}
}