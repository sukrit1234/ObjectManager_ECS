#pragma once
#include "Types.h"

/*Example of component*/

/*Current Position of entity*/
struct Position
{
	float x = 0.0f;
	float y = 0.0f;
};

/*Current angle of entity*/
struct Angle
{
	float Angle;
};

/*Current Velocity of entity*/
struct Velocity
{
	float x = 0.0f;
	float y = 0.0f;
};

/*Current rotation speed of entity*/
struct RotateSpeed
{
	float RotSpeed = 0.0f;
};

/*Trait info for entity*/
struct CharacterTraits
{
	std::string name = "";
	std::string race = "";
};