#pragma once
#include "EntityAction.h"
#include <iostream>

/*
	Example of Action.
*/

/*
	Move entity position with speed.
	require : Position,Velocity
*/
class MovementAction : public EntityAction
{
public:
	
	virtual void Prepare() override;
	virtual void BeforeExecute() override;
	virtual void DoExecute(Archetype& _arch, float DeltaSeconds) override;
};

/*
	Rotate entity with speed
	require : Angle,RotateSpeed
*/
class RotateAction : public EntityAction
{
public:

	virtual void Prepare() override;
	virtual void BeforeExecute() override;
	virtual void DoExecute(Archetype& _arch, float DeltaSeconds) override;
};

/*
	Render current Position and Rotation of each entity as Text
	require : CharacterTraits , Position , Angle
*/
class RenderAction : public EntityAction
{
public:

	virtual void Prepare() override;
	virtual void BeforeExecute() override;
	virtual void DoExecute(Archetype& _arch, float DeltaSeconds) override;
};