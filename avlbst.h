#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    AVLNode<Key, Value>* leftRotate(AVLNode<Key, Value>* node);
    AVLNode<Key, Value>* rightRotate(AVLNode<Key, Value>* node);
    AVLNode<Key, Value>* checkNodebalance(AVLNode<Key, Value>* node);
    AVLNode<Key, Value>* recursiveInsert(AVLNode<Key, Value>* node, const std::pair<const Key, Value>& new_item, AVLNode<Key, Value>* parent_node, bool &height_change);

};

//helper function to fix right heavy imbalance
//takes the right child of current node
//then makes the left subtree of the child become the new right subtree of current node
//will the update parent pointers so current node becomes left child 
//and lastly reset balance factors of involved nodes
//returns new root of rotated subtree
template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::leftRotate(AVLNode<Key, Value>* x)
{
  if(x == nullptr || x->getRight() == nullptr)
  {
    return x;
  }
  AVLNode<Key, Value>* y = x->getRight();
  x->setRight(y->getLeft());
  if(y->getLeft() != nullptr)
  {
    y->getLeft()->setParent(x);
  }
  y->setLeft(x);
  y->setParent(x->getParent());
  x->setParent(y);

  AVLNode<Key, Value>* grandparent = y->getParent();
  if(grandparent != nullptr)
  {
    if(grandparent->getLeft() == x)
    {
      grandparent->setLeft(y);
    }
    else if(grandparent->getRight() == x)
    {
      grandparent->setRight(y);
    }
  }

  // Update balance factors using standard formulas.
  x->setBalance(x->getBalance() + 1 - std::min(y->getBalance(), (int8_t)0));
  y->setBalance(y->getBalance() + 1 + std::max(x->getBalance(), (int8_t)0));
  // x->setBalance(0);
  // y->setBalance(0);
  return y;
}

//helper function to fix left heavy imbalance
//takes the left child of current node
//then makes the right subtree of the child become the new left subtree of current node
//will the update parent pointers so current node becomes right child 
//and lastly reset balance factors of involved nodes
//returns new root of rotated subtree
template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rightRotate(AVLNode<Key, Value>* x)
{
  AVLNode<Key, Value>* y = x->getLeft();
  x->setLeft(y->getRight());
  if(y->getRight() != nullptr)
  {
    y->getRight()->setParent(x);
  }
  y->setRight(x);
  y->setParent(x->getParent());
  x->setParent(y);

  AVLNode<Key, Value>* grandparent = y->getParent();
  if(grandparent != nullptr)
  {
    if(grandparent->getLeft() == x)
    {
      grandparent->setLeft(y);
    }
    else if(grandparent->getRight() == x)
    {
      grandparent->setRight(y);
    }
  }

  // Update balance factors using standard formulas.
  x->setBalance(x->getBalance() - 1 - std::max(y->getBalance(), (int8_t)0));
  y->setBalance(y->getBalance() - 1 + std::min(x->getBalance(), (int8_t)0));
  // x->setBalance(0);
  // y->setBalance(0);

  return y;
}
//helper function to check if the subtree at given node meets AVL requirement
//basically checking if balance fator is between -1 and 1 
//first will check for balance factor of the node
//if balance>1 (left heavy node), perform either a single right rotation or
//first rotate left child to left, and then node to right
//if balance<-1 (right heavy node), perform opposite of left heavy rotations
//return new root of subtree
template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::checkNodebalance(AVLNode<Key, Value>* node)
{
  if(node->getBalance() > 1)
  {
    if(node->getLeft() != nullptr)
    {
      if(node->getLeft()->getBalance() < 0)
      {
        node->setLeft(leftRotate(node->getLeft()));
        if(node->getLeft() != nullptr)
        {
          node->getLeft()->setParent(node);
        }
      }
    }
    node=rightRotate(node);
    return node;
  }

  if(node->getBalance() < -1)
  {
    if(node->getRight() !=nullptr)
    {
      if(node->getRight()->getBalance() > 0)
      {
        node->setRight(rightRotate(node->getRight()));
        if(node->getRight() != nullptr)
        {
          node->getRight()->setParent(node);
        }
      }
    }
    node=leftRotate(node);
    return node;
  }

  return node;
}

