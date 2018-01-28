#define RADPERDEG 0.0174533

#include "RenderingSurface.h"
#include "RenderingCurve.h"

#include <GL/glut.h>
#include <NURBS_Curve.h>
#include <NURBS_Surface.h>
#include <algorithm>	
#include <stdio.h>	
#include <iostream>

void drawSurfacePoints(const std::vector<Vec4f> &points)
{
	// TODO: draw points of the surface
	// note: Vec4f provides a method to homogenize a vector
	// =====================================================
        glColor3f(0.5f,0.5f,0.5f);
	glBegin(GL_POINTS);
	for (Vec4f p : points)
		glVertex3f(p.homogenized().x, p.homogenized().y, p.homogenized().z);
	glEnd();
	// =====================================================
}
void drawNormals(const std::vector<Vec4f> &points, const std::vector<Vec3f> &normals)
{
	// TODO: draw normals as lines (homogenized)
	// note: Vec4f provides a method to homogenize a vector
	// =====================================================
        glColor3f(0.8f,0.8f,0.8f);
        for (int i = 0; i < points.size(); i++) {
            glBegin(GL_LINES);
            glVertex3f(points[i].homogenized().x, points[i].homogenized().y, points[i].homogenized().z);
            glVertex3f(points[i].homogenized().x + normals[i].x, points[i].homogenized().y + normals[i].y, points[i].homogenized().z + normals[i].z);
            glEnd();
        }
	// =====================================================
}
void drawNURBSSurfaceCtrlP(const NURBS_Surface &surface)
{
	// TODO: draw control polygon an points (homogenized)
	// =====================================================
        NURBSCurve tempCurve = NURBSCurve(surface.controlPoints[0],surface.knotVectorU,surface.degree);
	for (std::vector<Vec4f> row: surface.controlPoints) {
            tempCurve = NURBSCurve(row,surface.knotVectorU,surface.degree);
            drawNURBSCtrlPolygon_H(tempCurve,Vec3f(255.0f,0.0f,0.0f));
        }
        std::vector<Vec4f> column = std::vector<Vec4f>();
        for (int i = 0; i < surface.controlPoints[0].size(); i++) {
            column.clear();
            for (int j = 0; j < surface.controlPoints.size(); j++) {
                column.push_back(surface.controlPoints[j][i]);
            }
            tempCurve = NURBSCurve(column,surface.knotVectorV,surface.degree);
            drawNURBSCtrlPolygon_H(tempCurve,Vec3f(255.0f,0.0f,0.0f));
        }
	// =====================================================
}

