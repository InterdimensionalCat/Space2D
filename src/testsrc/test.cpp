#include <iostream>
#include <map>
#include <functional>
#include <cassert>
#include "Space2D.h"
#include "gtest/gtest.h"


using namespace s2d;

TEST(VecTest, VecConstructor) {
	Vec2f v1;
	Vec2f v2(1.0f, 3.0f);
	Vec2f v3(Point2f(1.0f, 2.0f));
	Vec2f v4(Point2f(1.0f, 2.0f), Point2f(1.5f, 1.0f));

	ASSERT_EQ(v1.x, 0.0f);
	ASSERT_EQ(v1.y, 0.0f);

	ASSERT_EQ(v2.x, 1.0f);
	ASSERT_EQ(v2.y, 3.0f);
	ASSERT_EQ(v3.x, 1.0f);
	ASSERT_EQ(v3.y, 2.0f);
	ASSERT_EQ(v4.x, 0.5f);
	ASSERT_EQ(v4.y, -1.0f);
}

TEST(VecTest, VecItr) {

	Vec2f v1(0.0f, -3.6f);

	ASSERT_EQ(v1.size(), 2);

	ASSERT_EQ(v1[0], 0.0f);
	ASSERT_EQ(v1[1], -3.6f);

	size_t counter = 0;

	for (auto it = v1.begin(); it != v1.end(); it++, counter++) {
		ASSERT_EQ(*it, v1[counter]);
	}

	std::for_each(v1.begin(), v1.end(), [](auto& a) {
		a = -1.0f;
		}
	);

	ASSERT_EQ(v1.x, -1.0f);
	ASSERT_EQ(v1.y, -1.0f);

	const Vec2f v2(0.0f, -3.6f);
	counter = 0;
	for (auto it = v2.cbegin(); it != v2.cend(); ++it, counter++) {
		ASSERT_EQ(*it, v2[counter]);
	}
}

TEST(VecTest, VecComp) {
	Vec2f v1(0.0f, -3.6f);
	Vec2f v2(0.0f, -2.4f);
	Vec2f v3(1.0f,  -7.4f);
	Vec2f v4(1.0f, -7.4f);

	ASSERT_TRUE(v1 < v2);
	ASSERT_TRUE(v1 < v3);
	ASSERT_TRUE(v3 > v2);
	ASSERT_TRUE(v4 == v3);
	ASSERT_TRUE(v4 != v2);
}

TEST(VecTest, VecOps) {
	Vec2f v1(3.0f, 4.0f);
	Vec2f v2(3.0f, -4.0f);

	ASSERT_EQ(v1.mag(), 5.0f);
	ASSERT_EQ(v2.mag(), 5.0f);

	ASSERT_EQ(v1.magSquared(), 25.0f);

	NormVec2f nv1 = Vec2f(3.0, 5.0).normalize();
	ASSERT_TRUE(abs(nv1.x - 0.51449575542) <  s2d::epsilon);
	ASSERT_TRUE(abs(nv1.y - 0.85749292571) <  s2d::epsilon);

	NormVec2f nv2 = Vec2f(3.0, 5.0).unitNormal();

	ASSERT_TRUE(abs(nv2.x - 0.85749292571)  < s2d::epsilon);
	ASSERT_TRUE(abs(nv2.y - -0.51449575542) < s2d::epsilon);

	Vec2f v3 = v1 + v2;
	Vec2f v4 = v1 - v2;
	v2 += Vec2f(1.0f, 1.0f);
	v1 -= Vec2f(1.0f, 1.0f);

	Vec2f v5 = v1 + NormVec2(1.0f, 0.0f);

	ASSERT_EQ(v3, Vec2f(6.0f, 0.0f));
	ASSERT_EQ(v4, Vec2f(0.0f, 8.0f));

	ASSERT_EQ(v2, Vec2f(4.0f, -3.0f));
	ASSERT_EQ(v1, Vec2f(2.0f, 3.0f));

	ASSERT_EQ(v5, Vec2f(3.0f, 3.0f));

	ASSERT_EQ(-v2, Vec2f(-4.0f, 3.0f));
	ASSERT_EQ(+v2, Vec2f(4.0f, -3.0f));

	ASSERT_EQ(v2/2, Vec2f(2.0f, -1.5f));
	ASSERT_EQ(v2*2, Vec2f(8.0f, -6.0f));

	v1 *= 3.0f;
	ASSERT_EQ(v1, Vec2f(6.0f, 9.0f));
	v1 /= 2.0f;
	ASSERT_EQ(v1, Vec2f(3.0f, 4.5f));

	std::ostringstream st;
	Vec2f v6(1.0f, -1.0f);
	st << v6;
	std::cout << v6 << "\n";
	ASSERT_EQ(st.str(), "Vec2<float>(1, -1)");
}


//struct TestResult {
//	bool result = true;
//	std::string error = "";
//};
//
//static std::map<std::string, std::function<TestResult()>> testfnmap;
//
//class PopulateMap {
//public:
//	PopulateMap(const std::string& str, const std::function<TestResult()>& fn) {
//		testfnmap.insert(std::pair(str, fn));
//	}
//};
//
//#ifndef _S2D_TEST
//#define _S2D_TEST(fname) \
//    PopulateMap pmap##fname(#fname, fname);
//#endif
//
//
//////MAKE TEST CLASSES HERE////
//
//
//
//TestResult TestVec() {
//
//	Vec2f v1;
//	Vec2f v2(1.0f, 3.0f);
//	Vec2f v3(s2d::Point2f(1.0f, 2.0f));
//	Vec2f v4(s2d::Point2f(1.0f, 2.0f), s2d::Point2f(1.5f, 1.0f));
//
//	return TestResult();
//}
//
//TestResult Test2() {
//	return TestResult(false, "Test2 Failed\n");
//}
//
////////////////////////////////
//
//
//void populateMap() {
//	_S2D_TEST(Test1);
//	_S2D_TEST(Test2);
//}

//int main(int argc, char* argv[]) {
//	std::cout.flush();
//
//	populateMap();
//
//	if (argc != 2) {
//		std::cerr << "Test name must be provided with no other arguments\n";
//		return 1;
//	}
//	else {
//		std::cout << "Space2D_TEST\n";
//		std::cout << "Test selected: " << argv[1] << "\n";
//		std::string argname = argv[1];
//		if (testfnmap.find(argname) == testfnmap.end()) {
//			std::cerr << "Test name must match a valid test\n";
//			return 1;
//		}
//
//		auto testresult = testfnmap[argname]();
//
//		if (!testresult.result) {
//			std::cerr << "Test Failed" << "\n";
//			std::cout << testresult.error << "\n";
//			return 1;
//		}
//	}
//
//	return 0;
//}