#include "NoaEngine.h"
#include "MainMenuUI.h"
#include "Box.h"
#include "Circle.h"


void MainMenuDelegateFunc(noa::Scene* scene) {
	noa::TileMapInfo mapInfo;
	mapInfo.LoadTileSet("tileSet.tsd")
		.LoadTileLayer({ "map.csv" });
	
	noa::TileMap* map = noa::TileMap::Create(scene);
	noa::MapLayer layer = mapInfo.GetLayer(0);
	noa::TileMapRenderer* layer1 = 
		noa::TileMapRenderer::Create(map)
		->SetTileMap(layer,mapInfo.GetTileSet())
		.SetCollision(52)
		.Apply();

	//生成若干个box预制体
	Box* box = noa::NObject<Box>::Create(scene);
	box->transform.position.x = 5;
	box->transform.position.y = 5;

	noa::Camera2D* camera = noa::Camera2D::Create(scene)
		->SetFar(32)
		.SetFollow(box)
		.ApplyHorizonalBoundary(true)
		.ApplyVerticalBoundary(true)
		.SetHorizonalBoundary(0,mapInfo.GetSize().x)
		.SetVerticalBoundary(0, mapInfo.GetSize().y)
		.Apply();

	box->camera = camera;
	box->tileCollider2D->SetTileMap(map);
	
}

void OnTick(noa::Scene* scene)
{
	

	if (noa::Input::GetKeyDownOnce(noa::KeyCode::KEY_K))
	{
		noa::Vector<double> mousePos = noa::Input::GetMousePosition();
		noa::Vector<float> worldPos = noa::SceneManager::FindActorWithType<noa::Camera2D>()->ScreenPointToWorld(mousePos.x, mousePos.y);
		Circle* box = noa::NObject<Circle>::Create(scene);
		box->transform.position.x = worldPos.x + (rand() % 10)*0.01f;
		box->transform.position.y = worldPos.y + (rand() % 10) * 0.01f;
		//box->camera = noa::SceneManager::FindActorWithType<noa::Camera2D>();
		box->tileCollider2D->SetTileMap(noa::SceneManager::FindActorWithType<noa::TileMap>());
		
	}

	if (noa::Input::GetKeyDownOnce(noa::KeyCode::KEY_L))
	{
		noa::Vector<double> mousePos = noa::Input::GetMousePosition();
		noa::Vector<float> worldPos = noa::SceneManager::FindActorWithType<noa::Camera2D>()->ScreenPointToWorld(mousePos.x, mousePos.y);
		Box* box = noa::NObject<Box>::Create(scene);
		box->transform.position.x = worldPos.x + (rand() % 10) * 0.01f;
		box->transform.position.y = worldPos.y + (rand() % 10) * 0.01f;
		box->camera = noa::SceneManager::FindActorWithType<noa::Camera2D>();
		box->tileCollider2D->SetTileMap(noa::SceneManager::FindActorWithType<noa::TileMap>());
	}
}

class Engine :public noa::NoaEngine {
public:
	Engine() :noa::NoaEngine(
		1920/2,1080/2,noa::WindowMode::WINDOW,noa::GraphicsAPI::OPENGL,"Shooter"
	) 
	{
		noa::Scene* menuScene = noa::Scene::Create("MainMenu");
		menuScene->onLoad += MainMenuDelegateFunc;
		menuScene->onTick += OnTick;

		noa::SceneManager::LoadScene("MainMenu");
	}

	~Engine() {

	}

	void Update() override
	{
		noa::renderer->DrawString(std::to_wstring
		(1.0f / noa::Time::deltaTime),10,10,noa::RED,50);
		//noa::Debug::Log(std::to_string(1.0f/noa::Time::deltaTime));
	}


};

int main() {
	Engine game;
	game.Run();
	return 0;
}