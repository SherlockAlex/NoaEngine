#ifndef NOAENGINE_STACK_H
#define NOAENGINE_STACK_H

#include <iostream>

namespace noa {
	template<class T>
	class Stack {
	private:
		T* elements = nullptr;
		size_t count = 0;
		long long topPtr = -1;
		size_t maxSize = 100;
	public:
		Stack()
		{
			count = 0;
			topPtr = -1;
			maxSize = 100;
			elements = new T[maxSize];
		}
		~Stack() {
			delete[] elements;
			elements = nullptr;
		}

		void Push(T item)
		{
			if (count == maxSize)
			{
				T* oldElements = elements;
				maxSize = maxSize + 100;
				elements = new T[maxSize];
				for (size_t i = 0; i < count; i++)
				{
					elements[i] = oldElements[i];
				}
				delete[] oldElements;
			}

			topPtr++;
			count++;
			elements[topPtr] = item;

		}

		void Pop() {
			if (count <= 0)
			{
				return;
			}
			topPtr--;
			count--;
		}

		T Top() {
			if (topPtr < 0)
			{
				throw std::runtime_error("Stack is empty");
			}
			return elements[topPtr];
		}

		void Clear() {
			count = 0;
			topPtr = -1;
		}

		size_t Count() {
			return count;
		}

	};
}



#endif // !NOAENGINE_STACK_H
