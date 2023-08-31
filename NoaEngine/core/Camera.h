#ifndef NOAENGINE_CAMERA_H
#define NOAENGINE_CAMERA_H

#include "NoaMath.h"
#include "Scene.h"

namespace noa {
	//�������Ϸ�������������ʾ��Ϸ�Ļ�������
	//��Ҫ��������Ⱦ��Ϸ�ĳ���
	// Խ��ǰ�ľ��ȱ���Ⱦ
	class Camera
	{
	
	public:
		Vector<float>* follow = nullptr;
		Vector<float> position;

	public:
		/// <summary>
		/// ������캯��
		/// </summary>
		/// <param name="follow"></param>
		Camera();
		Camera(Vector<float> * follow);
		~Camera();
	};

	class TileMapCamera :public Camera
	{
	private:
		Vector<int> tileScale = Vector<int>(64, 64);
		Vector<float> visibleTiles;
		Vector<float> offset;
		Vector<int> followPositionOnScreen = Vector<int>(0.0, 0.0);
	public:
		TileMapCamera();
		TileMapCamera(Vector<int> tileScale, Vector<float>* follow);
		
		/// <summary>
		/// ��Ⱦ��Ƭ��ͼ����Ļ��
		/// </summary>
		/// <param name="tileMap">Ҫ��Ⱦ����Ƭ��ͼ</param>
		/// <param name="frontDelta">���ǰ�߽�ƫ����</param>
		/// <param name="endDelta">�����߽�ƫ����</param>
		Vector<int> Render(TileMap& tileMap,Vector<float> & frontDelta,Vector<float> & endDelta);
	};

}


#endif // !NOAENGINE_CAMERA_H


