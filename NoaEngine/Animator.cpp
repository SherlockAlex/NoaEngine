#include "Animator.h"
#include "NoaEngine.h"

/// <summary>
/// Animator类的实现
/// </summary>

vector<Animator*> animatorList;

Animator::Animator(float speed)
{
	game.Debug("Init Animator");
	this->speed = speed;
	animatorList.push_back(this);
}

/// <summary>
/// 初始化Animator
/// </summary>
/// <param name="frameImage">插入帧图像</param>
Animator::Animator(Uint32* frameImage, float speed)
{
	framesImage.push_back(frameImage);
	currentFrame = frameImage;
	this->speed = speed;
	game.Debug("Init Animator");

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
		game.Debug("无法打开文件");
		return {};
	}

	// 读取二维列表的行数
	uint32_t rows = 0;
	infile.read(reinterpret_cast<char*>(&rows), sizeof(rows));

	std::vector<uint32_t*> result;

	for (uint32_t i = 0; i < rows; i++)
	{
		// 读取每行的元素个数
		uint32_t cols = 0;
		infile.read(reinterpret_cast<char*>(&cols), sizeof(cols));

		// 分配内存空间
		uint32_t* row = new uint32_t[cols];

		// 读取每个元素
		infile.read(reinterpret_cast<char*>(row), sizeof(uint32_t) * cols);

		result.push_back(row);
	}

	infile.close();

	return result;
}

/// <summary>
/// 从本地动画文件加载动画
/// </summary>
/// <param name="filePath">动画文件路径</param>
void Animator::LoadFromAnimatorFile(const char* filePath) {
	vector<Uint32*> animatorData = LoadAnimator(filePath);
	for (Uint32* frame : animatorData)
	{
		InsertFrameImage(frame);
	}
}

/// <summary>
/// 获取当前帧的图像
/// </summary>
/// <returns></returns>
Uint32* Animator::GetCurrentFrameImage() {
	return currentFrame;
}

/// <summary>
/// 获取指定帧的图像
/// </summary>
/// <param name="frame">第frame帧</param>
/// <returns></returns>
Uint32* Animator::GetFrameImage(int frame) {
	if (framesImage.empty())
	{
		return nullptr;
	}
	frame = frame & (framesImage.size()-1);
	return framesImage[frame];
}

void Animator::SetFrameEvent(int frame, eventFunc e) {
	//设置帧事件
	this->framesEvent[frame] = e;
}

/// <summary>
/// 播放动画化
/// </summary>
/// <param name="frame">播放第frame帧的图像</param>
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
/// 插入帧图像
/// </summary>
/// <param name="frameImage"></param>
void Animator::InsertFrameImage(Uint32* frameImage)
{
	game.Debug("Insert Animator Frame");
	framesImage.push_back(frameImage);
	if (currentFrame == nullptr)
	{
		currentFrame = frameImage;
	}
}

/// <summary>
/// 获取当前帧的像素
/// </summary>
/// <param name="index">帧</param>
/// <returns></returns>
Uint32 Animator::GetCurrentFramePixel(int index)
{
	if (currentFrame==nullptr)
	{
		return BLACK;
	}
	if (index < 0 || currentFrame == nullptr)
	{
		return BLACK;
	}
	//index = index & (currentFrame)
	return currentFrame[index];
}

/// <summary>
/// 实时更新刷新
/// </summary>
void Animator::Update() {
	if (!isPlaying)
	{
		return;
	}

	i = i + game.DeltaTime() * speed;
	if (i >= this->framesImage.size())
	{
		i = 0;
		isPlaying = false;
	}

	this->Play(i);
	
	bool isFrameStart = abs(i - (int)i) < speed * game.DeltaTime();

	if (isFrameStart && framesEvent[i] != nullptr)
	{
		framesEvent[i]();
	}

	//cout << "Animator is update:" << isPlaying << ",frame:" << i << ",eps:" << abs(i - (int)i) << endl;

	//更新相应的图像
	//gunSprite.UpdateImage(gunAniamtor.GetCurrentFrameImage());

}

