//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
// James Yim
// Cruz ID: jnyim
// #1798845
// CSE 101-01 Tantalo
// Programming Assignment 7
// Dictionary.cpp
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------

#include <string>
#include <iostream>
#include <stdexcept>
#include "Dictionary.h"

//---= Definitions =---

#define RED -88
#define BLACK -28

//---= Constructors and Destructors =---

//Private Node Constructor
Dictionary::Node::Node(keyType k, valType v)
{
	key = k;
	val = v;
	parent = nullptr;
	left = nullptr; 
	right = nullptr;
	color = BLACK;
}

//Constructing the Dictionary
Dictionary::Dictionary()
{
	nil = new Node("qazwsxedcrfvtgbyhnujmik", -53874);
	root = nil;
	current = nil;
	num_pairs = 0;
}

//Copying a Dictionary
Dictionary::Dictionary(const Dictionary& d)
{
	num_pairs = 0;
	nil = new Node("qazwsxedcrfvtgbyhnujmik", -53874);
	root = nil;
	preOrderCopy(d.root, d.nil);

}

//Deleting a Dictionary
Dictionary::~Dictionary()
{
	clear();
	delete nil;
}

//---= Helper Functions =---

//inOrderString()
void Dictionary::inOrderString(std::string& s, Node* r) const
{
	if (r != nil)
	{
		inOrderString(s, r->left);
		s += r->key; 
	       	s += " : ";
		s += std::to_string(r->val);
		s += "\n";
		inOrderString(s, r->right);
	}
}
//preOrderString()
void Dictionary::preOrderString(std::string& s, Node* r) const
{
	if (r != nil)
	{
		s += r->key;
		s += "\n";
		preOrderString(s, r->left);
		preOrderString(s, r->right);
	}
}

//search()
Dictionary::Node* Dictionary::search(Node* r, keyType k) const
{

	if (r == nil || r->key == k)
	{
		return r;
	}
	else if (k < r->key)
	{
		return search(r->left, k);
	}
	else //(k > r->key)
	{
		return search(r->right, k);
	}
}

//preOrderCopy()
void Dictionary::preOrderCopy(Node* r, Node* n)
{
	if (r != n)
	{
		this->setValue(r->key, r->val);
		preOrderCopy(r->left, n);
		preOrderCopy(r->right, n);
	}
}


//postOrderDelete()
void Dictionary::postOrderDelete(Node* r)
{
	if (r != nil)
	{
		postOrderDelete(r->left);
		postOrderDelete(r->right);
		num_pairs--;
		r = nil;
	}
}

//findMin()
Dictionary::Node* Dictionary::findMin(Node* r)
{
	while (r->left != nil)
	{
		r = r->left;
	}
	return r;
}

//findMax()
Dictionary::Node* Dictionary::findMax(Node* r)
{
	while (r->right != nil)
	{
		r = r->right;
	}
	return r;
}

//findNext()
Dictionary::Node* Dictionary::findNext(Node* n)
{
	if (n == findMax(root) || n == nil)
	{
		return nil;
	}
	if (n->right != nil)
	{
		return findMin(n->right);
	}
	Dictionary::Node* holderNode = n->parent;
	while (holderNode != nil && n == holderNode->right)
	{
		n = holderNode;
		holderNode = holderNode->parent;
	}
	return holderNode;
}

//findprev()
Dictionary::Node* Dictionary::findPrev(Node* n)
{
	if (n == findMin(root) || n == nil)
	{
		return nil;
	}
	if (n->left != nil)
	{
		return findMax(n->left);
	}
	Dictionary::Node* holderNode = n->parent;
	while (holderNode != nil && n == holderNode->left)
	{
		n = holderNode;
		holderNode = holderNode->parent;
	}
	return holderNode;
}

//---= Accesor Functions =---

//Getting the size of Dictionary
int Dictionary::size() const
{
	return num_pairs;
}
	
