#include "pch.h"
#include <Serialization/GameObjectSerializer.h>

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
	std::unique_ptr<InlineGameObject> deserialized = GameObjectSerializer::DeserializeGameObject(serialized);

	// Assert
	EXPECT_EQ(deserialized->GetResult().GetPosition(), pos);
	EXPECT_EQ(deserialized->GetResult().GetSize(), size);
	EXPECT_EQ(deserialized->GetResult().GetName(), name);
}