// TestProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>
#include "Types.h"
#include "ExampleComponent.h"
#include "ExampleActions.h"
#include "EntityActionManager.h"
#include "Entity.h"

//Example and project runing.

int main()
{
	EntityActionManager* EntityActionMgrPtr = EntityActionManager::Get();
	EntityActionManager& EntityActionMgr = (*EntityActionMgrPtr);

	/*Define Action for all of entities*/
	/*1. Movement Action  move enitity position with linear velocity*/
	EntityActionMgr.CreateAction<MovementAction>();
	/*2. Rotate Action rotate enitity angle with rotate speed*/
	EntityActionMgr.CreateAction<RotateAction>();
	/*3. Render enitity position and angle to screen*/
	EntityActionMgr.CreateAction<RenderAction>();

	/*Create two entities*/
	/*For Example just player and goblin*/
	Entity player, goblin;

	/*
		Add support component for player entity
		Position , Velocity , Angle , Rotate Speed, and Trait Info.
		so Player will move and rotate
	*/
	player.Assign(Position{ 234.0f, 456.0f });
	player.Assign(Velocity{ 3.4f , 6.7f });
	player.Assign(Angle{ 0.0f });
	player.Assign(RotateSpeed{ 40.0f });
	player.Assign(CharacterTraits{ "Bob", "Human" });

	/*
		Add Support component for goblin entity
		Position , Angle , Rotate Speed , and Trait Info.
		so Goblin will only rotate.
	*/
	goblin.Assign(Position{ 567.0f , 876.0f });
	goblin.Assign(Angle{ 0.0f });
	goblin.Assign(RotateSpeed{ 0.5f });
	goblin.Assign(CharacterTraits{ "Griblub", "Goblin" });


	/*Simple game loop lock at 30 frame per seconds*/
	using clock = std::chrono::high_resolution_clock;
	auto time_start = clock::now();
	bool quit_game = false;

	
	float acc_deltatime = 0.0f;
	float acc_check_reference = 0.0f;
	const float lock_30fps_deltatime = 0.034f;
	const float check_reference_ThesholdTime = 2.0f;

	std::chrono::duration<float> delta_time;
	while (!quit_game) {
		delta_time = clock::now() - time_start;
		time_start = clock::now();

		acc_deltatime += delta_time.count();
		if (acc_deltatime > lock_30fps_deltatime)
		{
			//Process all logic of all processors.
			EntityActionMgr.Execute(acc_deltatime);
			acc_deltatime = 0.0f;
		}

		/*
			Do check reference and delete object.
			in some interval of time.
		*/
		acc_check_reference += delta_time.count();
		if (acc_check_reference > check_reference_ThesholdTime)
		{
			EntityActionMgr.CheckAndDeleteUnuseObjects();
			acc_check_reference = 0.0f;
		}

	}
}