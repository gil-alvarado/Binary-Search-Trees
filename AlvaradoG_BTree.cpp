 /*

 File: AlvaradoG_BTree.cpp
 Compiler: g++


g++ -o sim AlvaradoG_BTree.cpp 

AlvaradoG_BTreeSource.cpp

NOTE: the DELETED NODE is replaced by the INORDER SUCCESSOR

*/
#include <time.h>
#include <iostream>
using std::cin;
using std::cout;
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//                      CLASS BST
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
class BST
{

	private:
		struct node
		{
			int key;
			node *left;
			node *right;
		};
		node *root;
		unsigned int numNodes;

		node* createNewNode(int key);
		node* getNodePrivate(int key, node* p);
		node* addNodePrivate(int key, node* ptr);
		node* deleteNodePrivate(node* root, int key);

        node* getSmallestNode(int key, node* p);

		bool hasChildrenPrivate(node*, int);

		void printParentsPrivate(node*);
		void printInOrderPrivate(node* ptr);
		void printFamily(node*);


		int getSmallestPrivate(node* p);
		void removeSubTree(node* ptr);

		bool searchPrivate(node* , int);
	public:

		BST();
		BST(int*, int);
		~BST();

		void addNode(int key);
		void removeNode(int key);
		void printInOrder();

		int returnRootKey();
		void printParents();
		bool hasChildren(int);
		void printChildren(int key);

		int getNumNodes(){ return numNodes;}//
        bool search(int );

