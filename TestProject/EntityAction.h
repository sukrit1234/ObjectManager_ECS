#pragma once

#include "Object.h"
#include "Types.h"
#include <vector>
#include "Archetype.h"

class AchetypeCollectionObject;
class EntityActionManager;

/*
	Entity Action
	 + Logic class that apply to all of entity that match with combination
	 + assign required component type combination by calling Assign<Component>()
	 + if EntityAction::CanBeVisible() return false , this action will be skipped.
	 + See example of action in ExampleAction.h file.
*/
class EntityAction : public Object
{
	friend class EntityActionManager;

public:

	/*
		return true , All enitity that match with EntityAction::componentCombinations
		will do action otherwise (false) , this action will skipped.
	*/
	virtual bool CanBeVisible() const { return true; }

protected:

	/*Call this action*/
	void Execute(float DeltaSeconds);

	/*Override this for apply action to entities in this batch*/
	virtual void DoExecute(Archetype& _arch, float DeltaSeconds) {}
	virtual void BeforeExecute() {}
	virtual void AfterExecute() {}
	virtual void NotifyArchetypeCreated(std::shared_ptr<Archetype> CreatedArchetype);
	
	//Batch of entities , categorize and assign to archetypes.
	std::vector<std::shared_ptr<Archetype>> archetypes;

	/*Let Assign require component by call Assign in this function*/
	virtual void Prepare();

	/*Assign require component*/
	template<typename C>
	void Assign();

private:

	void applyArchetypes();
	std::shared_ptr<EntityActionManager> manager;

	/*We use ArchetypeID to hold combination of support components*/
	ArchetypeID componentCombinations;
};

template<typename C>
void EntityAction::Assign()
{
	if (manager != nullptr)
	{
		ComponentTypeID componentTypeId = GetComponentTypeId<C>();
		assert(componentTypeId < MAX_COMPONENTS && "Too many component types registered.");

		componentCombinations.set(componentTypeId, true);
		applyArchetypes();
	}
}