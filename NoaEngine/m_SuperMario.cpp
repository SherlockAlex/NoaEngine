#define M_SUPERMARIO
#ifdef M_SUPERMARIO
#include "./core/NoaEngine.h"

using namespace noa;

TileMap* currentMap = nullptr;
Vector<int> tileScale = Vector<int>(70, 70);

AnimationFrame idleFrame = AnimationFrame("./Assets/JumpMan/Animator/mario_idle.amt");
AnimationFrame runFrame = AnimationFrame("./Assets/JumpMan/Animator/mario_run.amt");
AnimationFrame jumpFrame = AnimationFrame("./Assets/JumpMan/Animator/mario_jump.amt");

class PlayerIdleState :public State 
{
private:
	PlayerIdleState(StateMachine* stateMachine) :State(stateMachine)
	{

	}

	~PlayerIdleState() override 
	{

	}

public:
	static PlayerIdleState* Create(StateMachine* stateMachine) {
		return new PlayerIdleState(stateMachine);
	}

	void Delete() override 
	{
		delete this;
	}

};



class Player:public GameObject
{
private:
	
	Player(Scene * scene) :GameObject(scene,new Sprite(resource.LoadSprFile("./Assets/JumpMan/JumpMan.spr"), tileScale))
	{
		transform.position = Vector<float>(0.0, 0.0);
		rigid->damping = 0;

		idle->SetFrame(&idleFrame);

		run->SetFrame(&runFrame);

		jump->SetFrame(&jumpFrame);

		currentAnimatorState = idle;

		fsm->AddState(idleState);

	}
	~Player()
	{
		GameObject::~GameObject();
	}

public:

	static Player* Create(Scene* scene) 
	{
		return new Player(scene);
	}

	void Delete() override {
		delete this;
	}

	void InitPosition(TileMap& tileMap,const int targetTileID) {
		for (int i = 0; i < tileMap.w; i++)
		{
			for (int j = 0; j < tileMap.h; j++)
			{

				if (tileMap.GetTileID(i, j) == targetTileID)
				{
					transform.position.x = i;
					transform.position.y = j;
					rigid->velocity.y = 0;
				}

			}
		}
	}

	void ActorControl() {
		rigid->velocity.x = 0;
		//velocity.y = 0;

		if (inputSystem.GetKeyHold(KeyA))
		{
			isLeft = true;
			rigid->velocity.x = -speed;
		}
		if (inputSystem.GetKeyHold(KeyD))
		{
			isLeft = false;
			rigid->velocity.x = speed;
		}
		if (inputSystem.GetKeyHold(KeyW))
		{
			rigid->velocity.y = -speed;

		}
		if (inputSystem.GetKeyHold(KeyS))
		{
			rigid->velocity.y = speed;
		}

		//Debug(to_string(collision.isGrounded));
		if ((inputSystem.GetKeyHold(KeySpace)||inputSystem.GetKeyHold(KeyK)) && rigid->collision.isGrounded)
		{
			rigid->AddForce(jumpForce, rigid->Impulse);
			jumpSFX.Play(false);
		}
	}

	void AnimatorControl() {
		if (NoaAbs<float>(rigid->velocity.y * deltaTime)<0.001) {
			currentAnimatorState = idle;
		}
		else {
			currentAnimatorState = jump;
		}

		if (inputSystem.GetKeyHold(KeyA)|| inputSystem.GetKeyHold(KeyD))
		{
			//isLeft = true;

			if (NoaAbs<float>(rigid->velocity.y * deltaTime) < 0.001)
			{
				currentAnimatorState = run;
			}
			else {
				currentAnimatorState = jump;
			}

		}

		currentAnimatorState->Play();

		const SpriteFile nextFrame = move(currentAnimatorState->GetCurrentFrameImage());
		sprite->UpdateImage(nextFrame);

	}

	void Start() override 
	{
		rigid->gravityWeight = 3.5;
	}

	Vector<float> jumpForce = Vector<float>(0.0, -15);
	void Update() override 
	{

		ActorControl();
		AnimatorControl();

		if (currentMap->GetTileID(transform.position.x+0.5,transform.position.y+0.5)==0)
		{
			currentMap->level[int(transform.position.y+0.5) * currentMap->w + int(transform.position.x+0.5)] = 5;
			coinSFX.Play(false);
		}

	}

public:
	Rigidbody* rigid = Rigidbody::Create(this);

	float speed = 8;
	bool isLeft = false;

