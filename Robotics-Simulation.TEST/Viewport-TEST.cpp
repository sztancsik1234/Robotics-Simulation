#include "pch.h"
#include "graphics/Viewport.h"
#include <cmath>

class ViewportTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Standard test configuration:
        // - Screen: 800x600 pixels
        // - View centered at world origin (0, 0)
        // - View size: 10x10 world units
        // This gives us a view from (-5, -5) to (5, 5) in world space
        viewport = new Viewport(
            Vector2{ 0.f, 0.f },      // viewCenterWorld
            Vector2{ 800.f, 600.f },  // screenSizePixels
            Vector2{ 10.f, 10.f }     // viewSizeWorld
        );
    }

    void TearDown() override
    {
        delete viewport;
        viewport = nullptr;
    }

    Viewport* viewport = nullptr;

    // Helper to check if two vectors are approximately equal
    void ExpectVectorNear(const Vector2& actual, const Vector2& expected, float tolerance = 0.5f)
    {
        EXPECT_NEAR(actual.x, expected.x, tolerance) << "X coordinate mismatch";
        EXPECT_NEAR(actual.y, expected.y, tolerance) << "Y coordinate mismatch";
    }
};

// Test 1: World point at center maps to screen center
TEST_F(ViewportTest, WorldToPixel_CenterPoint)
{
    // World origin (0, 0) should map to screen center (400, 300)
    Vector2 worldPos{ 0.f, 0.f };
    Vector2 screenPos = viewport->WorldToPixelPos(worldPos);
    
    ExpectVectorNear(screenPos, Vector2{ 400.f, 300.f });
}

// Test 2: World point at top-left of view maps to top-left corner of screen
TEST_F(ViewportTest, WorldToPixel_TopLeftCorner)
{
    // World position (-5, 5) is at the top-left of the view
    // Should map to screen position (0, 0)
    Vector2 worldPos{ -5.f, 5.f };
    Vector2 screenPos = viewport->WorldToPixelPos(worldPos);
    
    ExpectVectorNear(screenPos, Vector2{ 0.f, 0.f });
}

// Test 3: World point at top-right of view maps to top-right corner of screen
TEST_F(ViewportTest, WorldToPixel_TopRightCorner)
{
    // World position (5, 5) is at the top-right of the view
    // Should map to screen position (800, 0)
    Vector2 worldPos{ 5.f, 5.f };
    Vector2 screenPos = viewport->WorldToPixelPos(worldPos);
    
    ExpectVectorNear(screenPos, Vector2{ 800.f, 0.f });
}

// Test 4: World point at bottom-left of view maps to bottom-left corner of screen
TEST_F(ViewportTest, WorldToPixel_BottomLeftCorner)
{
    // World position (-5, -5) is at the bottom-left of the view
    // Should map to screen position (0, 600)
    Vector2 worldPos{ -5.f, -5.f };
    Vector2 screenPos = viewport->WorldToPixelPos(worldPos);
    
    ExpectVectorNear(screenPos, Vector2{ 0.f, 600.f });
}

// Test 5: World point at bottom-right of view maps to bottom-right corner of screen
TEST_F(ViewportTest, WorldToPixel_BottomRightCorner)
{
    // World position (5, -5) is at the bottom-right of the view
    // Should map to screen position (800, 600)
    Vector2 worldPos{ 5.f, -5.f };
    Vector2 screenPos = viewport->WorldToPixelPos(worldPos);
    
    ExpectVectorNear(screenPos, Vector2{ 800.f, 600.f });
}

// Reverse conversion tests: Pixel to World

TEST_F(ViewportTest, PixelToWorld_CenterPoint)
{
    // Screen center (400, 300) should map back to world origin (0, 0)
    Vector2 pixelPos{ 400.f, 300.f };
    Vector2 worldPos = viewport->PixelToWorldPos(pixelPos);
    
    ExpectVectorNear(worldPos, Vector2{ 0.f, 0.f });
}

TEST_F(ViewportTest, PixelToWorld_TopLeftCorner)
{
    // Screen position (0, 0) should map to world position (-5, 5)
    Vector2 pixelPos{ 0.f, 0.f };
    Vector2 worldPos = viewport->PixelToWorldPos(pixelPos);
    
    ExpectVectorNear(worldPos, Vector2{ -5.f, 5.f });
}

TEST_F(ViewportTest, PixelToWorld_TopRightCorner)
{
    // Screen position (800, 0) should map to world position (5, 5)
    Vector2 pixelPos{ 800.f, 0.f };
    Vector2 worldPos = viewport->PixelToWorldPos(pixelPos);
    
    ExpectVectorNear(worldPos, Vector2{ 5.f, 5.f });
}

TEST_F(ViewportTest, PixelToWorld_BottomLeftCorner)
{
    // Screen position (0, 600) should map to world position (-5, -5)
    Vector2 pixelPos{ 0.f, 600.f };
    Vector2 worldPos = viewport->PixelToWorldPos(pixelPos);
    
    ExpectVectorNear(worldPos, Vector2{ -5.f, -5.f });
}

TEST_F(ViewportTest, PixelToWorld_BottomRightCorner)
{
    // Screen position (800, 600) should map to world position (5, -5)
    Vector2 pixelPos{ 800.f, 600.f };
    Vector2 worldPos = viewport->PixelToWorldPos(pixelPos);
    
    ExpectVectorNear(worldPos, Vector2{ 5.f, -5.f });
}

// Test round-trip conversion accuracy
TEST_F(ViewportTest, RoundTrip_WorldToPixelToWorld)
{
    Vector2 originalWorld{ 2.5f, 3.7f };
    Vector2 screen = viewport->WorldToPixelPos(originalWorld);
    Vector2 backToWorld = viewport->PixelToWorldPos(screen);
    
    ExpectVectorNear(backToWorld, originalWorld, 0.1f);
}

// Test Transform conversion
TEST_F(ViewportTest, ToScreenSpace_CenterTransform)
{
    Transform worldTransform;
    worldTransform.position = Vector2{ 0.f, 0.f };
    worldTransform.size = Vector2{ 1.f, 1.f };  // 1 world unit
    worldTransform.rotation = 0.f;
    
    Transform screenTransform = viewport->ToScreenSpace(worldTransform);
    
    ExpectVectorNear(screenTransform.position, Vector2{ 400.f, 300.f });
    // 1 world unit = 80 pixels in X, 60 pixels in Y
    ExpectVectorNear(screenTransform.size, Vector2{ 80.f, 60.f });
    EXPECT_FLOAT_EQ(screenTransform.rotation, 0.f);
}

// Test viewport reconfiguration
TEST_F(ViewportTest, SetViewCenter_UpdatesConversion)
{
    // Move viewport center to (5, 5) in world space
    viewport->SetViewCenter(Vector2{ 5.f, 5.f });
    
    // World position (5, 5) should now be at screen center
    Vector2 screenPos = viewport->WorldToPixelPos(Vector2{ 5.f, 5.f });
    ExpectVectorNear(screenPos, Vector2{ 400.f, 300.f });
}

TEST_F(ViewportTest, SetViewSize_UpdatesScale)
{
    // Change view size to 20x20 (zooming out)
    viewport->SetViewSize(Vector2{ 20.f, 20.f });
    
    // Now the view covers (-10, -10) to (10, 10)
    // World position (10, 10) should be at top-right corner
    Vector2 screenPos = viewport->WorldToPixelPos(Vector2{ 10.f, 10.f });
    ExpectVectorNear(screenPos, Vector2{ 800.f, 0.f });
}