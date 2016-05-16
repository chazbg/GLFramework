#include "Math/GeometryAlgorithm.hpp"
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <queue>
#include <map>
#include <iostream>
#include <sstream>
#include "Math/LineSegment.hpp"
#include "Math/Matrix.hpp"

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

    cout << "Intersecting Ray " << rayPointA.toString() << ", " << rayPointB.toString() << " with segment " << segmentEndA.toString() << ", " << segmentEndB.toString() << endl;
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

    cout << "Intersecting segment " << a.toString() << ", " << b.toString() << " with segment " << c.toString() << ", " << d.toString() << endl;

    if (abs(rs) < EPS && abs(cas) < EPS)
    {
        ////collinear
        //float t0 = (c - a).dot(r) / r.dot(r);
        //float t1 = (c + s - a).dot(r) / r.dot(r);

        //if ((t0 >= 0.0f && t0 <= 1.0f) || (t1 >= 0.0f && t1 <= 1.0f))
        //{
        //    return true;
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
        cout << "U: " << u << ", T:" << t << endl;
        return true;
    }

    return false;
}

bool GeometryAlgorithm::ComputeIntersection2(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& d, Vec2& intersectionPoint, float& t, float& u)
{
    Vec2 r = b - a;
    Vec2 s = d - c;
    float cas = (c - a).perp(s);
    float car = (c - a).perp(r);
    float rs = r.perp(s);
    t = cas / rs;
    u = car / rs;

    cout << "Intersecting segment " << a.toString() << ", " << b.toString() << " with segment " << c.toString() << ", " << d.toString() << endl;

    if (abs(rs) < EPS && abs(cas) < EPS)
    {
        ////collinear
        //float t0 = (c - a).dot(r) / r.dot(r);
        //float t1 = (c + s - a).dot(r) / r.dot(r);

        //if ((t0 >= 0.0f && t0 <= 1.0f) || (t1 >= 0.0f && t1 <= 1.0f))
        //{
        //    return true;
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
        cout << "U: " << u << ", T:" << t << endl;
        return true;
    }

    return false;
}

bool GeometryAlgorithm::ComputePerspectiveIntersection(const Vec3& a, const Vec3& b, const Vec3& c, const Vec3& d, Vec3& fi, Vec3& fj, const Matrix4& mat, const float projCenterZ)
{
    bool res = false;

    Vec4 p1(a.x, a.y, a.z, 1);
    Vec4 p2(b.x, b.y, b.z, 1);
    Vec4 p3(c.x, c.y, c.z, 1);
    Vec4 p4(d.x, d.y, d.z, 1);
    Vec2 intersectionPoint;
    float t = 0;
    float u = 0;
    float t1 = 0;
    float u1 = 0;

    p1 = mat * p1;
    p2 = mat * p2;
    p3 = mat * p3;
    p4 = mat * p4;

    if (ComputeIntersection2(Vec2(p1.x, p1.y), Vec2(p2.x, p2.y), Vec2(p3.x, p3.y), Vec2(p4.x, p4.y), intersectionPoint, t, u))
    {
        res = true;
        t1 = (t * (projCenterZ - a.z)) / (projCenterZ - t * a.z - (1 - t) * b.z);
        u1 = (u * (projCenterZ - c.z)) / (projCenterZ - u * c.z - (1 - u) * d.z);
        fi = (1 - t1) * a + t1 * b;
        fj = (1 - u1) * c + u1 * d;
        cout << fi.toString() << fj.toString() << endl;
    }

    return res;
}

bool GeometryAlgorithm::ComputeParallelIntersection(const Vec3& a, const Vec3& b, const Vec3& c, const Vec3& d, Vec3& fi, Vec3& fj, const Matrix4& mat)
{
    bool res = false;
    Vec4 p1(a.x, a.y, a.z, 1);
    Vec4 p2(b.x, b.y, b.z, 1);
    Vec4 p3(c.x, c.y, c.z, 1);
    Vec4 p4(d.x, d.y, d.z, 1);
    Vec2 intersectionPoint;
    float t = 0;
    float u = 0;

    p1 = mat * p1;
    p2 = mat * p2;
    p3 = mat * p3;
    p4 = mat * p4;
    
    if (ComputeIntersection2(Vec2(p1.x, p1.y), Vec2(p2.x, p2.y), Vec2(p3.x, p3.y), Vec2(p4.x, p4.y), intersectionPoint, t, u))
    {
        res = true;
        fi = (1 - t) * a + t * b;
        fj = (1 - u) * c + u * d;
        cout << fi.toString() << fj.toString() << endl;
    }

    return res;
}

//TODO: Refactor