//Checking if there is a key k
bool Dictionary::contains(keyType k) const
{
	if (search(root, k) == nil)
	{
		return false;
	}
	return true;
}

//Getting a value corresponding to k
valType& Dictionary::getValue(keyType k) const
{
	if (!contains(k))
	{
		throw std::logic_error("[ADT: Dictionary] [Function: getValue()] Reason: inputted key is not in the dictionary.");
	}
	Dictionary::Node* currentNode = search(root, k);
	return currentNode->val;
}

//Checking if the current iterator is defined
bool Dictionary::hasCurrent() const
{
	if (current != nil)
	{
		return true;
	}
	return false;
}

//Returning the current key
keyType Dictionary::currentKey() const
{
	
	if (hasCurrent() == false)
	{
		throw std::logic_error("[ADT: Dictionary] [Function: currentKey()] Reason: current is undefined");
	}
	
	return current->key;
}

//Returning the current value
valType& Dictionary::currentVal() const
{
	if (!(hasCurrent()))
	{
		throw std::logic_error("[ADT: Dictonary] [Function: currentVal()] Reason: Current is undefined");
	}
	return current->val;
}

/*
//Transplanting a subtree
void Dictionary::Transplant(Node* u, Node* v)
{
	if (u->parent == nil)
	{
		root = v;
	}
	else if (u == u->parent->left)
	{
		u->parent->left = v;
	}
	else
	{
		u->parent->right = v;
	}
	if (v != nil)
	{
		v->parent = u->parent;
	}
}
*/
//---= Red Black Tree Helper Functions =---

//LeftRotate()
void Dictionary::LeftRotate(Node* n)
{
	//Setting y
	Dictionary::Node* y = n->right;

	//Turning y's left subtree into n's subtree
	n->right = y->left;
	if (y->left != nil)
	{
		y->left->parent = n;
	}

	//Linking y's parent to x
	y->parent = n->parent;
	if (n->parent == nil)
	{
		root = y;
	}
	else if (n == n->parent->left)
	{
		n->parent->left = y;

	}
	else
	{
		n->parent->right = y;
	}

	//putting x on y's left
	y->left = n;
	n->parent = y;
}

//RightRotate()
void Dictionary::RightRotate(Node* n)
{
	// set y
   	Node* y = n->left;

   	// turn y's right subtree into x's left subtree
   	n->left = y->right;
   	if (y->right != nil)  // not necessary if using sentinal nil node
	{
		y->right->parent = n;
	}

   	// link y's parent to x
   	y->parent = n->parent;
   	if (n->parent == nil)
	{
		root = y;
	}
   	else if (n == n->parent->right)
	{
		n->parent->right = y;
	}
   	else
	{
      		n->parent->left = y;
	}

   	// put x on y's right
   	y->right = n;
   	n->parent = y;
}

//RB Insert Fix Up
void Dictionary::RB_InsertFixUp(Node* n)
{
	while (n->parent->color == RED)
	{
		if (n->parent == n->parent->parent->left)
		{
			Node* y = n->parent->parent->right;
			if (y->color == RED)
			{
				n->parent->color = BLACK;
				y->color = BLACK;
				n->parent->parent->color = RED;
				n = n->parent->parent;
			}
			else
			{
				if (n == n->parent->right)
				{
					n = n->parent;
					LeftRotate(n);
				}
				n->parent->color = BLACK;
				n->parent->parent->color = RED;
				RightRotate(n->parent->parent);
			}
		}
		else
		{
			Node* y = n->parent->parent->left;
                        if (y->color == RED)
                        {
                                n->parent->color = BLACK;
                                y->color = BLACK;
                                n->parent->parent->color = RED;
                                n = n->parent->parent;
                        }
                        else
                        {
                                if (n == n->parent->left)
                                {
                                        n = n->parent;
					RightRotate(n);
                                }
                                n->parent->color = BLACK;
                                n->parent->parent->color = RED;
                                LeftRotate(n->parent->parent);
                        }
		}
	}
	root->color = BLACK;
}

