#include "pch.h"
#include "core/Vector2.h"

TEST(NewVector2Case, ConversionTest) {
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