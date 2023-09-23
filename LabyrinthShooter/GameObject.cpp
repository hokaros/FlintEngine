#include "GameObject.h"
#include "ObjectComponent.h"

GameObject::GameObject(const std::list<GameObject*>& allObjects)
	: size(1, 1), allObjects(allObjects) {

}

GameObject::GameObject(const Vector& size, const std::list<GameObject*>& allObjects)
	: size(size), allObjects(allObjects) {

}

GameObject::GameObject(const Vector& size, const Vector& position, const std::list<GameObject*>& allObjects)
	: position(position), size(size), allObjects(allObjects) {

}

GameObject::GameObject(const GameObject& other) 
	: GameObject(other.size, other.position, other.allObjects) {
	// Skopiowanie komponentów
	for (IUpdateable* component : other.components) {
		IUpdateable* cmpCpy;

		// Rzutowanie na ObjectComponent, aby zmieniæ w³aœciciela
		ObjectComponent* objCmp = static_cast<ObjectComponent*>(component);
		if (objCmp != NULL) {
			cmpCpy = objCmp->Copy(*this);
		}
		else {
			cmpCpy = component->Copy();
		}

		components.push_back(cmpCpy);
	}

	IUpdateable* rendererCpy;
	if (other.renderer != NULL) {
		ObjectComponent* objCmp = static_cast<ObjectComponent*>(other.renderer);
		if (objCmp != NULL) {
			rendererCpy = objCmp->Copy(*this);
		}
		else {
			rendererCpy = other.renderer->Copy();
		}
		renderer = rendererCpy;
	}

	// Skopiowanie dzieci
	for (GameObject* child : other.children) {
		GameObject* childCopy = new GameObject(*child);

		AddChild(childCopy);
	}
}

GameObject::~GameObject() {
	for (IUpdateable* component : components) {
		delete component;
	}

	delete renderer;
}

void GameObject::AddComponent(IUpdateable* component) {
	components.push_back(component);
}

void GameObject::SetRenderer(IUpdateable* renderer) {
	this->renderer = renderer;
}

void GameObject::Update() {
	if (!isEnabled)
		return;

	for (IUpdateable* component : components) {
		component->Update();
	}

	HandleCollisions();
}

void GameObject::RenderUpdate() {
	if (!isEnabled)
		return;

	if (renderer != NULL)
		renderer->RenderUpdate();
}

void GameObject::Start() {
	if (!isEnabled)
		return;

	for (IUpdateable* component : components) {
		component->Start();
	}
}

const Vector& GameObject::GetSize() const {
	return size;
}

double GameObject::GetRotation() const {
	return rotation;
}

Vector GameObject::LookingDirection() const {
	return Vector(
		cos(rotation * M_PI / 180),
		sin(rotation * M_PI / 180)
	);
}

const Vector& GameObject::GetPosition() const {
	return position;
}

Vector GameObject::GetMiddle() const {
	return position + size / 2;
}

void GameObject::SetPosition(const Vector& newPosition) {
	Vector offset = newPosition - position;
	position = newPosition;

	for (GameObject* child : children) {
		child->Translate(offset);
	}
}

void GameObject::Translate(const Vector& offset) {
	position += offset;

	for (GameObject* child : children) {
		child->Translate(offset);
	}
}

void GameObject::SetSize(const Vector& newSize) {
	Vector sizeChange(newSize.x / size.x, newSize.y / size.y);

	size.x = newSize.x;
	size.y = newSize.y;

	// Rozmiar dzieci
	for (GameObject* child : children) {
		Vector childNewSize(child->size.x * sizeChange.x, child->size.y * sizeChange.y);
		child->SetSize(childNewSize);
	}
}

void GameObject::Rotate(double angle) {
	double prevRot = rotation;
	double newRot = rotation + angle;
	double newRotRadians = newRot * M_PI / 180;

	Vector middle = GetMiddle();
	
	for (GameObject* child : children) {
		child->Rotate(angle);

		Vector childMid = child->GetMiddle();
		double radius = (middle - childMid).Length();

		Vector childNewPos(
			cos(newRotRadians) * radius,
			sin(newRotRadians) * radius
		);
		childNewPos += middle;
		Vector dPos = childNewPos - childMid;

		child->Translate(dPos);
	}


	rotation += angle;
}

void GameObject::SetRotation(double newRot) {
	double dRot = newRot - rotation;
	Rotate(dRot);
}

void GameObject::LookAt(const Vector& point) {
	Vector toPoint = point - GetMiddle();
	double lookRotation = atan2(toPoint.y, toPoint.x) * 180 / M_PI;

	double dRot = lookRotation - rotation;
	Rotate(dRot);
}

Vector GameObject::LocalToWorld(const Vector& localPos) const {
	Vector fromMid = localPos - size / 2;
	//double radius = fromMid.Length();
	fromMid.Rotate(rotation * M_PI / 180);

	Vector rotatedSize = size;
	rotatedSize.Rotate(rotation * M_PI / 180);

	return fromMid + position + rotatedSize / 2;
	/*double localAngle = atan2(fromMid.y, fromMid.x) * 180 / M_PI;
	double targetAngle = localAngle - rotation;

	Vector unrotatedPos(
		cos(targetAngle * M_PI / 180) * radius,
		sin(targetAngle * M_PI / 180) * radius
	);*/
	//return unrotatedPos + position + rotatedSize / 2;
}

void GameObject::AddChild(GameObject* child) {
	child->parent = this;

	children.push_back(child);
}

void GameObject::RemoveChild(GameObject* child) {
	child->parent = NULL;

	children.remove(child);
}

const std::list<GameObject*>& GameObject::GetChildren() const {
	return children;
}

