#pragma once

#include <vector>
#include <unordered_map>
#include <cassert>
#include <memory>
#include "Types.h"
#include "ComponentArray.h"

/*
	Archetype
	 + Batch of Enitity's data categorize by combination of component type (called ArchetypeID)
	 + If entity add/remove/change require some of component types. Entity will switch to other archetype.
	 + Can access entity's data reference both constant and mutable.
	 + this version is simple implement.
	 + @Todo - should have Archetype Chunck instead of Archetype itself to hold data and 
	          Archetype hold reference of chucks. To more performance and reduce cache miss.
			 - should change both Archetype and Archetype chunk from class to struct.

	@see more explanation of Archetype 
	https://ajmmertens.medium.com/building-an-ecs-2-archetypes-and-vectorization-fe21690805f9
*/
class Archetype
{
public:

	Archetype(ArchetypeID _archetypeId);

	friend class EntityActionManager;

	int GetNumEntities() const;

	template<typename T>
	const std::vector<T>& GetComponentDataArray() const;

	template<typename T>
	std::vector<T>& GetMutableComponentDataArray();


	template<typename T>
	void AddComponent(EntityID entity, const T& component);
	void RemoveComponentInternal(const EntityID& entity, ComponentTypeID compTypeID);
	
	ArchetypeID GetArchetypeId() const { return archetypeId; }
	int GetEntityCount() const { return numEntities; }

private:

	ArchetypeID archetypeId;
	int numEntities;

	std::unordered_map<ComponentTypeID, std::shared_ptr<ComponentArrayObject>> ComponentArrays;

	void NotifyEntityDestroyed(const EntityID& entity);

	template<typename T>
	std::shared_ptr<ComponentArrayObject> GetComponentArray();

	template<typename T>
	void RemoveComponent(const EntityID& entity);

	std::shared_ptr<ComponentArrayObject> GetComponentArrayInternal(ComponentTypeID comTypeID) const;

	void TransferComponents(const EntityID& entity, std::shared_ptr<Archetype> newArchetype);
};

template<typename T>
std::shared_ptr<ComponentArrayObject> Archetype::GetComponentArray()
{
	return GetComponentArrayInternal(GetComponentTypeId<T>());
}

template<typename T>
void Archetype::RemoveComponent(const EntityID& entity)
{
	RemoveComponentInternal(GetComponentTypeId<T>());
}

template<typename T>
const std::vector<T>& Archetype::GetComponentDataArray() const
{
	ComponentTypeID ComTypeID = GetComponentTypeId<T>();
	return std::static_pointer_cast<ComponentArray<T>>(ComponentArrays.at(ComTypeID))->GetDataArray();
}

template<typename T>
std::vector<T>& Archetype::GetMutableComponentDataArray()
{
	ComponentTypeID ComTypeID = GetComponentTypeId<T>();
	return std::static_pointer_cast<ComponentArray<T>>(ComponentArrays.at(ComTypeID))->GetMutableDataArray();
}

template<typename T>
void Archetype::AddComponent(EntityID entity,const T& component)
{
	ComponentTypeID ComTypeID = GetComponentTypeId<T>();
	if (ComponentArrays.find(ComTypeID) == ComponentArrays.end())
		ComponentArrays.insert({ ComTypeID, std::make_shared<ComponentArray<T>>() });
	std::static_pointer_cast<ComponentArray<T>>(ComponentArrays.at(ComTypeID))->Assign(entity, component);
}