struct QueueElement
{
    Vec2 point;
    Vec2 otherEnd;
    OrderedLineSegment e1;
    OrderedLineSegment e2;

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

bool above(const std::map<OrderedLineSegment, int>& m, const OrderedLineSegment& s, OrderedLineSegment& a)
{
    std::map<OrderedLineSegment, int>::const_iterator it = m.find(s);
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

bool below(const std::map<OrderedLineSegment, int>& m, const OrderedLineSegment& s, OrderedLineSegment& a)
{
    std::map<OrderedLineSegment, int>::const_iterator it = m.find(s);
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

void removeSegment(std::map<OrderedLineSegment, int>& m, const OrderedLineSegment& s)
{
    m.erase(s);
}

std::vector<Vec2> GeometryAlgorithm::SweepingLineIntersection(const std::vector<std::vector<Vec2>>& inputLines)
{
    std::vector<Vec2> intersections;
    std::priority_queue<QueueElement> points;
    std::map<OrderedLineSegment, int> sl;

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
        OrderedLineSegment segE(e.point, e.otherEnd);
        OrderedLineSegment segA(e.point, e.otherEnd);
        OrderedLineSegment segB(e.point, e.otherEnd);
        
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
            removeSegment(sl, OrderedLineSegment(segE.b, segE.a));
        }
        else
        {
            intersections.push_back(e.point);
            OrderedLineSegment segE(e.point, e.point);
            OrderedLineSegment segE1 = e.e1;
            OrderedLineSegment segE2 = e.e2;

            OrderedLineSegment segAA(e.point, e.otherEnd);
            OrderedLineSegment segBB(e.point, e.otherEnd);
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
    unsigned int k = 2;
    if (p.size() >= 4)
    {
        while (area(p[0], p[1], p[k + 1]) > area(p[0], p[1], p[k]))
        {
            k++;
        }

        antipodPoints.push_back(makePair(p[0], p[k]));

        std::vector<float> f;
        std::vector<float> s;
        for (unsigned int j = 0; j < k; j++)
        {
            Vec2 v2 = p[j + 1] - p[j];
            float angle = atan2(v2.y, v2.x) - atan2(v.y, v.x);
            if (angle < 0.0f)
            {
                angle += 2.0f * 3.14f;
            }
            f.push_back(angle);
        }

        for (unsigned int j = k; j < p.size(); j++)
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
        unsigned int j = 0;
        unsigned int l = k;
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

struct State
{
    LineSegment current[2];
    Vec2 intersectionPoint;
    int current1Index;
    int current2Index;
    int currentContour;
    bool outer;
    std::string toString()
    {
        stringstream s;
        s << intersectionPoint.toString() + ", " + current[0].toString() + ", " + current[1].toString() + ", " << current1Index << ", " << current2Index << ", " << currentContour;
        return  s.str();
    }
};

void next(const std::vector<Vec2>& p1, const std::vector<Vec2>& p2, State& state, const unsigned int index)
{
    if (index == 0)
    {
        state.current1Index++;
        state.current[0] = LineSegment(p1[state.current1Index % p1.size()], p1[(state.current1Index + 1) % p1.size()]);
    }
    else
    {
        state.current2Index++;
        state.current[1] = LineSegment(p2[state.current2Index % p2.size()], p2[(state.current2Index + 1) % p2.size()]);
    }

}
bool firstIntersectionPoint(const std::vector<Vec2>& p1, const std::vector<Vec2>& p2, State& state)
{
    state.outer = 0;
    state.current1Index = 0;
    state.current2Index = 0;
    state.current[0] = LineSegment(p1[0], p1[1]);
    state.current[1] = LineSegment(p2[0], p2[1]);

    bool empty = false;
    for (state.current1Index = 0, state.current2Index = 0; state.current1Index < (int) p1.size() - 1, state.current2Index < (int)p2.size() - 1;)
    {
        if (GeometryAlgorithm::Determinant(state.current[!state.outer].a, state.current[state.outer].a, state.current[!state.outer].b) > 0)
        {
            empty = true;
        }

        if (!GeometryAlgorithm::LineIntersection(state.current[0].a, state.current[0].b, state.current[1].a, state.current[1].b, state.intersectionPoint))
        {
            state.outer = !state.outer;
        }
        else if (!GeometryAlgorithm::ComputeIntersection2(state.current[0].a, state.current[0].b, state.current[1].a, state.current[1].b, state.intersectionPoint))
        {        
        }
        else
        {
            cout << "IntersectionPoint: " << state.intersectionPoint.toString() << endl;
            return true;
        }
        next(p1, p2, state, state.outer);
    }

    return false;
}

bool nextIntersectionPoint(const std::vector<Vec2>& p1, const std::vector<Vec2>& p2, State& state, std::vector<Vec2>& outputPolygon)
{
    bool found = false;

    while (!found && state.current1Index  - 1 < (int) p1.size() && state.current2Index - 1 < (int) p2.size())
    {
        if (!GeometryAlgorithm::LineIntersection(state.current[!state.outer].a, state.current[!state.outer].b, state.current[state.outer].a, state.current[state.outer].b, state.intersectionPoint))
        {
            next(p1, p2, state, state.outer);
        }
        else
        {
            if (!GeometryAlgorithm::ComputeIntersection2(state.current[0].a, state.current[0].b, state.current[1].a, state.current[1].b, state.intersectionPoint))
            {
                outputPolygon.push_back(state.current[!state.outer].b);
                next(p1, p2, state, !state.outer);
            }
            else
            {
                found = true;
                outputPolygon.push_back(state.intersectionPoint);
                state.outer = !state.outer;
                next(p1, p2, state, state.outer);
            }
        }
    }

    return found;
}

std::vector<Vec2> GeometryAlgorithm::IntersectPolygons(const std::vector<Vec2>& p1, const std::vector<Vec2>& p2)
{
    std::vector<Vec2> outputPolygon;
    State state;
    if (firstIntersectionPoint(p1, p2, state))
    {
        cout << state.toString() << endl;
        while (nextIntersectionPoint(p1, p2, state, outputPolygon))
        {
        }
    }
    else
    {
        cout << "Non-intersecting polygons" << endl;
    }
    return outputPolygon;
}

//void intersectInPerspective(cosnt Vec3&)
std::vector<Vec2> GeometryAlgorithm::TestVisibility(const std::vector<Vec2>& p1, const std::vector<Vec2>& p2)
{
    Vec2 intersectionPoint;

    if (!GeometryAlgorithm::ComputeIntersection2(p1[0], p1[1], p2[0], p2[1], intersectionPoint))
    {

    }

    return p1;
}

std::vector<Vec2> GeometryAlgorithm::ProjectPointList(const std::vector<Vec3>& points, const Matrix4& mat)
{
    std::vector<Vec2> projections;

    for (unsigned int i = 0; i < points.size(); i++)
    {
        const Vec4 p(points[i].x, points[i].y, points[i].z, 1);
        Vec4 res = mat * p;
        projections.push_back(Vec2(res.x, res.y));
    }

    return projections;
}

Matrix4 GeometryAlgorithm::CreatePerspectiveMatrix(const float angleOfView, const float aspectRatio, const float zNear, const float zFar)
{
    float f = 1 / tan(angleOfView / 2);
    float yMax = zNear * tan(angleOfView / 2);
    float yMin = -yMax;
    float xMax = yMax * aspectRatio;
    float xMin = yMin * aspectRatio;

    float x = 2 * zNear / (xMax - xMin);
    float y = 2 * zNear / (yMax - yMin);
    float a = (xMax + xMin) / (xMax - xMin);
    float b = (yMax + yMin) / (yMax - yMin);
    float c = -(zFar + zNear) / (zFar - zNear);
    float d = -2 * zFar * zNear / (zFar - zNear);
    return Matrix4(
        Vec4(x, 0.0f, a, 0.0f),
        Vec4(0.0f, y, b, 0.0f),
        Vec4(0.0f, 0.0f, c, d),
        Vec4(0.0f, 0.0f, -1.0f, 0.0f));
}

Matrix4 GeometryAlgorithm::CreateLookAtMatrix(const Vec3& cameraPosition, const Vec3& cameraTarget, const Vec3& cameraUpVector)
{
    Vec3 zAxis = (cameraPosition - cameraTarget).normalize();
    Vec3 xAxis = (cameraUpVector * zAxis).normalize();
    Vec3 yAxis = zAxis * xAxis;

    return Matrix4(
        Vec4(xAxis, -xAxis.dot(cameraPosition)),
        Vec4(yAxis, -yAxis.dot(cameraPosition)),
        Vec4(zAxis, -zAxis.dot(cameraPosition)),
        Vec4(0, 0, 0, 1));
}

Matrix4 GeometryAlgorithm::CreateSRTMatrix(const Vec3& scale, const Vec3& rotationInRadians, const Vec3& translation)
{
    //TODO: include rotation
    return Matrix4(
        Vec4(scale.x, 0, 0, translation.x),
        Vec4(0, scale.y, 0, translation.y),
        Vec4(0, 0, scale.z, translation.z),
        Vec4(0, 0, 0, 1));
}