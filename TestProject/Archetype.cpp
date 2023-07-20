#include "Archetype.h"

void Archetype::NotifyEntityDestroyed(const EntityID& entity)
{
	for (std::pair<ComponentTypeID, std::shared_ptr<ComponentArrayObject>> pair : ComponentArrays)
		pair.second->NotifyEntityDestroyed(entity);
	numEntities--;
}

int Archetype::GetNumEntities() const
{
	return numEntities;
}
Archetype::Archetype(ArchetypeID _archetypeId) :
	archetypeId(_archetypeId) ,numEntities(0)
{

}
void Archetype::RemoveComponentInternal(const EntityID& entity, ComponentTypeID compTypeID)
{
	ComponentArrays.at(compTypeID)->Remove(entity);
}
std::shared_ptr<ComponentArrayObject> Archetype::GetComponentArrayInternal(ComponentTypeID comTypeID) const
{
	return ComponentArrays.at(comTypeID);
}
void Archetype::TransferComponents(const EntityID& entity, std::shared_ptr<Archetype> newArchetype)
{
	for (std::pair<ComponentTypeID, std::shared_ptr<ComponentArrayObject>> pair : ComponentArrays)
	{
		// If the new archetype does not contain a ComponentVector of this type yet, create it
		if (newArchetype->ComponentArrays.count(pair.first) == 0)
			newArchetype->ComponentArrays.insert({ pair.first, pair.second->CreateEmptyClone() });
	
		// Copy entity's component from this archetype to new archetype
		pair.second->TransferComponents(entity, newArchetype->GetComponentArrayInternal(pair.first));

		// Once component is moved, remove it from this archetype
		RemoveComponentInternal(entity, pair.first);
	}
}