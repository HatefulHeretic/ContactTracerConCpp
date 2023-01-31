
#include <iostream>
#include "ContactTree.h"

using namespace std;

//---------------------------------------------
//
// Contact Tracing Application, using Breadth First Traversal and Recursion
// 
// Program written by Coleman Ostach and Sean Hall
//
//---------------------------------------------

int main()
{
	ContactTree* tree = new ContactTree();

	cout << tree->IsEmpty() << endl << endl; // Tests if empty

	tree->AddPatient0("SHALL2022"); // Add first contact
	tree->DisplayContact(tree->LookUpContact("SHALL2022")); // Search for contact in tree, then display
	tree->DisplayContact("SHALL2000");
	tree->DisplayContact("SHALL2022");
	
	cout << endl;

	tree->AddContact("SHALL2022", "COSTA2021"); // Add a contact to the tree
	tree->AddContact("SHALL2022", "GGYGA1967");
	tree->AddContact("COSTA2021", "CPERK2005");
	tree->PrintContactTree(); // Print whole tree

	tree->TraceSource("CPERK2005");
	cout << "Total Cases in Contact Tree: " << tree->GetSize() << endl << endl;

	tree->DeleteContact("COSTA2021"); //Deleting Node and it's subtree
	tree->PrintContactTree();
	tree->DisplayContact("COSTA2021");
	tree->DisplayContact("CPERK2005");
}
