#include "Animator.h"
#include "NoaEngine.h"

namespace noa {

	extern float deltaTime;

	AnimatorFile LoadAnimatorFile(const char* file) {
		AnimatorFile animator;

		std::ifstream inputFile(file, std::ios::binary);
		if (!inputFile) {
			Debug("Opening file: "+string(file)+" error");
			//std::cout << "Error opening file." << std::endl;
			return animator;
		}

		int spriteCount;
		inputFile.read(reinterpret_cast<char*>(&spriteCount), sizeof(int));

		for (int i = 0; i < spriteCount; ++i) {
			SpriteFile sprite;
			int imageCount;
			inputFile.read(reinterpret_cast<char*>(&imageCount), sizeof(int));

			//sprite.images = new uint32_t[imageCount];
			for (int j = 0; j < imageCount; ++j) {
				uint32_t pixel;
				inputFile.read(reinterpret_cast<char*>(&pixel), sizeof(uint32_t));
				sprite.images.push_back(pixel);
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

	//vector<Animator*> animatorList;

	Animator::Animator(float speed) :Behaviour()
	{
		Debug("Init Animator");
		this->speed = speed;
		//animatorList.push_back(this);
		if (!framesImage.empty())
		{
			currentFrame = move(framesImage[0]);
		}
		frameSize = framesImage.size();

	}

	Animator::Animator(float speed, const char* filePath) :Behaviour()
	{
		Debug("Init Animator");
		this->speed = speed;
		//animatorList.push_back(this);

		const AnimatorFile animatorFile = move(LoadAnimatorFile(filePath));
		for (SpriteFile frame : animatorFile.data)
		{
			InsertFrameImage(frame);
		}

		if (framesImage.size() > 0)
		{
			currentFrame = move(framesImage[0]);
		}

		frameSize = framesImage.size();

	}

	Animator::~Animator()
	{
		Behaviour::~Behaviour();
	}

	/// <summary>
	/// 从本地动画文件加载动画
	/// </summary>
	/// <param name="filePath">动画文件路径</param>
	void Animator::LoadFromAnimatorFile(const char* filePath)
	{
		const AnimatorFile animatorFile = move(LoadAnimatorFile(filePath));
		for (SpriteFile frame : animatorFile.data)
		{
			InsertFrameImage(frame);
		}

	}

	/// <summary>
	/// 获取当前帧的图像
	/// </summary>
	/// <returns></returns>
	SpriteFile& Animator::GetCurrentFrameImage()
	{
		return currentFrame;
	}

	/// <summary>
	/// 获取指定帧的图像
	/// </summary>
	/// <param name="frame">第frame帧</param>
	/// <returns></returns>
	SpriteFile& Animator::GetFrameImage(int frame) 
	{
		frame = frame & (framesImage.size() - 1);
		return framesImage[frame];
	}

	void Animator::SetFrameEvent(int frame, function<void()> e)
	{
		//设置帧事件
		this->framesEvent[frame] += e;
	}

	/// <summary>
	/// 播放动画化
	/// </summary>
	/// <param name="frame">播放第frame帧的图像</param>
	void Animator::Play(int frame)
	{
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
		currentFrame = framesImage[0];
		frameSize = framesImage.size();
	}

	void Animator::Start()
	{
	}

	/// <summary>
	/// 实时更新刷新
	/// </summary>
	void Animator::Update() {
		if (!isPlaying)
		{
			return;
		}

		i = i + deltaTime * speed;
		if (i >= frameSize)
		{
			i = 0;
			isPlaying = false;
		}

		this->Play(i);

		const bool isFrameStart = abs(i - (int)i) < speed * deltaTime;

		if (isFrameStart && (framesEvent.empty()))
		{
			framesEvent[i].Invoke();
		}

	}

}

