#pragma once

#include "Archetype.h"
#include <unordered_map>


/*
	Abstract class for collection of archetype.
	in this project will derived by EntityActionManager.
*/
class AchetypeCollectionObject
{
public:
	virtual const std::unordered_map<ArchetypeID, std::shared_ptr<Archetype>>& GetArchetypes() const = 0;
};