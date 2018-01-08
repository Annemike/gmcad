// ========================================================================= //
// Authors: Felix Schuwirth                                                  //
// mailto:felix.schuwirth@igd.fraunhofer.de                                  //
//                                                                           //
// GRIS - Graphisch Interaktive Systeme                                      //
// Technische Universität Darmstadt                                          //
// Fraunhoferstrasse 5                                                       //
// D-64283 Darmstadt, Germany                                                //
//                                                                           //
// Creation Date: 06.12.2016                                                 //
// Content: Simple and extendable openGL program offering                    //
//   * basic navigation and basic pipeline rendering                         //
// ========================================================================= //

#include "main.h"		// this header
#include <list>
#include <stdlib.h>		// standard library
#include <cmath>		// fmod
#include <stdio.h>		// cout
#include <iostream>		// cout

#include <CurveRendering.h>

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
	glutCreateWindow("TU Darmstadt, FSchuwirth");
	// link functions to certain openGL events
	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshape);
	glutMouseFunc(mousePressed);
	glutMotionFunc(mouseMoved);
	glutKeyboardFunc(keyPressed);
	// further initializations
	setDefaults();
	createCurves();
	initializeGL();
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
	evalParameter = 0.5f;
}

void initializeGL()
{
	// black screen
	glClearColor(0, 0, 0, 0);
	// enable depth buffer
	glEnable(GL_DEPTH_TEST);
	// Use Point Smoothing
	glEnable(GL_POINT_SMOOTH);
	glPointSize(5.0f);
	glLineWidth(2.0f);
	// set shading model
	glShadeModel(GL_SMOOTH);
	// key bindings => cout
	coutHelp();
}

void createCurves()
{
	bezierCurves.clear();
	// TODO: create at least one bezier and one degree 2 quarter circle rational bezier curve
	// ==========================================================================
	float w = 1.0f;
	float x = 2.0f;
	float y = 3.0f;
	Vec3f v1 = Vec3f(0.0f, 0.0f, 0.0f);
	Vec3f v2 = Vec3f(0.0f, 2.0f, 0.0f);
	Vec3f v3 = Vec3f(0.0f, 2.0f, 2.0f);




	std::vector<Vec3f> cps = { v1,v2,v3 };
	BezierCurve  curve1(cps);

	bezierCurves.push_back(curve1);
	// ==========================================================================
	for (auto &b : bezierCurves)
		std::cout << b << std::endl;

	nurbsCurves.clear();
	// TODO: set values to describe a degree 2 quarter circle in first quadrant, XY-plane
	// ==================================================================================
	Vec4f v1qc = Vec4f(1.0f, 0.0f, 0.0f, 1.0f);
	Vec4f v2qc = Vec4f(1.0f, 1.0f, 0.0f, 1.0f);
	Vec4f v3qc = Vec4f(0.0f, 2.0f, 0.0f, 2.0f);
	std::vector<Vec4f> cpsqc = { v1qc,v2qc,v3qc };
	std::vector<float> knots = { 0,0,0,1,1,1 };
	NURBSCurve quarterCircle(cpsqc, knots, 2);
	nurbsCurves.push_back(quarterCircle);

	Vec4f v1h = Vec4f(-1.0f, 0.0f, -2.0f, 1.0f);
	Vec4f v2h = Vec4f(0.0f, 8.0f/3.0f, -4.0f, 2.0f);
	Vec4f v3h = Vec4f(1.0f, 0.0f, -2.0f, 1.0f);
	std::vector<Vec4f> cpsh = { v1h,v2h,v3h };
	NURBSCurve hyperbel(cpsh, knots, 2);
	nurbsCurves.push_back(hyperbel);
	/*Vec4f tan;
	std::cout << curve2.evaluteDeBoor(0.1f, tan) << std::endl << tan << std::endl;
	std::pair<std::vector<Vec4f>, std::vector<Vec4f>> res = curve2.evaluateCurve(11);
	for (int i = 0; i < 11; i++)
	{
		std::cout << std::get<0>(res)[i] << " tangiert durch " << std::get<1>(res)[i] << std::endl;
	}*/
	// ==================================================================================
	for (auto &n : nurbsCurves)
		std::cout << n << std::endl;
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
	glBegin(GL_LINES);
	// red X
	glColor3f(0.8f, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0.5, 0, 0);
	// green Y
	glColor3f(0, 0.8f, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0.5, 0);
	// blue Z
	glColor3f(0, 0, 0.8f);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 0.5);
	glEnd();
}

