#include "BezierCurve.h"

#include <stdio.h>		// cout
#include <iostream>		// cout

BezierCurve::BezierCurve(bool isRational_)
	: rational(isRational_)
{
}

BezierCurve::BezierCurve(const std::vector<Vec3f>& controlPoints_, bool isRational_)
	: controlPoints(controlPoints_)
	, rational(isRational_)
{
}




// splits the bezier curve into two at u=t
std::pair<BezierCurve, BezierCurve> BezierCurve::separateCurveAt(const float t)
{
	std::vector<Vec3f> cps1, cps2;
	int n = getControlPoints().size();

	// TODO: implement the de Casteljau algorithm and create two separated bezier curves with the new control points.
	// ==========================================================================================================
	for (int i = 0; i < n; i++) {
		cps2.push_back(getControlPoints()[i]);
	}
	cps1.push_back(cps2[0]);
	for (int k = 1; k < n; k++) {
		for (int i = 0; i < n - k; i++) {
			cps2[i] = cps2[i].operator*(1.0f - t).operator+(cps2[i + 1].operator*(t));
		}
		cps1.push_back(cps2[0]);
	}
	/*std::cout << "start" << std::endl;
	for (auto &p : cps1)
	{
		std::cout << p << std::endl;
	}
	std::cout << "middle" << std::endl;
	for (auto &p : cps2)
	{
		std::cout << p << std::endl;
	}
	std::cout << "end" << std::endl;*/
	// ==========================================================================================================
	BezierCurve curve1(cps1, rational);
	BezierCurve curve2(cps2, rational);
	return std::pair<BezierCurve, BezierCurve>(curve1, curve2);
}



// evaluates the bezier curve at u=t, returns that point and calculates its tangent
Vec3f BezierCurve::evaluateCurveAt(const float t, Vec3f &tangent)
{
	Vec3f point;

	// TODO: implement the evaluation of the bezier curve and the tangent at t.
	// Note: use the seperateCurveAt(t) function.
	// ==========================================================================================================
	if (t == 0)
	{
		point = controlPoints[0];
		tangent = controlPoints[1] - point;
	}
	else if (t == 1)
	{
		point = controlPoints[controlPoints.size() - 1];
		tangent = point - controlPoints[controlPoints.size() - 2];
	}
	else 
	{
		point = separateCurveAt(t).second.getControlPoints()[0];
		tangent = separateCurveAt(t).second.getControlPoints()[1] - point;
	}
	// ==========================================================================================================
	return point;
}

std::pair<std::vector<Vec3f>, std::vector<Vec3f>> BezierCurve::evaluateCurve(const size_t numberSamples)
{
	std::vector<Vec3f> points;
	points.reserve(numberSamples);
	std::vector<Vec3f> tangents;
	tangents.reserve(numberSamples);
	// TODO: implement evaluation of the bezier curve at 'numberSamples' equidistant points
	// Note: use the 'evaluateCurveAt' function. 
	// ==========================================================================================================
	Vec3f tangent;
	for (float i = 0; i < numberSamples; i++) {
		points.push_back(evaluateCurveAt(i / (numberSamples - 1), tangent));
		tangents.push_back(tangent);
	}
	// ==========================================================================================================

	return std::make_pair(points, tangents);
}




std::ostream& operator<< (std::ostream& os, BezierCurve& bezierCurve)
{
	if (bezierCurve.controlPoints.empty())
	{
		os << "Bezier curve without control points!";
		return os;
	}
	// degree
	os << "Bezier curve, degree " << bezierCurve.controlPoints.size() - 1 << "\n";
	// control points
	for (size_t i = 0; i < bezierCurve.controlPoints.size(); i++) os << "  " << bezierCurve.controlPoints[i] << std::endl;
	return os;
}