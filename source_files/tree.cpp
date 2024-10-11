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
#include <random>
using namespace std;

/************************************************************************************
 *
 *
 * TREE CLASS METHODS
 *
 *
 ************************************************************************************/

Tree::Tree()
{
	position.x = (int)(-groundX) + (rand() % (int)(groundX - (-groundX) + 1));
	position.y = -groundY;
	position.z = (int)(-groundZ) + (rand() % (int)(robotSpawnBoundary - (-groundZ) + 1));
}

Tree::~Tree()
{

}

void Tree::draw()
{
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, texture[5]);

	glPushMatrix();
	drawStump();
	glPopMatrix();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

}

void Tree::drawStump()
{
	glPopMatrix();

		glPushMatrix();
	
		glTranslatef(position.x, position.y, position.z);

			glPushMatrix();
			glScalef(treeWidth, treeLength, treeDepth);
			glRotatef(-90, 1, 0, 0);
			gluCylinder(gluNewQuadric(), 0.1f, 0.1f, 3.0f, 32, 32);
			glPopMatrix();

		glPopMatrix();

	// Hierarchy
	drawLeaves();
	glPopMatrix();
}

void Tree::drawLeaves()
{
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, texture[3]);

	glPushMatrix();
	glTranslatef(position.x, position.y + treeLength*3, position.z);
		glPushMatrix();
		glScalef(leafWidth, leafLength, leafDepth);
		gluSphere(gluNewQuadric(), 1.0f, 20, 20);
		glPopMatrix();
	glPopMatrix();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
}