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
        for (NURBS_Surface nurbs : NURBSs) {
            std::cout << nurbs << std::endl;
        }
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

	enableCtrl = true;
	enableEval = 0;
        enablePoints = false;
        enableNormals = false;
        enableSurface = false;
        enableWire = false;
        triangles = false;
        nurbsmade = false;
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
}

void calculatePoints()
{
        points.clear();
        normals.clear();
	// objects (test surface via empty constructor)
        if (!nurbsmade) {
            NURBSs.clear();
            auto nurbs = NURBS_Surface();
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
            
            NURBSs.push_back(NURBS_Surface(controlPoints1, knotVectorU1, knotVectorV1, degreeEx1));
        
            std::vector<std::vector<Vec4f>> controlPoints2;

            std::vector<Vec4f> pRow11;
            pRow11.push_back(Vec4f(1.0f, 1.0f, 5.0f, 1.0f));
            pRow11.push_back(Vec4f(0.0f, 2.0f, 4.0f, 1.0f));
            pRow11.push_back(Vec4f(0.0f, 2.0f, 3.0f, 1.0f));
            pRow11.push_back(Vec4f(2.0f, 0.0f, 2.0f, 1.0f));
            pRow11.push_back(Vec4f(2.0f, 0.0f, 1.0f, 1.0f));
            pRow11.push_back(Vec4f(1.0f, 1.0f, 0.0f, 1.0f)* 2.0f);
            controlPoints2.push_back(pRow11);

            std::vector<Vec4f> pRow12;
            pRow12.push_back(Vec4f(1.0f, 3.0f, 5.0f, 1.0f));
            pRow12.push_back(Vec4f(0.0f, 3.0f, 4.0f, 1.0f));
            pRow12.push_back(Vec4f(0.0f, 3.0f, 3.0f, 1.0f));
            pRow12.push_back(Vec4f(2.0f, 3.0f, 2.0f, 1.0f));
            pRow12.push_back(Vec4f(2.0f, 3.0f, 1.0f, 1.0f));
            pRow12.push_back(Vec4f(1.0f, 3.0f, 0.0f, 1.0f)* 2.0f);
            controlPoints2.push_back(pRow12);

            std::vector<Vec4f> pRow13;
            pRow13.push_back(Vec4f(1.0f, 4.0f, 5.0f, 1.0f));
            pRow13.push_back(Vec4f(0.0f, 4.0f, 4.0f, 1.0f));
            pRow13.push_back(Vec4f(0.0f, 4.0f, 3.0f, 1.0f));
            pRow13.push_back(Vec4f(2.0f, 4.0f, 2.0f, 1.0f));
            pRow13.push_back(Vec4f(2.0f, 4.0f, 1.0f, 1.0f));
            pRow13.push_back(Vec4f(1.0f, 4.0f, 0.0f, 1.0f)* 2.0f);
            controlPoints2.push_back(pRow13);

            std::vector<Vec4f> pRow14;
            pRow14.push_back(Vec4f(1.0f, 5.0f, 5.0f, 1.0f));
            pRow14.push_back(Vec4f(0.0f, 5.0f, 4.0f, 1.0f));
            pRow14.push_back(Vec4f(0.0f, 5.0f, 3.0f, 1.0f));
            pRow14.push_back(Vec4f(2.0f, 5.0f, 2.0f, 1.0f));
            pRow14.push_back(Vec4f(2.0f, 5.0f, 1.0f, 1.0f));
            pRow14.push_back(Vec4f(1.0f, 5.0f, 0.0f, 1.0f)* 2.0f);
            controlPoints2.push_back(pRow14);

            std::vector<Vec4f> pRow15;
            pRow15.push_back(Vec4f(1.0f, 6.0f, 5.0f, 1.0f));
            pRow15.push_back(Vec4f(0.0f, 6.0f, 4.0f, 1.0f));
            pRow15.push_back(Vec4f(0.0f, 6.0f, 3.0f, 1.0f));
            pRow15.push_back(Vec4f(2.0f, 6.0f, 2.0f, 1.0f));
            pRow15.push_back(Vec4f(2.0f, 6.0f, 1.0f, 1.0f));
            pRow15.push_back(Vec4f(1.0f, 6.0f, 0.0f, 1.0f)* 2.0f);
            controlPoints2.push_back(pRow15);
        
            std::vector<Vec4f> pRow16;
            pRow16.push_back(Vec4f(1.0f, 8.0f, 5.0f, 1.0f));
            pRow16.push_back(Vec4f(0.0f, 7.0f, 4.0f, 1.0f));
            pRow16.push_back(Vec4f(0.0f, 7.0f, 3.0f, 1.0f));
            pRow16.push_back(Vec4f(2.0f, 9.0f, 2.0f, 1.0f));
            pRow16.push_back(Vec4f(2.0f, 9.0f, 1.0f, 1.0f));
            pRow16.push_back(Vec4f(1.0f, 8.0f, 0.0f, 1.0f)* 2.0f);
            controlPoints2.push_back(pRow16);

            std::vector<float> knotVectorU2;
            knotVectorU2.push_back(0.0f);
            knotVectorU2.push_back(0.0f);
            knotVectorU2.push_back(0.0f);
            knotVectorU2.push_back(0.0f);
            knotVectorU2.push_back(0.0f);
            knotVectorU2.push_back(0.0f);
            knotVectorU2.push_back(1.0f);
            knotVectorU2.push_back(1.0f);
            knotVectorU2.push_back(1.0f);
            knotVectorU2.push_back(1.0f);
            knotVectorU2.push_back(1.0f);
            knotVectorU2.push_back(1.0f);

            std::vector<float> knotVectorV2;
            knotVectorV2.push_back(0.0f);
            knotVectorV2.push_back(0.0f);
            knotVectorV2.push_back(0.0f);
            knotVectorV2.push_back(0.0f);
            knotVectorV2.push_back(0.0f);
            knotVectorV2.push_back(0.0f);
            knotVectorV2.push_back(1.0f);
            knotVectorV2.push_back(1.0f);
            knotVectorV2.push_back(1.0f);
            knotVectorV2.push_back(1.0f);
            knotVectorV2.push_back(1.0f);
            knotVectorV2.push_back(1.0f);
        
            unsigned int degreeEx2 = 5;
	
            NURBSs.push_back(NURBS_Surface(controlPoints2, knotVectorU2, knotVectorV2, degreeEx2));
            
            nurbsmade = true;
        }
        
        
        std::vector<Vec4f> pointrow = std::vector<Vec4f>();
        Vec4f point = Vec4f(0.0f,0.0f,0.0f,1.0f);
        std::vector<Vec3f> normalrow = std::vector<Vec3f>();
        Vec3f normal = Vec3f(0.0f,0.0f,0.0f);
        Vec4f tanU = Vec4f(0.0f,0.0f,0.0f,1.0f);
        Vec4f tanV = Vec4f(0.0f,0.0f,0.0f,1.0f);
        Vec3f tmpTanU = Vec3f(0.0f,0.0f,0.0f);
        Vec3f tmpTanV = Vec3f(0.0f,0.0f,0.0f);
        for (int i = 0; i < NURBSs.size(); i++) {
            pointrow.clear();
            normalrow.clear();
            for (int j = 0; j <= nrPoints; j++) {
                for (int k = 0; k <= nrPoints; k++) {
                    point = NURBSs[i].evaluteDeBoor((float)k/nrPoints,(float)j/nrPoints,tanU,tanV);
                    pointrow.push_back(point);
                    tanU.homogenize();
                    tanV.homogenize();
                    tmpTanU = Vec3f(tanU.x,tanU.y,tanU.z);
                    tmpTanV = Vec3f(tanV.x,tanV.y,tanV.z);
                    tmpTanU.normalize();
                    tmpTanV.normalize();
                    normal = tmpTanU^tmpTanV;
                    normalrow.push_back(normal);
                }
            }
            points.push_back(pointrow);
            normals.push_back(normalrow);
        }
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
                if (enablePoints) {
                    drawSurfacePoints(points[nurbsSelect]);
                }
                if (enableNormals) {
                    drawNormals(points[nurbsSelect],normals[nurbsSelect]);
                }
                drawNURBSSurface(points[nurbsSelect],normals[nurbsSelect],nrPoints+1,nrPoints+1,enableSurface,enableWire,triangles);
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
                calculatePoints();
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
                break;
	case 'a':
	case 'A':
		nurbsSelect = (nurbsSelect + 1) % NURBSs.size();
		glutPostRedisplay();
		break;
		// TODO: place custom functions on button events here to present your results
		// ==========================================================================
        case 'p':
	case 'P':
		enablePoints = !enablePoints;
		glutPostRedisplay();
		std::cout << "Points: "<< (enablePoints ? "enabled" : "disabled")<<"\n";
		break;
        case 'n':
	case 'N':
		enableNormals = !enableNormals;
		glutPostRedisplay();
		std::cout << "Normals: "<< (enableNormals ? "enabled" : "disabled")<<"\n";
		break;
        case 'w':
	case 'W':
		if (!enableWire) {
                    enableWire = true;
                    triangles = true;
                }
                else if (triangles) {
                    triangles = false;
                }
                else {
                    enableWire = false;
                }
		glutPostRedisplay();
                if (!enableWire) std::cout << "Wire frame: disabled" << std::endl;
                else {
                    if (triangles) std::cout << "Wire frame: enabled, triangles" << std::endl;
                    else std::cout << "Wire frame: enabled, quads" << std::endl;
                }
		break;
        case 's':
	case 'S':
                if (!enableSurface) {
                    enableSurface = true;
                    triangles = true;
                }
                else if (triangles) {
                    triangles = false;
                }
                else {
                    enableSurface = false;
                }
		glutPostRedisplay();
                if (!enableSurface) std::cout << "Surface: disabled" << std::endl;
                else {
                    if (triangles) std::cout << "Surface: enabled, triangles" << std::endl;
                    else std::cout << "Surface: enabled, quads" << std::endl;
                }
		break;
        case 'i':
        case 'I':
                nrPoints++;
                calculatePoints();
                glutPostRedisplay();
                std::cout << "Number of evaluated points in both directions: " << nrPoints+1 << std::endl;
                break;
        case 'd':
        case 'D':
                if (nrPoints > 1) {
                    nrPoints--;
                }
                calculatePoints();
                glutPostRedisplay();
                std::cout << "Number of evaluated points in both directions: " << nrPoints+1 << std::endl;
                break;
        case 'u':
                if (u > 0)
                    u -= 0.01;
                if (u < 0)
                    u = 0;
                glutPostRedisplay();
                break;
        case 'U':
                if (u < 1)
                    u += 0.01;
                if (u > 1)
                    u = 1;
                glutPostRedisplay();
                break;
        case 'v':
                if (v > 0)
                    v -= 0.01;
                if (v < 0)
                    v = 0;
                glutPostRedisplay();
                break;
        case 'V':
                if (v < 1)
                    v += 0.01;
                if (v > 1)
                    v = 1;
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
        std::cout << "P: switch (P)oint visualization" << std::endl;
        std::cout << "N: switch (N)ormal visualization" << std::endl;
        std::cout << "W: switch (W)ire frame visualization (none, triangles, quads" << std::endl;
        std::cout << "S: switch (S)urface visualization (none, triangles, quads)" << std::endl;
        std::cout << "I: (I)ncrease m (number of evaluated Points)" << std::endl;
        std::cout << "D: (D)ecrease m (number of evaluated Points)" << std::endl;
        std::cout << "U: Increase u" << std::endl;
        std::cout << "u: Decrease u" << std::endl;
        std::cout << "V: Increase v" << std::endl;
        std::cout << "v: Decrease v" << std::endl;
	// ================================================
	std::cout << "==========================" << std::endl;
	std::cout << std::endl;
}
