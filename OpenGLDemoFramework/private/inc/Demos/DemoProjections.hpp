#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Math/GeometryAlgorithm.hpp"
#include "Geometry/LineListMesh.hpp"
#include "Math/LineSegment.hpp"
#include "Geometry/PointListMesh.hpp"
#include "Math/Matrix.hpp"
#include <iostream>

namespace PRJDemo
{
    class TestWindowApp : public IApplication
    {
    public:
        TestWindowApp() : renderer(0) {}
        ~TestWindowApp() {}
        virtual void onInit()
        {
            renderer = new Renderer();
            renderer->setDepthTest(false);

            int demoIndex = 2;
            std::vector<Vec3> points;
            Matrix4 projectionMat;
            std::vector<std::vector<LineSegment3>> sides;

            if (demoIndex == 1)
            {
                projectionMat = Matrix4(Vec4(1, 0, 0, 1), Vec4(0, 1, 0, 1), Vec4(0, 0, 0, 0), Vec4(0, 0, 1, 2));
                points.push_back(Vec3(0, 0, 0));
                points.push_back(Vec3(2, 0, 0));
                points.push_back(Vec3(2, 0, 2));
                points.push_back(Vec3(0, 0, 2));
                points.push_back(Vec3(0, 2, 0));
                points.push_back(Vec3(2, 2, 0));
                points.push_back(Vec3(2, 2, 2));
                points.push_back(Vec3(0, 2, 2));
                sides = initCubeSides(points);
            }
            else
            {
                projectionMat = Matrix4(Vec4(1, 0, -0.35f, 1), Vec4(0, 1, -0.35f, 1), Vec4(0, 0, 0, 0), Vec4(0, 0, 0, 1));
                points.push_back(Vec3(0, 0, 0));
                points.push_back(Vec3(2, 0, 0));
                points.push_back(Vec3(2, 0, 2));
                points.push_back(Vec3(0, 0, 2));
                points.push_back(Vec3(1, 2.4f, 1));
                points.push_back(Vec3(1, -2.4f, 1));
                sides = initOctaederSides(points);
            }
            
            std::vector<Vec3> outPoints;
            std::vector<Vec3> invisPoints;
            std::vector<Vec2> projections = GeometryAlgorithm::ProjectPointList(points, projectionMat);
            std::vector<Vec2> convexHull = GeometryAlgorithm::ConvexHullGraham(projections);
            std::vector<Vec3> convexHullOrigins = getConvexHullOrigins(points, projections, convexHull);

            for (unsigned int i = 0; i < sides.size(); i++)
            {
                for (unsigned int j = 0; j < sides.size(); j++)
                {
                    if (i == j)    continue;

                    for (unsigned int k = 0; k < sides[i].size(); k++)
                    {
                        for (unsigned int l = 0; l < sides[j].size(); l++)
                        {
                            if (sides[i][k] == sides[j][l]) continue;

                            Vec3 fi, fj;
                            bool intersects;
                            if (demoIndex == 1)
                            {
                                intersects = GeometryAlgorithm::ComputePerspectiveIntersection(sides[i][k].a, sides[i][k].b, sides[j][l].a, sides[j][l].b, fi, fj, projectionMat, -2);
                            }
                            else
                            {
                                intersects = GeometryAlgorithm::ComputeParallelIntersection(sides[i][k].a, sides[i][k].b, sides[j][l].a, sides[j][l].b, fi, fj, projectionMat);
                            }
                            if (intersects)
                            {
                                if (!alreadyAdded(outPoints, fi)) { outPoints.push_back(fi); }
                                if (!alreadyAdded(outPoints, fj)) { outPoints.push_back(fj); }

                                unsigned int side = fi.z > fj.z ? i : j;

                                for (unsigned int m = 0; m < sides[side].size(); m++)
                                {
                                    if (!isContourPoint(convexHullOrigins, sides[side][m].a)) { invisPoints.push_back(sides[side][m].a); }
                                    if (!isContourPoint(convexHullOrigins, sides[side][m].b)) { invisPoints.push_back(sides[side][m].b); }
                                    sides[side][m] = LineSegment3(Vec3(0, 0, 0), Vec3(0, 0, 0));
                                }
                            }
                        }
                    }
                }
            }

            if (demoIndex == 2)
            {
                removeInvisibleEdges(sides, invisPoints);
            }

            for (unsigned int i = 0; i < sides.size(); i++)
            {
                std::vector<Vec3> lines;
                lines.push_back(sides[i][0].a);
                lines.push_back(sides[i][0].b);
                lines.push_back(sides[i][1].a);
                lines.push_back(sides[i][1].b);
                lines.push_back(sides[i][2].a);
                lines.push_back(sides[i][2].b);

                if (demoIndex == 1)
                {
                    lines.push_back(sides[i][3].a);
                    lines.push_back(sides[i][3].b);
                }

                lm1.push_back(new LineListMesh(lines, Vec3(0, 1, 1), 2.0f));
            }

            plm = new PointListMesh(outPoints, Vec3(1, 1, 0));
        }
        virtual void onUpdate(const unsigned int deltaTime) {}
        virtual void onRender(const unsigned int deltaTime)
        {
            renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

            for (unsigned int i = 0; i < lm1.size(); i++)
            {
                lm1[i]->Render();
            }

            plm->Render();
        }

