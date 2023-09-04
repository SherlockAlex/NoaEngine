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
		//����
		float mass = 1;
		//����ϵ��
		float damping = 0.02;
		Transform* colliderPos = nullptr;
	private:
		
		//��������֮�����ײ���
		int indexInMap = -1;
		Rigidbody* collisionRigid;
	protected:
		float invMass = 1;

		float g = 9.81;
		bool useGravity = true;
		bool isGrounded = false;
		bool useCollision = true;
		
		void* gameObject = nullptr;

	public:
		bool isFrozen = false;
		//������ٶ�
		Vector<float> velocity = Vector<float>(0.0,0.0);
		//�������ײ�Ĵ�С
		Vector<float> colliderSize = Vector<float>(0.0, 0.0);

		Collision collision;
		bool isTrigger = false;
		bool isHitWall = false;

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
		void AddForce(const Vector<float> force, ForceType forceType);
		void SetCollisionTileID(std::vector<int> collisionTileIDs);
		void UpdateMap(void * map);
		float FixPosition();
		void UpdateCollision(const Vector<float> nextPosition);

		/// <summary>
		/// ������֮��ײ����Ʒ
		/// </summary>
		/// <returns></returns>
		Rigidbody* GetCollisionRigidbody();

		template<class T>
		T GetGameObjectAs() {
			return (T)this->gameObject;
		}

		void SetCollisionRigidbody(Rigidbody* rigid);

		int GetIndexInMap() const;

	};
}




#endif // !NOAENGINE_PHISICS
