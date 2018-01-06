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
std::vector<Vec3f> BezierCurve::getBezierCurvePoints()
{
	std::vector<Vec3f> tmpList;

	for (float t = 0; t <= 1; t += 0.05f) {


		for (int i = 0; i < separateCurveAt(t).first.getControlPoints().size(); i++) {

			for (int k = 0; k < separateCurveAt(t).second.getControlPoints().size(); k++) {
				if (separateCurveAt(t).first.getControlPoints()[i] == separateCurveAt(t).second.getControlPoints()[k]) {
					tmpList.push_back(separateCurveAt(t).first.getControlPoints()[i]);
				}
			}
		}
	}
	return tmpList;
}

std::pair<BezierCurve, BezierCurve> BezierCurve::separateRationalCurveAt(const float t, float w)
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
			/*
			//(1-t0)
			float ttemp = 1.0f - t;

			//(Pi * w)
			Vec3f PiW = cps1[i].operator*(w);

			//(1-t0)*(Pi*w)
			Vec3f temVec = PiW.operator*(ttemp);

			//(Pi+1*w)
			Vec3f PiW2 = cps1[i + 1].operator*(w);

			//(Pi+1*w)*t0
			Vec3f temVec2 = PiW2.operator*(t);

			cps1[i] = temVec.operator+(temVec2);*/
			/*
			Vec3f temp1 = cps1[i].operator*(w).operator*(1.0f - t);
			Vec3f temp2 = cps1[i + 1].operator*(w).operator*(t);

			cps1[i] = temp1.operator+(temp2);
			*/
			
		cps1[i] = (w*cps1[i].operator*(1.0f - t).operator+(w*cps1[i + 1].operator*(t)));
		}
		cps2.push_back(cps1[0]);
	}
	// ==========================================================================================================
	BezierCurve curve1(cps1, rational);
	BezierCurve curve2(cps2, rational);
	return std::pair<BezierCurve, BezierCurve>(curve1, curve2);
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
	
	// TODO: implement the evaluation of the bezier curve and the tangent at t.
	// Note: use the seperateCurveAt(t) function.
	// ==========================================================================================================
	

	for (int i = 0; i <
		separateCurveAt(t).first.getControlPoints().size(); i++) {

		for (int k = 0; k <
			separateCurveAt(t).second.getControlPoints().size(); k++) {
			if (separateCurveAt(t).first.getControlPoints()[i] ==
				separateCurveAt(t).second.getControlPoints()[k]) {
				point = separateCurveAt(t).first.getControlPoints()[i];
			}
		}
	}
	std::cout << tangent ;
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
	// u is between 0 and 1 so I guess it would be best  to calcualte 1 / (numberSamples + 1) and then calcualate the tangent and the points for the calcualted values

	float toBeCalculatedAt = 1 / (numberSamples + 1);



	//TODO : Ich habe leider keine Agnung was mit tangents gemeint ist...
	//calculates points and tangents for numberSamples
	for (float i = 0; i < points.size(); i = i + toBeCalculatedAt) {
		points[i] = evaluateCurveAt(i, Vec3f());

		std::cout << points[i] << std::endl;
	}

	

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