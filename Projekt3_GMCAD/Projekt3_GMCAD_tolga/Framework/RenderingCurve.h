// ========================================================================= //
// Authors: Felix Schuwirth                                                  //
// mailto:felix.schuwirth@igd.fraunhofer.de                                  //
//                                                                           //
// GRIS - Graphisch Interaktive Systeme                                      //
// Technische Universität Darmstadt                                          //
// Fraunhoferstrasse 5                                                       //
// D-64283 Darmstadt, Germany                                                //
// ========================================================================= //

#ifndef CURVE_RENDERING_H
#define CURVE_RENDERING_H
#include <Vec3.h>
#include <Vec4.h>
#include <vector>

class NURBSCurve;

void drawNURBS(NURBSCurve &nurbsCurve, Vec3f color);
void drawNURBS_H(NURBSCurve &nurbsCurve, Vec3f color);
void drawNURBSCtrlPolygon(const NURBSCurve &nurbsCurve, Vec3f color);
void drawNURBSCtrlPolygon_H(const NURBSCurve &nurbsCurve, Vec3f color);


void renderNURBS(NURBSCurve &nurbsCurve);
void renderNURBSEvaluation( NURBSCurve &nurbsCurve, float t);

#endif //