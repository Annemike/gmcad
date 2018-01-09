#include "NURBSCurve.h"

#include <stdio.h>		// cout
#include <iostream>		// cout

NURBSCurve::NURBSCurve()
{

	isValidNURBS();
}

// constructor which takes given control points P, knot vector U and degree p
NURBSCurve::NURBSCurve(const std::vector<Vec4f>& controlPoints_, const std::vector<float>& knotVector_, const unsigned int degree_)
	: controlPoints(controlPoints_)
	, knotVector(knotVector_)
	, degree(degree_)
{
	isValidNURBS();
}

bool NURBSCurve::isValidNURBS()
{
	// knot vector verification
	bool validU = true;
	for (unsigned int i = 1; i < knotVector.size() && validU == true; i++) if (knotVector[i] < knotVector[i - 1])
	{
		std::cout << "INVALID (unsorted) knotVector.\n";
		validU = false;
		break;
	}
	// size verification
	bool validSize = true;
	if (controlPoints.size() + degree + 1 != knotVector.size())
	{
		std::cout << "INVALID sizes (controlPoints.size() + degree + 1 != knotVector.size()).\n";
		validSize = false;
	}
	return (validU && validSize);
}


bool NURBSCurve::insertKnot(const float newKnot)
{
	// TODO: implement knot insertion with de boor algorithm
	// =====================================================
	int k;
	bool kFound = false;
	for (unsigned int i = 0; i < knotVector.size() && !kFound; i++)
		if (newKnot < knotVector[i])
		{
			k = i - 1;
			kFound = true;

		}

	std::vector<Vec4f> newControlPoints(controlPoints);
	newControlPoints.push_back(Vec4f(0, 0, 0, 0));
	float alpha, zaehler, nenner;
	for (unsigned int i = k - degree + 1; i <= k; i++)
	{
		zaehler = newKnot - knotVector[i];
		nenner = knotVector[i + degree] - knotVector[i];
		if (nenner == 0)
			alpha = 0;
		else
			alpha = zaehler / nenner;
		newControlPoints[i] = (controlPoints[i] * alpha + controlPoints[i - 1] * (1 - alpha));
	}

	for (unsigned int i = k + 1; i <= controlPoints.size(); i++)
		newControlPoints[i] = controlPoints[i - 1];
	knotVector.insert(knotVector.begin() + k + 1, newKnot);
	controlPoints = newControlPoints;

	return true;
}

Vec4f NURBSCurve::evaluteDeBoor(const float t, Vec4f& tangent)
{
	// create a copy of this NURBS curve
	NURBSCurve tempNURBS(*this);
	Vec4f point;
	// TODO: use insertKnot to evaluate the curve and its tangent. Take care to NOT modify this NURBS curve. Instead use the temporary copy.
	// =====================================================================================================================================
	if (t == tempNURBS.getKnotVector().front())
	{
		Vec4f first = tempNURBS.getControlPoints().front();
		Vec4f second = tempNURBS.getControlPoints().at(1);
		tangent = second - first;
		/*tangent.x = first.w * second.x - second.w * first.x;
		tangent.y = first.w * second.y - second.w * first.y;
		tangent.z = first.w * second.z - second.w * first.z;
		tangent.w = first.w * second.w;*/
		return first;
	}
	if (t == tempNURBS.getKnotVector().back())
	{
		Vec4f first = tempNURBS.getControlPoints().at(tempNURBS.getControlPoints().size() - 2);
		Vec4f second = tempNURBS.getControlPoints().back();
		tangent = second - first;
		/*tangent.x = first.w * second.x - second.w * first.x;
		tangent.y = first.w * second.y - second.w * first.y;
		tangent.z = first.w * second.z - second.w * first.z;
		tangent.w = first.w * second.w;*/
		return second;
	}
	float  counter = 0;
	unsigned int k = 0;
	bool kFound = false;
	for (int i = 0; i < tempNURBS.getKnotVector().size(); i++) {
		if (tempNURBS.getKnotVector()[i] == t) {
			counter += 1;
		}
		if (!kFound && t <= tempNURBS.getKnotVector()[i])
		{
			k = i - 1;
			kFound = true;
		}
	}
	for (int i = 0; i < tempNURBS.getDegree() - counter; i++) {
		tempNURBS.insertKnot(t);
	}
	point = tempNURBS.getControlPoints().at(k);
	Vec4f previous = tempNURBS.getControlPoints().at(k - 1);
	tangent = point - previous;
	/*tangent.x = previous.w * point.x - point.w * previous.x;
	tangent.y = previous.w * point.y - point.w * previous.y;
	tangent.z = previous.w * point.z - point.w * previous.z;
	tangent.w = previous.w * point.w;*/
	// =====================================================================================================================================
	return point;
}


std::pair<std::vector<Vec4f>, std::vector<Vec4f>> NURBSCurve::evaluateCurve(const size_t numberSamples)
{
	std::vector<Vec4f> points;
	points.reserve(numberSamples);
	std::vector<Vec4f> tangents;
	tangents.reserve(numberSamples);
	// TODO: implement evaluation of the NURBS curve at 'numberSamples' equidistant points
	// Note: use the evaluteDeBoor(t) function. 
	// ==========================================================================================================
	float begin = knotVector.front();
	float end = knotVector.back();
	float dist = end - begin;
	float step = dist / (numberSamples - 1);
	Vec4f tangent;
	for (int i = 0; i < numberSamples; i++)
	{
		points.push_back(evaluteDeBoor(begin + i * step, tangent));
		tangents.push_back(tangent);
	}
	// ==========================================================================================================

	return std::make_pair(points, tangents);
}

std::ostream& operator<< (std::ostream& os, NURBSCurve& nurbs)
{
	// degree
	os << "NURBS curve, degree " << nurbs.getDegree() << "\n";
	// control points
	os << "  " << nurbs.getControlPoints().size() << " controlPoints:\n";
	for (unsigned int i = 0; i < nurbs.getControlPoints().size(); i++) os << "    " << nurbs.getControlPoints()[i] << "\n";
	// knot vector
	os << "  " << nurbs.getKnotVector().size() << " knotVector: ";
	for (unsigned int i = 0; i < nurbs.getKnotVector().size(); i++) os << nurbs.getKnotVector()[i] << ", ";
	os << "\n";
	// knot vector verification
	nurbs.isValidNURBS();
	return os;
}