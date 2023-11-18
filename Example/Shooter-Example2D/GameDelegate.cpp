#include "GameDelegate.h"
#include "Player.h"
#include "BulletFactory.h"
#include "BulletPool.h"
#include "Test.h"

noa::TileMapCamera* camera = nullptr;

Player* player = nullptr;
void GameDelegate::OnLoad(noa::Scene* scene)
{
	noa::TileMap* map = noa::TileMap::Create(scene)
		->LoadTileSet("tileSet.tsd")
		.LoadTileLayer({"map_Layer1.csv","map_Layer2.csv"})
		.SetCollisionTileID(40)
		.Apply();

	Test* test1 = noa::NObject<Test>::Create(scene);
	test1->transform.position = { 2,3 };

	//player = noa::NObject<Player>::Create(scene);
	
	camera = noa::TileMapCamera::Create(scene)
		->SetTileScale(32,32)
		.SetFollow(test1)
		.Apply();
	//player->camera = camera;

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

	if (noa::Input::GetMouseKeyUp(noa::MouseButton::LEFT_BUTTON))
	{
		hold = nullptr;
		currentSelect = nullptr;
	}

	if (hold) 
	{
		noa::Debug::Log(
			"动量:" + noa::ToString<float>(hold->rigid->GetMomentum())
			+",速度:" + noa::ToString<float>(hold->rigid->velocity)
			+",约束:" + noa::ToString<bool>(hold->rigid->GetConstraint())
		);
	}

	//选择鼠标点击到的Test
	noa::Vector<double> pos = noa::Input::GetMousePosition();
	//通过鼠标位置获取Test

	if (noa::Input::GetMouseKeyDown(noa::MouseButton::LEFT_BUTTON))
	{
		currentSelect = camera->GetRayHitInfoAs<Test>(
			static_cast<int>(pos.x)
			, static_cast<int>(pos.y)
		);
	}
	
	if (currentSelect!=nullptr&&hold == nullptr) 
	{
		hold = currentSelect;
	}

	camera->SetFollow(hold);

	if (hold != nullptr) 
	{
		if (noa::Input::GetMouseKeyHold(noa::MouseButton::LEFT_BUTTON))
		{
			//鼠标左键按住
			noa::Vector<float> world = camera->ScreenPointToWorld(
				static_cast<float>(pos.x)
				, static_cast<float>(pos.y)
			);

			noa::Vector<float> velocity = world - hold->transform.position;;

			hold->rigid->velocity = velocity*10.0f;
		}
	}

	noa::renderer->DrawString(
		(L"FPS:"+std::to_wstring(1.0f/noa::Time::deltaTime))
		,10
		,10
		,noa::RED,50
	);

}
