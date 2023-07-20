#pragma once
#include "Types.h"

/*
	Abstract class for component data buffer.
*/
class ComponentArrayObject
{
public:

	virtual bool ExitsForEntity(const EntityID& entity) const = 0;
	virtual void Remove(const EntityID& entity) = 0;
	virtual void NotifyEntityDestroyed(const EntityID& entity) = 0;

	virtual  std::shared_ptr<ComponentArrayObject> CreateEmptyClone() const = 0;
	virtual void TransferComponents(const EntityID& entity, std::shared_ptr<ComponentArrayObject> NewArray) = 0;
};

