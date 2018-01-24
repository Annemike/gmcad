// ========================================================================= //
// Authors: Felix Schuwirth                                                  //
// mailto:felix.schuwirth@igd.fraunhofer.de                                  //
//                                                                           //
// GRIS - Graphisch Interaktive Systeme                                      //
// Technische Universit�t Darmstadt                                          //
// Fraunhoferstrasse 5                                                       //
// D-64283 Darmstadt, Germany                                                //
//                                                                           //
// Creation Date: 12.01.2017                                                 //
// Content: Simple and extendable openGL program offering                    //
//   * basic navigation and basic pipeline rendering                         //
// ========================================================================= //

#include "main.h"		// this header

#include <stdlib.h>		// standard library
#include <cmath>		// fmod
#include <stdio.h>		// cout
#include <iostream>		// cout
#include "RenderingSurface.h"

// ==============
// === BASICS ===
// ==============

int main(int argc, char** argv)
{
	// initialize openGL window
	glutInit(&argc, argv);
	glutInitWindowPosition(300, 200);
	glutInitWindowSize(600, 400);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("TU Darmstadt, GMCAD P3"); 
	// link functions to certain openGL events
	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshape);  
	glutMouseFunc(mousePressed);
	glutMotionFunc(mouseMoved);
	glutKeyboardFunc(keyPressed);
	// further initializations
	setDefaults();
	calculatePoints();
	initializeGL();
        Vec4f tanU = Vec4f(0.0f,0.0f,0.0f,1.0f);
        Vec4f tanV = Vec4f(0.0f,0.0f,0.0f,1.0f);
        std::cout << NURBSs[0].evaluteDeBoor(0.4f,0.6f,tanU,tanV) << std::endl;
        std::cout << tanU << std::endl << tanV << std::endl;
	// activate main loop
	glutMainLoop();
	return 0;
}

void setDefaults()
{
	// scene Information
	transX = -1.0f;
	transY = -1.0f;
	transZ = -4.0f;
	angleX = 0.0f;
	angleY = 0.0f;
	// mouse information
	mouseX = 0;
	mouseY = 0;
	mouseButton = 0;
	mouseSensitivy = 1.0f;	

	enableCtrl = true;
	enableEval = 0;
	nurbsSelect = 0;
	nrPoints = 30;
	u = 0.5f;
	v = 0.5f;
}