//RB_Transplant
void Dictionary::RB_Transplant(Node* u, Node* v)
{
	if (u->parent == nil)
	{
      		root = v;
	}
   	else if (u == u->parent->left)
	{	
      		u->parent->left = v;
	}
   	else
	{	
      		u->parent->right = v;
	}
   	v->parent = u->parent;
}

//RB_DeleteFixUp()
void Dictionary::RB_DeleteFixUp(Node* x)
{
	while (x != root && x->color == BLACK)
	{
      		if (x == x->parent->left)
		{
         		Node* w = x->parent->right;	
         		if (w->color == RED)
			{
            			w->color = BLACK;                       
            			x->parent->color = RED;              
            			LeftRotate(x->parent);                
            			w = x->parent->right;
			}			
         		if (w->left->color == BLACK && w->right->color == BLACK)
			{
            			w->color = RED;                          
            			x = x->parent; 
			}		
         		else
			{
            			if (w->right->color == BLACK)
				{
               				w->left->color = BLACK;            
               				w->color = RED;  
               				RightRotate(w);                  
               				w = x->parent->right; 
	       			}				
            			w->color = x->parent->color;       
            			x->parent->color = BLACK;           
            			w->right->color = BLACK;       
            			LeftRotate(x->parent);                
            			x = root;   
			}
		}		
      		else
		{
         		Node* w = x->parent->left;
         		if (w->color == RED)
			{
            			w->color = BLACK;                      
            			x->parent->color = RED;
            			RightRotate(x->parent);              
            			w = x->parent->left;
			}			
         		if (w->right->color == BLACK && w->left->color == BLACK)
			{
            			w->color = RED;                          
            			x = x->parent;
			}
					
         		else
			{
            			if (w->left->color == BLACK)
				{
               				w->right->color = BLACK;              
               				w->color = RED;
               				LeftRotate(w);                    
               				w = x->parent->left; 
				}			
            		w->color = x->parent->color;
            		x->parent->color = BLACK;                 
            		w->left->color = BLACK;
            		RightRotate(x->parent);               
            		x = root;
			}
		}
	}		
   x->color = BLACK;
}

//RB_Delete()
void Dictionary::RB_Delete(Node* z)
{
	Node* y = z;
	int yOgColor = y->color;
	Node* x;
	if (z->left == nil)
	{
		x = z->right;
		RB_Transplant(z, z->right);
	}
	else if (z->right == nil)
	{
		x = z->left;
		RB_Transplant(z, z->left);
	}
	else
	{
		y = findMin(z->right);
		yOgColor = y->color;
		x = y->right;
		if (y->parent == z)
		{
			x->parent = y;
		}
		else
		{
			RB_Transplant(y, y->right);
         		y->right = z->right;
         		y->right->parent = y;
		}
		RB_Transplant(z, y);
      		y->left = z->left;
      		y->left->parent = y;
      		y->color = z->color;
	}
	if (yOgColor == BLACK)
	{
		RB_DeleteFixUp(x);
	}
}


//--= Manipulation Functions =---

//Clearing the Dictionary
void Dictionary::clear()
{

	postOrderDelete(root);
	num_pairs = 0;
	current = nil;
	root = nil;
}

//Setting the value of a key
void Dictionary::setValue(keyType k, valType v)
{
	Node* newNode = new Node(k, v);
	newNode->left = nil;
	newNode->right = nil;
	Node* holderNode = nil;
	Node* currentNode = root;
	while (currentNode != nil)
	{
		holderNode = currentNode;
		if (newNode->key == currentNode->key)
		{
			currentNode->val = v;
			return;
		}
		else if (newNode->key < currentNode->key)
		{
			currentNode = currentNode->left;
		}
		else
		{
			currentNode = currentNode->right;
		}
	}
	newNode->parent = holderNode;
	if (holderNode == nil)
	{
		root = newNode;
	}
	else if (newNode->key < holderNode->key)
	{
		holderNode->left = newNode;
	}
	else
	{
		holderNode->right = newNode;
	}
	newNode->color = RED;
	RB_InsertFixUp(newNode);
	num_pairs++;
}