void drawNURBSSurface(std::vector<Vec4f> &points, const std::vector<Vec3f> &normals, const int numPointsU, const int numPointsV, bool enableSurf, bool enableWire, bool tri)
{
    if (tri) {
        if (enableWire)
	{
		glDisable(GL_LIGHTING);
		glColor3f(0.0f,0.0f,1.0f);
		// TODO: draw surface wire mesh
		// =====================================================
		for (int i = 0; i < numPointsV-1; i++) {
                    for (int j = 0; j < numPointsU-1; j++) {
                        glBegin(GL_LINES);
                        glVertex3f(points[j+1+numPointsU*i].homogenized().x,points[j+1+numPointsU*i].homogenized().y,points[j+1+numPointsU*i].homogenized().z);
                        glVertex3f(points[j+numPointsU*i].homogenized().x,points[j+numPointsU*i].homogenized().y,points[j+numPointsU*i].homogenized().z);
                        glVertex3f(points[j+numPointsU*i].homogenized().x,points[j+numPointsU*i].homogenized().y,points[j+numPointsU*i].homogenized().z);
                        glVertex3f(points[j+numPointsU*(i+1)].homogenized().x,points[j+numPointsU*(i+1)].homogenized().y,points[j+numPointsU*(i+1)].homogenized().z);
                        glVertex3f(points[j+numPointsU*i].homogenized().x,points[j+numPointsU*i].homogenized().y,points[j+numPointsU*i].homogenized().z);
                        glVertex3f(points[j+1+numPointsU*(i+1)].homogenized().x,points[j+1+numPointsU*(i+1)].homogenized().y,points[j+1+numPointsU*(i+1)].homogenized().z);
                        glEnd();
                    }
                }
                glBegin(GL_LINES);
                for (int i = 0; i < numPointsU-1; i++) {
                    glVertex3f(points[i+numPointsU*(numPointsV-1)].homogenized().x,points[i+numPointsU*(numPointsV-1)].homogenized().y,points[i+numPointsU*(numPointsV-1)].homogenized().z);
                    glVertex3f(points[i+1+numPointsU*(numPointsV-1)].homogenized().x,points[i+1+numPointsU*(numPointsV-1)].homogenized().y,points[i+1+numPointsU*(numPointsV-1)].homogenized().z);
                }
                glEnd();
                glBegin(GL_LINES);
                for (int i = 1; i < numPointsV; i++) {
                    glVertex3f(points[i*numPointsU-1].homogenized().x,points[i*numPointsU-1].homogenized().y,points[i*numPointsU-1].homogenized().z);
                    glVertex3f(points[(i+1)*numPointsU-1].homogenized().x,points[(i+1)*numPointsU-1].homogenized().y,points[(i+1)*numPointsU-1].homogenized().z);
                }
                glEnd();
		// =====================================================

	}

	if (enableSurf)
	{
		glEnable(GL_LIGHTING);
		glColor3f(0.99f, 0.99f, 0.99f);
		// TODO: draw surface with quads
		// =====================================================
        	for (int i = 0; i < numPointsV-1; i++) {
                    for (int j = 0; j < numPointsU-1; j++) {
                        glBegin(GL_TRIANGLES);
                        glNormal3fv(&normals[j+numPointsU*i].x);
                        glVertex3f(points[j+numPointsU*i].homogenized().x,points[j+numPointsU*i].homogenized().y,points[j+numPointsU*i].homogenized().z);
                        glNormal3fv(&normals[j+numPointsU*(i+1)].x);
                        glVertex3f(points[j+numPointsU*(i+1)].homogenized().x,points[j+numPointsU*(i+1)].homogenized().y,points[j+numPointsU*(i+1)].homogenized().z);
                        glNormal3fv(&normals[j+1+numPointsU*(i+1)].x);
                        glVertex3f(points[j+1+numPointsU*(i+1)].homogenized().x,points[j+1+numPointsU*(i+1)].homogenized().y,points[j+1+numPointsU*(i+1)].homogenized().z);
                        glEnd();
                        glBegin(GL_TRIANGLES);
                        glNormal3fv(&normals[j+numPointsU*i].x);
                        glVertex3f(points[j+numPointsU*i].homogenized().x,points[j+numPointsU*i].homogenized().y,points[j+numPointsU*i].homogenized().z);
                        glNormal3fv(&normals[j+1+numPointsU*(i+1)].x);
                        glVertex3f(points[j+1+numPointsU*(i+1)].homogenized().x,points[j+1+numPointsU*(i+1)].homogenized().y,points[j+1+numPointsU*(i+1)].homogenized().z);
                        glNormal3fv(&normals[j+1+numPointsU*i].x);
                        glVertex3f(points[j+1+numPointsU*i].homogenized().x,points[j+1+numPointsU*i].homogenized().y,points[j+1+numPointsU*i].homogenized().z);
                        glEnd();
                    }
                }
		// =====================================================
	}
    }
    else {
	if (enableWire)
	{
		glDisable(GL_LIGHTING);
		glColor3f(0.0f,0.0f,1.0f);
		// TODO: draw surface wire mesh
		// =====================================================
		for (int i = 0; i < numPointsV-1; i++) {
                    for (int j = 0; j < numPointsU-1; j++) {
                        glBegin(GL_LINES);
                        glVertex3f(points[j+1+numPointsU*i].homogenized().x,points[j+1+numPointsU*i].homogenized().y,points[j+1+numPointsU*i].homogenized().z);
                        glVertex3f(points[j+numPointsU*i].homogenized().x,points[j+numPointsU*i].homogenized().y,points[j+numPointsU*i].homogenized().z);
                        glVertex3f(points[j+numPointsU*i].homogenized().x,points[j+numPointsU*i].homogenized().y,points[j+numPointsU*i].homogenized().z);
                        glVertex3f(points[j+numPointsU*(i+1)].homogenized().x,points[j+numPointsU*(i+1)].homogenized().y,points[j+numPointsU*(i+1)].homogenized().z);
                        glEnd();
                    }
                }
                glBegin(GL_LINES);
                for (int i = 0; i < numPointsU-1; i++) {
                    glVertex3f(points[i+numPointsU*(numPointsV-1)].homogenized().x,points[i+numPointsU*(numPointsV-1)].homogenized().y,points[i+numPointsU*(numPointsV-1)].homogenized().z);
                    glVertex3f(points[i+1+numPointsU*(numPointsV-1)].homogenized().x,points[i+1+numPointsU*(numPointsV-1)].homogenized().y,points[i+1+numPointsU*(numPointsV-1)].homogenized().z);
                }
                glEnd();
                glBegin(GL_LINES);
                for (int i = 1; i < numPointsV; i++) {
                    glVertex3f(points[i*numPointsU-1].homogenized().x,points[i*numPointsU-1].homogenized().y,points[i*numPointsU-1].homogenized().z);
                    glVertex3f(points[(i+1)*numPointsU-1].homogenized().x,points[(i+1)*numPointsU-1].homogenized().y,points[(i+1)*numPointsU-1].homogenized().z);
                }
                glEnd();
		// =====================================================

	}

	if (enableSurf)
	{
		glEnable(GL_LIGHTING);
		glColor3f(0.99f, 0.99f, 0.99f);
		// TODO: draw surface with quads
		// =====================================================
        	for (int i = 0; i < numPointsV-1; i++) {
                    for (int j = 0; j < numPointsU-1; j++) {
                        glBegin(GL_QUADS);
                        glNormal3fv(&normals[j+numPointsU*i].x);
                        glVertex3f(points[j+numPointsU*i].homogenized().x,points[j+numPointsU*i].homogenized().y,points[j+numPointsU*i].homogenized().z);
                        glNormal3fv(&normals[j+numPointsU*(i+1)].x);
                        glVertex3f(points[j+numPointsU*(i+1)].homogenized().x,points[j+numPointsU*(i+1)].homogenized().y,points[j+numPointsU*(i+1)].homogenized().z);
                        glNormal3fv(&normals[j+1+numPointsU*(i+1)].x);
                        glVertex3f(points[j+1+numPointsU*(i+1)].homogenized().x,points[j+1+numPointsU*(i+1)].homogenized().y,points[j+1+numPointsU*(i+1)].homogenized().z);
                        glNormal3fv(&normals[j+1+numPointsU*i].x);
                        glVertex3f(points[j+1+numPointsU*i].homogenized().x,points[j+1+numPointsU*i].homogenized().y,points[j+1+numPointsU*i].homogenized().z);
                        glEnd();
                    }
                }
		// =====================================================
	}
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
        Vec4f tan = Vec4f(0.0f,0.0f,0.0f,1.0f);
        
	if (vFirst)
	{
		// 1. draw the nurbs curves and its control polygons first in v direction
            NURBSCurve tempCurve = NURBSCurve(surface.controlPoints[0],surface.knotVectorV,surface.degree);
            std::vector<Vec4f> vPoints = std::vector<Vec4f>();
            std::vector<Vec4f> column = std::vector<Vec4f>();
            for (int i = 0; i < surface.controlPoints[0].size(); i++) {
                column.clear();
                for (int j = 0; j < surface.controlPoints.size(); j++) {
                    column.push_back(surface.controlPoints[j][i]);
                }
                tempCurve = NURBSCurve(column,surface.knotVectorV,surface.degree);
                drawNURBS_H(tempCurve,colorCurveV);
                drawNURBSCtrlPolygon_H(tempCurve,colorPolyV);
                vPoints.push_back(tempCurve.evaluteDeBoor(v,tan));
            }   
		// 2. then the resulting curve and its control polygon at v in u direction.
            tempCurve = NURBSCurve(vPoints,surface.knotVectorU,surface.degree);
            drawNURBS_H(tempCurve,colorCurveU);
            drawNURBSCtrlPolygon_H(tempCurve,colorPolyU);
		// 3. draw the evaluated surface point
            Vec4f resPoint = tempCurve.evaluteDeBoor(u,tan);
            glColor3fv(&colorPoint.x);
            glBegin(GL_POINTS);
            glVertex3f(resPoint.homogenized().x, resPoint.homogenized().y, resPoint.homogenized().z);
            glEnd();
	}
	else
	{
		// 1. draw the nurbs curves and its control polygons first in u direction
            NURBSCurve tempCurve = NURBSCurve(surface.controlPoints[0],surface.knotVectorU,surface.degree);
            std::vector<Vec4f> uPoints = std::vector<Vec4f>();
            for (std::vector<Vec4f> row: surface.controlPoints) {
                tempCurve = NURBSCurve(row,surface.knotVectorU,surface.degree);
                drawNURBS_H(tempCurve,colorCurveU);
                drawNURBSCtrlPolygon_H(tempCurve,colorPolyU);
                uPoints.push_back(tempCurve.evaluteDeBoor(u,tan));
            }
		// 2. then the resulting curve and its control polygon at u in v direction.
            tempCurve = NURBSCurve(uPoints,surface.knotVectorV,surface.degree);
            drawNURBS_H(tempCurve,colorCurveV);
            drawNURBSCtrlPolygon_H(tempCurve,colorPolyV);
		// 3. draw the evaluated surface point 
            Vec4f resPoint = tempCurve.evaluteDeBoor(v,tan);
            glColor3fv(&colorPoint.x);
            glBegin(GL_POINTS);
            glVertex3f(resPoint.homogenized().x, resPoint.homogenized().y, resPoint.homogenized().z);
            glEnd();
	}

	// =====================================================

	
}
