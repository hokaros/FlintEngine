#include "pch.h"
#include "../FlatEngine/GameObjectSerializer.h"

TEST(GameObjectSerializer, DeserializesSizePositionAndName)
{
	// Arrange
	Vector pos = Vector(10, 1);
	Vector size = Vector(1.742, 2.5);
	std::string name = "The greatest showman";

	GameObjectStringDesc serialized;
	serialized.params.emplace("m_Pos", SerializableTypeInterface<Vector>::ToString(pos));
	serialized.params.emplace("m_Size", SerializableTypeInterface<Vector>::ToString(size));
	serialized.params.emplace("m_Name", name);

	// Act
	std::unique_ptr<GameObject> deserialized = GameObjectSerializer::DeserializeGameObject(serialized);

	// Assert
	EXPECT_EQ(deserialized->GetPosition(), pos);
	EXPECT_EQ(deserialized->GetSize(), size);
	EXPECT_EQ(deserialized->GetName(), name);
}