	//bool checkExistence(int);	
};
//non member function
void numberGenerator(int arr[], int SIZE, int RANGE);
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//                      MAIN SIMULATOR
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
int main()
{
	unsigned int SIZE = 20;
	const int RANGE = 100;

	//GENERATE 20 RANDOM NUMBERS FROM
	int treeKeys[SIZE] ;

	cout << "\n\nREQ1: generating numbers...\n";
	numberGenerator(treeKeys, SIZE, RANGE);
	
	cout << "REQ2: adding numbers to tree...";
	BST* tree = new BST(treeKeys, SIZE);//req 2

	cout << "----------------------------------------------\n";
	cout << "REQ3: TREE DETAILS\nROOT NODE:" << tree->returnRootKey() << '\n';//req 3
    //tree representation
	tree->printChildren(tree->returnRootKey());
	tree->printParents();

	cout << "----------------------------------------------\n";
	//tree->removeNode(treeKeys[4]);
	// tree->printInOrder();
	
	cout << "REQ4: ENTER A NON-LEAF NODE TO DELETE:";
	int input;
	do
	{
		cin >> input;
        //ASSUMING USER ENTERS A KEY THAT EXISTS
		if( !(tree->hasChildren(input)) )
			cout <<"INVALID ENTRY, ENTER A NON-LEAF NODE(look at tree details)\n";

	}
	while(!(tree->hasChildren(input)));
	{
		cout << "correct input, deleting parent " << input;
		cout << "\n\nprevious IN ORDER\n";
		tree->printInOrder();
		tree->printChildren(input);
		tree->removeNode(input);
		cout << "\nUPDATED traversal";
		tree->printInOrder();
	}


	cout << "----------------------------------------------\n";
	int randNum = (rand() %RANGE +1);

	cout << "REQ5: INSERT RANDOM NUMBER: " << randNum << '\n';
	
	tree->addNode(randNum);
	tree->printChildren(randNum);
	cout << "\nREQ6: displaying tree via IN ORDER TRAVERSAL\n";
		tree->printInOrder();
    cout << "----------------------------------------------\n";

	return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void numberGenerator(int arr[], int SIZE, int RANGE)
{
	srand (time(NULL));
	for (int i=0;i<SIZE;i++)
		arr[i] = rand() % RANGE +1;
}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//                      BST IMPLEMENTATION
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
BST::BST(int *list, int size)
{
    root = NULL;
    numNodes = 0;//making suree
    cout << "\nORIGINAL SEQUENCE:\n[";
    for(int i = 0; i < size; i++)
    {
        addNode(list[i]);
        cout << list[i] << ",";
    }
    cout << "]\nNum Nodes : " << numNodes  ;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//CONSTRUCTION PROCESS
BST::node* BST::createNewNode(int key)
{
    node* n = new node;
    n->key = key;
    n->left = NULL;
    n->right = NULL;
    return n;
}
//PUBLIC function to add values
void BST::addNode(int key)
{
    root = addNodePrivate(key,root);
    numNodes++;
}
//PRIVATE function to add number(new node) to the tree
BST::node* BST::addNodePrivate(int key, node* ptr)
{
    //check to see if root/current node is null
    //ists either the root, left child or right child
    if(ptr == NULL)
    {
        ptr = createNewNode(key); //create new node
    }
    else if(key < ptr->key)//add to left subtree
    {
        ptr->left = addNodePrivate(key,ptr->left);
    }

    else if(key >= ptr->key)///add to right subtree
    {
        ptr->right = addNodePrivate(key,ptr->right);
    }

    return ptr;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//user friendly 
void BST::printInOrder()
{
    cout << "[INORDER TRAVERSAL]\n[";
    printInOrderPrivate(root);
    cout << ']';
    cout << "\nnumNodes: " << numNodes << '\n';
}
//privation BST operation
void BST::printInOrderPrivate(node* root)
{
    if(root != NULL)
    {
        if(root->left != NULL)
            printInOrderPrivate(root->left);
        cout << root->key << ',';
        if(root->right != NULL)
            printInOrderPrivate(root->right);
    }
    else
        cout << "The tree is empty.";
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void BST::printParents()
{
    //first printing NON-LEAF NODES
    cout << "\nPARENT NODES\n[";
    printParentsPrivate(root);
    cout << "]\n(and their children)";
    printFamily(root);
}

//PRINTING PARENT NODES ONLY
void BST::printParentsPrivate(node* root)
{
    if(root != NULL)
    {
        //if(root->left != NULL || root->right !=NULL)
        if(hasChildrenPrivate(root, root->key))
        {
            cout << root->key << ',';
            printParentsPrivate(root->left);
            printParentsPrivate(root->right);
        }

    }
}
//PARENT NODES and their children
void BST::printFamily(node* root)
{
    if(root != NULL)
    {
        // if(hasChildrenPrivate(root, root->key))
        if(root->left != NULL || root->right !=NULL)
        {
            printChildren(root->key);
            printFamily(root->left);
            printFamily(root->right);
        }

    }
}
//printing nodes and the value of their left/right children 
void BST::printChildren(int key)
{
    node * ptr = getNodePrivate(key,root);

    if(ptr !=NULL)
    {
        if(hasChildrenPrivate(ptr, ptr->key))
        {
            cout << "\nParent Node: " << ptr->key << std::endl;

            ptr->left == NULL ?
            cout << "Left child = NULL \n":
            cout << "Left child = " << ptr->left->key << std::endl;

            ptr->right == NULL ?
            cout << "Right child = NULL \n":
            cout << "Right child = " << ptr->right->key << std::endl;
        }
        else//no children
        {
            cout << "\nLEAF Node: " << ptr->key << std::endl;
            cout << "NO CHILDREN\n";
        }
    }
    else
        cout << "Key " << key << " is not in the tree.\n";
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//user function
bool BST::hasChildren(int key){
    return hasChildrenPrivate(root, key);
}
//private BST operation
bool BST::hasChildrenPrivate(node *root, int key)//
{
    //works with the public hasChildren
    node* parent = getNodePrivate(key, root);

    return (parent->left != NULL || parent->right != NULL);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//GET root/first value of the BST
int BST::returnRootKey()
{
    if(root!=NULL)
        return root->key;
    else
        return -1;
}
//get node* in relation to the value of the key
BST::node* BST::getNodePrivate(int key, node* node)
{
    if(node != NULL)
    {
        if(node->key == key)//match
            return node;      
        else//traverse throught BST until the match is found
        {
            if(key < node->key)
                return getNodePrivate(key, node->left);
            else//>
                return getNodePrivate(key,node->right);
        }
    }
    else//if no match is found, return null
        return node;
}
//traverse through tree until the furthest left (smallest) node is reached
//and return its key
int BST::getSmallestPrivate(node* n)
{
    return n->left == NULL ? n->key : getSmallestPrivate(n->left);
}
//return the smallest node (both methods work)
BST::node* BST::getSmallestNode(int key, node* n)
{
    return n->left == NULL ? n : getSmallestNode(key,n->left);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//user friendly 
void BST::removeNode(int key)
{
    root = deleteNodePrivate(root, key);
}
//private BST operation
BST::node* BST::deleteNodePrivate(node* parent, int key)
{
    if(parent == NULL)//empty tree
        return parent;
    else if(key < parent->key)
        parent->left = deleteNodePrivate(parent->left, key);
    else if(key > parent->key)
        parent->right = deleteNodePrivate(parent->right, key);
    else//THINK OF THE children()
    {
        //CASES FOR CHILDREN: 0 children, 1 child, 2 children

        //0 CHILDREN
        if((parent->left == NULL && parent->right ==NULL))
        {
            delete parent; 
            parent = NULL;
            numNodes--;
        }
        //ONE right CHILD
        else if(parent->left == NULL && parent->right !=NULL)//has right child
        {
                node* temp = parent;
                parent = parent->right;
                delete temp;
                numNodes--;
        }
        //ONE left CHILD
        else if(parent->left !=NULL && parent->right == NULL)//has left child
        {
                node* temp = parent;
                parent = parent->left;
                delete temp;
                numNodes--;
        }
        //TWO CHILDREN
        else
        {
            //IN ORDER SUCCESSOR
            //int SUCCESSOR =  getSmallestPrivate(parent->right);//get smallest key
            node* SUCCESSOR = getSmallestNode(key, parent->right);
            parent->key = SUCCESSOR->key;//have duplicate values
            parent->right = deleteNodePrivate(parent->right,SUCCESSOR->key);//and delete the duplucate
        }    
    }
    
    //maintain tree reference
    return parent;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
bool BST::search(int key){
    return searchPrivate(root, key);
}
bool BST::searchPrivate(node* root, int key){
    if(root == NULL)
        return false;
    if(root->key == key)
            return true;
    if(root->key > key)
        return searchPrivate(root->left, key);
    return searchPrivate(root->right, key);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
BST::~BST()//dynamically delete memory
{
    //removeSubTree(root);
}
//not used
void BST::removeSubTree(node* ptr)
{
    if(ptr != NULL)
    {
        if(ptr->left != NULL)
        {
            removeSubTree(ptr->left);
        }
        if(ptr->right != NULL)
        {
            removeSubTree(ptr->right);
        }
        cout << "Deleting the node containing key: " << ptr->key << std::endl;
        delete ptr;
    }
}
