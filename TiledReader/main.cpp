#include <iostream>
#include <vector>
#include <string>

template<typename Key,typename Value>
struct Node
{

	Key key;
	Value value;
	Node<Key,Value>* left = nullptr;
	Node<Key, Value>* right = nullptr;

};

template<typename K,typename V>
class Dictionary {
private:
	Node<K, V> * root = nullptr;

public:
	Dictionary() {}

	void Add(K key,V value) 
	{
		Insert(root,key,value);
	}

	V Get(K key) 
	{
		//返回对应的值
		V value;
		FindValueByKey(root,key,value);
		return value;
	}

private:
	void Insert(Node<K,V> *& root,K key,V value)
	{
		if (!root) 
		{
			root = new Node<K,V>();
			root->key = key;
			root->value = value;
			root->left = nullptr;
			root->right = nullptr;
			return;
		}

		if (root->key == key) 
		{
			root->value = value;
			return;
		}

		if (root->key>key) 
		{
			Insert(root->left,key,value);
			return;
		}
		Insert(root->right,key,value);
	}

	bool FindValueByKey(Node<K,V> * root,K key,V & value) 
	{
		if (!root)
		{
			return false;
		}
		if (root->key == key) 
		{
			value = root->value;
			return true;
		}

		return FindValueByKey(root->left, key, value) || FindValueByKey(root->right, key, value);

	}

};


int main() {

	Dictionary<int, std::string> dict;
	dict.Add(0,"hello");
	dict.Add(1, "world");
	dict.Add(2, "66");
	dict.Add(3, "77");
	dict.Add(4, "55");
	dict.Add(5, "88");

	std::cout << dict.Get(0) << std::endl;
	std::cout << dict.Get(5) << std::endl;
	std::cout << dict.Get(4) << std::endl;
	std::cout << dict.Get(2) << std::endl;
	std::cout << dict.Get(3) << std::endl;

	return 0;
}