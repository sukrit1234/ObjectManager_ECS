#include "EntityAction.h"
#include "EntityActionManager.h"
#include "AchetypeCollectionObject.h"


void EntityAction::applyArchetypes()
{
	archetypes.clear();
	const EntityActionManager::ArchetypeBuffer& archetype_collection = manager->GetArchetypes();
	for (std::pair<ArchetypeID, std::shared_ptr<Archetype>> pair : archetype_collection)
	{
		if ((pair.first & componentCombinations) == componentCombinations)
			archetypes.push_back(pair.second);
	}
}
void EntityAction::NotifyArchetypeCreated(std::shared_ptr<Archetype> CreatedArchetype)
{
	ArchetypeID NewArchetypeID = CreatedArchetype->GetArchetypeId();
	if ((componentCombinations & NewArchetypeID) == componentCombinations)
		archetypes.push_back(CreatedArchetype);
}
void EntityAction::Execute(float DeltaSeconds)
{
	/*Call for prepare something before batch execute*/
	BeforeExecute();

	// The physics system iterates through each archetype that contains Position and Velocity 
	// components. Each system has a vector of pointers to the archetypes it cares about
	for (std::shared_ptr<Archetype> archetype : archetypes)
	{
		if (archetype != nullptr)
		{
			Archetype& _arch = (*archetype);
			DoExecute(_arch, DeltaSeconds);
		}
	}
	AfterExecute();
}
void EntityAction::Prepare()
{

}