	const Audio coinSFX = Audio("./Assets/JumpMan/Music/coin.mp3", Chunk);
	const Audio jumpSFX = Audio("./Assets/JumpMan/Music/jump.mp3", Chunk);

	Animation* currentAnimatorState = nullptr;
	Animation* idle = Animation::Create(this,5,false);
	Animation* run = Animation::Create(this,10,false);
	Animation* jump = Animation::Create(this,10,false);

	StateMachine* fsm = StateMachine::Create(this);

	PlayerIdleState* idleState = PlayerIdleState::Create(fsm);

};

class TestScene1 :public Scene {
private:
	TestScene1() :Scene("Test")
	{

	}

	~TestScene1() {
		
	}

public:
	static TestScene1* Create() {
		return new TestScene1();
	}

	void Delete() override {
		delete this;
	}

	void Awake() override {
		player = Player::Create(this);
		camera = new TileMapCamera();
		camera->SetFollow(&player->transform);
		camera->SetTileScale(tileScale);


		player->rigid->SetTileMap(&tileMap);
		tileMap.SetCollisionTileID({ 1,2 });
		player->InitPosition(tileMap, 87);

		BGM.Play(true);

		currentMap = &tileMap;
	}

	void Update() override {
		Vector<int> playerDrawPos = camera->Render(tileMap, frontDelta, endDelta);

		//Draw player
		player->sprite->DrawSprite(playerDrawPos.x, playerDrawPos.y, true, !player->isLeft);
	}

	void Unload() override{
		delete camera;
	}

private:

	TileMap tileMap = TileMap(
		resource.LoadTileFromTsd("./Assets/JumpMan/Tile/tileSet.tsd"),
		resource.LoadMapFromCSV("./Assets/JumpMan/Map/level1.csv")
	);

	Player* player = nullptr;

	TileMapCamera* camera = nullptr;

	Vector<float> frontDelta = Vector<float>(0.0, 0.0);
	Vector<float> endDelta = Vector<float>(-1, -1);


	Audio BGM = Audio("./Assets/JumpMan/Music/BGM.ogg", Music);
	Audio gameOverMusic = Audio("./Assets/JumpMan/Music/gameover.mp3", Chunk);
};

class MainMenu :public Scene {
private:
	MainMenu() :Scene("MainMenu") {

	}

	~MainMenu() 
	{
		
	}

public:
	static MainMenu* Create() {
		return new MainMenu();
	}

	void Delete() override {
		delete this;
	}

	void Awake() override {

		backgroundSprite = new Sprite(resource.LoadSprFile("./Assets/JumpMan/Texture/mainMenu.spr"), {pixelWidth,pixelHeight});

		canvas = NoaCanvase::Create(this);
		backGround = NoaImage::Create();
		startButton = NoaButton::Create();

		backGround->color = WHITE;
		backGround->sprite = backgroundSprite;
		backGround->scale = { pixelWidth,pixelHeight };
		
		startButton->text = "START";
		startButton->scale = { (int)(pixelWidth * 0.25),(int)(pixelHeight * 0.25) };

		startButton->transform.position = { (int)(pixelWidth * 0.5 - 0.5 * 0.25 * pixelWidth),(int)(pixelHeight * 0.5) };

		startButton->AddClickEvent([this]() 
			{
				sceneManager.LoadScene("Test");
			});

		canvas->AddComponent(backGround);
		canvas->AddComponent(startButton);
	}

	void Update() override {

	}

	void Unload() override {
		delete backgroundSprite;
	}

private:
	NoaCanvase* canvas = nullptr;

	NoaImage* backGround = nullptr;

	NoaButton* startButton = nullptr;

	Sprite* backgroundSprite = nullptr;

};

class Platformer :public NoaGameEngine {
public:
	Platformer(int width, int height, GameWindowMode windowMode, string gameName) :NoaGameEngine(width, height, windowMode, gameName) 
	{
		sceneManager.LoadScene("MainMenu");
	}

	void Start() override
	{

	}

	void Update() override 
	{
		if (inputSystem.GetKeyDown(KeyESC)) 
		{
			sceneManager.LoadScene("MainMenu");
		}
	}

private:
	MainMenu* menu = MainMenu::Create();
	TestScene1 * scene = TestScene1::Create();

};

int main(int argc,char * argv[])
{
	Platformer game(1920/1.5, 1080/1.5, NoaGameEngine::WindowMode, "SuperMario");
	game.Run();
	return 0;
}
#endif