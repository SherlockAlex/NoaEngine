#include "NoaEngine.h"

float deltaTime = 0.0f;

int surfaceWidth = 0;
int surfaceHeight = 0;

SDL_Event ioEvent;
GameWindow game;

vector<Animator *> animatorList;

void DrawPixel(int x, int y, Uint32 color)
{
	if (x<0||x>=surfaceWidth||y<0||y>=surfaceHeight)
	{
		return;
	}
	Uint32* pixel = (Uint32*)game.pixelBuffer;
	pixel[y * surfaceWidth + x] = color;
}

void DrawLine(nVector vetex1,nVector vetex2,Uint32 color)
{
	//����Ļ�ϻ���
}

void DrawImage(
	int posX,
	int posY,
	int inmageW,
	int imageH,
	int scaleForSurface,
	bool isDrawAlpha,
	Uint32* imageRGB
)
{
	
	//����Ŵ�
	int wannaW = surfaceWidth/ scaleForSurface;
	int wannaH = (int)(((float)imageH / (float)inmageW) * wannaW);

	for (int width = 0; width < wannaW; width++)
	{
		for (int height = 0; height < wannaH; height++)
		{
			int x = posX - ((float)wannaW * 0.5f) + width;
			int y = posY - ((float)wannaH * 0.5f) + height;

			float fSimpleX = (float)(width) / (float)wannaW;
			float fSimpleY = (float)(height) / (float)wannaH;

			Uint32 pixelColor = GetSpriteColor(fSimpleY, fSimpleX, imageH, inmageW, imageRGB);

			if (isDrawAlpha) 
			{
				if (pixelColor == BLACK)
				{
					continue;
				}
			}
			
			if (x<0||x>=surfaceWidth||y<0||y>=surfaceHeight)
			{
				continue;
			}
			DrawPixel(x, y, pixelColor);
		}
	}
}

void Game(int width, int height)
{
	int init = SDL_Init(SDL_INIT_EVERYTHING);
	if (init != 0)
	{
		printf("[error]:Game init failed\n");
		return;
	}

	game.window = SDL_CreateWindow(
		"DEMO",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		gameWindowMode
		//SDL_WINDOW_FULLSCREEN
		//SDL_WINDOW_FOREIGN
	);
	if (game.window == nullptr)
	{
		printf("[error]:Create window faild\n");
		return;
	}

	game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED);
	if (game.renderer == nullptr)
	{
		return;
	}

	game.texture = SDL_CreateTexture(game.renderer,
		SDL_PIXELFORMAT_BGR888,
		SDL_TEXTUREACCESS_STREAMING, 
		//surface->w,
		//surface->h
		width,
		height
	);
	if (game.texture==nullptr) {
		return;
	}

	int pitch;
	game.format = SDL_AllocFormat(SDL_PIXELFORMAT_BGR888);
	SDL_LockTexture(game.texture, NULL, &game.pixelBuffer, &pitch);

	//surfaceWidth = width;
	//surfaceHeight = height;

	surfaceWidth = width;
	surfaceHeight = height;

	//������Ƶ�豸��ʼ��
	if (Mix_OpenAudio(
		MIX_DEFAULT_FREQUENCY,
		MIX_DEFAULT_FORMAT,
		MIX_CHANNELS,
		4096
		)==-1)
	{
		printf("[error]:��ʼ����Ƶ�豸ʧ��\n");
		return;
	}

	auto tp1 = chrono::system_clock::now();
	chrono::duration<float> elapsedTime;
	auto tp2 = chrono::system_clock::now();

	Start();

	while (1)
	{
		tp2 = chrono::system_clock::now();
		elapsedTime = tp2 - tp1;
		tp1 = tp2;
		deltaTime = elapsedTime.count();
		
		SDL_PollEvent(&ioEvent);

		Update();

		for (int i = 0;i<animatorList.size();i++) 
		{
			animatorList[i]->Update();
		}


		SDL_UnlockTexture(game.texture);
		SDL_RenderCopy(game.renderer, game.texture, NULL, NULL);
		SDL_RenderPresent(game.renderer);

		if (ioEvent.type==SDL_QUIT)
		{
			break;
		}

	}

	free(game.pixelBuffer);
	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyTexture(game.texture);
	SDL_DestroyWindow(game.window);
	SDL_Quit();
}


