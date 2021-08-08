#include <iostream>
#include <map>
#include <functional>
#include <cassert>
#include "Space2D.h"
#include "gtest/gtest.h"
#ifdef _SFML_ENABLED
#include "SFML\Graphics.hpp"
#endif


using namespace s2d;


TEST(PointTest, PointConstructor) {
	Point2f p1;
	Point2f p2(1.0f, 3.0f);
	Point2f p3(Vec2f(1.0f, 2.0f));
	Point2f p4(Point2f(1.0f, 2.0f), Vec2f(1.5f, 1.0f));

	ASSERT_EQ(p1.x, 0.0f);
	ASSERT_EQ(p1.y, 0.0f);

	ASSERT_EQ(p2.x, 1.0f);
	ASSERT_EQ(p2.y, 3.0f);
	ASSERT_EQ(p3.x, 1.0f);
	ASSERT_EQ(p3.y, 2.0f);
	ASSERT_EQ(p4, Point2f(2.5f, 3.0f));
}

TEST(PointTest, PointItr) {

	Point2f p1(0.0f, -3.6f);

	ASSERT_EQ(p1[0], 0.0f);
	ASSERT_EQ(p1[1], -3.6f);

	size_t counter = 0;

	for (auto it = p1.begin(); it != p1.end(); it++, counter++) {
		ASSERT_EQ(*it, p1[counter]);
	}

	std::for_each(p1.begin(), p1.end(), [](auto& a) {
		a = -1.0f;
		}
	);

	ASSERT_EQ(p1.x, -1.0f);
	ASSERT_EQ(p1.y, -1.0f);

	const Point2f p2(0.0f, -3.6f);
	counter = 0;
	for (auto it = p2.cbegin(); it != p2.cend(); ++it, counter++) {
		ASSERT_EQ(*it, p2[counter]);
	}
}

TEST(PointTest, PointComp) {
	Vec2f v1(0.0f, -3.6f);
	Vec2f v2(0.0f, -2.4f);
	Vec2f v3(1.0f, -7.4f);
	Vec2f v4(1.0f, -7.4f);

	ASSERT_TRUE(v1 < v2);
	ASSERT_TRUE(v1 < v3);
	ASSERT_TRUE(v3 > v2);
	ASSERT_TRUE(v4 == v3);
	ASSERT_TRUE(v4 != v2);
}

TEST(PointTest, PointOps) {
	Point2f v1(3.0f, 4.0f);
	Point2f v2(3.0f, -4.0f);

	Point2f v3 = v1 + v2;
	Point2f v4 = v1 - v2;
	v2 += Dim2f(1.0f, 1.0f);
	v1 -= Vec2f(1.0f, 1.0f);

	Point2f v5 = v1 + NormVec2(1.0f, 0.0f);

	ASSERT_EQ(v3, Point2f(6.0f, 0.0f));
	ASSERT_EQ(v4, Point2f(0.0f, 8.0f));

	ASSERT_EQ(v2, Point2f(4.0f, -3.0f));
	ASSERT_EQ(v1, Point2f(2.0f, 3.0f));

	ASSERT_EQ(v5, Point2f(3.0f, 3.0f));

	ASSERT_EQ(v2 / 2, Point2f(2.0f, -1.5f));
	ASSERT_EQ(v2 * 2, Point2f(8.0f, -6.0f));

	v1 *= 3.0f;
	ASSERT_EQ(v1, Point2f(6.0f, 9.0f));
	v1 /= 2.0f;
	ASSERT_EQ(v1, Point2f(3.0f, 4.5f));

	std::ostringstream st;
	Point2f v6(1.0f, -1.0f);
	st << v6;
	std::cout << v6 << "\n";
	ASSERT_EQ(st.str(), "Point2<float>(1, -1)");

#ifdef _SFML_ENABLED

	ASSERT_EQ(v6.toSFMLVec(), sf::Vector2f(1.0f, -1.0f));

	sf::FloatRect rect(Point2f(1.0f, 1.0f), Vec2f(5.0f, 6.0f));
	ASSERT_EQ(rect, sf::FloatRect(1.0f, 1.0f, 5.0f, 6.0f));

#endif

	if (S2D_PIXEL_TO_METER == 64) {
		Point2p vp1(128_px, 128_px);
		ASSERT_EQ(Point2m(2_mtr, 2_mtr), (Point2m)vp1);
	}
}

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

	ASSERT_EQ(Vec2f(1.0, 2.0).dot(Vec2f(-3.0, 4.0)), 5);
	ASSERT_EQ(Vec2f(1.0, 2.0).dot(NormVec2f(-3.0, 4.0)), 1);

	ASSERT_EQ(Vec2f(1, 2).cross(Vec2f(3, -1)), -7);
	ASSERT_EQ(Vec2f(1, 2).cross(NormVec2f(1, 0)), -2);
	ASSERT_EQ(Vec2f(1, 2).cross(2), Vec2f(4, -2));

	ASSERT_TRUE(Vec2f(1.0, 2.0).perp(Vec2f(-2.0, 1.0)));
	ASSERT_FALSE(Vec2f(1.0, 2.0).perp(NormVec2f(-3.0, 4.0)));

	if (S2D_PIXEL_TO_METER == 64) {
		Vec2p vp1(128_px, 128_px);
		ASSERT_EQ(Vec2m(2_mtr, 2_mtr), (Vec2m)vp1);
	}

