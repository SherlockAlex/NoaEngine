#include "Animator.h"
#include "NoaEngine.h"

/// <summary>
/// Animator类的实现
/// </summary>

vector<Animator*> animatorList;

std::vector<uint32_t*> LoadAnimator(const char* file);

AnimatorFile LoadAnimatorFile(const char* file) {
	AnimatorFile animator;

	std::ifstream inputFile(file, std::ios::binary);
	if (!inputFile) {
		std::cout << "Error opening file." << std::endl;
		return animator;
	}

	int spriteCount;
	inputFile.read(reinterpret_cast<char*>(&spriteCount), sizeof(int));

	for (int i = 0; i < spriteCount; ++i) {
		SpriteFile sprite;
		int imageCount;
		inputFile.read(reinterpret_cast<char*>(&imageCount), sizeof(int));

		sprite.images = new uint32_t[imageCount];
		for (int j = 0; j < imageCount; ++j) {
			inputFile.read(reinterpret_cast<char*>(&sprite.images[j]), sizeof(uint32_t));
		}

		inputFile.read(reinterpret_cast<char*>(&sprite.x), sizeof(int));
		inputFile.read(reinterpret_cast<char*>(&sprite.y), sizeof(int));
		inputFile.read(reinterpret_cast<char*>(&sprite.width), sizeof(int));
		inputFile.read(reinterpret_cast<char*>(&sprite.height), sizeof(int));

		animator.data.push_back(sprite);
	}

	inputFile.read(reinterpret_cast<char*>(&animator.posx), sizeof(int));
	inputFile.read(reinterpret_cast<char*>(&animator.posy), sizeof(int));
	inputFile.read(reinterpret_cast<char*>(&animator.w), sizeof(int));
	inputFile.read(reinterpret_cast<char*>(&animator.h), sizeof(int));

	inputFile.close();
	return animator;
}

Animator::Animator(float speed)
{
	Debug("Init Animator");
	this->speed = speed;
	animatorList.push_back(this);
	if (framesImage.size()>0)
	{
		currentFrame = &framesImage[0];
	}

}

//Animator::Animator(const char* amtFilePath,float speed)
//{
//	const vector<Uint32*> animatorData = LoadAnimator(amtFilePath);
//	this->speed = speed;
//	animatorList.push_back(this);
//	currentFrame = animatorData[0];
//	for (Uint32* frame : animatorData)
//	{
//		InsertFrameImage(frame);
//	}
//}

//Animator::Animator(AnimatorFile animatorFile, float speed)
//{
//	this->speed = speed;
//	const vector<SpriteFile> animatorData = animatorFile.data;
//	animatorList.push_back(this);
//	for (SpriteFile frame : animatorData)
//	{
//		InsertFrameImage(frame.images);
//	}
//
//}

/// <summary>
/// 初始化Animator
/// </summary>
/// <param name="frameImage">插入帧图像</param>
//Animator::Animator(Uint32* frameImage, float speed)
//{
//	framesImage.push_back(frameImage);
//	currentFrame = frameImage;
//	this->speed = speed;
//	Debug("Init Animator");
//
//	animatorList.push_back(this);
//
//}

Animator::~Animator()
{

}

std::vector<uint32_t*> LoadAnimator(const char* file)
{
	std::ifstream infile(file, std::ios::binary);
	if (!infile)
	{
		Debug("无法打开文件");
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
void Animator::LoadFromAnimatorFile(const char* filePath) 
{
	const AnimatorFile animatorFile = LoadAnimatorFile(filePath);
	for (SpriteFile frame:animatorFile.data) 
	{
		InsertFrameImage(frame);
	}

	

}

/// <summary>
/// 获取当前帧的图像
/// </summary>
/// <returns></returns>
SpriteFile* Animator::GetCurrentFrameImage() {
	return currentFrame;
}

/// <summary>
/// 获取指定帧的图像
/// </summary>
/// <param name="frame">第frame帧</param>
/// <returns></returns>
SpriteFile* Animator::GetFrameImage(int frame) {
	if (framesImage.empty())
	{
		return nullptr;
	}
	frame = frame & (framesImage.size()-1);
	return &framesImage[frame];
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
void Animator::InsertFrameImage(SpriteFile frameImage)
{
	Debug("Insert Animator Frame");
	framesImage.push_back(frameImage);
	if (currentFrame == nullptr)
	{
		currentFrame = &framesImage[0];
	}
	//currentFrame = nullptr;
}

/// <summary>
/// 获取当前帧的像素
/// </summary>
/// <param name="index">帧</param>
/// <returns></returns>
//Uint32 Animator::GetCurrentFramePixel(int index)
//{
//	if (currentFrame==nullptr)
//	{
//		return BLACK;
//	}
//	if (index < 0 || currentFrame == nullptr)
//	{
//		return BLACK;
//	}
//	//index = index & (currentFrame)
//	return currentFrame[index].images;
//}

/// <summary>
/// 实时更新刷新
/// </summary>
void Animator::Update(float deltaTime) {
	if (!isPlaying)
	{
		return;
	}

	i = i + deltaTime * speed;
	if (i >= this->framesImage.size())
	{
		i = 0;
		isPlaying = false;
	}

	this->Play(i);
	
	const bool isFrameStart = abs(i - (int)i) < speed * deltaTime;

	if (isFrameStart && framesEvent[i] != nullptr)
	{
		framesEvent[i]();
	}

}

//inline AnimatorFile LoadAnimatorFile(const char* file)
//{
//	return AnimatorFile();
//}