        virtual void onDestroy()
        {
            delete renderer;
            for (unsigned int i = 0; i < lm1.size(); i++)
            {
                delete lm1[i];
            }
            delete plm;
        }
        virtual void onEvent(const unsigned int event) { cout << "onEvent: " << event << endl; }
        virtual void onMouseEvent(int button, int state, int x, int y) { cout << button << " " << state << " " << x << " " << y << endl; }
        virtual void onKeyboardEvent(unsigned char c, int x, int y) { cout << c << " " << x << " " << y << endl; }
    private:
        Renderer* renderer;
        std::vector<LineListMesh*> lm1;
        PointListMesh* plm;

        std::vector<std::vector<LineSegment3>> initCubeSides(const std::vector<Vec3>& p)
        {
            std::vector<std::vector<LineSegment3>> sides;
            for (unsigned int i = 0; i < 6; i++)
            {
                sides.push_back(std::vector<LineSegment3>());
            }

            sides[0].push_back(LineSegment3(p[0], p[1]));
            sides[0].push_back(LineSegment3(p[1], p[2]));
            sides[0].push_back(LineSegment3(p[2], p[3]));
            sides[0].push_back(LineSegment3(p[3], p[0]));

            sides[1].push_back(LineSegment3(p[0], p[1]));
            sides[1].push_back(LineSegment3(p[1], p[5]));
            sides[1].push_back(LineSegment3(p[5], p[4]));
            sides[1].push_back(LineSegment3(p[4], p[0]));

            sides[2].push_back(LineSegment3(p[0], p[3]));
            sides[2].push_back(LineSegment3(p[3], p[7]));
            sides[2].push_back(LineSegment3(p[7], p[4]));
            sides[2].push_back(LineSegment3(p[4], p[0]));

            sides[3].push_back(LineSegment3(p[2], p[1]));
            sides[3].push_back(LineSegment3(p[1], p[5]));
            sides[3].push_back(LineSegment3(p[5], p[6]));
            sides[3].push_back(LineSegment3(p[6], p[2]));

            sides[4].push_back(LineSegment3(p[2], p[3]));
            sides[4].push_back(LineSegment3(p[3], p[7]));
            sides[4].push_back(LineSegment3(p[7], p[6]));
            sides[4].push_back(LineSegment3(p[6], p[2]));

            sides[5].push_back(LineSegment3(p[4], p[5]));
            sides[5].push_back(LineSegment3(p[5], p[6]));
            sides[5].push_back(LineSegment3(p[6], p[7]));
            sides[5].push_back(LineSegment3(p[7], p[4]));

            return sides;
        }

