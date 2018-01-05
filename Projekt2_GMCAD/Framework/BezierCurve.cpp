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
/*
std::vector<Vec3f> BezierCurve::derivateBezier(const float t, std::vector<Vec3f> p)
{
	std::vector<Vec3f> a, b;
	std::vector<Vec3f> tmpList;
	std::vector<Vec3f> resulta, resultb;
	Vec3f z ,y;

	for (int i = 0; i < p.size(); i++) {
		tmpList.push_back(p[i]);
	}
	

	for (int i = 1; i <= 2; i++) {
		tmpList[i].operator*(calculateBlendingFunction(i - 1, 2 - 1, t));
		//resulta.push_back(tmpList[i].operator*(2));
		z = z + tmpList[i].operator*(2);
	}
	a = resulta;

	for (int i = 0; i < 3; i++) {
		tmpList[i].operator*(calculateBlendingFunction(i, 2 - 1, t));
		//resultb.push_back(tmpList[i].operator*(2));
		y = y + tmpList[i].operator*(2);
	}
	b = resultb;

	for(int i = 0 ; i < b.size(); i++){
		std::cout << b[i] << std::endl;
		std::cout << calculateBlendingFunction(i, 2 - 1, t)<<std::endl;
		std::cout <<(z - y) << std::endl;
	}
	

	return resulta;
}

const float BezierCurve::calculateBlendingFunction(int n, int i, const float t)
{
	
	return binomialCoefficient(n, i)* pow(t,i)* pow((1.0f - t), n - i);
}





int BezierCurve::binomialCoefficient(int i, int n) {
	return factorial(i) / (factorial(n)*factorial(i - n));
}

int BezierCurve::factorial(int n)
{
	if (n > 1) {
		return n * factorial(n - 1);
	}
	else {
		return 1;
	}
}
*/

// splits the bezier curve into two at u=t
std::pair<BezierCurve, BezierCurve> BezierCurve::separateCurveAt(const float t)
{
	std::vector<Vec3f> cps1, cps2;
	int n = getControlPoints().size();
	
	// TODO: implement the de Casteljau algorithm and create two separated bezier curves with the new control points.
	// ==========================================================================================================
	for (int i = 0; i < n; i++) {
		cps1.push_back(getControlPoints()[i]);
	}

	for (int k = 1; k <= n; k++) {
		for (int i = 0; i < n - k; i++) {
			cps1[i] = cps1[i].operator*(1.0f - t).operator+(cps1[i + 1].operator*(t));	
		}
		cps2.push_back(cps1[0]);
	}
	// ==========================================================================================================
	BezierCurve curve1(cps1, rational);
	BezierCurve curve2(cps2, rational);
	return std::pair<BezierCurve, BezierCurve>(curve1, curve2);
}



// evaluates the bezier curve at u=t, returns that point and calculates its tangent
Vec3f BezierCurve::evaluateCurveAt(const float t, Vec3f &tangent)
{
	Vec3f point;
	float m;
		
	
	
	


		for (int i = 0; i < separateCurveAt(t).first.getControlPoints().size(); i++) {

			for (int k = 0; k < separateCurveAt(t).second.getControlPoints().size(); k++) {
				if (separateCurveAt(t).first.getControlPoints()[i] == separateCurveAt(t).second.getControlPoints()[k]) {
					point = separateCurveAt(t).second.getControlPoints()[i];
				}
			}
		}
	
	// TODO: implement the evaluation of the bezier curve and the tangent at t.
	// Note: use the seperateCurveAt(t) function.
	// ==========================================================================================================
	m = (separateCurveAt(t).second.getControlPoints()[0].y - separateCurveAt(t).first.getControlPoints()[0].y) /
		(separateCurveAt(t).second.getControlPoints()[0].x - separateCurveAt(t).first.getControlPoints()[0].x);
	
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


	// ==========================================================================================================

	return std::make_pair(points,tangents);
}




std::ostream& operator<< (std::ostream& os, BezierCurve& bezierCurve)
{
	if(bezierCurve.controlPoints.empty())
	{
		os << "Bezier curve without control points!";
		return os;
	}
	// degree
	os << "Bezier curve, degree " << bezierCurve.controlPoints.size() - 1 << "\n";
	// control points
	for(size_t i = 0; i < bezierCurve.controlPoints.size(); i++) os << "  " << bezierCurve.controlPoints[i] << std::endl;
	return os;
}