#ifdef _SFML_ENABLED

	ASSERT_EQ(v6.toSFMLVec(), sf::Vector2f(1.0f, -1.0f));

	sf::FloatRect rect(Vec2f(1.0f, 1.0f), Vec2f(5.0f, 6.0f));
	ASSERT_EQ(rect, sf::FloatRect(1.0f, 1.0f, 5.0f, 6.0f));

#endif
}

TEST(DimTest, DimConstructor) {
	Dim2m v1;
	Dim2m v2(1.0_mtr, 3.0_mtr);
	Dim2m v3(Point2m(1.0_mtr, 2.0_mtr));
	Dim2m v4(Point2m(1.0_mtr, 2.0_mtr), Point2m(1.5_mtr, 1.0_mtr));

	Dim2m v5(Vec2m(1.0_mtr, 2.0_mtr));
	Dim2m v6(NormVec2m(1.0_mtr, 0.0_mtr));

	ASSERT_EQ(v1.x, 0.0_mtr);
	ASSERT_EQ(v1.y, 0.0_mtr);

	ASSERT_EQ(v2.x, 1.0_mtr);
	ASSERT_EQ(v2.y, 3.0_mtr);
	ASSERT_EQ(v3.x, 1.0_mtr);
	ASSERT_EQ(v3.y, 2.0_mtr);
	ASSERT_EQ(v4.x, 0.5_mtr);
	ASSERT_EQ(v4.y, -1.0_mtr);

	ASSERT_EQ(v5.x, 1.0_mtr);
	ASSERT_EQ(v5.y, 2.0_mtr);

	ASSERT_EQ(v6.x, 1.0_mtr);
	ASSERT_EQ(v6.y, 0.0_mtr);
}

TEST(DimTest, DimItr) {

    Dim2m v1(0.0f, -3.6f);

	ASSERT_EQ(v1[0], 0.0_mtr);
	ASSERT_EQ(v1[1], -3.6_mtr);

	size_t counter = 0;

	for (auto it = v1.begin(); it != v1.end(); it++, counter++) {
		ASSERT_EQ(*it, v1[counter]);
	}

	std::for_each(v1.begin(), v1.end(), [](auto& a) {
		a = -1.0_mtr;
		}
	);

	ASSERT_EQ(v1.x, -1.0_mtr);
	ASSERT_EQ(v1.y, -1.0_mtr);

	const Dim2m v2(0.0_mtr, -3.6_mtr);
	counter = 0;
	for (auto it = v2.cbegin(); it != v2.cend(); ++it, counter++) {
		ASSERT_EQ(*it, v2[counter]);
	}
}