void drawObjects()
{
	for (unsigned int i = 0; i < bezierCurves.size(); ++i)
	{
		renderBezier(bezierCurves[i]);
		if (i == activeBezier)
			renderBezierEvaluation(bezierCurves[i], evalParameter);
	}
	for (unsigned int i = 0; i < nurbsCurves.size(); ++i)
	{
		renderNURBS(nurbsCurves[i]);
		if (i == activeNURBS)
			renderNURBSEvaluation(nurbsCurves[i], evalParameter);
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
	glDisable(GL_LIGHTING);
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
	case 'h':
	case 'H':
		coutHelp();
		break;
		// reset view
	case 'r':
	case 'R':
		setDefaults();
		glutPostRedisplay();	// use this whenever 3d data changed to redraw the scene
		break;
		// TODO: place custom functions on button events here to present your results
		// like changing the active Bbezier/NURBS curve (activeNURBS, activeBezier)
		// and varying the evaluation parameter (evalParameter) for the bezier curve
		// ==========================================================================
	case 'i':
	case 'I':
		if (evalParameter < 0.99f) {
			evalParameter += 0.01f;
			std::cout << "t= ";
			std::cout << evalParameter << std::endl;
		}
		glutPostRedisplay();
		break;
	case 'd':
	case 'D':

		if (evalParameter >= 0.01f) {

			evalParameter -= 0.01f;
			std::cout << "t= ";
			std::cout << evalParameter << std::endl;
		}
		glutPostRedisplay();
		break;
	case 'e':
	case 'E':
		int a;
		std::cout << "Bitte geben sie an in wieviele aquidistante Punkte sich die Bezier-Kurve aufteilen soll: ";
		std::cin >> a;
		std::cout << "Evaluated Points: " << std::endl;
		for (int i = 0; i < bezierCurves[0].evaluateCurve(a - 1).first.size(); i++) {
			std::cout << bezierCurves[0].evaluateCurve(a - 1).first[i] << std::endl;
		}
		glutPostRedisplay();
		break;
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
	// rotate (cap angleY within [-85°, +85°])
	if (mouseButton == GLUT_LEFT_BUTTON)
	{
		angleX = fmod(angleX + (x - mouseX) * mouseSensitivy, 360.0f);
		angleY += (y - mouseY) * mouseSensitivy;
		if (angleY > 85) angleY = 85;
		if (angleY < -85) angleY = -85;
		glutPostRedisplay();
	}
	// zoom (here translation in z)
	if (mouseButton == GLUT_RIGHT_BUTTON)
	{
		transZ -= 0.2f * (y - mouseY) * mouseSensitivy;
		glutPostRedisplay();
	}
	// translation in xy
	if (mouseButton == GLUT_MIDDLE_BUTTON)
	{
		transX += 0.2f * (x - mouseX) * mouseSensitivy;
		transY -= 0.2f * (y - mouseY) * mouseSensitivy;
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
	std::cout << "I: (I)ncrease the value t" << std::endl;
	std::cout << "D: (D)ecrease the value t" << std::endl;
	std::cout << "E: (E)valuate aquidistant points" << std::endl;
	// TODO: update help text according to your changes
	// ================================================


	// ================================================
	std::cout << "==========================" << std::endl;
	std::cout << std::endl;
}