GameObject* GameObject::GetParent() const {
	return parent;
}

bool GameObject::Collides(const GameObject& other) const {
	if (!collisionEnabled || !other.collisionEnabled)
		return false;

	// Rozpatrzenie kolizji odwrotnych (mog¹ siê znosiæ)
	bool inverse = false;
	if (insideOutCollision)
		inverse = !inverse;
	if (other.insideOutCollision)
		inverse = !inverse;

	bool intersect = DoesIntersect(other);
	if (inverse) {
		if (IsInside(*this, other))
			return false;  // wewn¹trz
		if (intersect)
			return true;  // na zewn¹trz
	}

	return intersect;
}

bool GameObject::CollidesWithAny() const {
	for (GameObject* go : allObjects) {
		if (go != this && Collides(*go))
			return true;
	}

	return false;
}

std::vector<VectorInt>* GameObject::GetPixels() const {
	std::vector<VectorInt>* pixels = new std::vector<VectorInt>();

	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			pixels->push_back(position + Vector(x, y));
		} 
	}

	return pixels;
}

void GameObject::SetDestroyed(bool destroyed) {
	isEnabled = !destroyed;

	for (function<void(GameObject*)> handler : onDestroyedChanged) {
		if (handler)
			handler(this);
	}
}

void GameObject::SetEnabled(bool enabled) {
	isEnabled = enabled;

	for (GameObject* child : children) {
		child->SetEnabled(enabled);
	}
}

bool GameObject::IsDestroyed() const {
	return !isEnabled;
}

bool GameObject::IsEnabled() const {
	return isEnabled;
}

void GameObject::SubscribeDestroyed(function<void(GameObject*)> handler) {
	onDestroyedChanged.push_back(handler);
}

bool GameObject::DoesIntersect(const GameObject& other) const {
	float yMin1 = position.y;
	float yMax1 = yMin1 + size.y;
	float yMin2 = other.position.y;
	float yMax2 = yMin2 + other.size.y;

	if (!DoLinesIntersect(yMin1, yMax1, yMin2, yMax2))
		return false;

	float xMin1 = position.x;
	float xMax1 = xMin1 + size.x;
	float xMin2 = other.position.x;
	float xMax2 = xMin2 + other.size.x;

	if (!DoLinesIntersect(xMin1, xMax1, xMin2, xMax2))
		return false;

	return true;
}

bool GameObject::IsInside(const GameObject& go1, const GameObject& go2) {
	float yMin1 = go1.position.y;
	float yMax1 = yMin1 + go1.size.y;
	float yMin2 = go2.position.y;
	float yMax2 = yMin2 + go2.size.y;

	if (!IsLineInside(yMin1, yMax1, yMin2, yMax2))
		return false;


	float xMin1 = go1.position.x;
	float xMax1 = xMin1 + go1.size.x;
	float xMin2 = go2.position.x;
	float xMax2 = xMin2 + go2.size.x;

	if (!IsLineInside(xMin1, xMax1, xMin2, xMax2))
		return false;

	return true;
}

Rect GameObject::GetIntersection(const GameObject& other) const {
	float yMin1 = position.y;
	float yMax1 = yMin1 + size.y;
	float yMin2 = other.position.y;
	float yMax2 = yMin2 + other.size.y;

	Vector yIntersection = LinesIntersection(yMin1, yMax1, yMin2, yMax2);

	float xMin1 = position.x;
	float xMax1 = xMin1 + size.x;
	float xMin2 = other.position.x;
	float xMax2 = xMin2 + other.size.x;

	Vector xIntersection = LinesIntersection(xMin1, xMax1, xMin2, xMax2);

	Vector size(
		xIntersection.y - xIntersection.x,
		yIntersection.y - yIntersection.x
	);
	return Rect(
		Vector(xIntersection.x, yIntersection.x),
		size
	);
}

bool GameObject::IsLineInside(float min1, float max1, float min2, float max2) {
	return (min1 >= min2 && max1 <= max2)
		|| (min2 >= min1 && max2 <= max1);
}
bool GameObject::DoLinesIntersect(float min1, float max1, float min2, float max2) {
	return (min1 >= min2 && min1 <= max2)
		|| (max1 >= min2 && max1 <= max2)
		|| (min2 >= min1 && min2 <= max1)
		|| (max2 >= min1 && max2 <= max1);
}

Vector GameObject::LinesIntersection(float min1, float max1, float min2, float max2) {
	if (!DoLinesIntersect(min1, max1, min2, max2))
		return Vector(0, 0);

	float start = (min1 < min2) ? min2 : min1;
	float end = (max1 < max2) ? max1 : max2;
	return Vector(start, end);
}

void GameObject::HandleCollisions() {
	for (GameObject* go : allObjects) {
		if (go == this || !go->isEnabled)
			continue;

		if (Collides(*go)) {
			if (onCollision) {
				onCollision(*go);
			}
			if (go->onCollision) {
				go->onCollision(*this);
			}

			if (!go->isStatic && go->bumping && bumping) {
				BumpOut(*go);
			}
		}
	}
}

void GameObject::BumpOut(GameObject& other) {
	// Tylko jeœli koliduje z other
	Rect intersection = GetIntersection(other);

	Vector outVector = other.GetMiddle() - intersection.GetMiddle();
	if (outVector.Length() == 0) {
		outVector = Vector(0, -FLT_EPSILON);
	}

	float speed = BUMPOUT_SPEED / outVector.Length(); // im bli¿ej, tym bardziej odpycha
	outVector.Normalize();
	Vector dPos = outVector * speed * Timer::Main()->GetDeltaTime();
	other.Translate(dPos);
}