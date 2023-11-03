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

	noa::TileMap* map = noa::TileMap::Create("tileSet.tsd", { "map.csv" }, scene);
	map->SetCollisionTileID({ 40 });
	camera = noa::TileMapCamera::Create(scene);
	//player = noa::NObject<Player>::Create(scene);

	Test* test1 = noa::NObject<Test>::Create(scene);

	test1->transform.position = { 2,3 };


	camera->SetFollow(test1);
	camera->SetTileScale({32,32});

	//player->camera = camera;

	bulletPool->SetFactory(bulletFactory.get());
	bulletPool->Prewarm(10);

}


Test* currentSelect = nullptr;
Test* hold = nullptr;

int i = 0;
void GameDelegate::OnUpdate(noa::Scene* scene)
{
	if (i<2) 
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
		//noa::Debug::Log("�ٶ�:" + noa::ToString<float>(hold->rigid->velocity));
		//noa::Debug::Log("����:" + noa::ToString<float>(hold->rigid->GetSumForce()));
		noa::Debug::Log("����:" + noa::ToString<float>(hold->rigid->GetMomentum()));
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

	//noa::Actor * follow = (hold == nullptr ? player->GetActor() : hold->GetActor());

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
