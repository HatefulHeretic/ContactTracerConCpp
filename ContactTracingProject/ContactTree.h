#pragma once
#include <list>
#include <string>
#include <queue>
#include <iostream>

using namespace std;
typedef string String;

struct TreeNode {
/*
	typedef string String; //health card id number

public:
*/
	String medicareId;						// unique identifier cannot be duplicated
	int directContacts = 0;					// no. of children a node has, default 0
	int totalCases = 1;						// no. of nodes rooted at this tree node including self
	TreeNode* parentPtr;					// pointer to the parent node
	list<TreeNode*> directContactsPtrList;	// list of pointers to children nodes

	TreeNode(const String& medId) { medicareId = medId; parentPtr = nullptr; };
	TreeNode(TreeNode* parent, const String& medId) { medicareId = medId; parentPtr = parent; }; //Changed parentPtr to parent from template
};

class ContactTree
{
	//typedef string String; //health card id number

private:
	TreeNode* rootNode;								//Root node of the contact tree	
	void DeleteContactTree(int&, TreeNode*);		//Deletes trees recursively

public:
	ContactTree();		//Constructor
	~ContactTree();		//Destructor
	bool IsEmpty();		//Returns true if the tree is empty
	int GetSize();		//Returns the number of nodes in the tree including root	
	void AddPatient0(const String&);				//Patient0 becomes the root of the tree
	void AddContact(const String&, const String&);	//Second Parameter is the child of the first
	TreeNode* LookUpContact(const String&);			//Returns a pointer to the node if found, nullptr otherwise
	void DeleteContact(const String&);				//Performs deleting duties and cleanup	
	void DisplayContact(const String&);				//Displays the nide's content given an ID
	void DisplayContact(TreeNode*);					//Displays the node's content given a pointer to the node
	void TraceSource(const String&);				//Displays all the nodes from tis node till the root
	void PrintContactCases(const String&);			//Print direct children contacts, given a contact
	void PrintContactTree();						//An iterative Breadth First Traversal
	void PrintHierarchicalTree();					//Hierarchical printout based on recursion
};

ContactTree::ContactTree() {

	this->rootNode = nullptr;
}

ContactTree::~ContactTree() {

	DeleteContact(rootNode->medicareId);
}

bool ContactTree::IsEmpty() {

	//returns true if the tree has no nodes or if the size of the tree is 0
	if (rootNode == nullptr)
		return true;
	else
		return false;
}

int ContactTree::GetSize() {

	//returns the total number of nodes in the tree including the root
	return rootNode->totalCases;
}

void ContactTree::AddPatient0(const String& id) {

	//Creating a new node and having the root node point to this new node
	TreeNode* patient0 = new TreeNode(id);

	rootNode = patient0;
}

void ContactTree::AddContact(const String& id1, const String& id2) {

	//Locating the id1 in our database and if found we create a new node with id2 and add it to direct contacts
	TreeNode* node1 = LookUpContact(id1);

	if (node1 != nullptr) {

		TreeNode* node2 = new TreeNode(node1, id2);			//Use overload to create the new node

		node1->directContactsPtrList.push_front(node2);		//Pushing node2 to the parent's list of contacts
		node1->directContacts++;							//Increment the direct contacts
		
		//*
		deque<TreeNode*> dq;
		dq.push_front(node1);

		while (dq.front() != NULL) {

			dq.front()->totalCases++;				//Increment total cases
			dq.push_front(dq.front()->parentPtr);	//Add parent to queue
			dq.pop_back();							//Remove node after it has been updated
		}
		//*/
	}
	else {

		//If id1 is not found
		cout << "ID: " << id2 << " not found..." << endl;
		return;
	}
}

TreeNode* ContactTree::LookUpContact(const String& id) {

	//If the list of direct contacts is not empty we loop
	if (!IsEmpty()) {

		deque<TreeNode*> dq;

		dq.push_front(rootNode);

		while (!dq.empty()) {

			//If the rootNode is the one we are looking for we return it
			if (dq.front()->medicareId == id)
				return dq.front();
			else {

				//Loop through each element in the list of contacts and push them into the dq
				for (list<TreeNode*>::iterator it = dq.front()->directContactsPtrList.begin(); it != dq.front()->directContactsPtrList.end(); ++it) {

					dq.push_back(*it);
				}

				//Once all the children are added we pop the one at the front and we get a new front
				dq.pop_front();
			}
		}

		//If the node is not found
		cout << "ID: " << id << " not found..." << endl;
	}
	else {
		//If the contact tree is empty
		cout << "The contact list is empty" << endl;
	}

	//If it's not found we return nullptr
	return nullptr;
}