TEST(DimTest, DimComp) {
	Dim2m v1(0.0_mtr, -3.6_mtr);
	Dim2m v2(0.0_mtr, -2.4_mtr);
	Dim2m v3(1.0_mtr, -7.4_mtr);
	Dim2m v4(1.0_mtr, -7.4_mtr);

	ASSERT_TRUE(v1 < v2);
	ASSERT_TRUE(v1 < v3);
	ASSERT_TRUE(v3 > v2);
	ASSERT_TRUE(v4 == v3);
	ASSERT_TRUE(v4 != v2);
}

TEST(DimTest, DimOps) {
	Dim2m v1(3.0_mtr, 4.0_mtr);
	Dim2m v2(3.0_mtr, -4.0_mtr);

	Dim2m v3 = v1 + v2;
	Dim2m v4 = v1 - v2;
	v2 += Dim2m(1.0_mtr, 1.0_mtr);
	v1 -= Dim2m(1.0_mtr, 1.0_mtr);

	Dim2m v5 = v1 + NormVec2m(1.0_mtr, 0.0_mtr);

	ASSERT_EQ(v3, Dim2m(6.0_mtr, 0.0_mtr));
	ASSERT_EQ(v4, Dim2m(0.0_mtr, 8.0_mtr));

	ASSERT_EQ(v2, Dim2m(4.0_mtr, -3.0_mtr));
	ASSERT_EQ(v1, Dim2m(2.0_mtr, 3.0_mtr));

	ASSERT_EQ(v5, Dim2m(3.0_mtr, 3.0_mtr));

	ASSERT_EQ(v2 / 2, Dim2m(2.0_mtr, -1.5_mtr));
	ASSERT_EQ(v2 * 2, Dim2m(8.0_mtr, -6.0_mtr));

	v1 *= 3.0_mtr;
	ASSERT_EQ(v1, Dim2m(6.0_mtr, 9.0_mtr));
	v1 /= 2.0_mtr;
	ASSERT_EQ(v1, Dim2m(3.0_mtr, 4.5_mtr));

	std::ostringstream st;
	Dim2f v6(1.0f, -1.0f);
	st << v6;
	std::cout << v6 << "\n";
	ASSERT_EQ(st.str(), "Dim2<float>(1, -1)");

	if (S2D_PIXEL_TO_METER == 64) {
		Dim2p vp1(128_px, 128_px);
		ASSERT_EQ(Dim2m(2_mtr, 2_mtr), (Dim2m)vp1);
	}

	Dim2m d1(2.0_mtr, 5.0_mtr);
	ASSERT_EQ(d1 * Dim2m(3.0, 2.0), Dim2m(6_mtr, 10_mtr));
	d1 *= Dim2m(-1_mtr, -2_mtr);
	ASSERT_EQ(d1, Dim2m(-2_mtr, -10_mtr));

#ifdef _SFML_ENABLED

	ASSERT_EQ(v6.toSFMLVec(), sf::Vector2f(1.0f, -1.0f));

	sf::FloatRect rect(Vec2f(1.0f, 1.0f), Vec2f(5.0f, 6.0f));
	ASSERT_EQ(rect, sf::FloatRect(1.0f, 1.0f, 5.0f, 6.0f));

#endif
}

TEST(NormVecTest, NormVecConstructor) {
	NormVec2f v1(1.0f, 3.0f);
	NormVec2f v2(45_deg);

	ASSERT_LT(std::abs(v1.x - 0.31622776601683794), epsilon);
	ASSERT_LT(std::abs(v1.y - 0.9486832980505138), epsilon);
}

TEST(NormVecTest, NormVecItr) {

	NormVec2f v1(0.0f, -3.6f);

	ASSERT_EQ(v1[0], 0.0f);
	ASSERT_EQ(v1[1], -1.0f);

	size_t counter = 0;

	for (auto it = v1.begin(); it != v1.end(); it++, counter++) {
		ASSERT_EQ(*it, v1[counter]);
	}
}

