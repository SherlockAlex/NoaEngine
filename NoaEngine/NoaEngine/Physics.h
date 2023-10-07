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


	//�洢������Tile����ײ��Ϣ
	typedef struct Collision 
	{
		Vector<float> sacle = Vector<float>(0, 0);
		bool isHitCollisionTile = false;
		bool isGrounded = false;
		bool isTrigger = false;
		Rigidbody * other = nullptr;
		int hitTileID = -1;
		

	}Collision;

	extern float gravityAcceration;

	class Scene;
	class Actor;
	class TileMap;
	class Transform;
	class Collider2D;
	/// <summary>
	/// �����࣬������ʹ�ø���ʱ��������Ʒ���ݻ��ǣ���ʹ��
	/// </summary>
	
	
	enum class ForceType
	{
		CONTINUOUS_FORCE = 1 << 0,
		IMPULSE_FORCE = 1 << 1,
	};

	class Rigidbody final
	{
	public:
		std::string tag = "default";
		float mass = 1;
		float damping = 0.02f;
		Actor* actor;
		Vector<float> newPosition;
		float gravityWeight = 1.0;
	private:
		
		//��������֮�����ײ���
		int indexInMap = -1;
		TileMap* tileMap = nullptr;
		
		Vector<float> force = Vector<float>(0.0, 0.0);

		float invMass = 1;
		bool active = true;

		std::vector<Collider2D*> colliders;

	public:
		
		bool isRemoved = false;
		bool useMotion = true;
		bool isFrozen = false;

		Vector<float> velocity = Vector<float>(0.0,0.0);

		// �洢�������������ײ��Ϣ
		Collision collision;
		bool useGravity = true;
		bool useCollision = true;

	private:
		Rigidbody(Actor* actor);
	public:
		~Rigidbody();
		static Rigidbody* Create(Actor* actor);

		void Start();
		void Update(float deltaTime);
		void LateUpdate(float deltaTime);

		void UpdatePosition();

		void Destroy();

		void AddForce(const Vector<float> & force, ForceType forceType);
		
		void SetTileMap(TileMap * map);

		TileMap* GetTileMap();

		//��ײ����߳�
		void ApplyCollision();

		void BindCollider(Collider2D * collider);

		void ApplyTrigger();

	public:

		template<class T>
		T GetActorAs() {
			return dynamic_cast<T>(this->actor);
		}

		template<class T>
		bool TryGetActorAs(T & out)
		{
			T buffer = dynamic_cast<T>(this->actor);
			if (buffer == nullptr)
			{
				return false;
			}
			out = buffer;
			return true;
		}

		int GetIndexInMap() const;

		// ��ȡ��ϣֵ
		size_t GetHashCode() const {
			return id;
		}
		
		void SetActive(bool value);

		bool GetActive();


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
