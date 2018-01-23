#define RADPERDEG 0.0174533

#include "RenderingSurface.h"
#include "RenderingCurve.h"

#include <GL/glut.h>
#include <NURBS_Curve.h>
#include <NURBS_Surface.h>
#include <algorithm>

void drawSurfacePoints(const std::vector<Vec4f> &points)
{
	// TODO: draw points of the surface
	// note: Vec4f provides a method to homogenize a vector
	// =====================================================
	

	// =====================================================
}
void drawNormals(const std::vector<Vec4f> &points, const std::vector<Vec3f> &normals)
{
	// TODO: draw normals as lines (homogenized)
	// note: Vec4f provides a method to homogenize a vector
	// =====================================================
	

	// =====================================================
}
void drawNURBSSurfaceCtrlP(const NURBS_Surface &surface)
{
	// TODO: draw control polygon an points (homogenized)
	// =====================================================
	
	glBegin(GL_POINTS);
	glColor3fv(&surface.controlPoints[0][0].x);
	for (int i = 0; i < surface.controlPoints.size(); i++) {
		for (int j = 0; j < surface.controlPoints[i].size(); j++) {
			glVertex3f(surface.controlPoints[i][j].x/ surface.controlPoints[i][j].w, surface.controlPoints[i][j].y/ surface.controlPoints[i][j].w, surface.controlPoints[i][j].z/ surface.controlPoints[i][j].w);
		}
	}
	
	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor3fv(&surface.controlPoints[0][0].y);
	for (int i = 0; i < surface.controlPoints.size(); i++) {
		for (int j = 0; j < surface.controlPoints[i].size(); j++) {
			glVertex3f(surface.controlPoints[i][j].homogenized().x, surface.controlPoints[i][j].homogenized().y, surface.controlPoints[i][j].homogenized().z);
		}
	}

	glEnd();
	// =====================================================
}

void drawNURBSSurface(std::vector<Vec4f> &points, const std::vector<Vec3f> &normals, const int numPointsU, const int numPointsV, bool enableSurf, bool enableWire)
{

	if (enableWire)
	{
		glDisable(GL_LIGHTING);
		glColor3f(0.0f,0.0f,1.0f);
		// TODO: draw surface wire mesh
		// =====================================================
		

		// =====================================================

	}

	if (enableSurf)
	{
		glEnable(GL_LIGHTING);
		glColor3f(0.99f, 0.99f, 0.99f);
		// TODO: draw surface with quads
		// =====================================================
		
		
		// =====================================================
	}
}
void evaluateNURBSSurface(const NURBS_Surface &surface,float u, float v, bool vFirst /*= true*/)
{
	Vec3f colorPolyU =  {1.0f, 0.7f, 0.4f};
	Vec3f colorPolyV =  {0.6f, 1.0f, 0.6f};
	Vec3f colorCurveU = {1.0f, 0.5f, 0.0f};
	Vec3f colorCurveV = {0.0f, 0.8f, 0.0f};
	Vec3f colorPoint =  {0.0f, 0.5f, 1.0f};
	// TODO: implement the visualization of the point evaluation starting with u and v direction whether vFirst is true or not
	// note: use the NURBSCurve class and the CurveRendering functions 'drawNURBSCtrlPolygon_H' 'drawNURBS_H'
	// =====================================================

	if (vFirst)
	{
		// 1. draw the nurbs curves and its control polygons first in v direction
		   
		// 2. then the resulting curve and its control polygon at v in u direction.
		   
		// 3. draw the evaluated surface point
		
	}
	else
	{
		// 1. draw the nurbs curves and its control polygons first in u direction

		// 2. then the resulting curve and its control polygon at u in v direction.

		// 3. draw the evaluated surface point
		
	}

	// =====================================================

	
}
