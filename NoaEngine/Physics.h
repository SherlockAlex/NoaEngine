#ifndef NOAENGINE_PHISICS
#define NOAENGINE_PHISICS

#include <vector>
#include "NoaMath.h"

namespace noa {
	//�����һ��������壬����ģ��������˶���һЩ��ײ
	//������ù����������Ч����ֻҪ�̳д���
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
		Vector<float>* colliderPos = nullptr;
	protected:
		float mass = 1;
		float g = 9.81;
		bool useGravity = true;
		bool isGrounded = false;
		bool useCollision = true;
	public:
		//������ٶ�
		Vector<float> & velocity = *(new Vector<float>(0.0,0.0));
		//�������ײ�Ĵ�С
		Vector<float> & colliderSize =*(new Vector<float>(0.0, 0.0));
	protected:
		Rigidbody(Vector<float>* colliderPos);
		~Rigidbody();

	public:
		void RigidbodyUpdate(float deltaTime);

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

	};
}




#endif // !NOAENGINE_PHISICS
