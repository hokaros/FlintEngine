#include "GameObject.h"
#include "ObjectComponent.h"
#include "ObjectManager.h"

GameObject::GameObject(const std::list<GameObject*>& allObjects)
	: size(1, 1), allObjects(allObjects) {

}

GameObject::GameObject(const Vector& size, const std::list<GameObject*>& allObjects)
	: size(size), allObjects(allObjects) {

}

GameObject::GameObject(const Vector& size, const Vector& position, const std::list<GameObject*>& allObjects)
	: position(position), size(size), allObjects(allObjects) {

}

GameObject::GameObject(const std::list<GameObject*>& allObjects, PrefabCreationKey)
	: GameObject(allObjects) {

}

GameObject::GameObject(const Vector& size, const std::list<GameObject*>& allObjects, PrefabCreationKey)
	: GameObject(size, allObjects) {

}

GameObject::GameObject(const Vector& size, const Vector& position, const std::list<GameObject*>& allObjects, PrefabCreationKey)
	: GameObject(size, position, allObjects) {

}

GameObject::GameObject(const GameObject& other) 
	: GameObject(other.size, other.position, other.allObjects, {}) {
	// Skopiowanie komponentów
	for (IUpdateable* component : other.components) {
		IUpdateable* cmpCpy;

		// Rzutowanie na ObjectComponent, aby zmienić właściciela
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

GameObject* GameObject::Instantiate(const Vector& size)
{
	ObjectManager* object_manager = ObjectManager::Main();

	GameObject* game_object = new GameObject(size, object_manager->GetAllObjects(), {});
	object_manager->AddObject(game_object);

	return game_object;
}

GameObject* GameObject::Instantiate(const Vector& size, const Vector& position)
{
	ObjectManager* object_manager = ObjectManager::Main();

	GameObject* game_object = new GameObject(size, position, object_manager->GetAllObjects(), {});
	object_manager->AddObject(game_object);

	return game_object;
}

GameObject* GameObject::Instantiate(const GameObject& other)
{
	GameObject* game_object = new GameObject(other);
	ObjectManager::Main()->AddObject(game_object);

	return game_object;
}

void GameObject::Destroy(GameObject* game_object)
{
	ObjectManager::Main()->DestroyObject(game_object);
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

void GameObject::Awake()
{
	if (!isEnabled)
		return;

	for (IUpdateable* component : components) {
		component->Awake();
	}
}

void GameObject::OnDestroy()
{
	for (IUpdateable* component : components) {
		component->OnDestroy();
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