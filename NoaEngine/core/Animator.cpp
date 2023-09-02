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
	/// �ӱ��ض����ļ����ض���
	/// </summary>
	/// <param name="filePath">�����ļ�·��</param>
	void Animator::LoadFromAnimatorFile(const char* filePath)
	{
		const AnimatorFile animatorFile = move(LoadAnimatorFile(filePath));
		for (SpriteFile frame : animatorFile.data)
		{
			InsertFrameImage(frame);
		}

	}

	/// <summary>
	/// ��ȡ��ǰ֡��ͼ��
	/// </summary>
	/// <returns></returns>
	SpriteFile& Animator::GetCurrentFrameImage()
	{
		return currentFrame;
	}

	/// <summary>
	/// ��ȡָ��֡��ͼ��
	/// </summary>
	/// <param name="frame">��frame֡</param>
	/// <returns></returns>
	SpriteFile& Animator::GetFrameImage(int frame) 
	{
		frame = frame & (framesImage.size() - 1);
		return framesImage[frame];
	}

	void Animator::SetFrameEvent(int frame, function<void()> e)
	{
		//����֡�¼�
		this->framesEvent[frame] += e;
	}

	/// <summary>
	/// ���Ŷ�����
	/// </summary>
	/// <param name="frame">���ŵ�frame֡��ͼ��</param>
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
	/// ����֡ͼ��
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
	/// ʵʱ����ˢ��
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