//recursive helper function for inserting a new key-value pair
//if current node is NULL, I'll create a new AVLNode and change height_change flag
//if key to insert matches current node's key, I will updae node's value and keep flag value same
//now to recursively implement, if new key is less than current node's key, recursion is on left subtree, otherwise right
// after recursion, balance factor is updated 
// balance factor increases by 1 if inserted on left 
//balance factor reduces by 1 if inserted on right
//call checkNodebalance if there is imbalance after 
//return new root of subtree
template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::recursiveInsert(AVLNode<Key, Value>* node, const std::pair<const Key, Value>& new_item, AVLNode<Key, Value>* parent_node, bool &height_change)
{
  if(node == nullptr)
  {
    AVLNode<Key, Value>* inserted_node = new AVLNode<Key, Value>(new_item.first, new_item.second, parent_node);
    height_change=true;
    return inserted_node;
  }
  if(new_item.first == node->getKey())
  {
    node->setValue(new_item.second);
    height_change=false;
    return node;
  }

  bool child_flag = false;
  if(new_item.first < node->getKey())
  {
    AVLNode<Key, Value>* left_side = recursiveInsert(static_cast<AVLNode<Key, Value>*>(node->getLeft()), new_item, node, child_flag);
    node->setLeft(left_side);
    if(left_side != nullptr)
    {
      left_side->setParent(node);
    }
    if(child_flag==true)
    {
      node->updateBalance(1);
      if(node->getBalance() == 0)
      {
        height_change=false;
      }
      else if (node->getBalance() == 1)
      {
        height_change=true;
      }
      else if(node->getBalance() == 2)
      {
        height_change=false;
        node = checkNodebalance(node);
      }
      else
      {
        height_change=false;
      }
    }
    else 
    {
      height_change=false;
    }
  }
  else
  {
    AVLNode<Key, Value>* right_side = recursiveInsert(static_cast<AVLNode<Key, Value>*>(node->getRight()), new_item, node, child_flag);
    node->setRight(right_side);
    if(right_side != nullptr)
    {
      right_side->setParent(node);
    }
    if(child_flag==true)
    {
      node->updateBalance(-1);
      if(node->getBalance() == 0)
      {
        height_change=false;
      }
      else if (node->getBalance() == -1)
      {
        height_change=true;
      }
      else if(node->getBalance() == -2)
      {
        height_change=false;
        node = checkNodebalance(node);
      }
      else
      {
        height_change=false;
      }
    }
    else 
    {
      height_change=false;
    }
  }
  return node;
}


/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    AVLNode<Key, Value>* temp_root = static_cast<AVLNode<Key, Value>*>(this->root_);
    bool height_changed = false;
    temp_root = recursiveInsert(temp_root, new_item, 0, height_changed);
    this->root_ = temp_root;
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO

    AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if(!node)
    {
      return;
    }

    if(node->getLeft() != nullptr && node->getRight() !=nullptr)
    {
      AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(node));
      nodeSwap(node, pred);
    }

    AVLNode<Key, Value>* child = (node->getLeft() != nullptr) ? node->getLeft() : node->getRight();
    AVLNode<Key, Value>* parent = node->getParent();
    bool left_remove = false;

    if(parent)
    {
      left_remove = (parent->getLeft() == node);
    }
    if(!parent){
      this->root_=child;
      if(child)
      {
        child->setParent(nullptr);
      }
      delete node;
      return;
    }
    if(left_remove){
      parent->setLeft(child);
    }
    else
    {
      parent->setRight(child);
    }
    if(child)
    {
      child->setParent(parent);
    }
    delete node;

    AVLNode<Key, Value>* current = parent;

    while (current)
    {
      if(left_remove)
      {
        current->updateBalance(-1);
      }
      else
      {
        current->updateBalance(1);
      }

      int bal = current->getBalance();

      if(bal==1||bal==-1)
      {
        break;
      }
      if(bal == 0)
      {
        left_remove = (current->getParent() && (current == current->getParent()->getLeft()));
        current = current->getParent();
        continue;
      }

      if(bal==2 || bal==-2)
      {
        current = checkNodebalance(current);
        if(current->getParent() == nullptr){
          this->root_ = current;
          break;
        }

        if(current->getBalance()!=0){
          break;
        }
      }
      left_remove = (current->getParent() && (current == current->getParent()->getLeft()));
      current = current->getParent();
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
