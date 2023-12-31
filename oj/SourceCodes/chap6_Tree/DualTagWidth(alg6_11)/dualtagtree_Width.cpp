#include <iostream>
#include <queue>
using namespace std;                                     

#define N 10                             // 结点数量

template <class T> class Tree;           //声明树类


// 带“双标记”的次序结点类
template <class T>
class DualTagWidthTreeNode  {
public:
	T info;                   //树结点信息
	int ltag;				  //左标记
	int rtag;                 //右标记
	
	DualTagWidthTreeNode() {};               //构造函数
	virtual ~DualTagWidthTreeNode() {};      //析构函数
};


// 树结点类的ADT
template <class T>
class TreeNode  {
	friend class Tree<T>;
private:
	T m_Value;								//树结点的值
	TreeNode<T>*	pChild;					//左子结点
	TreeNode<T>*	pSibling;				//右兄弟结点
public:
	TreeNode() {};                
	TreeNode(const T &value) {m_Value = value; pChild = NULL; pSibling = NULL;};
	T Value() {return m_Value;};                     // 获得结点的信息
	TreeNode<T> *LeftMostChild() {return pChild;};   // 返回第一个左子树
	TreeNode<T> *RightSibling() {return pSibling;};  // 返回第一个右兄弟
	void setValue(T &value) {m_Value = value;};      //设置结点的值
	void setChild(TreeNode<T> *pointer) {pChild = pointer;};   //设置左孩子
	void setSibling(TreeNode<T> *pointer) {pSibling = pointer;};  //设置右孩子
};

// 树类ADT，只写出了相关的变量及函数
template <class T>
class Tree  {
private:
	TreeNode<T> *root;                                  
	TreeNode<T> *getParent(TreeNode<T> *root, TreeNode<T> *current);
public:
	Tree() {root = NULL;};
	Tree(DualTagWidthTreeNode<T> *nodeArray, int count); //带双标记先根次序构造算法 
	TreeNode<T> *getRoot() {return root;};          //返回树中的根结点
	void Visit(T Value) {cout << Value;};           //访问
	void RootFirstTraverse(TreeNode<T> *root);      //先根次序周游，作为测试使用
};

// 函数功能：带双标记层次次序构造算法 
template <class T>
Tree<T>::Tree(DualTagWidthTreeNode<T>* nodeArray, int count)  {
	//由带双标记位的层次次序表示构造左孩子右兄弟方式表示的树
	using std::queue;                                 //使用STL队列
	queue<TreeNode<T>*> aQueue;
	//准备建立根结点
	TreeNode<T>* pointer=new TreeNode<T>;
	root=pointer;
	//处理一个结点
	for(int i=0;i<count-1;i++) {
		pointer->setValue(nodeArray[i].info);
		if(nodeArray[i].ltag==0)
			aQueue.push(pointer);					   //将结点入队
		else
			pointer->setChild(NULL);				   //左孩子设为空
		TreeNode<T>* temppointer=new TreeNode<T>;
		if(nodeArray[i].rtag == 0)
			pointer->setSibling(temppointer);
		else {
			pointer->setSibling(NULL);					//右兄弟设为空
			pointer=aQueue.front();                     //取队列首结点指针
			aQueue.pop();                            //队首元素出队列
			pointer->setChild(temppointer);
		}
		pointer=temppointer;
	}
	//处理最后一个结点
	pointer->setValue(nodeArray[count-1].info);
	pointer->setChild(NULL);
	pointer->setSibling(NULL);
}

// 函数功能：先根深度遍历
template <class T>
void Tree<T>::RootFirstTraverse(TreeNode<T> *root)  {
	while (root != NULL)   {
		Visit(root->Value());
		RootFirstTraverse(root->LeftMostChild());
		root = root->RightSibling();
	}
}

// 函数功能：周游树，在这里只列举一种（先根次序）
void Traverse(Tree<char> *tree)  {
	cout << "FirstRoot traverse:  ";
	tree->RootFirstTraverse(tree->getRoot()); // 先根深度优先周游
	cout << endl;
}


// 函数功能：显示森林结构.图6.5(a)所示的森林
void DisplayTree()  {
	cout << "      A              G      \n";
	cout << "   /  |  \\         /   \\   \n";
	cout << "  B   C    D      H     I   \n";
	cout << "     / \\          |         \n";
	cout << "    E   F         J         \n";
	cout << "\n";
	cout << "rtag =  0  have right sibling" << endl;
	cout << "rtag =  1  havn't right sibling" << endl;
	cout << "ltag =  0  have left child" << endl;
	cout << "ltag =  1  havn't left child" << endl << endl;
}

// 函数功能：显示森林结构的带双标记层次次序表示 
void DisplayNode(char *Info, int *nRtag, int *nLtag)  {
	if (Info != NULL)  {
		cout << "info   ";
		for (int i = 0; i < N; i ++)
			cout << Info[i] << " ";
		cout << endl;
	}
	
	if (nRtag != NULL)  {
		cout << "rtag   ";
		for (int i = 0; i < N; i ++)
			cout << nRtag[i] << " ";
		cout << endl;
	}
	
	if (nLtag != NULL)  {
		cout << "ltag   ";
		for (int i = 0; i < N; i ++)
			cout << nLtag[i] << " ";
		cout << endl;
	}
}


void main()  {
	// 显示森林结构
	DisplayTree();
	
	// 带双标记层次次序构造算法，图6.5(a)所示的森林的带双标记位的层次次序表示为例
	char strInfo[N] = {'A', 'G', 'B', 'C', 'D', 'H', 'I', 'E', 'F', 'J'};
	int nRtag[N] = {0, 1, 0, 0, 1, 0, 1, 0, 1, 1}; 
	int nLtag[N] = {0, 0, 1, 0, 1, 0, 1, 1, 1, 1}; 
	
	cout << "Dualtag_Width create tree." << endl;       //打印带双标记位的层次次序表示 
	DisplayNode(strInfo, nRtag, nLtag);
	
	DualTagWidthTreeNode<char> *nodeArray = new DualTagWidthTreeNode<char>[N];
	
	for (int i = 0; i < N; i ++)  {                //设置带双标记位的层次次序结点类 
		nodeArray[i].info = strInfo[i];
		nodeArray[i].rtag = nRtag[i];
		nodeArray[i].ltag = nLtag[i];
	}
	
	Tree<char> aTree(nodeArray, N); // 建树
	Traverse(&aTree);               // 周游树
}
