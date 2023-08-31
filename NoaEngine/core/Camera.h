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
		/// ��Ⱦ��ͼ
		/// </summary>
		/// <param name="tileMap"></param>
		/// <returns>���ظ�����������Ļ�ϵ�����</returns>
		Vector<int> Render(TileMap& tileMap);
	};

}


#endif // !NOAENGINE_CAMERA_H


