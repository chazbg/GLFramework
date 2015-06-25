#include "GeometryAlgorithm.hpp"
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <queue>
#include <map>
#include <iostream>
using namespace std;

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
		
	for (int i = 1; i <= M; i++)
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
	unsigned int size = clippingPolygon.size();

	for (unsigned int i = 0; i < size; i++)
	{
		std::vector<Vec2> inputPolygon = outputPolygon;

		outputPolygon.clear();
		Vec2 end = inputPolygon[inputPolygon.size() - 1];
		for (unsigned int j = 0; j < inputPolygon.size(); j++)
		{
			if (Determinant(clippingPolygon[i], inputPolygon[j], clippingPolygon[(i + 1) % size]) < 0)
			{
				if (Determinant(clippingPolygon[i], end, clippingPolygon[(i + 1) % size]) > 0)
				{
					outputPolygon.push_back(ComputeIntersection(clippingPolygon[i], clippingPolygon[(i + 1) % size], end, inputPolygon[j]));
				}
				outputPolygon.push_back(inputPolygon[j]);
			}
			else if (Determinant(clippingPolygon[i], end, clippingPolygon[(i + 1) % size]) < 0)
			{
				outputPolygon.push_back(ComputeIntersection(clippingPolygon[i], clippingPolygon[(i + 1) % size], end, inputPolygon[j]));
			}

			end = inputPolygon[j];
		}
	}

	return outputPolygon;
}

Vec2 GeometryAlgorithm::ComputeIntersection(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& d)
{
	Vec2 intersectionPoint;
	Vec2 r = b - a;
	Vec2 s = d - c;
	float cas = (c - a).perp(s);
	float rs = r.perp(s);
	float t = cas / rs;

	intersectionPoint = a + t * r;

	return intersectionPoint;
}

bool GeometryAlgorithm::ComputeIntersection2(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& d, Vec2& intersectionPoint)
{
	Vec2 r = b - a;
	Vec2 s = d - c;
	float cas = (c - a).perp(s);
	float car = (c - a).perp(r);
	float rs = r.perp(s);
	float t = cas / rs;
	float u = car / rs;

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

	if ((0.0f < t && t < 1.0f) && (0.0f < u && u < 1.0f))
	{
		intersectionPoint = a + t * r;
		cout << "Intersection point: " << intersectionPoint.toString() << endl;
		return true;
	}

	return false;
}

bool onLineSegment(const Vec2& point, const Vec2& lineEndA, const Vec2& lineEndB)
{
	return true;
}

//TODO: Refactor
struct LineSegment {
	LineSegment()
	{

	}

	LineSegment(Vec2 a, Vec2 b) : a(a), b(b)
	{

	}
	bool operator==(const LineSegment& rhs) const { return  a == rhs.a && b == rhs.b; }
	bool operator<(const LineSegment& rhs) const { return a.y < rhs.a.y; }

	Vec2 a;
	Vec2 b;
};

struct QueueElement
{
	Vec2 point;
	Vec2 otherEnd;
	LineSegment e1;
	LineSegment e2;

	bool operator==(const QueueElement& rhs) const { return point == rhs.point; }
	bool operator<(const QueueElement& rhs) const { return (point.x > rhs.point.x) || (point.x == rhs.point.x && point.y > rhs.point.y); }
};

bool isLeftEndPoint(const QueueElement& el)
{
	return (el.point.x < el.otherEnd.x) || (el.point.x == el.otherEnd.x && el.point.y < el.otherEnd.y);
}

bool isRightPoint(const QueueElement& el)
{
	return el.point != el.otherEnd;
}

bool above(const std::map<LineSegment, int>& m, const LineSegment& s, LineSegment& a)
{
	std::map<LineSegment, int>::const_iterator it = m.find(s);
	if (it == m.begin())
	{
		return false;
	}
	else
	{
		it--;
		a = it->first;
		return true;
	}
}

bool below(const std::map<LineSegment, int>& m, const LineSegment& s, LineSegment& a)
{
	std::map<LineSegment, int>::const_iterator it = m.find(s);
	if (it == m.end())
	{
		return false;
	}
	else
	{
		it++;
		if (it == m.end())
		{
			return false;
		}
		a = it->first;
		return true;
	}
}

void removeSegment(std::map<LineSegment, int>& m, const LineSegment& s)
{
	m.erase(s);
}

