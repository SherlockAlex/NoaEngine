#ifndef NOAENGINE_PHISICS
#define NOAENGINE_PHISICS

#include <iostream>

#include <vector>
#include "NoaMath.h"
#include "Actor.h"

namespace noa {
	//�����һ��������壬����ģ��������˶���һЩ��ײ
	//������ù����������Ч����ֻҪ�̳д���

	//Rigidbodyֻ��ʵ������û�취�̳�
	class Rigidbody;

	//�洢�������ײ��Ϣ
	typedef struct Collision 
	{
		Vector<float> sacle = Vector<float>(0, 0);
		float radius = 0.5;

		bool isHitCollisionTile = false;

		bool isGrounded = false;
		bool isTrigger = false;
		Rigidbody * other = nullptr;
		
		int hitTileID = -1;

		bool collisionInfo[2][2] = { false,false,false,false };
		void Update(bool isHitCollisionTile,bool isGrounded) 
		{
			this->isHitCollisionTile = isHitCollisionTile;
			this->isGrounded = isGrounded;
		}

	}Collision;

	extern float gravityAcceration;

	class Scene;
	class Actor;
	class TileMap;
	class Transform;
	/// <summary>
	/// �����࣬������ʹ�ø���ʱ��������Ʒ���ݻ��ǣ���ʹ��RemoveRigidbody
	/// </summary>
	class Rigidbody final
	{
	public:
		std::string tag = "default";

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
		Actor* actor;
		Vector<float> newPosition;
		float gravityWeight = 1.0;
	private:
		
		//��������֮�����ײ���
		int indexInMap = -1;
		TileMap* tileMap = nullptr;
		
		Vector<float> sumForce = Vector<float>(0.0, 0.0);

		float invMass = 1;
		

	public:
		
		bool useMotion = true;
		bool isFrozen = false;
		//������ٶ�
		Vector<float> velocity = Vector<float>(0.0,0.0);
		//����֮����ײ��뾶
		

		Collision collision;

		bool useGravity = true;
		//bool isGrounded = false;
		bool useCollision = true;
		//void* gameObject = nullptr;

	private:
		Rigidbody(Actor* actor);
		
		Rigidbody(const Rigidbody&);
		Rigidbody& operator = (const Rigidbody&);

	public:
		~Rigidbody();
		static Rigidbody* Create(Actor* actor);

		void Start();
		void Update();
		void Destroy();

		/// <summary>
		/// ������ʩ����
		/// </summary>
		/// <param name="force">������ֵ�������������Ϊ����������ֵ��ʾ��������ǳ��������ʾ�����Ĵ�С</param>
		/// <param name="forceType">��������</param>
		void AddForce(const Vector<float> force, ForceType forceType);
		
		void SetTileMap(TileMap * map);

		TileMap* GetTileMap();

		//��ײ����߳�
		void ApplyCollision();

		

		/// <summary>
		/// ������֮��ײ����Ʒ
		/// </summary>
		/// <returns></returns>
		//Rigidbody* GetCollisionRigidbody();

		/*template<class T>
		T GetGameObjectAs() {
			return (T)this->gameObject;
		}*/

		template<class T>
		T GetActorAs() {
			return dynamic_cast<T>(this->actor);
		}

		//void SetCollisionRigidbody(Rigidbody* rigid);

		int GetIndexInMap() const;

		//void RemoveRigidbody() const;

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

	public:
		bool CollisionWithinRigidbody();

	};

	
}




#endif // !NOAENGINE_PHISICS
