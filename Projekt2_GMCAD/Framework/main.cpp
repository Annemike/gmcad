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
	Vec3f v1b = Vec3f(0.0f, 0.0f, 0.0f);
	Vec3f v2b = Vec3f(0.0f, 2.0f, 0.0f);
	Vec3f v3b = Vec3f(0.0f, 2.0f, 2.0f);
	std::vector<Vec3f> cpsb = { v1b,v2b,v3b };
	BezierCurve  bezier(cpsb);
	bezierCurves.push_back(bezier);

	Vec3f v1rb = Vec3f(1.0f, 0.0f, 1.0f);
	Vec3f v2rb = Vec3f(1.0f, 1.0f, 1.0f);
	Vec3f v3rb = Vec3f(0.0f, 2.0f, 2.0f);
	std::vector<Vec3f> cpsrb = { v1rb,v2rb,v3rb };
	BezierCurve rationalBezier(cpsrb, true);
	bezierCurves.push_back(rationalBezier);

	Vec3f v1bh = Vec3f(-1.0f, 0.0f, 1.0f);
	Vec3f v2bh = Vec3f(0.0f, 8.0f / 3.0f, 2.0f);
	Vec3f v3bh = Vec3f(1.0f, 0.0f, 1.0f);
	std::vector<Vec3f> cpsbh = { v1bh,v2bh,v3bh };
	BezierCurve bezierHyperbel(cpsbh, true);
	bezierCurves.push_back(bezierHyperbel);

	Vec3f v1g = Vec3f(0.0f, 0.0f, 0.0f);
	Vec3f v2g = Vec3f(1.0f, 0.0f, 0.2f);
	Vec3f v3g = Vec3f(1.0f, 1.0f, 0.4f);
	Vec3f v4g = Vec3f(0.0f, 1.0f, 0.6f);
	Vec3f v5g = Vec3f(0.0f, 0.0f, 0.8f);
	Vec3f v6g = Vec3f(0.5f, 0.0f, 1.0f);
	Vec3f v7g = Vec3f(0.5f, 0.5f, 0.0f);
	Vec3f v8g = Vec3f(0.5f, 0.5f, -1.0f);
	std::vector<Vec3f> cpsg = { v1g,v2g,v3g,v4g,v5g,v6g,v7g,v8g };
	BezierCurve  giganticBezier(cpsg);
	bezierCurves.push_back(giganticBezier);
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

	Vec4f v1h = Vec4f(-1.0f, 0.0f, 0.0f, 1.0f);
	Vec4f v2h = Vec4f(0.0f, 8.0f / 3.0f, 0.0f, 2.0f);
	Vec4f v3h = Vec4f(1.0f, 0.0f, 0.0f, 1.0f);
	std::vector<Vec4f> cpsh = { v1h,v2h,v3h };
	NURBSCurve hyperbel(cpsh, knots, 2);
	nurbsCurves.push_back(hyperbel);
	
	Vec4f v2hd = Vec4f(0.0f, 16.0f / 3.0f, 0.0f, 4.0f);
	std::vector<Vec4f> cpshd = { v1h,v2hd,v3h };
	NURBSCurve hyperbelDoubled(cpshd, knots, 2);
	nurbsCurves.push_back(hyperbelDoubled);

	Vec4f v2hq = Vec4f(0.0f, 2.0f / 3.0f, 0.0f, 0.5f);
	std::vector<Vec4f> cpshq = { v1h,v2hq,v3h };
	NURBSCurve hyperbelQuartered(cpshq, knots, 2);
	nurbsCurves.push_back(hyperbelQuartered);

	Vec4f v1r = Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	Vec4f v2r = Vec4f(1.0f, 0.0f, 0.1f, 1.0f);
	Vec4f v3r = Vec4f(1.0f, 1.0f, 0.2f, 1.0f);
	Vec4f v4r = Vec4f(-1.0f, 1.0f, 0.4f, 1.0f);
	Vec4f v5r = Vec4f(-1.0f, -1.0f, 0.6f, 1.0f);
	Vec4f v6r = Vec4f(1.0f, -1.0f, 0.8f, 1.0f);
	std::vector<Vec4f> cpsr = { v1r,v2r,v3r,v4r,v5r,v6r };
	std::vector<float> knotsa = { 0,0,0,1,2,3,4,4,4 };
	NURBSCurve random(cpsr, knotsa, 2);
	nurbsCurves.push_back(random);

	std::vector<float> knotss = { 0,0,0,1,1.5f,3,4,4,4 };
	NURBSCurve randomSmall(cpsr, knotss, 2);
	nurbsCurves.push_back(randomSmall);

	std::vector<float> knotsb = { 0,0,0,1,2.5f,3,4,4,4 };
	NURBSCurve randomBig(cpsr, knotsb, 2);
	nurbsCurves.push_back(randomBig);

	std::vector<float> knotsd = { 0,0,0,1,1,2,3,3,3 };
	NURBSCurve randomDouble(cpsr, knotsd, 2);
	nurbsCurves.push_back(randomDouble);

	/*Vec4f tan;
	std::cout << curve2.evaluteDeBoor(0.1f, tan) << std::endl << tan << std::endl;
	std::pair<std::vector<Vec4f>, std::vector<Vec4f>> res = curve2.evaluateCurve(11);
	for (int i = 0; i < 11; i++)
	{
		std::cout << std::get<0>(res)[i] << " tangiert durch " << std::get<1>(res)[i] << std::endl;
	}*/
	// angezeigte Objekte
	activeBezier = activeNURBS = 1337;
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
		if (i == activeBezier)
		{
			renderBezier(bezierCurves[i]);
			renderBezierEvaluation(bezierCurves[i], evalParameter);
		}
	}
	for (unsigned int i = 0; i < nurbsCurves.size(); ++i)
	{
		if (i == activeNURBS)
		{
			renderNURBS(nurbsCurves[i]);
			renderNURBSEvaluation(nurbsCurves[i], evalParameter);
		}
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
		if (activeBezier == 1337)
			break;
		if (evalParameter < 0.99f) {
			evalParameter += 0.01f;
			std::cout << "t= ";
			std::cout << evalParameter << std::endl;
		}
		glutPostRedisplay();
		break;
	case 'd':
	case 'D':
		if (activeBezier == 1337)
			break;
		if (evalParameter >= 0.01f) {

			evalParameter -= 0.01f;
			std::cout << "t= ";
			std::cout << evalParameter << std::endl;
		}
		glutPostRedisplay();
		break;
	case 'e':
	case 'E':
		if (activeBezier == 1337)
			break;
		int anzahl;
		std::cout << "Bitte geben Sie an in wieviele aquidistante Punkte sich die Bezier-Kurve aufteilen soll: ";
		std::cin >> anzahl;
		std::cout << "Evaluated Points: " << std::endl;
		for (int i = 0; i < bezierCurves[activeBezier].evaluateCurve(anzahl).first.size(); i++) {
			std::cout << bezierCurves[activeBezier].evaluateCurve(anzahl).first[i] << std::endl;
		}
		glutPostRedisplay();
		break;
	case 'a':
	case 'A':
		if (activeBezier == 1337)
			break;
		float stelle;
		std::cout << "Bitte geben Sie die Stelle im Intervall [0,1] an, an der der Punkt berechnet werden soll:" << std::endl;
		std::cin >> stelle;
		while (stelle < 0 || stelle > 1)
		{
			std::cout << "Bitte geben Sie eine Zahl zwischen 0 und 1 ein!" << std::endl;
			std::cin >> stelle;
		}
		std::cout << "An der Stelle " << stelle << " wird die Kurve zu " << bezierCurves[activeBezier].evaluateCurveAt(stelle, Vec3f()) << " ausgewertet." << std::endl;
		evalParameter = stelle;
		glutPostRedisplay();
		break;
	case 'b':
	case 'B':
		activeBezier = 0;
		activeNURBS = 1337;
		glutPostRedisplay();
		break;
	case 'q':
	case 'Q':
		activeBezier = 1;
		activeNURBS = 1337;
		glutPostRedisplay();
		break;
	case 'p':
	case 'P':
		activeBezier = 2;
		activeNURBS = 1337;
		glutPostRedisplay();
		break;
	case 'g':
	case 'G':
		activeBezier = 3;
		activeNURBS = 1337;
		glutPostRedisplay();
		break;
	case 'n':
	case 'N':
		activeBezier = 1337;
		activeNURBS = 4;
		glutPostRedisplay();
		break;
	case 'c':
	case 'C':
		activeBezier = 1337;
		activeNURBS = 0;
		glutPostRedisplay();
		break;
	case 'y':
	case 'Y':
		activeBezier = 1337;
		activeNURBS = 1;
		glutPostRedisplay();
		break;
	case 'w':
	case 'W':
		if (activeNURBS == 1)
		{
			activeNURBS = 2;
			std::cout << "Weight of middle control point doubled" << std::endl;
			glutPostRedisplay();
			break;
		}
		if (activeNURBS == 2)
		{
			activeNURBS = 3;
			std::cout << "Weight of middle control point quartered" << std::endl;
			glutPostRedisplay();
			break;
		}
		if (activeNURBS == 3)
		{
			activeNURBS = 1;
			std::cout << "Weight of middle control point normal" << std::endl;
			glutPostRedisplay();
			break;
		}
		break;
	case 'k':
	case 'K':
		if (activeNURBS == 4)
		{
			activeNURBS = 5;
			std::cout << "Knot decreased" << std::endl;
			glutPostRedisplay();
			break;
		}
		if (activeNURBS == 5)
		{
			activeNURBS = 6;
			std::cout << "Knot increased" << std::endl;
			glutPostRedisplay();
			break;
		}
		if (activeNURBS == 6)
		{
			activeNURBS = 7;
			std::cout << "Knot doubled" << std::endl;
			glutPostRedisplay();
			break;
		}
		if (activeNURBS == 7)
		{
			activeNURBS = 4;
			std::cout << "Knots equidistant" << std::endl;
			glutPostRedisplay();
			break;
		}
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
	std::cout << "B: show (B)ezier curve" << std::endl;
	std::cout << "G: show (G)igantic Bezier curve" << std::endl;
	std::cout << "Q: show Rational Bezier curve (Q)uartercircle" << std::endl;
	std::cout << "P: show Rational Bezier curve Hyperbel" << std::endl;
	std::cout << "I: (I)ncrease the value t (Bezier exclusive)" << std::endl;
	std::cout << "D: (D)ecrease the value t (Bezier exclusive)" << std::endl;
	std::cout << "E: (E)valuate aquidistant points (Bezier exclusive)" << std::endl;
	std::cout << "A: Evaluate point (A)t (Bezier exclusive)" << std::endl;
	std::cout << "N: show 'random' (N)URBS curve" << std::endl;
	std::cout << "K: modify (K)not vector of 'random' NURBS curve" << std::endl;
	std::cout << "C: show (C)ircle NURBS" << std::endl;
	std::cout << "Y: show Hyperbel NURBS" << std::endl;
	std::cout << "W: modify (W)eight of middle control point of Hyperbel NURBS" << std::endl;
	// TODO: update help text according to your changes
	// ================================================


	// ================================================
	std::cout << "==========================" << std::endl;
	std::cout << std::endl;
}