std::vector<Vec2> GeometryAlgorithm::SweepingLineIntersection(const std::vector<std::vector<Vec2>>& inputLines)
{
	std::vector<Vec2> intersections;
	std::priority_queue<QueueElement> points;
	std::map<LineSegment, int> sl;

	for (unsigned int i = 0; i < inputLines.size(); i++)
	{
		QueueElement element1, element2;
		element1.point = inputLines[i][0];
		element1.otherEnd = inputLines[i][1];
		element2.point = inputLines[i][1];
		element2.otherEnd = inputLines[i][0];
		points.push(element1);
		points.push(element2);
	}

	while (!points.empty())
	{
		QueueElement e = points.top();
		cout << e.point.toString() << endl;
		LineSegment segE(e.point, e.otherEnd);
		LineSegment segA(e.point, e.otherEnd);
		LineSegment segB(e.point, e.otherEnd);
		
		if (isLeftEndPoint(e))
		{
			int slSize = sl.size();
			sl[segE] = 0;
			if (above(sl, segE, segA))
			{
				Vec2 intersectionPoint;
				if (ComputeIntersection2(segE.a, segE.b, segA.a, segA.b, intersectionPoint))
				{
					QueueElement i;
					i.point = intersectionPoint;
					i.otherEnd = intersectionPoint;
					i.e1 = segE;
					i.e2 = segA;
					points.push(i);
				}
			}

			if (below(sl, segE, segB))
			{
				Vec2 intersectionPoint;
				if (ComputeIntersection2(segE.a, segE.b, segB.a, segB.b, intersectionPoint))
				{
					QueueElement i;
					i.point = intersectionPoint;
					i.otherEnd = intersectionPoint;
					i.e1 = segE;
					i.e2 = segB;
					points.push(i);
				}
			}
		}
		else if (isRightPoint(e))
		{
			if (above(sl, segE, segA) && 
				below(sl, segE, segB))
			{
				Vec2 intersectionPoint;
				if (ComputeIntersection2(segA.a, segA.b, segB.a, segB.b, intersectionPoint))
				{
					QueueElement i;
					i.point = intersectionPoint;
					i.otherEnd = intersectionPoint;
					i.e1 = segA;
					i.e2 = segB;
					points.push(i);
				}
			}
			removeSegment(sl, LineSegment(segE.b, segE.a));
		}
		else
		{
			intersections.push_back(e.point);
			LineSegment segE(e.point, e.point);
			LineSegment segE1 = e.e1;
			LineSegment segE2 = e.e2;

			LineSegment segAA(e.point, e.otherEnd);
			LineSegment segBB(e.point, e.otherEnd);
			if (above(sl, segE1, segAA))
			{
				Vec2 intersectionPoint;
				if (ComputeIntersection2(segE2.a, segE2.b, segAA.a, segAA.b, intersectionPoint))
				{
					QueueElement i;
					i.point = intersectionPoint;
					i.otherEnd = intersectionPoint;
					i.e1 = segE2;
					i.e2 = segAA;
					points.push(i);
				}
			}

			if (below(sl, segE2, segBB))
			{
				Vec2 intersectionPoint;
				if (ComputeIntersection2(segE1.a, segE1.b, segBB.a, segBB.b, intersectionPoint))
				{
					QueueElement i;
					i.point = intersectionPoint;
					i.otherEnd = intersectionPoint;
					i.e1 = segE1;
					i.e2 = segBB;
					points.push(i);
				}
			}
		}

		points.pop();
	}

	return intersections;
}

float angle(const std::vector<Vec2>& p, int m, int n)
{
	Vec2 vm = p[m + 1] - p[m];
	Vec2 vn = p[n + 1] - p[n];
	float a = atan2(vn.y, vn.x) - atan2(vm.y, vm.x);
	if (a < 0)
	{
		a += 2 * 3.14f;
	}
	cout << "Angle:" << a << endl;
	return a;
}

float area(const Vec2& a, const Vec2& b, const Vec2& c)
{
	return abs(a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
}

std::vector<Vec2> makePair(const Vec2& a, const Vec2& b)
{
	std::vector<Vec2> pair;
	pair.push_back(a);
	pair.push_back(b);
	return pair;
}

std::vector<std::vector<Vec2>> GeometryAlgorithm::RotatingCalipers(const std::vector<Vec2>& p)
{
	std::vector<std::vector<Vec2>> antipodPoints;

	Vec2 v = p[1] - p[0];
	int k = 2;
	if (p.size() >= 4)
	{
		while (area(p[0], p[1], p[k + 1]) > area(p[0], p[1], p[k]))
		{
			k++;
		}

		antipodPoints.push_back(makePair(p[0], p[k]));

		std::vector<float> f;
		std::vector<float> s;
		for (int j = 0; j < k; j++)
		{
			Vec2 v2 = p[j + 1] - p[j];
			float angle = atan2(v2.y, v2.x) - atan2(v.y, v.x);
			if (angle < 0.0f)
			{
				angle += 2.0f * 3.14f;
			}
			f.push_back(angle);
		}

		for (int j = k; j < p.size(); j++)
		{
			Vec2 v2 = p[(j + 1) % p.size()] - p[j];
			float angle = atan2(v2.y, v2.x) - atan2(-v.y, -v.x);
			if (angle < 0.0f)
			{
				angle += 2.0f * 3.14f;
			}
			s.push_back(angle);
		}

		f.push_back(s[0] + 3.14f);
		int j = 0;
		int l = k;
		while (j < k && l < p.size())
		{
			if (f[j] < s[l - k])
			{
				j++;
			}
			else if (s[l - k] < f[j])
			{
				l++;
			}
			else
			{
				j++;
				l++;
				antipodPoints.push_back(makePair(p[j - 1], p[l]));
				antipodPoints.push_back(makePair(p[j], p[l - 1]));
			}
			antipodPoints.push_back(makePair(p[j], p[l]));
		}
	}

	return antipodPoints;
}

std::vector<Vec2> IntersectPolygons(const std::vector<Vec2>& inputPolygon1, const std::vector<Vec2>& inputPolygon2)
{
	std::vector<Vec2> outputPolygon;
	return outputPolygon;
}