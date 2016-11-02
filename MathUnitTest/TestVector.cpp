#include "stdafx.h"
#include "CppUnitTest.h"
#include "Math/Vector.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define EPS 0.0000001f

namespace MathUnitTest
{		
	TEST_CLASS(TestVec2)
	{
	public:
		
		TEST_METHOD(TestVec2Multiply)
		{
            Vec2 v(2.0f, 3.0f);
            v = 3.0f * v;
            Assert::AreEqual(6.0f, v.x, EPS);
            Assert::AreEqual(9.0f, v.y, EPS);
		}

        TEST_METHOD(TestVec2Length)
        {
            Vec2 v(3.0f, 4.0f);

            Assert::AreEqual(5.0f, v.length(), EPS);
            Assert::AreEqual(1.0f, v.normalize().length(), EPS);
        }

        //TODO: Add tests for the rest of the operations
	};

    TEST_CLASS(TestVec3)
    {
    public:

        TEST_METHOD(TestVec3Multiply)
        {
            Vec3 v(2.0f, 3.0f, 1.0f);
            
            v = 3.0f * v;
            Assert::AreEqual(6.0f, v.x, EPS);
            Assert::AreEqual(9.0f, v.y, EPS);
            Assert::AreEqual(3.0f, v.z, EPS);

            v = v / 3.0f;
            Assert::AreEqual(2.0f, v.x, EPS);
            Assert::AreEqual(3.0f, v.y, EPS);
            Assert::AreEqual(1.0f, v.z, EPS);

            v *= 1.5f;
            Assert::AreEqual(3.0f, v.x, EPS);
            Assert::AreEqual(4.5f, v.y, EPS);
            Assert::AreEqual(1.5f, v.z, EPS);

            v /= 1.5f;
            Assert::AreEqual(2.0f, v.x, EPS);
            Assert::AreEqual(3.0f, v.y, EPS);
            Assert::AreEqual(1.0f, v.z, EPS);
        }

        TEST_METHOD(TestVec3Length)
        {
            Vec3 v(3.0f, 4.0f, 2.0f);

            Assert::AreEqual(sqrt(29.0f), v.length(), EPS);
            Assert::AreEqual(1.0f, v.normalize().length(), EPS);
        }

        //TODO: Add tests for the rest of the operations
    };

    TEST_CLASS(TestVec4)
    {
    public:

        TEST_METHOD(TestVec4Multiply)
        {
            Vec4 v(2.0f, 3.0f, 1.0f, 4.0f);

            v = 3.0f * v;
            Assert::AreEqual(6.0f, v.x, EPS);
            Assert::AreEqual(9.0f, v.y, EPS);
            Assert::AreEqual(3.0f, v.z, EPS);
            Assert::AreEqual(12.0f, v.w, EPS);

            v = v / 3.0f;
            Assert::AreEqual(2.0f, v.x, EPS);
            Assert::AreEqual(3.0f, v.y, EPS);
            Assert::AreEqual(1.0f, v.z, EPS);
            Assert::AreEqual(4.0f, v.w, EPS);

            v *= 1.5f;
            Assert::AreEqual(3.0f, v.x, EPS);
            Assert::AreEqual(4.5f, v.y, EPS);
            Assert::AreEqual(1.5f, v.z, EPS);
            Assert::AreEqual(6.0f, v.w, EPS);

            v /= 1.5f;
            Assert::AreEqual(2.0f, v.x, EPS);
            Assert::AreEqual(3.0f, v.y, EPS);
            Assert::AreEqual(1.0f, v.z, EPS);
            Assert::AreEqual(4.0f, v.w, EPS);
        }

        TEST_METHOD(TestVec4Length)
        {
            Vec4 v(3.0f, 4.0f, 2.0f, 1.0f);

            Assert::AreEqual(sqrt(30.0f), v.length(), EPS);
            Assert::AreEqual(1.0f, v.normalize().length(), EPS);
        }

        //TODO: Add tests for the rest of the operations
    };
}