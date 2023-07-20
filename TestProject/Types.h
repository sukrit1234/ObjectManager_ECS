#include <bitset>

//Define entity as usinged int.
using EntityID = unsigned int; 

//Define entity as usinged int.
using ComponentTypeID = unsigned int;

//Maximum Component types , Set max to 128 bit set.
const ComponentTypeID MAX_COMPONENTS = 128;

// Represents what components make up an archetype
using ArchetypeID = std::bitset<MAX_COMPONENTS>; 


extern ComponentTypeID S_ECS_ComponentIDCounter;
template <class T>
ComponentTypeID GetComponentTypeId()
{
	static ComponentTypeID S_ECS_ComponentTypeId = S_ECS_ComponentIDCounter++;
	return S_ECS_ComponentTypeId;
}

#pragma once