//Removing a node
void Dictionary::remove(keyType k)
{
	if (!contains(k))
	{
		throw std::logic_error("[ADT: Dictionary] [Function: remove()] Reason: The key is not in the tree");
	}
	Node* delNode = search(root, k);
	if (delNode == current)
	{
		current = nil;
	}
	Node* y = delNode;
	int yOgColor = y->color;
	Node* x;
	if (delNode->left == nil)
	{
		x = delNode->right;
		RB_Transplant(delNode, delNode->right);
	}
	else if (delNode->right == nil)
	{
		x = delNode->left;
		RB_Transplant(delNode, delNode->left);
	}
	else
	{
		y = findMin(delNode->right);
		yOgColor = y->color;
		x = y->right;
		if (y->parent == delNode)
		{
			x->parent = y;
		}
		else
		{
			RB_Transplant(y, y->right);
			y->right = delNode->right;
			y->right->parent = y;
		}
		RB_Transplant(delNode, y);
		y->left = delNode->left;
		y->left->parent = y;
		y->color = delNode->color;
	}
	if (yOgColor == BLACK)
	{
		RB_DeleteFixUp(x);
	}
	delete delNode;
	num_pairs--;
}



//Setting the cursor at the beginning
void Dictionary::begin()
{
	if (num_pairs == 0)
	{
		current = nil;
		return;
	}
	current = findMin(root);
}

//Setting the cursor at the end
void Dictionary::end()
{
	if (num_pairs == 0)
	{
		current = nil;
		return;
	}
	current = findMax(root);
}

//Moving the cursor to the next
void Dictionary::next()
{
	if (!hasCurrent())
	{
		throw std::logic_error("[ADT: Dictionary] [Function: next()] Reason: there is no current");
	}
	current = findNext(current);
}

//Moving the cursor to the prev
void Dictionary::prev()
{
	if (!hasCurrent())
	{
		throw std::logic_error("[ADT: Dictionary] [Function: prev()] Reason: There is no current");
	}
	current = findPrev(current);
}

//---= Other Functions =---

//to_string()
std::string Dictionary::to_string() const
{
        std::string s = "";
        inOrderString(s, root);
        return s;
}

//pre_string()
std::string Dictionary::pre_string() const
{
        std::string s = "";
        preOrderString(s, root);
        return s;
}

//equals()
bool Dictionary::equals(const Dictionary& d) const
{
	Dictionary thisDict = *this;
	Dictionary dDict = d;
	if (thisDict.num_pairs != dDict.num_pairs)
	{
		return false;
	}
	thisDict.begin();
	dDict.begin();
	for (int i = 0; i < thisDict.num_pairs; i++)
	{
		if (thisDict.currentKey() != dDict.currentKey() || thisDict.currentVal() != dDict.currentVal())
		{
			return false;
		}
		thisDict.next();
		dDict.next(); //might have to delete?
	}
	return true;
}

//--= Overloaded Operators =---

//operator<<()
std::ostream& operator<< (std::ostream& stream, Dictionary& d)
{
	return stream << d.Dictionary::to_string();
}

//operator==()
bool operator==(const Dictionary& a, const Dictionary& b)
{
	return a.Dictionary::equals(b);
}

//operator=()
Dictionary& Dictionary::operator=(const Dictionary& d)
{
	if (this != &d)
	{
		Dictionary temp = d;
		std::swap(root, temp.root);
		std::swap(nil, temp.nil);
		std::swap(current, temp.current);
		std::swap(num_pairs, temp.num_pairs);
	}
	return *this;
}


