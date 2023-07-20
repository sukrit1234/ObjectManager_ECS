#include "EntityActionManager.h"

std::atomic<EntityActionManager*> EntityActionManager::pinstance = nullptr;
std::mutex EntityActionManager::m_;


EntityActionManager::EntityActionManager() 
	: nextEntity(0)
{

}

EntityID EntityActionManager::CreateEntity()
{
	EntityID newEntity = nextEntity;
	entityToArchetypeMap.insert({ newEntity, ArchetypeID() });
	++nextEntity;
	return newEntity;
}

void EntityActionManager::DestroyEntity(const EntityID& entity)
{
	if (entityToArchetypeMap.at(entity).any())
	{
		std::shared_ptr<Archetype> archetype = archetypes.at(entityToArchetypeMap.at(entity));
		archetype->NotifyEntityDestroyed(entity);
		entityToArchetypeMap.erase(entity);
	}
}
ArchetypeID EntityActionManager::UpdateEntityArchetypeID(const EntityID& entity, const ComponentTypeID& componentType, const bool& isSet)
{
	ArchetypeID entityArchetype = entityToArchetypeMap.at(entity);

	// Make sure we aren't trying to add a component type more than once.
	if (isSet)
		assert(entityArchetype.test(componentType) == false && "Component type added to same entity more than once.");

	entityArchetype.set(componentType, isSet);
	entityToArchetypeMap.at(entity) = entityArchetype;

	return entityArchetype;
}

void EntityActionManager::CreateNewArchetype(const ArchetypeID& newArchetypeId)
{
	assert(archetypes.find(newArchetypeId) == archetypes.end() && "Added archetype more than once.");

	std::shared_ptr<Archetype> newArchetype = std::make_shared<Archetype>(newArchetypeId);
	archetypes.insert({ newArchetypeId, newArchetype });

	// Add new archetype to any systems that match it
	for (auto _iter : *this)
		_iter->NotifyArchetypeCreated(archetypes.at(newArchetypeId));
}
void EntityActionManager::Execute(float DeltaSeconds)
{
	for (auto _iter : *this)
		_iter->Execute(DeltaSeconds);
}
bool EntityActionManager::AllowToCreateObject(ObjectTypeID TypeID) const
{
	bool AlreadyCreated = HasObjectByType(TypeID);
	assert(!AlreadyCreated && "Added system more than once.");
	return !AlreadyCreated;
}
void EntityActionManager::OnObjectCreated(std::shared_ptr<Object> NewObject, ObjectTypeID ObjectTypeID)
{
	ObjectManager::OnObjectCreated(NewObject, ObjectTypeID);
	std::shared_ptr<EntityAction> action = std::dynamic_pointer_cast<EntityAction>(NewObject);
	if (action != nullptr)
	{
		action->manager = std::shared_ptr<EntityActionManager>(this);
		action->Prepare();
	}
}

void EntityActionManager::GetAllVisibleAction(std::vector<std::shared_ptr<EntityAction>>& OutObjects) const
{
	EntityActionManager* AsMgr = const_cast<EntityActionManager*>(this);
	for (auto _iter : (*AsMgr)) {
		if (_iter != nullptr && _iter->CanBeVisible())
			OutObjects.push_back(_iter);
	}
}

EntityActionManager* EntityActionManager::Get() {
	if (pinstance == nullptr) {
		std::lock_guard<std::mutex> lock(m_);
		if (pinstance == nullptr)
			pinstance = new EntityActionManager();
	}
	return pinstance;
}