TEST(NormVecTest, NormVecComp) {
	NormVec2f v1(1.0f, -3.6f);
	NormVec2f v2(2.0f, -2.4f);
	NormVec2f v3(1.0f, -7.4f);
	NormVec2f v4(1.0f, -7.4f);

	ASSERT_TRUE(v1 < v2);
	ASSERT_TRUE(v1 > v3);
	ASSERT_TRUE(v3 < v2);
	ASSERT_TRUE(v4 == v3);
	ASSERT_TRUE(v4 != v2);

	NormVec2f v5(0.0f, 1.0f);
	NormVec2f v6(0.0f, -1.0f);

	ASSERT_NE(v5, v6);
	ASSERT_GT(v5, v6);
}

TEST(NormVecTest, NormVecOps) {
	ASSERT_EQ(NormVec2f(0.0, 1.0).dot(NormVec2f(0.0, 4.0)), 1);
	ASSERT_EQ(NormVec2f(1.0, 0.0).dot(Vec2f(-3.0, 4.0)), -3);

	ASSERT_EQ(NormVec2f(1, 0).cross(Vec2f(1, 2)), 2);
	ASSERT_EQ(NormVec2f(1, 0).cross(NormVec2f(0, 1)), 1);
	ASSERT_EQ(NormVec2f(1, 0).cross(2), Vec2f(0, -2));

	ASSERT_TRUE(NormVec2f(1.0, 2.0).perp(NormVec2f(-2.0, 1.0)));
	ASSERT_FALSE(NormVec2f(1.0, 2.0).perp(Vec2f(-3.0, 4.0)));

	if (S2D_PIXEL_TO_METER == 64) {
		NormVec2p vp1(128_px, 0_px);
		ASSERT_EQ(NormVec2m(1_mtr, 0_mtr), (NormVec2m)vp1);
	}

#ifdef _SFML_ENABLED


	NormVec2f v1(1.0f, 0.0f);
	ASSERT_EQ(v1.toSFMLVec(), sf::Vector2f(1.0f, 0.0f));

	sf::FloatRect rect(NormVec2f(0.0f, 1.0f), Vec2f(5.0f, 6.0f));
	ASSERT_EQ(rect, sf::FloatRect(0.0f, 1.0f, 5.0f, 6.0f));

#endif
}

TEST(RectTest, RectConstructor) {
	Rect2f r1;
	Rect2f r2(1, 1, 5, 4);
	Rect2f r3(Point2f(2, 3), Point2f(5, 5));
	Rect2f r4(Point2f(2, 3), Dim2f(5, 5));

	Rect2f r5(r4, Vec2f(1, 1), Vec2f(2, 1));
	Rect2f r6(Rect2f(0, 0, 10, 10), Dim2(10_pcent, 20_pcent), Dim2(30_pcent, 10_pcent));


}

TEST(RectTest, RectItr) {

	Rect2f v1(1, 1, 5, 4);

	ASSERT_EQ(v1[0], Point2f(1,1));
	ASSERT_EQ(v1[1], Point2f(5,4));

	size_t counter = 0;

	for (auto it = v1.begin(); it != v1.end(); it++, counter++) {
		ASSERT_EQ(*it, v1[counter]);
	}

	std::for_each(v1.begin(), v1.end(), [](auto& a) {
		a += Point2f(1.0, -1.0);
		}
	);

	ASSERT_EQ(v1[0], Point2f(2, 0));
	ASSERT_EQ(v1[1], Point2f(6, 3));

	const Rect2f v2(2, -2, 3, 5);
	counter = 0;
	for (auto it = v2.cbegin(); it != v2.cend(); ++it, counter++) {
		ASSERT_EQ(*it, v2[counter]);
	}
}