bool GetKeyHold(char key) {
	//���������Ƿ�ס

#ifdef _WIN64
	if (GetAsyncKeyState((unsigned short)key) & 0x8000) {
		return true;
	}
	
#else 
#ifdef __linux__
	
	const Uint8* keyState = SDL_GetKeyboardState(nullptr);

	if (keyState[SDL_GetScancodeFromKey(key+32)])
	{
		SDL_PollEvent(&ioEvent);
		printf("key %c is hold on\n",key);
		return true;
	}
#endif // LINUX

#endif
	return false;
}

bool GetKeyDown(char key)
{
	//��ⰴ������˲��
#ifdef _WIN64
	if (GetAsyncKeyState((unsigned short)key) & 0x0001) {
		return true;
	}
#else
#ifdef __linux__
	const Uint8* keyState = SDL_GetKeyboardState(nullptr);

	if (keyState[SDL_GetScancodeFromKey(key + 32)])
	{
		printf("key %c is pressed\n", key);
		SDL_PollEvent(&ioEvent);
		return true;
	}
#endif // _LINUX

#endif // _WIN64
	return false;

}

Uint32 GetSpriteColor(float normalizedX, float normalizedY, int imageW, int imageH, Uint32* sprite)
{
	//��ͼƬ�л�ȡ����
	int sx = normalizedX * (float)imageW;
	int sy = normalizedY * (float)imageH - 1;
	//printf("%d,%d\n", sx, sy);
	if (sx<0 || sx>imageW || sy<0 || sy>imageH)
	{
		sx = sx - (int)sx;
		sy = sy - (int)sy;
		return BLACK;
	}

	return sprite[sy * imageW + sx];
}

Uint32* LoadTexture(const char* filename) {
	ifstream infile(filename, ios::binary);
	if (!infile) {
		cout << "�޷����ļ���" << endl;
		return nullptr;
	}

	// ��ȡdata�Ĵ�С
	uint32_t size = 0;
	infile.read(reinterpret_cast<char*>(&size), sizeof(size));

	// ���䶯̬�ڴ�ռ�
	uint32_t* data = new uint32_t[size];

	// ��ȡvector����
	infile.read(reinterpret_cast<char*>(data), sizeof(uint32_t) * size);

	infile.close();

	return data;

}

//����Ͷ���㷨
Ray RayCastHit(
	int pixelX,			//���ص������
	Player& player,		//��Ҷ�������
	LevelMap& map		//��ǰ�ؿ���ͼ����
)
{
	//����Ͷ���㷨
	Ray ray;
	ray.distance = 0.0f;
	ray.angle = player.angle -
		player.FOV * (0.5f - (float)(pixelX) / (float)(surfaceWidth));
	const float rayForwordStep = 0.03f;
	const float eyeX = sinf(ray.angle);
	const float eyeY = cosf(ray.angle);

	while (!ray.isHitDoor && !ray.isHitWall && ray.distance < player.viewDepth)
	{
		ray.distance += rayForwordStep;

		const float floatHitPointX = player.position.x + ray.distance * eyeX;
		const float floatHitPointY = player.position.y + ray.distance * eyeY;

		const int intHitPointX = (int)floatHitPointX;
		const int intHitPointY = (int)floatHitPointY;

		if (intHitPointX < 0 || intHitPointX >= map.w || intHitPointY < 0 || intHitPointY >= map.h)
		{
			ray.isHitWall = true;
			ray.distance = player.viewDepth;
			continue;
		}

		const char hitChar = map.level[intHitPointY * map.w + intHitPointX];
		if (hitChar == '#' || (hitChar == '*'))
		{

			if (hitChar == '#')
			{
				ray.isHitWall = true;
			}
			else if (hitChar == '*')
			{
				ray.isHitDoor = true;
			}

			const float fBlockMidX = (float)intHitPointX + 0.5f;
			const float fBlockMidY = (float)intHitPointY + 0.5f;
			const float fTestAngle = atan2f((floatHitPointY - fBlockMidY), (floatHitPointX - fBlockMidX));

			if (fTestAngle >= -PI * 0.25f && fTestAngle < PI * 0.25f)
			{
				ray.simple.x = floatHitPointY - (float)intHitPointY;

			}
			if (fTestAngle >= PI * 0.25f && fTestAngle < PI * 0.75f)
			{
				ray.simple.x = floatHitPointX - (float)intHitPointX;
			}
			if (fTestAngle < -PI * 0.25f && fTestAngle >= -PI * 0.75f)
			{
				ray.simple.x = floatHitPointX - (float)intHitPointX;
			}
			if (fTestAngle >= PI * 0.75f || fTestAngle < -PI * 0.75f)
			{
				ray.simple.x = floatHitPointY - (float)intHitPointY;
			}
		}
	}

	ray.distance = ray.distance * cosf(player.angle - ray.angle);

	return ray;

}

