#include "GameDelegate.h"
#include "Player.h"
#include "BulletFactory.h"
#include "BulletPool.h"
#include "Test.h"

noa::Camera2D* camera = nullptr;

Player* player = nullptr;

void InitGameMenu(noa::Scene* scene) {

	noa::Scene* ui = scene->CreateChild("game_menu");

	noa::UIDocument* document = 
		noa::UIDocumentActor::Create(ui)
		->SetID("menu_document")
		.Apply();

	noa::UIContainer* container = noa::UIContainer::Create(document)
		->SetID("gui_container")
		.SetPosition(noa::Screen::width/2,noa::Screen::height/2)
		.SetVisiable(false)
		.Apply();

	noa::Button* backMainMenu = noa::Button::Create(container)
		->SetID("back_main_menu")
		.SetText(L"返回主菜单")
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
		->SetID("ui_document")
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

	noa::PhysicsSystem::SetGrid(100,100);

	//tilemap 的内存释放还没有写，等下记得补充
	noa::TileMapInfo mapInfo = noa::TileMapInfo()
		.LoadTileSet("tileSet.tsd")
		.LoadTileLayer(
			{"map_Layer1.csv","map_Layer2.csv"}
		);

	noa::TileMap* tileMap = noa::TileMap::Create(scene);
	noa::TileMapRenderer* tileMapRenderer 
		= noa::TileMapRenderer::Create(tileMap)
		->SetTileMap(mapInfo.layers[0], mapInfo.GetTileSet())
		.SetCollision(40)
		.Apply();
	noa::TileMapRenderer* tileMapRenderer1
		= noa::TileMapRenderer::Create(tileMap)
		->SetTileMap(mapInfo.layers[1], mapInfo.GetTileSet())
		.SetOffset(10,8)
		.SetCollision(84)
		.Apply();

	Test* test1 = noa::NObject<Test>::Create(scene);
	test1->tileCollider->SetTileMap(tileMap);
	test1->transform.position = { 2,3 };

	player = noa::NObject<Player>::Create(scene);
	player->tileCollider->SetTileMap(tileMap);

	camera = noa::Camera2D::Create(scene)
		->SetFar(32.0f)
		.SetAnchor(0.5f,0.5f)
		.Apply();
	player->camera = camera;

	bulletFactory->SetActiveScene(scene);
	bulletPool = std::make_shared<noa::ActorPool<Bullet>>();
	bulletPool->SetFactory(bulletFactory.get());
	bulletPool->Prewarm(10);

	InitGameMenu(scene);
	InitGameUI(scene);

	//map->Delete();

}


Test* currentSelect = nullptr;
Test* hold = nullptr;

int i = 0;

void GameDelegate::OnTick(noa::Scene* scene)
{

	camera->transform.position = player->transform.position;

	if (noa::Input::GetKeyDown(noa::KeyCode::KEY_ESC)) 
	{
		noa::UIDocument* document = 
			noa::UIHub::GetDocumentByID("menu_document");

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
		noa::UIHub::GetDocumentByID("ui_document");

	if (ui_document) 
	{
		noa::ProcessBar* hpBar =
			ui_document->GetElementByID<noa::ProcessBar>("hp_bar");

		if (hpBar)
		{
			hpBar->SetAmount(
				static_cast<float>(player->hp)/player->maxHp);
		}

	}

	if (i<500) 
	{
		noa::TileMap* map = noa::sceneManager.FindActorWithType<noa::TileMap>();
		Test* test = noa::NObject<Test>::Create(scene);
		test->transform.position = { 2,3 };
		test->rigid->velocity = { 70,0 };
		test->tileCollider->SetTileMap(map);
		i++;
	}
	//

	//if (camera == nullptr)
	//{
	//	return;
	//}

	if (noa::Input::GetMouseKeyUp(noa::MouseButton::LEFT_BUTTON))
	{
		hold = nullptr;
		currentSelect = nullptr;
	}

	//if (hold) 
	//{
	//	noa::Debug::Log(
	//		"动量:" + noa::ToString<float>(hold->rigid->GetMomentum())
	//		+",速度:" + noa::ToString<float>(hold->rigid->velocity)
	//		+",约束:" + noa::ToString<bool>(hold->rigid->GetConstraint())
	//	);
	//}

	//选择鼠标点击到的Test
	noa::Vector<double> pos = noa::Input::GetMousePosition();
	//通过鼠标位置获取Test

	if (noa::Input::GetMouseKeyDown(noa::MouseButton::LEFT_BUTTON))
	{
		//currentSelect = camera->GetPointActor<Test>(pos.x, pos.y);
	}
	
	if (currentSelect!=nullptr&&hold == nullptr) 
	{
		hold = currentSelect;
	}

	//camera->SetFollow(hold);

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
		(L"FPS:"+std::to_wstring(noa::Time::fps))
		,10
		,10
		,noa::RED,50
	);

}
