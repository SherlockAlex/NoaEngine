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

		void Unique() 
		{
			//假设 elements[0...i]的子数组没有任何重复项
			//判断位置i的元素应该保留还是删除
			for (size_t i = 0;i<count;) 
			{
				bool flag = false;
				for (size_t j = 0;) 
				{
					if (this->elements[i] == this->elements[j])
					{
						flag = true;
						break;
					}
				}

				if (!flag) 
				{
					i++;
					continue;
				}

				//删除掉要插入的元素
				for (size_t k = i;k<count-1;k++) 
				{
					this->elements[k] = this->elements[k + 1];
				}
				this->count--;
				flag = false;

			}
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