TEST(RectTest, RectComp) {
	Rect2f v1(0.0f, 1.0f, -3.6f, 1.0f);
	Rect2f v2(0.0f, 1.0f, -2.4f, 1.0f);
	Rect2f v3(1.0f, 1.0f, -7.4f, 1.0f);
	Rect2f v4(1.0f, 1.0f, -7.4f, 1.0f);

	ASSERT_TRUE(v1 < v2);
	ASSERT_TRUE(v1 < v3);
	ASSERT_TRUE(v3 > v2);
	ASSERT_TRUE(v4 == v3);
	ASSERT_TRUE(v4 != v2);
}

TEST(RectTest, RectOps) {
	Rect2f r1(1, 2, 3, 7);
	ASSERT_EQ(r1.area(), 10);
	ASSERT_EQ(r1.center(), Point2f(2, 4.5));
	ASSERT_TRUE(r1.contains(Point2f(1.1, 3.1)));
	ASSERT_FALSE(r1.contains(Point2f(1.0, 2.2)));
	ASSERT_FALSE(r1.contains(10, 10));
	ASSERT_FALSE(r1.contains(-10, -10));
	ASSERT_TRUE(r1.contains(2, 6));

	ASSERT_EQ(r1.size(), Dim2f(2, 5));
	ASSERT_EQ(r1.width(), 2);
	ASSERT_EQ(r1.height(), 5);

	Rect2f r2(0, 0, 5, 6);
	r2.moveMinTo(Point2f(1, 1));
	ASSERT_EQ(r2.min, Point2f(1, 1));
	r2.moveMinTo(3, 2);
	ASSERT_EQ(r2.size(), Dim2f(5, 6));

	r2.moveCenterTo(Point2f(0, 0));
	ASSERT_EQ(r2.min, Point2f(-2.5, -3));
	r2.moveCenterTo(2.5, 3);
	ASSERT_EQ(r2.size(), Dim2f(5, 6));
	ASSERT_EQ(r2.getFaceNormal(RectFace::Up), NormVec2f(0, -1));
	ASSERT_EQ(r2.getFaceNormal(RectFace::Down), NormVec2f(0, 1));
	ASSERT_EQ(r2.getFaceNormal(RectFace::Right), NormVec2f(1, 0));
	ASSERT_EQ(r2.getFaceNormal(RectFace::Left), NormVec2f(-1, 0));

	auto a1 = std::array<Point2f, 2>{ Point2f(0, 0), Point2f(5, 0) };
	auto a2 = std::array<Point2f, 2>{ Point2f(0, 6), Point2f(5, 6) };
	auto a3 = std::array<Point2f, 2>{ Point2f(5, 0), Point2f(5, 6) };
	auto a4 = std::array<Point2f, 2>{ Point2f(0, 0), Point2f(0, 6) };

	ASSERT_EQ(r2.getFacePoints(RectFace::Up), a1);
	ASSERT_EQ(r2.getFacePoints(RectFace::Down), a2);
	ASSERT_EQ(r2.getFacePoints(RectFace::Right), a3);
	ASSERT_EQ(r2.getFacePoints(RectFace::Left), a4);

	ASSERT_EQ(r2.getFaceVec(RectFace::Up), Vec2f(-5,0));
	ASSERT_EQ(r2.getFaceVec(RectFace::Down), Vec2f(5, 0));
	ASSERT_EQ(r2.getFaceVec(RectFace::Right), Vec2f(0, -6));
	ASSERT_EQ(r2.getFaceVec(RectFace::Left), Vec2f(0, 6));

	Rect2f r3(0, 0, 5, 4);
	ASSERT_EQ(r3 + Vec2f(1,1), Rect2f(1, 1, 6, 5));
	r3 += Vec2f(1, 1);
	ASSERT_EQ(r3, Rect2f(1, 1, 6, 5));
	ASSERT_EQ(r3 + NormVec2f(0, 1), Rect2f(1, 2, 6, 6));
	r3 += NormVec2f(0, 1);
	ASSERT_EQ(r3, Rect2f(1, 2, 6, 6));


	ASSERT_EQ(r3 - NormVec2f(0, 1), Rect2f(1, 1, 6, 5));
	r3 -= NormVec2f(0, 1);
	ASSERT_EQ(r3, Rect2f(1, 1, 6, 5));
	ASSERT_EQ(r3 - Vec2f(1, 1), Rect2f(0, 0, 5, 4));
	r3 -= Vec2f(1, 1);
	ASSERT_EQ(r3, Rect2f(0, 0, 5, 4));

	r3 = Rect2f(1, -1, 5, 7);

	std::ostringstream st;
	st << r3;
	std::cout << r3 << "\n";
	ASSERT_EQ(st.str(), "Rect2<float>(min: (1, -1), max: (5, 7))");

	if (S2D_PIXEL_TO_METER == 64) {
		Rect2p vp1(64_px, 64_px, 128_px, 128_px);
		ASSERT_EQ(Rect2m(1_mtr, 1_mtr, 2_mtr, 2_mtr), (Rect2m)vp1);
	}

#ifdef _SFML_ENABLED

	ASSERT_EQ(r3.toSFMLRect(), sf::FloatRect(1,-1, 4, 8));

#endif


	Rect2f intr1(0, 0, 10, 10);
	Rect2f intr2(9, 9, 10, 11);
	ASSERT_TRUE(intr1.intersects(intr2));
	Rect2f intr3(11, 11, 15, 15);
	ASSERT_FALSE(intr1.intersects(intr3));
}

