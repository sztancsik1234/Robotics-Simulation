#include "pch.h"
#include "core/Angle.h"
#include "gtest/gtest.h"
#include <type_traits>
#include <cmath>
#include <numbers>

namespace {
constexpr double EPS = 1e-12;
constexpr float  FEPS = 1e-6f;
}

static_assert(std::is_abstract_v<Angle>, "Angle must be abstract");

TEST(AngleTests, RadianNormalization)
{
    Radian r0(0.0);
    EXPECT_NEAR(r0.toRadian(), 0.0, EPS);

    Radian r2pi(2.0 * Radian::Pi);
    EXPECT_NEAR(r2pi.toRadian(), 0.0, EPS);

    Radian rneg(-Radian::Pi / 2.0);
    EXPECT_NEAR(rneg.toRadian(), Radian::Tau - (Radian::Pi / 2.0), EPS);

    Radian rwrap(5.0 * Radian::Pi); // 5pi mod 2pi = pi
    EXPECT_NEAR(rwrap.toRadian(), Radian::Pi, EPS);
}

TEST(AngleTests, DegreeNormalization)
{
    Degree d0(0.0);
    EXPECT_NEAR(d0.toDegree(), 0.0, EPS);

    Degree d360(360.0);
    EXPECT_NEAR(d360.toDegree(), 0.0, EPS);

    Degree dneg(-90.0);
    EXPECT_NEAR(dneg.toDegree(), 270.0, EPS);

    Degree dwrap(765.5); // 765.5 mod 360 = 45.5
    EXPECT_NEAR(dwrap.toDegree(), 45.5, EPS);
}

TEST(AngleTests, ConversionsBetweenUnits)
{
    Degree d180(180.0);
    EXPECT_NEAR(d180.toRadian(), Radian::Pi, EPS);

    Radian r90(Radian::Pi / 2.0);
    EXPECT_NEAR(r90.toDegree(), 90.0, EPS);

    Radian r_from_deg(Degree(90.0));
    EXPECT_NEAR(r_from_deg.toRadian(), Radian::Pi / 2.0, EPS);

    Degree d_from_rad( Radian{std::numbers::pi} );
	EXPECT_NEAR(d_from_rad.toDegree(), 180.0, EPS);
}

TEST(AngleTests, TrigonometricValuesAtFortyFive)
{
    const double s = std::sqrt(2.0) / 2.0;

    Radian r45(Radian::Pi / 4.0);
    EXPECT_NEAR(r45.sine(),   s, EPS);
    EXPECT_NEAR(r45.cosine(), s, EPS);
    EXPECT_NEAR(r45.tangent(), 1.0, EPS);

    Degree d45(45.0);
    EXPECT_NEAR(d45.sine(),   s, EPS);
    EXPECT_NEAR(d45.cosine(), s, EPS);
    EXPECT_NEAR(d45.tangent(), 1.0, EPS);
}

TEST(AngleTests, SameTypeOperatorsRadian)
{
    Radian a(3.0 * Radian::Pi / 2.0);
    Radian b(Radian::Pi);
    Radian sum = a + b; // 3pi/2 + pi = 5pi/2 -> pi/2
    EXPECT_NEAR(sum.toRadian(), Radian::Pi / 2.0, EPS);

    Radian c(Radian::Pi / 2.0);
    Radian diff = c - b; // pi/2 - pi = -pi/2 -> 3pi/2
    EXPECT_NEAR(diff.toRadian(), 3.0 * Radian::Pi / 2.0, EPS);
}

TEST(AngleTests, SameTypeOperatorsDegree)
{
    Degree a(300.0);
    Degree b(90.0);
    Degree sum = a + b; // 300 + 90 = 390 -> 30
    EXPECT_NEAR(sum.toDegree(), 30.0, EPS);

    Degree c(45.0);
    Degree diff = c - b; // 45 - 90 = -45 -> 315
    EXPECT_NEAR(diff.toDegree(), 315.0, EPS);
}

TEST(AngleTests, CrossTypeOperatorsAndConstructors)
{
    Radian r(Radian::Pi / 2.0);
    Degree d(90.0);

    Radian rsum = r + d; // pi/2 + pi/2 = pi
    EXPECT_NEAR(rsum.toRadian(), Radian::Pi, EPS);

    Degree dsum = d + r; // 90 + 90 = 180
    EXPECT_NEAR(dsum.toDegree(), 180.0, EPS);

    Radian rdiff = Radian(Radian::Pi / 4.0) - Degree(90.0); // pi/4 - pi/2 = -pi/4 -> 7pi/4
    EXPECT_NEAR(rdiff.toRadian(), 7.0 * Radian::Pi / 4.0, EPS);

    Degree ddiff = Degree(10.0) - Radian(Radian::Pi / 2.0); // 10 - 90 = -80 -> 280
    EXPECT_NEAR(ddiff.toDegree(), 280.0, EPS);

    // Cross-type constructors compile and convert
    Radian r_from_d = Degree(180.0);
    EXPECT_NEAR(static_cast<double>(r_from_d), Radian::Pi, EPS);

    Degree d_from_r = Radian(Radian::Pi / 3.0);
    EXPECT_NEAR(static_cast<double>(d_from_r), 60.0, EPS);
}

TEST(AngleTests, ImplicitScalarCasts)
{
    double r_scalar = Radian(Radian::Pi / 3.0); // implicit to double
    EXPECT_NEAR(r_scalar, Radian::Pi / 3.0, EPS);

    Degree d405_5(405.5f);
    float d_scalar = d405_5; // implicit to float; normalized to 45.5
    EXPECT_NEAR(d_scalar, 45.5f, FEPS);
}