void ContactTree::DeleteContact(const String& id) {

	//Need to use the deque for this
	TreeNode* node1 = LookUpContact(id);

	//If our target isn't valid
	if (node1 == nullptr) {
		//Exit and don't proceed with delete
		return;
	}

	//If the target node has a parent
	if (node1->parentPtr != nullptr) {

		if (node1->parentPtr->directContacts > 0) {
			//Lower the parent's direct contacts by 1
			node1->parentPtr->directContacts--;
		}

		//Remove the target node from its parent's contact list
		node1->parentPtr->directContactsPtrList.remove(node1);
	}

	//Save parent pointer
	TreeNode* node2 = node1->parentPtr;

	//Keep track of number of nodes deleted (to update total cases)
	int totalNodesDeleted = 0;

	cout << "Deleting Node ID: " << id << endl << endl;

	//Recursively delete all child elements in the tree and keep the number of nodes deleted
	DeleteContactTree(totalNodesDeleted, node1);

	//Update ancestors' total cases count
	//*
	while (node2 != nullptr) {

		if(node2->totalCases >= totalNodesDeleted)
			node2->totalCases -= totalNodesDeleted;

		node2 = node2->parentPtr;
	}
	//*/
}

void ContactTree::DeleteContactTree(int& deleted, TreeNode* node) {

	//If not valid
	if (node == nullptr) {
		return;
	}

	//Go through each child node of the passed node
	for (list<TreeNode*>::iterator it = node->directContactsPtrList.begin(); it != node->directContactsPtrList.end(); ++it) {

		//Recursively iterate through the children in post-order traversal and increment nodes deleted count
		DeleteContactTree(deleted, *it);
	}
	cout << "deleting " << node->medicareId << endl; 

	delete node;		//Delete the pointer to the node
	deleted++;			//Increment the number of nodes deleted (to be subtracted from each ancestor node total cases)
}

void ContactTree::DisplayContact(const String& id) {

	TreeNode* node = LookUpContact(id);

	if (node != nullptr)
		DisplayContact(node);
	else
		cout << "Node: " << id << " not found...Cannot display..." << endl;
}

void ContactTree::DisplayContact(TreeNode* node) {

	cout << "----------------------------------------------" << endl;
	cout << "Medicare ID: " << node->medicareId << "     Total Cases: " << node->totalCases << endl;
	cout << "Direct Contacts: " << node->directContacts << endl;
	cout << "----------------------------------------------" << endl;
}

void ContactTree::TraceSource(const String& id) {

	TreeNode* node = LookUpContact(id);

	if (node != nullptr) {

		if (node != rootNode) {

			DisplayContact(node->parentPtr);
		}
	}
	else
		cout << "ID: " << id << " not found..." << endl;
}

void ContactTree::PrintContactCases(const String& id) {

	TreeNode* node = LookUpContact(id);

	if (node != nullptr) {

		cout << "List of Contact Cases : " << endl;
		for (list<TreeNode*>::iterator it = node->directContactsPtrList.begin(); it != node->directContactsPtrList.end(); ++it) {

			DisplayContact(*it);
		}

		cout << endl;
	}
}

void ContactTree::PrintContactTree() {

	//If the list of direct contacts is not empty we loop
	if (!IsEmpty()) {

		deque<TreeNode*> dq;

		dq.push_front(rootNode);

		cout << "The Contact Tree: " << endl;
		while (!dq.empty()) {

			//Loop through each element in the list of contacts and push them into the dq
			for (list<TreeNode*>::iterator it = dq.front()->directContactsPtrList.begin(); it != dq.front()->directContactsPtrList.end(); ++it) {

				dq.push_back(*it);
			}

			//Print the front of our deque using the TreeNode pointer data
			DisplayContact(dq.front());

			//Pop the front element
			dq.pop_front();
		}

		cout << endl;
	}
	else {
		//If the contact tree is empty
		cout << "The contact list is empty" << endl;
	}
}

void ContactTree::PrintHierarchicalTree() {


}



