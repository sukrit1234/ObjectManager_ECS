#pragma once

#include <vector>
#include <unordered_map>
#include <cassert>
#include <memory>
#include "Types.h"
#include "ComponentArrayObject.h"


/*
	Keep typed component data for entities. That will access in EntityAction
*/
template<typename T>
class ComponentArray : public ComponentArrayObject
{
public:


	/*Assign component data to entity*/
	void Assign(const EntityID& entity, const T& component);
	
	/*Remove component data to entity*/
	void Remove(const EntityID& entity);

	/*Get constant data buffer reference*/
	const std::vector<T>& GetDataArray() const;
	
	/*Get mutable data buffer reference*/
	std::vector<T>& GetMutableDataArray();

	/*Get constant reference of entity's component data*/
	const T& GetData(const EntityID& entity) const;

	/*Get mutable reference of entity's component data*/
	T& GetMutableData(const EntityID& entity);

	/*Check component data is exists for entity*/
	virtual bool ExitsForEntity(const EntityID& entity) const override;

	/*Called when entity destroy to cleanup space and finalize something*/
	virtual void NotifyEntityDestroyed(const EntityID& entity) override;

	/*create empty buffer for same component type*/
	virtual std::shared_ptr<ComponentArrayObject> CreateEmptyClone() const override
	{
		return std::make_shared<ComponentArray<T>>();
	}

	/*Move entity's component data to another archetype when combination of component type changed*/
	virtual void TransferComponents(const EntityID& entity, std::shared_ptr<ComponentArrayObject> newVector) override
	{
		std::shared_ptr<ComponentArray<T>> newPtr = std::static_pointer_cast<ComponentArray<T>>(newVector);
		newPtr->Assign(entity, this->GetData(entity));
	}

private:
	std::vector<T> data_buffer;

	// Map from entity ID => to component data index
	std::unordered_map<EntityID, size_t> entityToIndexMap;

	// Map from component data index to entity ID.
	std::unordered_map<size_t, EntityID> indexToEntityMap;
};

template<typename T>
bool ComponentArray<T>::ExitsForEntity(const EntityID& entity) const
{
	return entityToIndexMap.find(entity) != entityToIndexMap.end();
}

template<typename T>
void ComponentArray<T>::Assign(const EntityID& entity, const T& component)
{
	assert(!ExitsForEntity(entity) && "Component type added to same entity more than once.");

	// Put new entry at end and update the maps
	size_t index = data_buffer.size();
	data_buffer.push_back(component);
	entityToIndexMap.insert({ entity, index });
	indexToEntityMap.insert({ index, entity });
}

template<typename T>
void ComponentArray<T>::Remove(const EntityID& entity)
{
	assert(ExitsForEntity(entity) && "Attempting to remove component that does not exist.");

	/*Use swap and pop back to remove element*/
	size_t indexOfRemovedEntity = entityToIndexMap.at(entity);
	size_t indexOfLastEntity = data_buffer.size() - 1;

	data_buffer[indexOfRemovedEntity] = data_buffer[indexOfLastEntity];
	data_buffer.pop_back();

	// Update map to point to moved spot
	EntityID lastEntity = indexToEntityMap[indexOfLastEntity];
	entityToIndexMap.at(lastEntity) = indexOfRemovedEntity;
	indexToEntityMap.at(indexOfRemovedEntity) = lastEntity;

	entityToIndexMap.erase(entity);
	indexToEntityMap.erase(indexOfLastEntity);
}

template<typename T>
void ComponentArray<T>::NotifyEntityDestroyed(const EntityID& entity)
{
	if (ExitsForEntity(entity))
		Remove(entity);
}

template<typename T>
const std::vector<T>& ComponentArray<T>::GetDataArray() const
{
	return data_buffer;
}
template<typename T>
std::vector<T>& ComponentArray<T>::GetMutableDataArray()
{
	return data_buffer;
}

template<typename T>
const T& ComponentArray<T>::GetData(const EntityID& entity) const
{
	assert(ExitsForEntity(entity) && "Attempting to retrieve component that does not exist.");
	return data_buffer[entityToIndexMap.at(entity)];
}

template<typename T>
T& ComponentArray<T>::GetMutableData(const EntityID& entity)
{
	assert(ExitsForEntity(entity) && "Attempting to retrieve component that does not exist.");
	return data_buffer[entityToIndexMap.at(entity)];
}
