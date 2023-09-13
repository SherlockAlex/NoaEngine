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
		bool isHitCollisionTile = false;
		bool isGrounded = false;
		bool isTrigger = false;
		void* other = nullptr;
		bool collisionInfo[2][2] = { false,false,false,false };
		void Update(bool isHitCollisionTile,bool isGrounded) 
		{
			this->isHitCollisionTile = isHitCollisionTile;
			this->isGrounded = isGrounded;
		}

	}Collision;

	extern float gravityAcceration;

	class TileMap;
	class Transform;
	/// <summary>
	/// �����࣬������ʹ�ø���ʱ��������Ʒ���ݻ��ǣ���ʹ��RemoveRigidbody
	/// </summary>
	class Rigidbody
	{
	public:
		string tag = "default";

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
		Vector<float> newPosition;
		float gravityWeight = 1.0;
	private:
		
		//��������֮�����ײ���
		int indexInMap = -1;
		TileMap* tileMap = nullptr;
		
		Vector<float> sumForce = Vector<float>(0.0, 0.0);

	protected:
		float invMass = 1;
		bool useGravity = true;
		//bool isGrounded = false;
		bool useCollision = true;
		void* gameObject = nullptr;

	public:
		
		bool useMotion = true;
		bool isFrozen = false;
		//������ٶ�
		Vector<float> velocity = Vector<float>(0.0,0.0);
		//����֮����ײ��뾶
		float radius = 0.5;

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
		void AddForce(const Vector<float> force, ForceType forceType);
		
		void UpdateMap(TileMap * map);

		//��ײ����߳�
		void ApplyCollision();

		//���������������ڴ�������other��һ��void*���͵�ָ��
		virtual void OnTrigger(void* other) {}

		/// <summary>
		/// ������֮��ײ����Ʒ
		/// </summary>
		/// <returns></returns>
		//Rigidbody* GetCollisionRigidbody();

		template<class T>
		T GetGameObjectAs() {
			return (T)this->gameObject;
		}

		//void SetCollisionRigidbody(Rigidbody* rigid);

		int GetIndexInMap() const;

		void RemoveRigidbody() const;

		// ��ȡ��ϣֵ
		size_t GetHashCode() const {
			return id;
		}
		


	private:
		size_t id;
		static size_t nextId;

		// ��̬���������ڻ�ȡ��һ����һ�޶��Ĺ�ϣֵ
		static size_t GetNextId() {
			return nextId++;
		}

	};
}




#endif // !NOAENGINE_PHISICS
