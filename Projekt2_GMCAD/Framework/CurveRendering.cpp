#define RADPERDEG 0.0174533


#include "CurveRendering.h"

#include <GL/glut.h>
#include <BezierCurve.h>
#include <NURBSCurve.h>
#include <algorithm>
#include <stdio.h>		// cout
#include <iostream>		// cout


void drawBezier(BezierCurve &bezierCurve, Vec3f color)
{	
	// TODO: implement the visualization of the 3D bezier curve (e.g. with GL_LINE_STRIP)
	// ===============================================================================
	std::vector<Vec3f> tmpList1 = bezierCurve.evaluateCurve(21).first;
	glBegin(GL_LINE_STRIP);
	glColor3fv(&color.y);
	for (unsigned int i = 0; i < tmpList1.size(); i++) {
		glVertex3fv(&tmpList1[i].x);
	}
	glEnd();
	// ===============================================================================
}
void drawBezierCtrlPolygon(const BezierCurve &bezierCurve, Vec3f color)
{
	// TODO: implement the visualization of the 3D bezier curves control polygon (e.g. with GL_LINE_STRIP)
	// ===============================================================================
	// cps of the complete curve
	glBegin(GL_LINE_STRIP);
	glColor3fv(&color.x);

	for (unsigned int i = 0; i < bezierCurve.getControlPoints().size(); i++) {
		glVertex3f(bezierCurve.getControlPoints()[i].x, bezierCurve.getControlPoints()[i].y, bezierCurve.getControlPoints()[i].z);
	}
	glEnd();

	glBegin(GL_POINTS);
	glColor3fv(&color.y);

	for (unsigned int i = 0; i < bezierCurve.getControlPoints().size(); i++) {
		glVertex3f(bezierCurve.getControlPoints()[i].x, bezierCurve.getControlPoints()[i].y, bezierCurve.getControlPoints()[i].z);
	}
	glEnd();
	// ===============================================================================
}
void drawRationalBezier(BezierCurve &bezierCurve, Vec3f color)
{
	if (bezierCurve.isRational())
	{
		// TODO: implement the visualization of the 2D rational bezier curve in the plane w=1 (e.g. with GL_LINE_STRIP)
		// ===============================================================================
		std::vector<Vec3f> tmpList1 = bezierCurve.evaluateCurve(21).first;
		for (int i = 0; i < tmpList1.size(); i++)
		{
			tmpList1[i] /= tmpList1[i].z;
		}
		glBegin(GL_LINE_STRIP);
		glColor3fv(&color.y);
		for (unsigned int i = 0; i < tmpList1.size(); i++) {
			glVertex3fv(&tmpList1[i].x);
		}
		glEnd();
		// ===============================================================================
	}
}
void drawRationalBezierCtrlPolygon(const BezierCurve &bezierCurve, Vec3f color)
{
	if (bezierCurve.isRational())
	{
		// TODO: implement the visualization of the 2D rational bezier curves control polygon in the plane w=1 (e.g. with GL_LINE_STRIP)
		// ===============================================================================
		glBegin(GL_LINE_STRIP);
		glColor3fv(&color.x);
		std::vector<Vec3f> cps = bezierCurve.getControlPoints();
		for (int i = 0; i < cps.size(); i++)
		{
			cps[i] /= cps[i].z;
		}
		for (unsigned int i = 0; i < cps.size(); i++)
		{
			glVertex3fv(&cps[i].x);
		}
		glEnd();

		glBegin(GL_POINTS);
		glColor3fv(&color.y);

		for (unsigned int i = 0; i < bezierCurve.getControlPoints().size(); i++) {
			glVertex3fv(&cps[i].x);
		}
		glEnd();
		// ===============================================================================

	}
}

void drawNURBS(NURBSCurve &nurbsCurve, Vec3f color)
{
	// TODO: draw NURBS curve
	// NOT homogenized
	// ===================================================================================
	auto pointsAndTangents = nurbsCurve.evaluateCurve(size_t(50));
	auto points = pointsAndTangents.first;
	
	glBegin(GL_LINE_STRIP);
	glColor3fv(&color.x);
	for (unsigned int i = 0; i < points.size(); i++) {
		glVertex3fv(&points[i].x);
	}
	glEnd();
	// ===================================================================================
}
void drawNURBS_H(NURBSCurve &nurbsCurve, Vec3f color)
{
	// TODO: draw NURBS curve
	// homogenized
	// ===================================================================================
	auto pointsAndTangents = nurbsCurve.evaluateCurve(size_t(50));
	auto points = pointsAndTangents.first;

	for (int i = 0; i < points.size(); i++)
	{
		auto p = &points[i];
		(*p).homogenize();
	}

	glBegin(GL_LINE_STRIP);
	glColor3fv(&color.x);
	for (unsigned int i = 0; i < points.size(); i++) {
		glVertex3fv(&points[i].x);
	}
	glEnd();
	// ===================================================================================
}
void drawNURBSCtrlPolygon(const NURBSCurve &nurbsCurve, Vec3f color)
{
	// TODO: implement the visualization of the 
	// NOT homogenized 
	// NURBS' control polygon (e.g. with GL_LINE_STRIP)
	// =========================================================================================================
	glBegin(GL_LINE_STRIP);
	glColor3fv(&color.x);

	for (unsigned int i = 0; i < nurbsCurve.getControlPoints().size(); i++) {
		auto p = nurbsCurve.getControlPoints()[i];
		glVertex3fv(&p.x);
	}
	glEnd();

	glBegin(GL_POINTS);
	glColor3fv(&color.y);

	for (unsigned int i = 0; i < nurbsCurve.getControlPoints().size(); i++) {
		auto p = nurbsCurve.getControlPoints()[i];
		glVertex3fv(&p.x);
	}
	glEnd();
	// =========================================================================================================
}
void drawNURBSCtrlPolygon_H(const NURBSCurve &nurbsCurve, Vec3f color)
{
	// TODO: implement the visualization of the 
	// homogenized 
	// NURBS' control polygon (e.g. with GL_LINE_STRIP)
	// =========================================================================================================
	std::vector<Vec4f> tempPoints(nurbsCurve.getControlPoints());
	for (int i = 0 ; i < tempPoints.size(); i++)
	{
		auto p = &tempPoints[i];
		(*p).homogenize();
	}
	glBegin(GL_LINE_STRIP);
	glColor3fv(&color.x);

	for (unsigned int i = 0; i < tempPoints.size(); i++) {
		auto p = tempPoints[i];
		glVertex3fv(&p.x);
	}
	glEnd();

	glBegin(GL_POINTS);
	glColor3fv(&color.y);

	for (unsigned int i = 0; i < tempPoints.size(); i++) {
		auto p = tempPoints[i];
		glVertex3fv(&p.x);
	}
	glEnd();
	// =========================================================================================================
}

