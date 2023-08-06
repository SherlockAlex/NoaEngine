#include "Animator.h"
#include "NoaEngine.h"

/// <summary>
/// Animator���ʵ��
/// </summary>

vector<Animator*> animatorList;

Animator::Animator(float speed)
{
	game.Debug("Init Animator");
	this->speed = speed;
	animatorList.push_back(this);
}

/// <summary>
/// ��ʼ��Animator
/// </summary>
/// <param name="frameImage">����֡ͼ��</param>
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
		game.Debug("�޷����ļ�");
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
	for (Uint32* frame : animatorData)
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
		return nullptr;
	}
	frame = frame & (framesImage.size()-1);
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
	game.Debug("Insert Animator Frame");
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
/// ʵʱ����ˢ��
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

	//������Ӧ��ͼ��
	//gunSprite.UpdateImage(gunAniamtor.GetCurrentFrameImage());

}

