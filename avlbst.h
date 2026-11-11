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
    void rotateLeft(AVLNode<Key,Value>* node);
    void rotateRight(AVLNode<Key,Value>* node);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if(this->root_ == nullptr){
      this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
      return;
    }
    AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parent = nullptr;

    while(temp != nullptr){
      parent = temp;
      if(new_item.first < temp->getKey()){
        //go left
        temp= temp->getLeft();
      
      }
      else if(new_item.first > temp->getKey()){
        //go right
        temp= temp->getRight();

      }else{
        temp->setValue(new_item.second);
        return;
      }
    } 
    AVLNode<Key, Value>* newnode = new AVLNode<Key, Value>(new_item.first, 
                                      new_item.second, 
                                      parent);
    if(new_item.first < parent->getKey()){
      parent->setLeft(newnode);
    }else{
      parent->setRight(newnode);
    }


  AVLNode<Key, Value>* child = newnode;
  AVLNode<Key, Value>* parent_node = child->getParent();

  while(parent_node != nullptr){
    if(child == parent_node->getLeft()){
      parent_node->updateBalance(1);
    }else{
      parent_node->updateBalance(-1);
    }

    int balance = parent_node->getBalance();

    if(balance == 0){
      break;
    }

    if(balance == -1 || parent_node->getBalance() == 1){
      child = parent_node;
      parent_node = parent_node->getParent();
    }else{
      if(balance == 2){
        AVLNode<Key, Value>* l_child = parent_node->getLeft();
        if(l_child->getBalance() >= 0){
          rotateRight(parent_node);
        }else{
          rotateLeft(l_child);
          rotateRight(parent_node);
        }
      }else if(balance == -2){
        AVLNode<Key, Value>* r_child = parent_node->getRight();
        if(r_child->getBalance() <= 0){
          rotateLeft(parent_node);
        }else{
          rotateRight(r_child);
          rotateLeft(parent_node);
        }
      }
      break;
    }
  }


}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* node){
  AVLNode<Key, Value>* r_child = node->getRight();
  node->setRight(r_child->getLeft());
  if(r_child->getLeft() != nullptr){
    r_child->getLeft()->setParent(node);
  }

  r_child->setParent(node->getParent());
  if(node->getParent() == nullptr){
    this->root_ = r_child;
  }else if(node == node->getParent()->getLeft()){
    node->getParent()->setLeft(r_child);
  }else{
    node->getParent()->setRight(r_child);
  }

  //node under right child 
  r_child->setLeft(node);
  node->setParent(r_child);

  int8_t old_node_balance = node->getBalance();
  int8_t oldRightBal = r_child->getBalance();
  int8_t newBalance = old_node_balance + 1 - std::min((int)oldRightBal, 0);
  int8_t newRightBal = oldRightBal + 1 + std::max((int)newBalance, 0);

  node->setBalance(newBalance);
  r_child->setBalance(newRightBal);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* node){
  AVLNode<Key, Value>* l_child = node->getLeft();

  node->setLeft(l_child->getRight());

  if(l_child->getRight() != nullptr){
    l_child->getRight()->setParent(node);
  }

  l_child->setParent(node->getParent());

  if(node->getParent() == nullptr){
    this->root_ = l_child;
  }else if(node == node->getParent()->getLeft()){
    node->getParent()->setLeft(l_child);
  }else{
    node->getParent()->setRight(l_child);
  }

  //node under left child 
  l_child->setRight(node);
  node->setParent(l_child);

  int8_t old_node_balance = node->getBalance();
  int8_t oldLeftBal = l_child->getBalance();
  int8_t newBalance = old_node_balance - 1 - std::max((int)oldLeftBal, 0);
  int8_t newLeftBal = oldLeftBal - 1 + std::min((int)newBalance, 0);

  node->setBalance(newBalance);
  l_child->setBalance(newLeftBal);

}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
  AVLNode<Key, Value>* node = 
  static_cast<AVLNode<Key, Value>*>(this->internalFind(key));

  if(node == nullptr){
    return;
  }

  if(node->getLeft() != nullptr && node->getRight() != nullptr){
    AVLNode<Key, Value>* pred = 
    static_cast<AVLNode<Key, Value>*>(this->predecessor(node));
    this->nodeSwap(node, pred);
  }

  AVLNode<Key, Value>* parent = node->getParent();

  bool was_l_child = false;
  if(parent != nullptr){
    was_l_child = (node == parent->getLeft());
  }
  
  AVLNode<Key, Value>* child = nullptr;
  
  if(node->getLeft() != nullptr){
    child = node->getLeft();
  }else{
    child = node->getRight();
  }

  if(child != nullptr){
    child->setParent(parent);
  }
  if(parent == nullptr){
    this->root_ = child;
  }else if(was_l_child){
    parent->setLeft(child);
  }else{
    parent->setRight(child);
  }
  delete node;


  AVLNode<Key, Value>* curr = parent;
  while(curr != nullptr){
    if(was_l_child){
      curr->updateBalance(-1);
    }else{
      curr->updateBalance(1);
    }

    int balance = curr->getBalance();

    if(balance == -1 || balance == 1){
      break;
    }


    AVLNode<Key, Value>* old_parent = curr->getParent();
    bool currLeft = false;
    if(old_parent != nullptr){
      currLeft = (old_parent->getLeft() == curr);
    }

    AVLNode<Key, Value>* l_child = curr->getLeft();
    AVLNode<Key, Value>* r_child = curr->getRight();

    if(balance == 2){
      int childBalance = l_child->getBalance();
      if(childBalance >= 0){
        rotateRight(curr);
        if(childBalance == 0){
          break;
        }
      }else{
        rotateLeft(l_child);
        rotateRight(curr);
      }
    }else if(balance == -2){
      int childBal = r_child->getBalance();
      if(childBal <= 0){
        rotateLeft(curr);
        if(childBal == 0){
          break;
        }
      }else{
        rotateRight(r_child);
        rotateLeft(curr);
      }
    }

    //AVLNode<Key, Value>* old_curr = curr;
    curr = old_parent;
    if(curr != nullptr){
      was_l_child = currLeft;
    }
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
