#ifndef NOAENGINE_QUEUE_H
#define NOAENGINE_QUEUE_H

#include <iostream>

namespace noa {
	template<class T>
	class Queue {
	private:
		T* elements = nullptr;
		size_t count = 0;
		size_t maxSize = 100;
		size_t frontPtr = 0;
		size_t backPtr = 0;
	public:
		Queue() {
			count = 0;
			frontPtr = 0;
			backPtr = 0;
			maxSize = 100;

			elements = new T[maxSize];

		}

		~Queue() {
			delete[] elements;
			elements = nullptr;
		}

		void Enqueue(T item)
		{
			if (count==maxSize)
			{
				T* oldElements = elements;
				maxSize = maxSize + 100;
				elements = new T[maxSize];
				for (size_t i = 0;i<count;i++) 
				{
					elements[i] = oldElements[i];
				}
				delete[] oldElements;
			}

			elements[backPtr] = item;
			backPtr++;
			count++;

		}

		T Dequeue() 
		{
			if (backPtr == frontPtr)
			{
				throw std::runtime_error("This queue is empty");
			}

			T result = elements[frontPtr];
			frontPtr++;
			count--;
			return result;

		}

		void Clear() {
			count = 0;
			frontPtr = 0;
			backPtr = 0;
		}

	};
}

#endif // !NOAENGINE_QUEUE_H
