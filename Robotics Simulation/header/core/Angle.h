#pragma once
#include <string>


// Base interface
class Angle {
public:
    virtual ~Angle() = default;
    virtual double sine() const = 0;
    virtual double cosine() const = 0;
    virtual double tangent() const = 0;
    virtual double toRadian() const = 0;
    virtual double toDegree() const = 0;
	virtual std::string toString() const = 0;
};

// Forward declaration to allow cross-constructors/operators
class Degree;

class Radian : public Angle
{
public:
    // Constants
    inline static constexpr double Pi  = 3.141592653589793238462643383279502884;
    inline static constexpr double Tau = 2.0 * Pi;

    // Constructors
    Radian();
    Radian(double radians);
    Radian(float radians);
    // Implicit conversion from Degree
    Radian(const Degree& deg);

    // Virtuals
    double sine() const override;
    double cosine() const override;
    double tangent() const override;
    double toRadian() const override;
    double toDegree() const override;
	std::string toString() const override;

    // Implicit downcast to scalar (radians)
    operator double() const noexcept;
    operator float()  const noexcept;

    // Operators (result stays in [0, 2pi))
    Radian operator+(const Radian& rhs) const;
    Radian operator-(const Radian& rhs) const;

    Radian operator+(const Degree& rhs) const;
    Radian operator-(const Degree& rhs) const;

private:
    static double normalize(double angle) noexcept;

    double radians{0.0};
};

class Degree : public Angle
{
public:
    // Constructors
    Degree();
    Degree(double degrees);
    Degree(float degrees);
    // Implicit conversion from Radian
    Degree(const Radian& rad);

    // Virtuals
    double sine() const override;
    double cosine() const override;
    double tangent() const override;
    double toRadian() const override;
    double toDegree() const override;
	std::string toString() const override;

    // Implicit downcast to scalar (degrees)
    operator double() const noexcept;
    operator float() const noexcept;

    // Operators (result stays in [0°, 360°))
    Degree operator+(const Degree& rhs) const;
    Degree operator-(const Degree& rhs) const;

    Degree operator+(const Radian& rhs) const;
    Degree operator-(const Radian& rhs) const;

private:
    static double normalize(double angle) noexcept;

    double degrees{0.0};
};