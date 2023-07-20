#include "ExampleActions.h"
#include "ExampleComponent.h"
#include <stdlib.h>
#include <Windows.h>
#include <math.h> 

void MovementAction::Prepare()
{
	Assign<Position>(); 
	Assign<Velocity>();
}
void MovementAction::BeforeExecute()
{
	
}
void MovementAction::DoExecute(Archetype& _arch, float DeltaSeconds)
{
	std::vector<Position>& positions = _arch.GetMutableComponentDataArray<Position>();
	std::vector<Velocity>& velocities = _arch.GetMutableComponentDataArray<Velocity>();
	for (int i = 0; i < _arch.GetNumEntities(); ++i)
	{
		positions[i].x += velocities[i].x*DeltaSeconds;
		positions[i].y += velocities[i].y*DeltaSeconds;
	}
}

void RotateAction::Prepare()
{
	Assign<Angle>();
	Assign<RotateSpeed>();
}
void RotateAction::BeforeExecute()
{

}
void RotateAction::DoExecute(Archetype& _arch, float DeltaSeconds)
{
	std::vector<Angle>& angles = _arch.GetMutableComponentDataArray<Angle>();
	std::vector<RotateSpeed>& rotatespeeds = _arch.GetMutableComponentDataArray<RotateSpeed>();
	for (int i = 0; i < _arch.GetNumEntities(); ++i)
	{
		angles[i].Angle += rotatespeeds[i].RotSpeed * DeltaSeconds;
		angles[i].Angle = fmod(angles[i].Angle, 360.0f);
	}
}


void RenderAction::Prepare()
{
	Assign<CharacterTraits>();
	Assign<Position>();
	Assign<Angle>();
}
void RenderAction::BeforeExecute()
{
	// Function which cleans the screen without flickering
	COORD cursorPosition;

	cursorPosition.X = 0;
	cursorPosition.Y = 0;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);

	std::cout << "Render Action" << std::endl << std::endl;
}
void RenderAction::DoExecute(Archetype& _arch, float DeltaSeconds)
{
	const std::vector<CharacterTraits>& traits = _arch.GetComponentDataArray<CharacterTraits>();
	const std::vector<Position>& positions = _arch.GetComponentDataArray<Position>();
	const std::vector<Angle>& angles = _arch.GetComponentDataArray<Angle>();
	for (int i = 0; i < _arch.GetNumEntities(); ++i)
	{
		std::cout << "  Name: " << traits[i].name << std::endl;
		std::cout << "  Race: " << traits[i].race << std::endl;
		std::cout << "  Position: " << positions[i].x << ", " << positions[i].y << std::endl;
		std::cout << "  Angle: " << angles[i].Angle << std::endl << std::endl;
	}
}