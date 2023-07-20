#pragma once

#include "EntityAction.h"
#include "ObjectManager.h"
#include "Types.h"
#include "Archetype.h"
#include "EntityAction.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include "AchetypeCollectionObject.h"

/*
	EntityActionManager
	 + The ObjectManager that create and manage EntityAction object.
	 + Can create/destroy/manage data of entities via EntityID (can manual call or internal called by Entity object)
	 + Categorize entity as batch by combination of component types - called Archetype.
	 + Execute all visible action to all entitity by calling Execute() in app or game loop with delta time (seconds).
*/
class EntityActionManager : public GenericObjectManager<EntityAction>, public AchetypeCollectionObject
{
public:

	typedef const std::unordered_map<ArchetypeID, std::shared_ptr<Archetype>> ArchetypeBuffer;

	/*Retrieve all EnitityAction that EnitityAction::CanBeVisible return true;*/
	void GetAllVisibleAction(std::vector<std::shared_ptr<EntityAction>>& OutObjects) const;

	/*
		Create EntityAction
		A : is class that derived from EntityAction
	*/
	template<class A= EntityAction,class ...TArgs>
	std::shared_ptr<A> CreateAction(TArgs... Args) { return Create<A>(TArgs(Args)...); }

	static EntityActionManager* Get();

	/*Allocate entity id*/
	EntityID CreateEntity();

	void DestroyEntity(const EntityID& entity);

	/*Add component data to entity*/
	template<typename T>
	void AddComponentToEntity(EntityID entity, T component);

	virtual const ArchetypeBuffer& GetArchetypes() const { return archetypes; }

	void Execute(float DeltaSeconds);

protected:

	virtual bool AllowToCreateObject(ObjectTypeID TypeID) const override;
	virtual void OnObjectCreated(std::shared_ptr<Object> NewObject, ObjectTypeID ObjectTypeID) override;
	EntityActionManager();

private:

	EntityID nextEntity;

	std::unordered_map<EntityID, ArchetypeID> entityToArchetypeMap;
	std::unordered_map<ArchetypeID, std::shared_ptr<Archetype>> archetypes;

	ArchetypeID UpdateEntityArchetypeID(const EntityID& entity, const ComponentTypeID& componentType, const bool& isSet);
	void CreateNewArchetype(const ArchetypeID& newArchetypeId);

	template<typename T>
	void AddComponentToEntityArchetype(const EntityID& entity, T& component, const ComponentTypeID& componentType);

private:

	static std::atomic<EntityActionManager*> pinstance;
	static std::mutex m_;
};

template<typename T>
void EntityActionManager::AddComponentToEntity(EntityID entity, T component)
{
	AddComponentToEntityArchetype(entity, component, GetComponentTypeId<T>());
}

template<typename T>
void EntityActionManager::AddComponentToEntityArchetype(const EntityID& entity, T& component, const ComponentTypeID& componentType)
{
	// Update entity archetype ID
	ArchetypeID oldArchetypeId = entityToArchetypeMap.at(entity);
	ArchetypeID newArchetypeId = UpdateEntityArchetypeID(entity, componentType, true);

	// If archetype does not exist, create it
	if (archetypes.find(newArchetypeId) == archetypes.end())
		CreateNewArchetype(newArchetypeId);

	// Get the new archetype
	std::shared_ptr<Archetype> newArchetype = archetypes.at(newArchetypeId);
	newArchetype->numEntities++;

	// Move existing components from entity's old archetype to the new archetype
	if (oldArchetypeId.any())
	{
		std::shared_ptr<Archetype> oldArchetype = archetypes.at(oldArchetypeId);
		oldArchetype->numEntities--;
		oldArchetype->TransferComponents(entity, newArchetype);
	}

	// Add new component to the entity's new archetype
	newArchetype->AddComponent(entity, component);
}