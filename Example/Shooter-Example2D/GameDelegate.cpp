#include "GameDelegate.h"
#include "Player.h"
#include "BulletFactory.h"
#include "BulletPool.h"
#include "Test.h"

/**
* ����TileMap����ͽṹ��̫����
*/

noa::TileMapCamera* camera = nullptr;

Player* player = nullptr;
void GameDelegate::OnLoad(noa::Scene* scene)
{
	noa::TileMap* map = noa::TileMap::Create(scene,"tileSet.tsd", { "map.csv" });
	map->SetCollisionTileID(40);
	camera = noa::TileMapCamera::Create(scene);

	Test* test1 = noa::NObject<Test>::Create(scene);

	test1->transform.position = { 2,3 };


	camera->SetFollow(test1);
	camera->SetTileScale({32,32});

	bulletPool->SetFactory(bulletFactory.get());
	bulletPool->Prewarm(10);

}


Test* currentSelect = nullptr;
Test* hold = nullptr;

int i = 0;
void GameDelegate::OnUpdate(noa::Scene* scene)
{
	if (i<100) 
	{
		Test* test = noa::NObject<Test>::Create(scene);
		test->transform.position = { 2,3 };
		test->rigid->velocity = { 70,0 };
		i++;
	}
	

	if (camera == nullptr)
	{
		return;
	}

	if (noa::inputSystem.GetMouseKeyUp(noa::MouseButton::RIGHT_BUTTON))
	{
		hold = nullptr;
		currentSelect = nullptr;
	}

	if (hold) 
	{
		//noa::Debug::Log("����:" + noa::ToString<float>(hold->rigid->GetMomentum()));
		noa::Debug::Log(
			"����:" + noa::ToString<float>(hold->rigid->GetMomentum())
			+",�ٶ�:" + noa::ToString<float>(hold->rigid->velocity)
			+",Լ��:" + noa::ToString<bool>(hold->rigid->GetConstraint())
		);
	}

	//ѡ�����������Test
	noa::Vector<double> pos = noa::inputSystem.GetMousePosition();
	//ͨ�����λ�û�ȡTest

	if (noa::inputSystem.GetMouseKeyDown(noa::MouseButton::LEFT_BUTTON)) 
	{
		currentSelect = camera->GetRayHitInfoAs<Test>(pos.x, pos.y);
	}
	
	if (currentSelect!=nullptr&&hold == nullptr) 
	{
		hold = currentSelect;
	}

	camera->SetFollow(hold);

	if (hold != nullptr) 
	{
		if (noa::inputSystem.GetMouseKeyHold(noa::MouseButton::LEFT_BUTTON))
		{
			//��������ס
			noa::Vector<float> world = camera->ScreenPointToWorld(pos.x, pos.y);

			noa::Vector<float> velocity = world - hold->transform.position;;

			hold->rigid->velocity = velocity*10.0f;
		}
	}

	
	//noa::Debug::Log(std::to_string(1.0f/noa::Time::deltaTime));

}
