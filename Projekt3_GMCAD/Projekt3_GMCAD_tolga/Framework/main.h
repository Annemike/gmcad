// ========================================================================= //
// Authors: Felix Schuwirth                                                  //
// mailto:felix.schuwirth@igd.fraunhofer.de                                  //
//                                                                           //
// GRIS - Graphisch Interaktive Systeme                                      //
// Technische Universität Darmstadt                                          //
// Fraunhoferstrasse 5                                                       //
// D-64283 Darmstadt, Germany                                                //
//                                                                           //
// Creation Date: 12.01.2017                                                 //
// Content: Simple and extendable openGL program offering                    //
//   * basic navigation and basic pipeline rendering                         //
// ========================================================================= //

#include <GL/glut.h>	// openGL helper

#include "Vec4.h"
#include "Vec3.h"
#include "NURBS_Surface.h"

// ===================
// === GLOBAL DATA ===
// ===================

int enableEval;
bool enableCtrl;
int nurbsSelect; // for switching between the NURBS Surfaces
float u, v; // evaluation point on the Surface

std::vector<NURBS_Surface> NURBSs;
unsigned int nrPoints;

// TODO: define global variables here to present the exercises
// ===========================================================


// ===========================================================

// do not modify data below unless you know what you do
// camera Information
float transX, transY, transZ;
float angleX, angleY;
// mouse information
int mouseX, mouseY, mouseButton;
float mouseSensitivy;

// ==============
// === BASICS ===
// ==============

int main(int argc, char **argv);

void setDefaults();

void initializeGL();

void calculatePoints();

void reshape(GLint width, GLint height);

// =================
// === RENDERING ===
// =================

void drawCS();

void drawObjects();

void renderScene(void);

// =================
// === CALLBACKS ===
// =================

void keyPressed(unsigned char key, int x, int y);

void mousePressed(int button, int state, int x, int y);

void mouseMoved(int x, int y);

// ===============
// === VARIOUS ===
// ===============

void coutHelp();