TEST(PolyTest, PolyConstructor) {
	Poly2f p1;
	std::vector<Point2f> vec1({ Point2f(), Point2f(0, 1), Point2f(1,1) });
	Poly2f p2(vec1);
	Poly2f p3{ Point2f(), Point2f(0, 1), Point2f(1,1) };
	Poly2f p4(0, 0, 0, 1, 1, 1);
	Poly2f p5(vec1, Rect2f(1, 2, 11, 15));

	Poly2f p6{{ 0, 0 }, { 0, 1 }, { 1, 1 }};

	Rect2f rec1(1, 2, 11, 15);

	ASSERT_EQ(p2, p3);
	ASSERT_EQ(p4, p3);
	ASSERT_EQ(p6, p4);

	ASSERT_EQ(p5[0], rec1[0]);
	ASSERT_EQ(p5[2], rec1[1]);

	bool failed = false;

	try {
		Poly2f p7{ { 0, 0 }, { 0, 1 }, { 1, 1 }, {0.3, 0.7} };
	}
	catch(std::logic_error e) {
		if (std::string(e.what()) == std::string("Poly2 is not convex")) {
			std::cout << e.what() << "\n";
			failed = true;
		}
	}

	ASSERT_TRUE(failed);
}

TEST(PolyTest, PolyItr) {

	Poly2f v1{ { 1, 1 }, {5, 4} , {3, 2}};

	ASSERT_EQ(v1[0], Point2f(1, 1));
	ASSERT_EQ(v1[1], Point2f(5, 4));
	ASSERT_EQ(v1[2], Point2f(3, 2));

	size_t counter = 0;

	for (auto it = v1.begin(); it != v1.end(); it++, counter++) {
		ASSERT_EQ(*it, v1[counter]);
	}

	std::for_each(v1.begin(), v1.end(), [](auto& a) {
		    a += Point2f(1.0, -1.0);
		}
	);

	ASSERT_EQ(v1[0], Point2f(2, 0));
	ASSERT_EQ(v1[1], Point2f(6, 3));
	ASSERT_EQ(v1[2], Point2f(4, 1));

	counter = 0;
	for (auto it = v1.cbegin(); it != v1.cend(); ++it, counter++) {
		ASSERT_EQ(*it, v1[counter]);
	}
}

