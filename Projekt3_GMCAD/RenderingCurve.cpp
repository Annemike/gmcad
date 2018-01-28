#define RADPERDEG 0.0174533

#include "RenderingCurve.h"

#include <GL/glut.h>
#include <NURBS_Curve.h>
#include <algorithm>

void drawNURBS(NURBSCurve &nurbsCurve, Vec3f color)
{
	// draw NURBS curve
	// NOT homogenized
	// ===================================================================================
	auto points = nurbsCurve.evaluateCurveAt(50).first;
	glColor3fv(&color.x);
	glBegin(GL_LINE_STRIP);
	for (auto p : points)
	{
		glVertex3f(p.x, p.y, p.z);
	}
	glEnd();
	// ===================================================================================
}
void drawNURBS_H(NURBSCurve &nurbsCurve, Vec3f color)
{
	// draw NURBS curve
	// homogenized
	// ===================================================================================
	auto points = nurbsCurve.evaluateCurveAt(50).first;
	glColor3fv(&color.x);
	glBegin(GL_LINE_STRIP);
	for (auto p : points)
	{
		p = p / p.w;
		glVertex3f(p.x, p.y, p.z);
	}
	glEnd();
	// ===================================================================================
}
void drawNURBSCtrlPolygon(const NURBSCurve &nurbsCurve, Vec3f color)
{
	// implement the visualization of the 
	// NOT homogenized 
	// NURBS' control polygon (e.g. with GL_LINE_STRIP)
	// =========================================================================================================
	glColor3fv(&color.x);
	glBegin(GL_POINTS);
	for (auto& cp : nurbsCurve.getControlPoints())
		glVertex3f(cp.x, cp.y, cp.z);
	glEnd();
	color *= 0.7f;
	glColor3fv(&color.x);
	glBegin(GL_LINE_STRIP);
	for (auto ctrlp : nurbsCurve.getControlPoints())
	{
		glVertex3fv(&ctrlp.x);
	}
	glEnd();
	// =========================================================================================================
}
void drawNURBSCtrlPolygon_H(const NURBSCurve &nurbsCurve, Vec3f color)
{
	// implement the visualization of the 
	// homogenized 
	// NURBS' control polygon (e.g. with GL_LINE_STRIP)
	// =========================================================================================================
	glColor3fv(&color.x);
	glBegin(GL_POINTS);
	for (auto ctrlp : nurbsCurve.getControlPoints())
	{
		ctrlp = ctrlp / ctrlp.w;
		glVertex3f(ctrlp.x, ctrlp.y, ctrlp.z);
	}
	glEnd();
	color *= 0.7f;
	glColor3fv(&color.x);
	glBegin(GL_LINE_STRIP);
	for (auto ctrlp : nurbsCurve.getControlPoints())
	{
		ctrlp = ctrlp / ctrlp.w;
		glVertex3fv(&ctrlp.x);
	}
	glEnd();
	// =========================================================================================================
}
void renderNURBS(NURBSCurve &nurbsCurve)
{
	if (nurbsCurve.getControlPoints().size() > 1 && nurbsCurve.isValidNURBS())
	{
		// draw curve (x, y, w) not homogenized
		Vec3f color = Vec3f(1.0f, 0.8f, 0.0f);
		drawNURBS(nurbsCurve, color);
		color = Vec3f(0.8f, 1.0f, 0.0f);
		drawNURBS_H(nurbsCurve, color);
	}
}
void renderNURBSEvaluation(NURBSCurve &nurbsCurve, float t)
{
	Vec3f color = Vec3f(1.0f, 1.0f, 1.0f);
	if(!nurbsCurve.isValidNURBS())
		return;

	auto pointsAndTangents = nurbsCurve.evaluateCurveAt(size_t(50));
	auto points = pointsAndTangents.first;
	auto tangents = pointsAndTangents.second;

	if(points.size() > 1 && nurbsCurve.getControlPoints().size() > 1)
	{
	
		// draw control polygon (x, y, w) not homogenized
		color = Vec3f(0.7f, 0.5f, 0.0f);
		drawNURBSCtrlPolygon(nurbsCurve, color);
		// draw control polygon (x/w, y/w, w=1) homogenized
		color = Vec3f(0.7f, 0.0f, 0.7f);
		drawNURBSCtrlPolygon_H(nurbsCurve, color);

		// draw tangents of the not homogenized curve
		glColor3f(0.5f, 0.35f, 0.0f);
		glBegin(GL_LINES);
		for(unsigned int i = 0; i < tangents.size(); ++i)
		{
			auto p = points[i];
			auto t = tangents[i];
			t = Vec4f(t.w * p.x + p.w * t.x, t.w * p.y + p.w * t.y, t.w * p.z + p.w * t.z, p.w*t.w);
			//t = t / t.w;
			glVertex3fv(&p.x);
			t = (t - p).normalized() + p;
			glVertex3fv(&t.x);
		}
		glEnd();
		// draw tangents of the homogenized curve
		glColor3f(0.5f, 0.0f, 0.5f);
		glBegin(GL_LINES);
		for(unsigned int i = 0; i < tangents.size(); ++i)
		{
			auto p = points[i];
			auto t = tangents[i];
			p = p / p.w;
			t = t / t.w;
			t = p + t;
			glVertex3fv(&p.x);
			t = (t - p).normalized() + p;
			glVertex3fv(&t.x);
		}
		glEnd();
	}
}
