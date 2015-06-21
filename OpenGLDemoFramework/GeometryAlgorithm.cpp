#include "GeometryAlgorithm.hpp"
#include <cstdio>
#include <cmath>
#include <algorithm>

#define EPS 0.0001f

std::vector<Vec2> GeometryAlgorithm::ConvexHull(const std::vector<Vec2>& inputs)
{
	return inputs;
}

std::vector<Vec2> GeometryAlgorithm::ConvexHullGraham(const std::vector<Vec2>& inputs)
{
	std::vector<Vec2> convexHull;
	std::vector<Vec2> sortedInputs = inputs;
	Vec2 startingPoint = inputs[0];
	sortedInputs.insert(sortedInputs.begin(), Vec2(0,0));

	//find starting point
	for (unsigned int i = 0; i < inputs.size(); i++)
	{
		if ((inputs[i].y < startingPoint.y) || (inputs[i].y == startingPoint.y && inputs[i].x < startingPoint.x))
		{
			startingPoint = inputs[i];
			std::iter_swap(sortedInputs.begin() + 1, sortedInputs.begin() + i + 1);
		}
	}

	struct less_than_key
	{
	private:
		const Vec2& startingPoint;
	public:
		less_than_key(const Vec2& startingPoint) : startingPoint(startingPoint) {}
		inline bool operator() (const Vec2& u, const Vec2& v)
		{
			printf("-------------\n");
			printf("%s, %s, angle=%f\n", startingPoint.toString().c_str(), u.toString().c_str(), startingPoint.polarAngle(u));
			printf("%s, %s, angle=%f\n", startingPoint.toString().c_str(), v.toString().c_str(), startingPoint.polarAngle(v));
			return startingPoint.polarAngle(u) < startingPoint.polarAngle(v);
		}
	};

	std::sort(sortedInputs.begin() + 2, sortedInputs.end(), less_than_key(startingPoint));
	sortedInputs[0] = sortedInputs[sortedInputs.size() - 1];

	int M = 1;
	for (unsigned int i = 1; i < sortedInputs.size(); i++)
	{
		while (Determinant(sortedInputs[M - 1], sortedInputs[M], sortedInputs[i]) <= 0)
		{
			if (M > 1)
			{
				M--;
			}
			else if (i == sortedInputs.size())
			{
				break;
			}
			else
			{
				i++;
			}
		}

		M++;

		Vec2 tmp = sortedInputs[M];
		sortedInputs[M] = sortedInputs[i];
		sortedInputs[i] = tmp;
	}
		
	for (unsigned int i = 1; i <= M; i++)
	{
		convexHull.push_back(sortedInputs[i]);
	}

	return convexHull;
}

bool GeometryAlgorithm::PointInPolygon(const Vec2& inputPoint, const std::vector<Vec2>& inputPolygon)
{
	int l = 0;
	int i = 0;
	bool res = false;
	Vec2 lineA(-1, inputPoint.y);
	Vec2 lineB(0, inputPoint.y);

	for (unsigned int i = 0; i < inputPolygon.size(); i++)
	{
		const Vec2& a = inputPolygon[i];
		const Vec2& b = inputPolygon[(i + 1) % inputPolygon.size()];

		if (PointInLine(inputPoint, a, b))
		{
			res = true;
			printf("%s is a contour point\n", inputPoint.toString().c_str());
			break;
		}
		else if (a.y != b.y)
		{
			Vec2 intersectionPoint;
			if (LineIntersection(lineA, lineB, a, b, intersectionPoint) && intersectionPoint.x < inputPoint.x)
			{
				l++;
			}
		}
	}

	if (l % 2 == 1)
	{
		res = true;
	}

	return res;
}

