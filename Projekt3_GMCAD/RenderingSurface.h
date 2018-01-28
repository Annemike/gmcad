// ========================================================================= //
// Authors: Felix Schuwirth                                                  //
// mailto:felix.schuwirth@igd.fraunhofer.de                                  //
//                                                                           //
// GRIS - Graphisch Interaktive Systeme                                      //
// Technische Universit�t Darmstadt                                          //
// Fraunhoferstrasse 5                                                       //
// D-64283 Darmstadt, Germany                                                //
// ========================================================================= //

#ifndef SURFACE_RENDERING_H
#define SURFACE_RENDERING_H
#include <Vec3.h>
#include <Vec4.h>
#include <vector>

class NURBS_Surface;

void drawSurfacePoints(const std::vector<Vec4f> &points);
void drawNormals(const std::vector<Vec4f> &points, const std::vector<Vec3f> &normals);
void drawNURBSSurfaceCtrlP(const NURBS_Surface &surface);

void drawNURBSSurface(std::vector<Vec4f> &points, const std::vector<Vec3f> &normals, const int numPointsU, const int numPointsV, bool enableSurf, bool enableWire, bool tri);
void evaluateNURBSSurface(const NURBS_Surface &surface, float u, float v, bool vFirst = true);

#endif //