void renderBezier(BezierCurve &bezierCurve)
{
	auto pointsAndTangents = bezierCurve.evaluateCurve(size_t(100));
	bool rational = bezierCurve.isRational();
	Vec3f color = Vec3f(0.0f, 1.0f, 1.0f);
	if (bezierCurve.isRational())
	{
		drawRationalBezier(bezierCurve, color);
	}
	if (rational)
		color = Vec3f(1.0f, 0.0f, 1.0f);
	drawBezier(bezierCurve, color);
}
void renderBezierEvaluation(BezierCurve &bezierCurve, float t)
{
	Vec3f color = { 1.0f,1.0f,1.0f };
	if (bezierCurve.getControlPoints().size() > 1)
	{
		bool rational = bezierCurve.isRational();
		auto twoBezierCurves = bezierCurve.separateCurveAt(t);

		glLineWidth(2.0f);

		// first partial curve cps
		if (rational)color = Vec3f(0.8f, 5.0f, 0.4f);
		else		color = Vec3f(0.8f, 0.0f, 0.4f);
		drawBezierCtrlPolygon(twoBezierCurves.first, color);
		// second partial curve cps		
		if (rational)color = Vec3f(0.4f, 5.0f, 0.8f);
		else		color = Vec3f(0.4f, 0.0f, 0.8f);
		drawBezierCtrlPolygon(twoBezierCurves.second, color);

		glLineWidth(1.0f);
		// cps of the complete curve
		if (rational)color = Vec3f(0.3f, 0.2f, 0.7f);
		else		color = Vec3f(0.3f, 0.7f, 0.7f);
		drawBezierCtrlPolygon(bezierCurve, color);

		glLineWidth(2.0f);
		if (bezierCurve.isRational())
		{

			auto twoBezierCurves2 = bezierCurve.separateCurveAt(t);
			// first partial curve cps
			color = Vec3f(0.8f, 0.0f, 0.4f);
			drawRationalBezierCtrlPolygon(twoBezierCurves2.first, color);
			// second partial curve cps
			color = Vec3f(0.4f, 0.0f, 0.8f);
			drawRationalBezierCtrlPolygon(twoBezierCurves2.second, color);
			glLineWidth(1.0f);
			// cps of the complete curve
			color = Vec3f(0.3f, 0.7f, 0.7f);
			drawRationalBezierCtrlPolygon(bezierCurve, color);

		}
	}

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
	if (!nurbsCurve.isValidNURBS())
		return;

	auto pointsAndTangents = nurbsCurve.evaluateCurve(size_t(7));
	auto points = pointsAndTangents.first;
	auto tangents = pointsAndTangents.second;

	if (points.size() > 1 && nurbsCurve.getControlPoints().size() > 1)
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
		for (unsigned int i = 0; i < tangents.size(); ++i)
		{
			/*auto p = points[i];
			auto t = tangents[i];
			t = Vec4f(t.w * p.x + p.w * t.x, t.w * p.y + p.w * t.y, t.w * p.z + p.w * t.z, 1.0f);
			glVertex3fv(&p.x);
			t = (t - p).normalized() + p;
			glVertex3fv(&t.x);*/

			auto p = points[i];
			auto t = tangents[i];
			glVertex3fv(&p.x);
			Vec4f r = Vec4f(t.x, t.y, t.z, 0.0f);
			t = t.normalized() + p;
			glVertex3fv(&t.x);
		}
		glEnd();
		// draw tangents of the homogenized curve
		glColor3f(0.5f, 0.0f, 0.5f);
		glBegin(GL_LINES);
		for (unsigned int i = 0; i < tangents.size(); ++i)
		{
			/*auto p = points[i];
			auto t = tangents[i];
			p = p / p.w;
			t = t / t.w;
			t = p + t;
			glVertex3fv(&p.x);
			t = (t - p).normalized() + p;
			glVertex3fv(&t.x);*/
			auto p = points[i];
			auto t = tangents[i];
			t = p + t;
			p = p / p.w;
			t = t / t.w;
			p = Vec4f(p.x, p.y, p.z, 0.0f);
			t = Vec4f(t.x, t.y, t.z, 0.0f);
			glVertex3fv(&p.x);
			t = (t - p).normalized() + p;
			glVertex3fv(&t.x);
		}
		glEnd();
	}
}