void initializeGL()
{
	// black screen
	glClearColor(0, 0, 0, 0);
	// enable depth buffer
	glEnable(GL_DEPTH_TEST);
	// Use Point Smoothing
	glEnable(GL_POINT_SMOOTH);
	glPointSize(10.0f);
	glLineWidth(2.0f);
	// set shading model
	glShadeModel(GL_SMOOTH);
	// set lighting (white light)
	GLfloat global_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };  
	GLfloat ambientLight[] =   { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat diffuseLight[] =   { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specularLight[] =  { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shininess = 0.9f * 128.0f;
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightf(GL_LIGHT0, GL_SHININESS, shininess);
	glEnable(GL_LIGHT0);
	GLfloat lp[] = { 10.0f, 20.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lp);
	// enable lighting by glColor
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	// key bindings => cout
	coutHelp();
}

void calculatePoints()
{
	NURBSs.clear();
	// objects (test surface via empty constructor)
	auto nurbs = NURBS_Surface();
	std::cout << nurbs << std::endl;
	NURBSs.emplace_back(nurbs);

	// TODO: create two NURBS surfaces with different degrees k >= 2
	// calculate the points and their normals
	// emplace the resulting NURBS, points and normals into the vectors
	// =====================================================
	// Example 1: Hat
	
	std::vector<std::vector<Vec4f>> controlPoints1;

	std::vector<Vec4f> pRow1;
	pRow1.push_back(Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
	pRow1.push_back(Vec4f(0.0f, 1.0f, 1.0f, 1.0f));
	pRow1.push_back(Vec4f(0.0f, 2.0f, 1.0f, 1.0f));
	pRow1.push_back(Vec4f(0.0f, 3.0f, 1.0f, 1.0f));
	pRow1.push_back(Vec4f(0.0f, 4.0f, 1.0f, 1.0f)* 2.0f);
	controlPoints1.push_back(pRow1);

	std::vector<Vec4f> pRow2;
	pRow2.push_back(Vec4f(1.0f, 0.0f, 1.0f, 1.0f));
	pRow2.push_back(Vec4f(1.0f, 1.0f, 0.0f, 1.0f));
	pRow2.push_back(Vec4f(1.0f, 2.0f, 0.0f, 1.0f));
	pRow2.push_back(Vec4f(1.0f, 3.0f, 0.0f, 1.0f));
	pRow2.push_back(Vec4f(1.0f, 4.0f, 1.0f, 1.0f)* 2.0f);
	controlPoints1.push_back(pRow2);

	std::vector<Vec4f> pRow3;
	pRow3.push_back(Vec4f(3.0f, 0.0f, 1.0f, 1.0f));
	pRow3.push_back(Vec4f(3.0f, 1.0f, 0.0f, 1.0f));
	pRow3.push_back(Vec4f(3.0f, 2.0f, 3.0f, 1.0f));
	pRow3.push_back(Vec4f(3.0f, 3.0f, 0.0f, 1.0f));
	pRow3.push_back(Vec4f(3.0f, 4.0f, 1.0f, 1.0f)* 2.0f);
	controlPoints1.push_back(pRow3);

	std::vector<Vec4f> pRow4;
	pRow4.push_back(Vec4f(4.0f, 0.0f, 1.0f, 1.0f));
	pRow4.push_back(Vec4f(4.0f, 1.0f, 0.0f, 1.0f));
	pRow4.push_back(Vec4f(4.0f, 2.0f, 0.0f, 1.0f));
	pRow4.push_back(Vec4f(4.0f, 3.0f, 0.0f, 1.0f));
	pRow4.push_back(Vec4f(4.0f, 4.0f, 1.0f, 1.0f)* 2.0f);
	controlPoints1.push_back(pRow4);

	std::vector<Vec4f> pRow5;
	pRow5.push_back(Vec4f(5.0f, 0.0f, 1.0f, 1.0f));
	pRow5.push_back(Vec4f(5.0f, 1.0f, 1.0f, 1.0f));
	pRow5.push_back(Vec4f(5.0f, 2.0f, 1.0f, 1.0f));
	pRow5.push_back(Vec4f(5.0f, 3.0f, 1.0f, 1.0f));
	pRow5.push_back(Vec4f(5.0f, 4.0f, 1.0f, 1.0f)* 2.0f);
	controlPoints1.push_back(pRow5);

	std::vector<float> knotVectorU1;
	knotVectorU1.push_back(0.0f);
	knotVectorU1.push_back(0.0f);
	knotVectorU1.push_back(0.0f);
	knotVectorU1.push_back(0.0f);
	knotVectorU1.push_back(0.0f);
	knotVectorU1.push_back(1.0f);
	knotVectorU1.push_back(1.0f);
	knotVectorU1.push_back(1.0f);
	knotVectorU1.push_back(1.0f);
	knotVectorU1.push_back(1.0f);

	std::vector<float> knotVectorV1;
	knotVectorV1.push_back(0.0f);
	knotVectorV1.push_back(0.0f);
	knotVectorV1.push_back(0.0f);
	knotVectorV1.push_back(0.0f);
	knotVectorV1.push_back(0.0f);
	knotVectorV1.push_back(1.0f);
	knotVectorV1.push_back(1.0f);
	knotVectorV1.push_back(1.0f);
	knotVectorV1.push_back(1.0f);
	knotVectorV1.push_back(1.0f);

	unsigned int degreeEx1 = 4;
	
	NURBSs.emplace_back(NURBS_Surface(controlPoints1, knotVectorU1, knotVectorV1, degreeEx1));
	// =====================================================

}

void reshape(GLint width, GLint height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, (float)width / (float)height, 0.1f, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// =================
// === RENDERING ===
// =================

void drawCS()
{
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	// red X
	glColor3f(1, 0, 0); 
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	// green Y
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	// blue Z
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);
	glEnd();
}

void drawObjects()
{
	if (NURBSs.empty() || nurbsSelect > NURBSs.size() || nurbsSelect < 0)
		return;
	NURBS_Surface &nurbs = NURBSs[nurbsSelect];	

	if (nurbs.controlPoints.size() > 1)
	{

		if(enableEval > 0)
			evaluateNURBSSurface(nurbs,u,v,enableEval == 1);
		if (enableCtrl)
			drawNURBSSurfaceCtrlP(nurbs);
		// TODO: draw nurbs surface
		// ========================
	

		// ========================
	}
}

void renderScene()
{
	// clear and set camera
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	// translate scene in viewing direction
	glTranslatef(transX, transY, transZ);  
	// rotate scene
	glRotatef(angleX, 0.0f, 1.0f, 0.0f);
	glRotatef(angleY, 1.0f, 0.0f, 0.0f);
	// draw coordinate system without lighting
	drawCS();
	drawObjects();
	// swap Buffers
	glFlush();
	glutSwapBuffers();
}

// =================
// === CALLBACKS ===
// =================

void keyPressed(unsigned char key, int x, int y)
{
	switch (key)
	{
		// esc => exit
	case 27:
		exit(0);
		break;
		// help file
	case 'h' :
	case 'H' :
		coutHelp();
		break;
		// reset view
	case 'r' :
	case 'R' :
		setDefaults();
		glutPostRedisplay();	// use this whenever 3d data changed to redraw the scene
		std::cout << "view reset\n";
		break;
	case 'c':
	case 'C':
		enableCtrl = !enableCtrl;
		glutPostRedisplay();
		std::cout << "Ctrl Polygon: "<< (enableCtrl ? "enabled" : "disabled")<<"\n";
		break;
	case 'e':
	case 'E':
		enableEval = (enableEval + 1) % 3;
		glutPostRedisplay();
		if (enableEval == 0)	std::cout << "Evaluation disabled\n";
		if (enableEval == 1)	std::cout << "Evaluation enabled ( v first)\n";
		if (enableEval == 2)	std::cout << "Evaluation enabled ( u first)\n";
	case 'a':
	case 'A':
		nurbsSelect = (nurbsSelect + 1) % NURBSs.size();
		glutPostRedisplay();
		break;
		// TODO: place custom functions on button events here to present your results
		// ==========================================================================


		// ==========================================================================
	}
}

void mousePressed(int button, int state, int x, int y)
{
	mouseButton = button;
	mouseX = x; 
	mouseY = y;
}

void mouseMoved(int x, int y)
{
	// rotate (cap angleY within [-85�, +85�])
	if (mouseButton == GLUT_LEFT_BUTTON)
	{
		angleX = fmod(angleX + (x-mouseX) * mouseSensitivy, 360.0f);
		angleY += (y-mouseY) * mouseSensitivy;
		if (angleY > 85) angleY = 85;
		if (angleY < -85) angleY = -85;
		glutPostRedisplay();
	}
	// zoom (here translation in z)
	if (mouseButton == GLUT_RIGHT_BUTTON)
	{
		transZ -= 0.2f * (y-mouseY) * mouseSensitivy;
		glutPostRedisplay();
	}
	// translation in xy
	if (mouseButton == GLUT_MIDDLE_BUTTON) 
	{
		transX += 0.2f * (x-mouseX) * mouseSensitivy;
		transY -= 0.2f * (y-mouseY) * mouseSensitivy;
		glutPostRedisplay();
	}
	// update mouse for next relative movement
	mouseX = x;
	mouseY = y;
}

// ===============
// === VARIOUS ===
// ===============

void coutHelp()
{
	std::cout << std::endl;
	std::cout << "====== KEY BINDINGS ======" << std::endl;
	std::cout << "ESC: exit" << std::endl;
	std::cout << "H: show this (H)elp file" << std::endl;
	std::cout << "R: (R)eset view" << std::endl;
	std::cout << "C: toggle surface (C)ontrol polygon" << std::endl;
	std::cout << "E: switch (E)valuation visualization (none,u-first,v-fist)" << std::endl;
	std::cout << "A: switch between NURBS surfaces" << std::endl;
	// TODO: update help text according to your changes
	// ================================================


	// ================================================
	std::cout << "==========================" << std::endl;
	std::cout << std::endl;
}
