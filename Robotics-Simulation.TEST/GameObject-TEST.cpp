#include "pch.h"
#include "GameObject.h"

TEST(GAMEOBJECTTEST, SetOnePosition) {
	// Arrange
	GameObject obj;
	Vector2 initialPosition(10.0f, 20.0f);

	// Act
	obj.SetPosition(initialPosition);
	Vector2 retrievedPosition = obj.GetPosition();

	// Assert
	EXPECT_FLOAT_EQ(retrievedPosition.x, initialPosition.x);
	EXPECT_FLOAT_EQ(retrievedPosition.y, initialPosition.y);
}

