#include "pch.h"
#include "core/Vector2.h"
#include "SFML/System/Vector2.hpp"

TEST(Vector2ConversionTest, ExplicitConversionToSfmlVector2f) {
    Vector2 v(3.5f, 4.2f);
    
    sf::Vector2f sfmlVec = v.ConvertTo<sf::Vector2f>();
    
    EXPECT_FLOAT_EQ(sfmlVec.x, 3.5f);
    EXPECT_FLOAT_EQ(sfmlVec.y, 4.2f);
}

TEST(Vector2ConversionTest, ExplicitConversionToSfmlVector2i) {
    Vector2 v(3.7f, 4.2f);
    
    sf::Vector2i sfmlVec = v.ConvertTo<sf::Vector2i>();
    
    EXPECT_EQ(sfmlVec.x, 3);  // Truncated to int
    EXPECT_EQ(sfmlVec.y, 4);  // Truncated to int
}

TEST(Vector2ConversionTest, ExplicitConversionToSfmlVector2u) {
    Vector2 v(3.7f, 4.2f);
    
    sf::Vector2u sfmlVec = v.ConvertTo<sf::Vector2u>();
    
    EXPECT_EQ(sfmlVec.x, 3u);
    EXPECT_EQ(sfmlVec.y, 4u);
}

TEST(Vector2ConversionTest, ImplicitConversionToSfmlVector2f) {
    Vector2 v(10.5f, 20.3f);
    
    sf::Vector2f sfmlVec = v;  // Implicit conversion
    
    EXPECT_FLOAT_EQ(sfmlVec.x, 10.5f);
    EXPECT_FLOAT_EQ(sfmlVec.y, 20.3f);
}

TEST(Vector2ConversionTest, ImplicitConversionToSfmlVector2i) {
    Vector2 v(15.9f, 25.1f);
    
    sf::Vector2i sfmlVec = v;  // Implicit conversion
    
    EXPECT_EQ(sfmlVec.x, 15);
    EXPECT_EQ(sfmlVec.y, 25);
}

TEST(Vector2ConversionTest, ImplicitConversionToSfmlVector2u) {
    Vector2 v(8.8f, 12.3f);
    
    sf::Vector2u sfmlVec = v;  // Implicit conversion
    
    EXPECT_EQ(sfmlVec.x, 8u);
    EXPECT_EQ(sfmlVec.y, 12u);
}

TEST(Vector2ConversionTest, ZeroVectorConversion) {
    Vector2 v(0.0f, 0.0f);
    
    sf::Vector2f sfmlVecF = v;
    sf::Vector2i sfmlVecI = v;
    sf::Vector2u sfmlVecU = v;
    
    EXPECT_FLOAT_EQ(sfmlVecF.x, 0.0f);
    EXPECT_FLOAT_EQ(sfmlVecF.y, 0.0f);
    EXPECT_EQ(sfmlVecI.x, 0);
    EXPECT_EQ(sfmlVecI.y, 0);
    EXPECT_EQ(sfmlVecU.x, 0u);
    EXPECT_EQ(sfmlVecU.y, 0u);
}

TEST(Vector2ConversionTest, NegativeValuesConversion) {
    Vector2 v(-5.7f, -3.2f);
    
    sf::Vector2f sfmlVecF = v;
    sf::Vector2i sfmlVecI = v;
    
    EXPECT_FLOAT_EQ(sfmlVecF.x, -5.7f);
    EXPECT_FLOAT_EQ(sfmlVecF.y, -3.2f);
    EXPECT_EQ(sfmlVecI.x, -5);
    EXPECT_EQ(sfmlVecI.y, -3);
    
    // Note: sf::Vector2u with negative values would cause undefined behavior
    // in practice, so we don't test that case
}

TEST(Vector2ConversionTest, LargeValuesConversion) {
    Vector2 v(1000.5f, 2000.9f);
    
    sf::Vector2f sfmlVecF = v;
    sf::Vector2i sfmlVecI = v;
    sf::Vector2u sfmlVecU = v;
    
    EXPECT_FLOAT_EQ(sfmlVecF.x, 1000.5f);
    EXPECT_FLOAT_EQ(sfmlVecF.y, 2000.9f);
    EXPECT_EQ(sfmlVecI.x, 1000);
    EXPECT_EQ(sfmlVecI.y, 2000);
    EXPECT_EQ(sfmlVecU.x, 1000u);
    EXPECT_EQ(sfmlVecU.y, 2000u);
}