/// <summary>
/// Animator���ʵ��
/// </summary>

Animator::Animator(float speed)
{
	printf("Init Animator\n");
	this->speed = speed;
	animatorList.push_back(this);
}

/// <summary>
/// ��ʼ��Animator
/// </summary>
/// <param name="frameImage">����֡ͼ��</param>
Animator::Animator(Uint32* frameImage,float speed)
{
	framesImage.push_back(frameImage);
	currentFrame = frameImage;
	this->speed = speed;
	printf("Init Animator\n");

	animatorList.push_back(this);

}

Animator::~Animator()
{

}

std::vector<uint32_t*> LoadAnimator(const char* file)
{
	std::ifstream infile(file, std::ios::binary);
	if (!infile)
	{
		std::cout << "�޷����ļ���" << std::endl;
		return {};
	}

	// ��ȡ��ά�б������
	uint32_t rows = 0;
	infile.read(reinterpret_cast<char*>(&rows), sizeof(rows));

	std::vector<uint32_t*> result;

	for (uint32_t i = 0; i < rows; i++)
	{
		// ��ȡÿ�е�Ԫ�ظ���
		uint32_t cols = 0;
		infile.read(reinterpret_cast<char*>(&cols), sizeof(cols));

		// �����ڴ�ռ�
		uint32_t* row = new uint32_t[cols];

		// ��ȡÿ��Ԫ��
		infile.read(reinterpret_cast<char*>(row), sizeof(uint32_t) * cols);

		result.push_back(row);
	}

	infile.close();

	return result;
}

/// <summary>
/// �ӱ��ض����ļ����ض���
/// </summary>
/// <param name="filePath">�����ļ�·��</param>
void Animator::LoadFromAnimatorFile(const char* filePath) {
	vector<Uint32*> animatorData = LoadAnimator(filePath);
	for (Uint32* frame:animatorData) 
	{
		InsertFrameImage(frame);
	}
}

/// <summary>
/// ��ȡ��ǰ֡��ͼ��
/// </summary>
/// <returns></returns>
Uint32* Animator::GetCurrentFrameImage() {
	return currentFrame;
}

/// <summary>
/// ��ȡָ��֡��ͼ��
/// </summary>
/// <param name="frame">��frame֡</param>
/// <returns></returns>
Uint32* Animator::GetFrameImage(int frame) {
	if (framesImage.empty())
	{
		//printf("return frame image");
		return nullptr;
	}
	if (frame >= framesImage.size())
	{
		//printf("return frame image");
		return framesImage[0];
	}
	//printf("return frame image");
	return framesImage[frame];
}

void Animator::SetFrameEvent(int frame, eventFunc e) {
	//����֡�¼�
	this->framesEvent[frame] = e;
}

/// <summary>
/// ���Ŷ�����
/// </summary>
/// <param name="frame">���ŵ�frame֡��ͼ��</param>
void Animator::Play(int frame) {
	currentFrame = GetFrameImage(frame);
}

