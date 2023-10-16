#ifndef NOAENGINE_LIST_H
#define NOAENGINE_LIST_H

#include <iostream>

namespace noa{
	template<class T>
	class List
	{
	private:
		T* elements = nullptr;
		size_t count = 0;
		size_t maxSize = 100;
	public:
		List() {
			maxSize = 100;
			elements = new T[maxSize];
			count = 0;
		}
		~List() {
			delete[] elements;
			elements = nullptr;
		}
	public:
		size_t Count() {
			return count;
		}

		void Add(T item)
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
			elements[count] = item;
			count++;
		}

		void RemoveAt(size_t index)
		{
			if (index >= count)
			{
				return;
			}

			for (size_t i = index; i < count - 1; i++)
			{
				elements[i] = elements[i + 1];
			}
			count--;
		}

		void Remove(T item)
		{
			size_t itemIndex = -1;
			for (size_t i = 0;i<count;i++)
			{
				if (elements[i] == item)
				{
					itemIndex = i;
					break;
				}
			}

			RemoveAt(itemIndex);

		}

		bool Contains(T item) 
		{
			bool isInList = false;
			for (size_t i = 0;i<count;i++)
			{
				if (elements[i] == item)
				{
					isInList = true;
					break;
				}
			}
			return isInList;
		}

		void Clear() {
			count = 0;
		}

	public:

		T operator[] (size_t index)
		{
			if (index >= count)
			{
				throw std::runtime_error("List index out of bounds");
			}
			return elements[index];
		}

		T* begin() {
			return elements;
		}

		T* end() {
			return elements + count;
		}

	};
}



#endif // !NOAENGINE_LIST_H

