#include "GameDelegate.h"
#include "Player.h"
#include "BulletFactory.h"
#include "BulletPool.h"
#include "Test.h"

noa::TileMapCamera* camera = nullptr;

Player* player = nullptr;

void InitGameMenu(noa::Scene* scene) {

	noa::Scene* ui = scene->CreateChild("game_menu");

	noa::UIDocument* document = 
		noa::UIDocumentActor::Create(ui)
		->SetActorTag("menu_document")
		.Apply();

	noa::UIContainer* container = noa::UIContainer::Create(document)
		->SetID("gui_container")
		.SetPosition(noa::Screen::width/2,noa::Screen::height/2)
		.SetVisiable(false)
		.Apply();

	noa::Button* backMainMenu = noa::Button::Create(container)
		->SetID("back_main_menu")
		.SetText(L"�������˵�")
		.SetFontSize(20)
		.SetPosition(0, 0)
		.SetSize(240, 60)
		.SetAnchor(0.5f, 0.5f)
		.SetRadius(50)
		.SetTextOffset(0.0f, 0.0f)
		.SetNormalColor(noa::RGBA(20, 20, 20, 250))
		.SetHeightLightColor(noa::RGBA(255, 0, 0, 255))
		.SetTextNormalColor(noa::WHITE)
		.SetTextHeightLightColor(noa::WHITE)
		.SetTextClickColor(noa::BLACK)
		.SetNormalScale(1.0f)
		.SetHeightLightScale(1.1f)
		.SetClickScale(0.9f)
		.AddClickCallback([]() {noa::sceneManager.LoadScene("MainMenu"); })
		.Apply();

}

void InitGameUI(noa::Scene* scene) 
{
	noa::UIDocument* document = 
		noa::UIDocumentActor::Create(scene)
		->SetActorTag("ui_document")
		.Apply();

	noa::UIContainer* container =
		noa::UIContainer::Create(document)
		->SetPosition(0.0f,0.0f)
		.Apply();

	noa::ProcessBar* hpBar =
		noa::ProcessBar::Create(container)
		->SetAnchor(0.0f,0.0f)
		.SetID("hp_bar")
		.SetFillColor(noa::RGBA(255,0,0,255))
		.SetSize(320,10)
		.SetPosition(10,10)
		.SetInteractable(false)
		.SetRadius(10)
		.Apply();

	document->Display(container);

}

bool flag = true;
void GameDelegate::OnLoad(noa::Scene* scene)
{
	//flag = true;

	noa::TileMap* map = noa::TileMap::Create(scene)
		->LoadTileSet("tileSet.tsd")
		.LoadTileLayer({"map_Layer1.csv","map_Layer2.csv"})
		.SetCollisionTileID(40)
		.Apply();

	Test* test1 = noa::NObject<Test>::Create(scene);
	test1->tileCollider->SetTileMap(map);
	test1->transform.position = { 2,3 };

	player = noa::NObject<Player>::Create(scene);
	player->tileCollider->SetTileMap(map);

	camera = noa::TileMapCamera::Create(scene)
		->SetTileScale(32,32)
		.SetFollow(player)
		.SetTileMap(map)
		.Apply();
	player->camera = camera;

	bulletFactory->SetActiveScene(scene);
	bulletPool = std::make_shared<noa::ActorPool<Bullet>>();
	bulletPool->SetFactory(bulletFactory.get());
	bulletPool->Prewarm(10);

	InitGameMenu(scene);
	InitGameUI(scene);

}


Test* currentSelect = nullptr;
Test* hold = nullptr;

int i = 0;

void GameDelegate::OnTick(noa::Scene* scene)
{
	if (noa::Input::GetKeyDown(noa::KeyCode::KEY_ESC)) 
	{
		noa::UIDocument* document = 
			noa::sceneManager.FindActorWithTag("menu_document")
			->GetActorAs<noa::UIDocument>();

		if (!document) 
		{
			noa::Debug::Log("can find document");
			return;
		}
		if (flag) 
		{
			noa::Debug::Log("this gui open");
			scene->ActiveSceneChild("game_menu");
			document->Display("gui_container");
			flag = false;
		}
		else 
		{
			noa::Debug::Log("this gui close");
			scene->CloseSceneChild();
			document->Close();
			flag = true;
		}


	}

	noa::UIDocument* ui_document =
		noa::sceneManager.FindActorWithTag("ui_document")
		->GetActorAs<noa::UIDocument>();

	if (ui_document) 
	{
		noa::ProcessBar* hpBar =
			ui_document->GetElementByID<noa::ProcessBar>("hp_bar");

		if (hpBar)
		{
			hpBar->SetAmount(static_cast<float>(player->hp)/player->maxHp);
		}

	}

	//if (i<500) 
	//{
	//	Test* test = noa::NObject<Test>::Create(scene);
	//	test->transform.position = { 2,3 };
	//	test->rigid->velocity = { 70,0 };
	//	i++;
	//}
	//

	//if (camera == nullptr)
	//{
	//	return;
	//}

	//if (noa::Input::GetMouseKeyUp(noa::MouseButton::LEFT_BUTTON))
	//{
	//	hold = nullptr;
	//	currentSelect = nullptr;
	//}

	//if (hold) 
	//{
	//	noa::Debug::Log(
	//		"����:" + noa::ToString<float>(hold->rigid->GetMomentum())
	//		+",�ٶ�:" + noa::ToString<float>(hold->rigid->velocity)
	//		+",Լ��:" + noa::ToString<bool>(hold->rigid->GetConstraint())
	//	);
	//}

	////ѡ�����������Test
	//noa::Vector<double> pos = noa::Input::GetMousePosition();
	////ͨ�����λ�û�ȡTest

	//if (noa::Input::GetMouseKeyDown(noa::MouseButton::LEFT_BUTTON))
	//{
	//	currentSelect = camera->GetRayHitInfoAs<Test>(
	//		static_cast<int>(pos.x)
	//		, static_cast<int>(pos.y)
	//	);
	//}
	//
	//if (currentSelect!=nullptr&&hold == nullptr) 
	//{
	//	hold = currentSelect;
	//}

	//camera->SetFollow(hold);

	//if (hold != nullptr) 
	//{
	//	if (noa::Input::GetMouseKeyHold(noa::MouseButton::LEFT_BUTTON))
	//	{
	//		//��������ס
	//		noa::Vector<float> world = camera->ScreenPointToWorld(
	//			static_cast<float>(pos.x)
	//			, static_cast<float>(pos.y)
	//		);

	//		noa::Vector<float> velocity = world - hold->transform.position;;

	//		hold->rigid->velocity = velocity*10.0f;
	//	}
	//}

	//noa::renderer->DrawString(
	//	(L"FPS:"+std::to_wstring(1.0f/noa::Time::deltaTime))
	//	,10
	//	,10
	//	,noa::RED,50
	//);

}
