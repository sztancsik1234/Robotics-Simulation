#include <cmath>
#include "core/Angle.h"

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
    : m_radians(normalize(radians))
{
}

Radian::Radian(float radians)
    : m_radians(normalize(static_cast<double>(radians)))
{
}

Radian::Radian(const Degree& deg)
    : m_radians(normalize(deg.toRadian()))
{
}

// Virtuals
double Radian::sine() const     { return std::sin(m_radians); }
double Radian::cosine() const   { return std::cos(m_radians); }
double Radian::tangent() const  { return std::tan(m_radians); }
double Radian::toRadian() const { return m_radians; }
double Radian::toDegree() const { return m_radians * (180.0 / Pi); }

// Conversions
Radian::operator double() const noexcept { return m_radians; }
Radian::operator float()  const noexcept { return static_cast<float>(m_radians); }

// Operators
Radian Radian::operator+(const Radian& rhs) const { return Radian(m_radians + rhs.m_radians); }
Radian Radian::operator-(const Radian& rhs) const { return Radian(m_radians - rhs.m_radians); }

Radian Radian::operator+(const Degree& rhs) const
{
    return Radian(m_radians + rhs.toRadian());
}

Radian Radian::operator-(const Degree& rhs) const
{
    return Radian(m_radians - rhs.toRadian());
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
    : m_degrees(normalize(degrees))
{
}

Degree::Degree(float degrees)
    : m_degrees(normalize(static_cast<double>(degrees)))
{
}

Degree::Degree(const Radian& rad)
    : m_degrees(normalize(rad.toDegree()))
{
}

// Virtuals
double Degree::sine() const     { return std::sin(toRadian()); }
double Degree::cosine() const   { return std::cos(toRadian()); }
double Degree::tangent() const  { return std::tan(toRadian()); }
double Degree::toRadian() const { return m_degrees * (Radian::Pi / 180.0); }
double Degree::toDegree() const { return m_degrees; }

// Conversions
Degree::operator double() const noexcept { return m_degrees; }
Degree::operator float()  const noexcept { return static_cast<float>(m_degrees); }

// Operators
Degree Degree::operator+(const Degree& rhs) const { return Degree(m_degrees + rhs.m_degrees); }
Degree Degree::operator-(const Degree& rhs) const { return Degree(m_degrees - rhs.m_degrees); }

Degree Degree::operator+(const Radian& rhs) const
{
    return Degree(m_degrees + rhs.toDegree());
}

Degree Degree::operator-(const Radian& rhs) const
{
    return Degree(m_degrees - rhs.toDegree());
}