TEST(PolyTest, PolyComp) {
	Rect2f v1(0.0f, 1.0f, -3.6f, 2.0f);
	Rect2f v2(0.0f, 1.0f, -2.4f, 2.0f);
	Rect2f v3(1.0f, 1.0f, -7.4f, 2.0f);
	Rect2f v4(1.0f, 1.0f, -7.4f, 2.0f);

	Poly2f p1(v1);
	Poly2f p2(v2);
	Poly2f p3(v3);
	Poly2f p4(v4);

	ASSERT_TRUE(p1 < p2);
	ASSERT_TRUE(p1 < p3);
	ASSERT_TRUE(p3 > p2);
	ASSERT_TRUE(p4 == p3);
	ASSERT_TRUE(p4 != p2);
}

TEST(PolyTest, PolyOps) {
	Poly2f r1(Rect2f(1, 2, 3, 7));
	ASSERT_EQ(r1.area(), 10);
	ASSERT_EQ(r1.centroid(), Point2f(2, 4.5));

	Poly2f r2(Rect2f(0, 0, 5, 6));

	r2.moveCenterTo(Point2f(0, 0));
	ASSERT_EQ(r2[0], Point2f(-2.5, -3));
	r2.moveCenterTo(2.5, 3);

	ASSERT_EQ(r2.getFaceNormal(1), NormVec2f(0, -1));
	ASSERT_EQ(r2.getFaceNormal(3), NormVec2f(0, 1));
	ASSERT_EQ(r2.getFaceNormal(0), NormVec2f(1, 0));
	ASSERT_EQ(r2.getFaceNormal(2), NormVec2f(-1, 0));

	auto a1 = std::array<Point2f, 2>{ Point2f(5, 0), Point2f(0, 0) };
	auto a2 = std::array<Point2f, 2>{ Point2f(0, 6), Point2f(5, 6) };
	auto a3 = std::array<Point2f, 2>{ Point2f(5, 6), Point2f(5, 0) };
	auto a4 = std::array<Point2f, 2>{ Point2f(0, 0), Point2f(0, 6) };

	ASSERT_EQ(r2.getFacePoints(1), a2);
	ASSERT_EQ(r2.getFacePoints(3), a1);
	ASSERT_EQ(r2.getFacePoints(0), a4);
	ASSERT_EQ(r2.getFacePoints(2), a3);

	ASSERT_EQ(r2.getFaceVec(1), Vec2f(5, 0));
	ASSERT_EQ(r2.getFaceVec(3), Vec2f(-5, 0));
	ASSERT_EQ(r2.getFaceVec(0), Vec2f(0, 6));
	ASSERT_EQ(r2.getFaceVec(2), Vec2f(0, -6));

	Poly2f r3(Rect2f(0, 0, 5, 4));
	ASSERT_EQ(r3 + Vec2f(1, 1), Poly2f(Rect2f(1, 1, 6, 5)));
	r3 += Vec2f(1, 1);
	ASSERT_EQ(r3, Poly2f(Rect2f(1, 1, 6, 5)));
	ASSERT_EQ(r3 + NormVec2f(0, 1), Poly2f(Rect2f(1, 2, 6, 6)));
	r3 += NormVec2f(0, 1);
	ASSERT_EQ(r3, Poly2f(Rect2f(1, 2, 6, 6)));


	ASSERT_EQ(r3 - NormVec2f(0, 1), Poly2f(Rect2f(1, 1, 6, 5)));
	r3 -= NormVec2f(0, 1);
	ASSERT_EQ(r3, Poly2f(Rect2f(1, 1, 6, 5)));
	ASSERT_EQ(r3 - Vec2f(1, 1), Poly2f(Rect2f(0, 0, 5, 4)));
	r3 -= Vec2f(1, 1);
	ASSERT_EQ(r3, Poly2f(Rect2f(0, 0, 5, 4)));

	r3 = Poly2f(Rect2f(1, -1, 5, 7));

	std::cout << r3 << "\n";

	if (S2D_PIXEL_TO_METER == 64) {
		Poly2p vp1(Rect2p(64_px, 64_px, 128_px, 128_px));
		//Poly2m pm1 = (Poly2m)vp1;
		ASSERT_EQ(Poly2m(Rect2m(1_mtr, 1_mtr, 2_mtr, 2_mtr)), (Poly2m)vp1);
	}
}

