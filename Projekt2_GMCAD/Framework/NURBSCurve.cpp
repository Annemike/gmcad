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
	for (unsigned int i = 1; i < knotVector.size() && validU == true; i++) if (knotVector[i] < knotVector[i-1])
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
		newControlPoints.push_back(Vec4f(0,0,0,0));
	float alpha, zaehler, nenner;
	for (unsigned int i = k - degree + 1; i < k; i++)
	{
		zaehler = newKnot - knotVector[i];
		nenner = knotVector[i + degree] - knotVector[i];
		if (nenner == 0)
			alpha = 0;
		else
			alpha = zaehler / nenner;
		newControlPoints[i]=(controlPoints[i] * alpha + controlPoints[i - 1] * (1 - alpha));
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
	float  counter = 0;
	for (int i = 0; i < tempNURBS.getKnotVector().size(); i++) {
		if (tempNURBS.getKnotVector()[i] == t) {
			counter += 1;
		}
	}
	for (int i = 0; i < tempNURBS.getDegree()-counter+1; i++) {
		tempNURBS.insertKnot(t);
		
	}
	for (int i = 0; i < tempNURBS.getControlPoints().size(); i++) {
		std::cout << tempNURBS.getControlPoints()[i] << std::endl;

	}
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


	// ==========================================================================================================

	return std::make_pair(points,tangents);
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