        std::vector<std::vector<LineSegment3>> initOctaederSides(const std::vector<Vec3>& p)
        {
            std::vector<std::vector<LineSegment3>> sides;
            for (unsigned int i = 0; i < 8; i++)
            {
                sides.push_back(std::vector<LineSegment3>());
            }

            sides[0].push_back(LineSegment3(p[0], p[1]));
            sides[0].push_back(LineSegment3(p[0], p[4]));
            sides[0].push_back(LineSegment3(p[1], p[4]));

            sides[1].push_back(LineSegment3(p[0], p[3]));
            sides[1].push_back(LineSegment3(p[0], p[4]));
            sides[1].push_back(LineSegment3(p[3], p[4]));

            sides[2].push_back(LineSegment3(p[0], p[1]));
            sides[2].push_back(LineSegment3(p[0], p[5]));
            sides[2].push_back(LineSegment3(p[1], p[5]));

            sides[3].push_back(LineSegment3(p[0], p[3]));
            sides[3].push_back(LineSegment3(p[0], p[5]));
            sides[3].push_back(LineSegment3(p[3], p[5]));

            sides[4].push_back(LineSegment3(p[2], p[1]));
            sides[4].push_back(LineSegment3(p[2], p[4]));
            sides[4].push_back(LineSegment3(p[1], p[4]));

            sides[5].push_back(LineSegment3(p[2], p[3]));
            sides[5].push_back(LineSegment3(p[2], p[4]));
            sides[5].push_back(LineSegment3(p[3], p[4]));

            sides[6].push_back(LineSegment3(p[2], p[1]));
            sides[6].push_back(LineSegment3(p[2], p[5]));
            sides[6].push_back(LineSegment3(p[1], p[5]));

            sides[7].push_back(LineSegment3(p[2], p[3]));
            sides[7].push_back(LineSegment3(p[2], p[5]));
            sides[7].push_back(LineSegment3(p[3], p[5]));

            return sides;
        }

        std::vector<Vec3> getConvexHullOrigins(const std::vector<Vec3>& points, const std::vector<Vec2>& projections, const std::vector<Vec2>& convexHull)
        {
            std::vector<Vec3> origins;

            for (unsigned int i = 0; i < points.size(); i++)
            {
                if (std::find(convexHull.begin(), convexHull.end(), projections[i]) != convexHull.end())
                {
                    origins.push_back(points[i]);
                }
            }

            return origins;
        }

        void removeInvisibleEdges(std::vector<std::vector<LineSegment3>>& sides, std::vector<Vec3> invisPoints)
        {
            for (unsigned int i = 0; i < sides.size(); i++)
            {
                for (unsigned int j = 0; j < sides[i].size(); j++)
                {
                    if (std::find(invisPoints.begin(), invisPoints.end(), sides[i][j].a) != invisPoints.end() ||
                        std::find(invisPoints.begin(), invisPoints.end(), sides[i][j].b) != invisPoints.end())
                    {
                        sides[i][j] = LineSegment3(Vec3(0, 0, 0), Vec3(0, 0, 0));
                    }
                }
            }
        }

        bool isContourPoint(const std::vector<Vec3>& contour, const Vec3& point)
        {
            return std::find(contour.begin(), contour.end(), point) != contour.end();
        }

        bool alreadyAdded(const std::vector<Vec3>& points, const Vec3& point)
        {
            return std::find(points.begin(), points.end(), point) != points.end();
        }
    };

    void main()
    {
        WindowParameters params;
        params.width = 800;
        params.height = 800;
        params.posX = 100;
        params.posY = 100;
        params.name = "TestWindow";
        TestWindowApp app;
        Window window(params, app);
        window.startRenderLoop();
    }
}