TEST(MatTest, MatConstructor) {

	std::array<float, 9> a1({ 1, 0, 0, 0, 1, 0, 0, 0, 1 });
	std::array<float, 9> a2({ 2, 2, 2, 2, 2, 2, 2, 2, 2 });
	std::array<float, 9> a3({ 1,4,0,
		                      2,5,0,
		                      3,6,1 });

	Mat3f m1;
	ASSERT_EQ(m1.getMatrix(), a1);
	Mat3f m2(2);
	ASSERT_EQ(m2.getMatrix(), a2);
	Mat3f m3(1, 2, 3, 4, 5, 6);
	ASSERT_EQ(m3.getMatrix(), a3);
}

TEST(MatTest, MatTransformOps) {
	Mat3f m1;
	m1.translate(Vec2f(5, 8));
	m1.scale(2, 2);
	m1.rotate(60_deg);
	Point2f p1(3, 2);

	Point2f p2(6.0f * s2d::cos(60_deg) - 4.0f * s2d::sin(60_deg) + 5,
		6.0f * s2d::sin(60_deg) + 4.0f * s2d::cos(60_deg) + 8);

	Point2f p3(10.0f * s2d::cos(60_deg) - 10.0f * s2d::sin(60_deg) + 5,
		10.0f * s2d::sin(60_deg) + 10.0f * s2d::cos(60_deg) + 8);

	ASSERT_EQ(m1.transform(p1), p2);

	Rect2f r1(3, 2, 5, 5);
	ASSERT_EQ(m1.transform(r1), Rect2f(p2, p3));

	Vec2f v1(3, 5);
	Vec2f v2(6.0f * s2d::cos(60_deg) - 10.0f * s2d::sin(60_deg),
		6.0f * s2d::sin(60_deg) + 10.0f * s2d::cos(60_deg));

	ASSERT_EQ(m1.transform(v1), v2);

	Dim2f d1(3, 5);
	Dim2f d2(6.0f * s2d::cos(60_deg) - 10.0f * s2d::sin(60_deg),
		6.0f * s2d::sin(60_deg) + 10.0f * s2d::cos(60_deg));

	ASSERT_EQ(m1.transform(d1), d2);

	NormVec2f nv1(3, 5);
	Vec2f nv2(6.0f * s2d::cos(60_deg) - 10.0f * s2d::sin(60_deg),
		6.0f * s2d::sin(60_deg) + 10.0f * s2d::cos(60_deg));

	ASSERT_EQ(m1.transform(nv1), nv2.normalize());

	Poly2f pl1{ { 0, 0 }, { 0, 1 }, { 1, 1 } };
	Poly2f pl2{ { 5, 8 }, 
		{ -2.0f * s2d::sin(60_deg) + 5, 2.0f * s2d::cos(60_deg) + 8},
		{ 2.0f * s2d::cos(60_deg) - 2.0f * s2d::sin(60_deg) + 5,
		2.0f * s2d::sin(60_deg) + 2.0f * s2d::cos(60_deg) + 8 } };

	ASSERT_EQ(m1.transform(pl1), pl2);

	Poly2f pl3(Rect2f(Point2f(1, 1), Dim2f(5, 7)));
	Mat3f m2;
	m2.shear(0.5f, 0.5f);
	m2.reflX();
	m2.reflY();
	auto pl4 = m2.transform(pl3);
	std::cout << pl4 << "\n";
	Mat3f m3 = m2.inverse();
	//std::cout << m2 << "\n\n";
	//std::cout << m3 << "\n";
	//Mat3f m4(-1.33333, 0.666667,  0,
	//	     0.666667, -1.33333,  -0,
	//	     0,        -0,         1);
	//ASSERT_EQ(m3, m4);

	ASSERT_GT(m2, m3);
}