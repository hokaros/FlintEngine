#include "ITransformable.h"

Vector ITransformable::VectorLocalToWorld(const Vector& local_vector)
{
	return TransformPoint(local_vector) - TransformPoint(Vector::ZERO);
}