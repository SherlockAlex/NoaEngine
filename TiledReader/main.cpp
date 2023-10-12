#include <iostream>
#include <fstream>
#include <vector>

typedef struct Node {
	int value = -1;
	Node* left = nullptr;
	Node* right = nullptr;
}Node;

class BinaryTree {
public:
	Node* root = nullptr;
public:
	BinaryTree(const std::vector<int> & data) 
	{
		//根据data建立Tree
		int index = 0;
		CreateSubTree(root,index,data);
		std::cout << "Create Binary Tree successfully" << std::endl;
	}

	void CreateSubTree(Node *& root,int & index, const std::vector<int>& data) {
		if (index<0||index>=data.size()||data[index] == -1)
		{
			root = nullptr;
			index++;
			return;
		}

		//采用中序创建的
		root = new Node();
		root->value = data[index];
		root->left = nullptr;
		root->right = nullptr;
		index++;
		CreateSubTree(root->left,index,data);
		CreateSubTree(root->right, index, data);

	}

	void PreOrder(Node * node) 
	{
		if (node == nullptr)
		{
			return;
		}
		
		PreOrder(node->left);
		std::cout << node->value << " ";
		PreOrder(node->right);

	}

};

int main(int argc,char * argv[])
{
	//读取TMX文件

	std::vector<int> data = { 0,2,4,3,-1,0,-1,2,5,9,8,4,5,6,-1 };
	BinaryTree tree = BinaryTree(data);
	tree.PreOrder(tree.root);

	return 0;
}