bool GeometryAlgorithm::PointInLine(const Vec2& inputPoint, const Vec2& lineEndA, const Vec2& lineEndB)
{
	float t;
	bool res = false;

	if (lineEndA.x == lineEndB.x)
	{
		if (inputPoint.x == lineEndA.x)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	if (lineEndA.y == lineEndB.y)
	{
		if (inputPoint.y == lineEndA.y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	if (inputPoint.x != lineEndB.x)
	{
		t = (inputPoint.x - lineEndB.x) / (lineEndA.x - lineEndB.x);
		printf("%s, %s, %s\n", inputPoint.toString().c_str(), lineEndA.toString().c_str(), lineEndB.toString().c_str());
		if (t >= 0.0f && t <= 1.0f)
		{
			if (lineEndA.y * t + lineEndB.y * (1 - t) == inputPoint.y)
			{
				res = true;
			}
		}
	}
	else if (inputPoint.y != lineEndB.y)
	{
		printf("%s, %s, %s\n", inputPoint.toString().c_str(), lineEndA.toString().c_str(), lineEndB.toString().c_str());
		t = (inputPoint.y - lineEndB.y) / (lineEndA.y - lineEndB.y);
		printf("t = %f\n", t);

		if (t >= 0.0f && t <= 1.0f)
		{
			res = true;
		}
	}

	return res;
}

bool GeometryAlgorithm::LineIntersection(const Vec2& rayPointA, 
	const Vec2& rayPointB, 
	const Vec2& segmentEndA, 
	const Vec2& segmentEndB,
	Vec2& intersectionPoint)
{
	bool res = false;
	Vec2 u = rayPointB - rayPointA;
	Vec2 v = segmentEndB - segmentEndA;
	Vec2 w = rayPointA - segmentEndA;
	float d = u.perp(v);

	if (abs(d) >= EPS)
	{
		float tI = u.perp(w) / d;
		
		if (tI >= 0.0f && tI <= 1.0f)
		{
			res = true;
			intersectionPoint = segmentEndA + tI * v;
		}
	}

	return res;
}

float GeometryAlgorithm::Determinant(const Vec2& a, const Vec2& b, const Vec2& c)
{
	return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

std::vector<Vec2> GeometryAlgorithm::Clip(const std::vector<Vec2>& inputPolygon, const std::vector<Vec2>& clippingPolygon)
{
	std::vector<Vec2> outputPolygon = inputPolygon;

	for (unsigned int i = 0; i < clippingPolygon.size() - 1; i++)
	{
		std::vector<Vec2> inputPolygon = outputPolygon;

		printf("-------------------\n");

		for (unsigned int z = 0; z < outputPolygon.size(); z++)
		{
			printf("%s\n", outputPolygon[z].toString().c_str());
		}

		printf("-------------------\n");

		outputPolygon.clear();
		Vec2 end = inputPolygon[inputPolygon.size() - 1];
		for (unsigned int j = 0; j < inputPolygon.size(); j++)
		{
			if (Determinant(clippingPolygon[i], inputPolygon[j], clippingPolygon[i + 1]) > 0)
			{
				if (Determinant(clippingPolygon[i], end, clippingPolygon[i + 1]) < 0)
				{
					outputPolygon.push_back(ComputeIntersection2(end, inputPolygon[j], clippingPolygon[i], clippingPolygon[i + 1]));
				}
				outputPolygon.push_back(inputPolygon[j]);
			}
			else if (Determinant(clippingPolygon[i], end, clippingPolygon[i + 1]) > 0)
			{
				outputPolygon.push_back(ComputeIntersection2(end, inputPolygon[j], clippingPolygon[i], clippingPolygon[i + 1]));
			}

			end = inputPolygon[j];
		}
	}

	return outputPolygon;
}

bool GeometryAlgorithm::ComputeIntersection(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& d, Vec2& intersectionPoint)
{
	Vec2 r = b - a;
	Vec2 s = d - c;
	float cas = (c - a).perp(s);
	float acr = (a - c).perp(r);
	float rs = r.perp(s);
	float t = cas / rs;
	float u = acr / rs;

	if (abs(rs) < EPS && abs(cas) < EPS)
	{
		////collinear
		//float t0 = (c - a).dot(r) / r.dot(r);
		//float t1 = (c + s - a).dot(r) / r.dot(r);

		//if ((t0 >= 0.0f && t0 <= 1.0f) || (t1 >= 0.0f && t1 <= 1.0f))
		//{
		//	return true;
		//}
		
		return false;
	}

	if (abs(rs) < EPS)
	{
		return false;
	}

	if (0 < t && t < 1 && 0 < u && u < 1)
	{
		intersectionPoint = a + t * r;
		return true;
	}

	return false;
}

Vec2 GeometryAlgorithm::ComputeIntersection2(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& d)
{
	Vec2 intersectionPoint;
	Vec2 r = b - a;
	Vec2 s = d - c;
	float cas = (c - a).perp(s);
	float acr = (a - c).perp(r);
	float rs = r.perp(s);
	float t = cas / rs;
	float u = acr / rs;

	if (0 < t && t < 1 && 0 < u && u < 1)
	{
		intersectionPoint = a + t * r;
	}

	return intersectionPoint;
}