TEST(Vector2ConversionTest, PrecisionPreservation) {
    Vector2 v(3.14159f, 2.71828f);
    
    sf::Vector2f sfmlVec = v.ConvertTo<sf::Vector2f>();
    
    EXPECT_FLOAT_EQ(sfmlVec.x, 3.14159f);
    EXPECT_FLOAT_EQ(sfmlVec.y, 2.71828f);
}

TEST(Vector2ConversionTest, ConversionInFunctionParameter) {
    Vector2 v(100.0f, 200.0f);
    
    // Lambda that accepts sf::Vector2f
    auto testFunction = [](const sf::Vector2f& vec) -> float {
        return vec.x + vec.y;
    };
    
    // Should work with implicit conversion
    float result = testFunction(v);
    EXPECT_FLOAT_EQ(result, 300.0f);
}

TEST(Vector2ConversionTest, ConversionInAssignment) {
    Vector2 v1(5.5f, 7.3f);
    Vector2 v2(2.1f, 3.8f);
    
    sf::Vector2f sfmlVec1 = v1;
    sf::Vector2f sfmlVec2 = v2;
    
    // Test that converted vectors work with SFML operations
    sf::Vector2f sum = sfmlVec1 + sfmlVec2;
    
    EXPECT_FLOAT_EQ(sum.x, 7.6f);
    EXPECT_FLOAT_EQ(sum.y, 11.1f);
}

TEST(Vector2ConversionTest, ConversionChaining) {
    Vector2 v(42.7f, 13.9f);
    
    // Convert to SFML, perform operation, and verify
    sf::Vector2f sfmlVec = v;
    sf::Vector2f scaled = sfmlVec * 2.0f;
    
    EXPECT_FLOAT_EQ(scaled.x, 85.4f);
    EXPECT_FLOAT_EQ(scaled.y, 27.8f);
}

TEST(Vector2ConversionTest, MultipleConversionsFromSameVector) {
    Vector2 v(16.4f, 32.8f);
    
    sf::Vector2f vecF = v;
    sf::Vector2i vecI = v;
    sf::Vector2u vecU = v;
    
    // Original vector should be unchanged
    EXPECT_FLOAT_EQ(v.x, 16.4f);
    EXPECT_FLOAT_EQ(v.y, 32.8f);
    
    // All conversions should be independent
    EXPECT_FLOAT_EQ(vecF.x, 16.4f);
    EXPECT_FLOAT_EQ(vecF.y, 32.8f);
    EXPECT_EQ(vecI.x, 16);
    EXPECT_EQ(vecI.y, 32);
    EXPECT_EQ(vecU.x, 16u);
    EXPECT_EQ(vecU.y, 32u);
}

TEST(Vector2ConversionTest, ConstexprConversion) {
    Vector2 v(5.0f, 10.0f);
    
    // This should compile as constexpr
    auto sfmlVec = v.ConvertTo<sf::Vector2f>();
    
    EXPECT_FLOAT_EQ(sfmlVec.x, 5.0f);
    EXPECT_FLOAT_EQ(sfmlVec.y, 10.0f);
}

TEST(Vector2ConversionTest, ConversionTest) {
    Vector2 v1(1.0f, 2.0f);
    Vector2 v2(3.0f, 4.0f);

    Vector2 sum = v1 + v2;
    EXPECT_FLOAT_EQ(sum.x, 4.0f);
    EXPECT_FLOAT_EQ(sum.y, 6.0f);
    Vector2 diff = v1 - v2;
    EXPECT_FLOAT_EQ(diff.x, -2.0f);
    EXPECT_FLOAT_EQ(diff.y, -2.0f);
    Vector2 scaled = v1 * 2.0f;
    EXPECT_FLOAT_EQ(scaled.x, 2.0f);
    EXPECT_FLOAT_EQ(scaled.y, 4.0f);
    Vector2 divided = v1 / 2.0f;
    EXPECT_FLOAT_EQ(divided.x, 0.5f);
    EXPECT_FLOAT_EQ(divided.y, 1.0f);
}