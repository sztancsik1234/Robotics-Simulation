#include <cmath>
#include "core/Angle.h"
#include <format>

// Static helpers
double Radian::normalize(double angle) noexcept
{
    double t = std::fmod(angle, Tau);
    if (t < 0.0) t += Tau;
    return t;
}

// Constructors
Radian::Radian() = default;

Radian::Radian(double radians)
    : radians(normalize(radians))
{
}

Radian::Radian(float radians)
    : radians(normalize(static_cast<double>(radians)))
{
}

Radian::Radian(const Degree& deg)
    : radians(normalize(deg.toRadian()))
{
}

// Virtuals
double Radian::sine() const     { return std::sin(radians); }
double Radian::cosine() const   { return std::cos(radians); }
double Radian::tangent() const  { return std::tan(radians); }
double Radian::toRadian() const { return radians; }
double Radian::toDegree() const { return radians * (180.0 / Pi); }

std::string Radian::toString() const
{
	return std::format("{:.3f} rad", radians);

}

// Conversions
Radian::operator double() const noexcept { return radians; }
Radian::operator float()  const noexcept { return static_cast<float>(radians); }

// Operators
Radian Radian::operator+(const Radian& rhs) const { return Radian(radians + rhs.radians); }
Radian Radian::operator-(const Radian& rhs) const { return Radian(radians - rhs.radians); }

Radian Radian::operator+(const Degree& rhs) const
{
    return Radian(radians + rhs.toRadian());
}

Radian Radian::operator-(const Degree& rhs) const
{
    return Radian(radians - rhs.toRadian());
}

// Static helpers
double Degree::normalize(double angle) noexcept
{
    double t = std::fmod(angle, 360.0);
    if (t < 0.0) t += 360.0;
    return t;
}

// Constructors
Degree::Degree() = default;

Degree::Degree(double degrees)
    : degrees(normalize(degrees))
{
}

Degree::Degree(float degrees)
    : degrees(normalize(static_cast<double>(degrees)))
{
}

Degree::Degree(const Radian& rad)
    : degrees(normalize(rad.toDegree()))
{
}

// Virtuals
double Degree::sine() const     { return std::sin(toRadian()); }
double Degree::cosine() const   { return std::cos(toRadian()); }
double Degree::tangent() const  { return std::tan(toRadian()); }
double Degree::toRadian() const { return degrees * (Radian::Pi / 180.0); }
double Degree::toDegree() const { return degrees; }

std::string Degree::toString() const
{
    return std::format("{:.3f} rad", degrees);
}

// Conversions
Degree::operator double() const noexcept { return degrees; }
Degree::operator float()  const noexcept { return static_cast<float>(degrees); }

// Operators
Degree Degree::operator+(const Degree& rhs) const { return Degree(degrees + rhs.degrees); }
Degree Degree::operator-(const Degree& rhs) const { return Degree(degrees - rhs.degrees); }

Degree Degree::operator+(const Radian& rhs) const
{
    return Degree(degrees + rhs.toDegree());
}

Degree Degree::operator-(const Radian& rhs) const
{
    return Degree(degrees - rhs.toDegree());
}