void Animator::Play() {
	if (isPlaying)
	{
		return;
	}
	isPlaying = true;
}

/// <summary>
/// ����֡ͼ��
/// </summary>
/// <param name="frameImage"></param>
void Animator::InsertFrameImage(Uint32* frameImage)
{
	printf("Insert Animator\n");
	framesImage.push_back(frameImage);
	if (currentFrame == nullptr)
	{
		currentFrame = frameImage;
	}
}

/// <summary>
/// ��ȡ��ǰ֡������
/// </summary>
/// <param name="index">֡</param>
/// <returns></returns>
Uint32 Animator::GetCurrentFramePixel(int index)
{
	if (index < 0 || currentFrame == nullptr)
	{
		return BLACK;
	}
	return currentFrame[index];
}

/// <summary>
/// ʵʱ����ˢ��
/// </summary>
void Animator::Update() {
	if (isPlaying)
	{
		i = i + deltaTime * speed;
		if (i >= this->framesImage.size())
		{
			i = 0;
			isPlaying = false;
		}
		this->Play(i);
		if (abs(i-(int)i)< speed *deltaTime && framesEvent[i] != nullptr)
		{
			framesEvent[i]();
		}
		
		cout << "Animator is update:" << isPlaying << ",frame:" << i << ",eps:" << abs(i - (int)i) << endl;

		//������Ӧ��ͼ��
		//gunSprite.UpdateImage(gunAniamtor.GetCurrentFrameImage());
	}

	

}

/// <summary>
/// Audio���ʵ��
/// </summary>
/// 
Audio::Audio(const char* filePath, AudioType type) {
	this->type = type;
	if (type == Music)
	{
		this->music = Mix_LoadMUS(filePath);
	}
	else if (type == Chunk)
	{
		this->chunk = Mix_LoadWAV(filePath);
	}
}

Audio::~Audio() {

}

void Audio::Play(bool loop) {
	if (type == Music)
	{
		if (loop)
		{
			Mix_PlayMusic(music, -1);
		}
		else {
			Mix_PlayMusic(music, 0);
		}
	}
	else if (type == Chunk)
	{
		if (loop)
		{
			Mix_PlayChannel(-1, chunk, -1);
		}
		else {
			Mix_PlayChannel(-1, chunk, 0);
		}
	}
}

///Sprite���ʵ��
Sprite::Sprite() {

}

Sprite::Sprite(int w, int h, int size, Uint32* image)
{
	this->w = w;
	this->h = h;
	this->image = image;
	this->sizeForSurface = size;
}

Sprite::~Sprite()
{

}

void Sprite::UpdateImage(Uint32* image)
{
	this->image = image;
}

/// <summary>
/// ����Sprite����Ļ��Ӧλ��
/// </summary>
/// <param name="posX">��Ļ����x</param>
/// <param name="posY">��Ļ����y</param>
/// <param name="isRenderAlpha">�Ƿ����alphaͨ��</param>
void Sprite::DrawSprite(int posX, int posY, bool isRenderAlpha)
{
	//����Ŵ�
	int wannaW = surfaceWidth / sizeForSurface;
	int wannaH = (int)(((float)h / (float)w) * wannaW);

	for (int width = 0; width < wannaW; width++)
	{
		for (int height = 0; height < wannaH; height++)
		{
			int x = posX - ((float)wannaW * 0.5f) + width;
			int y = posY - ((float)wannaH * 0.5f) + height;

			float fSimpleX = (float)(width) / (float)wannaW;
			float fSimpleY = (float)(height) / (float)wannaH;

			Uint32 pixelColor = GetSpriteColor(fSimpleY, fSimpleX, h, w, image);

			if (isRenderAlpha)
			{
				if (pixelColor == BLACK)
				{
					continue;
				}
			}

			if (x < 0 || x >= surfaceWidth || y < 0 || y >= surfaceHeight)
			{
				continue;
			}
			DrawPixel(x, y, pixelColor);
		}
	}
}