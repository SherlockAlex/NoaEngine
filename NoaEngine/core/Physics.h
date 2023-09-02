#ifndef NOAENGINE_PHISICS
#define NOAENGINE_PHISICS

#include <vector>
#include "NoaMath.h"
#include "Behaviour.h"

namespace noa {
	//�����һ��������壬����ģ��������˶���һЩ��ײ
	//������ù����������Ч����ֻҪ�̳д���

	//�洢�������ײ��Ϣ
	typedef struct Collision 
	{
		int leftUpTile = -1;
		int leftDownTile = -1;
		int rightUpTile = -1;
		int rightDownTile =-1;

		void Update(int leftUp,int leftDown,int rightUp,int rightDown) 
		{
			this->leftUpTile = leftUp;
			this->leftDownTile = leftDown;
			this->rightUpTile = rightUp;
			this->rightDownTile = rightDown;
		}

	}Collision;

	class Transform;
	class Rigidbody
	{
	public:
		//��������
		enum ForceType
		{
			ContinuousForce = 1 << 0,
			Impulse = 1 << 1,
		};

	private:
		Transform* colliderPos = nullptr;
		//��������֮�����ײ���
		int indexInMap = -1;
	protected:
		float mass = 1;
		float g = 9.81;
		bool useGravity = true;
		bool isGrounded = false;
		bool useCollision = true;
		
	public:
		bool isFrozen = false;
		//������ٶ�
		Vector<float> velocity = Vector<float>(0.0,0.0);
		//�������ײ�Ĵ�С
		Vector<float> colliderSize = Vector<float>(0.0, 0.0);

		Collision collision;

	protected:
		Rigidbody(Transform* colliderPos);
		~Rigidbody();

	public:
		void Start();
		void Update();

		/// <summary>
		/// ������ʩ����
		/// </summary>
		/// <param name="force">������ֵ�������������Ϊ����������ֵ��ʾ��������ǳ��������ʾ�����Ĵ�С</param>
		/// <param name="forceType">��������</param>
		void AddForce(Vector<float> & force, ForceType forceType);
		void SetCollisionTileID(std::vector<int> collisionTileIDs);
		void UpdateMap(void * map);
		void ApplyCollision();
		float FixPosition();
		void UpdateCollision(Vector<float> & nextPosition);

		int GetIndexInMap();

	};
}




#endif // !NOAENGINE_PHISICS
