#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

//helper function to check if distance from leaves to root is same
//will return -1 if there is a mismatch
int checkDistance(Node* root)
{
  if (root==nullptr)
  {
    return 0;
  }

  if(root->left == nullptr && root->right==nullptr)
  {
    return 0;
  }

  //case if left is not there, only check right side
  if(root->left == nullptr)
  {
    int right_side= checkDistance(root->right);
    if(right_side==-1)
    {
      return -1;
    }
    else
    {
      return right_side+1;
    }
  }

  //case if right is not there, check left side
  if(root->right == nullptr)
  {
    int left_side= checkDistance(root->left);
    if(left_side==-1)
    {
      return -1;
    }
    else
    {
      return left_side+1;
    }
  }

  //case where both have children
  int left_side= checkDistance(root->left);
  int right_side= checkDistance(root->right);
  if (left_side ==-1 || right_side ==-1)
  {
    return -1;
  }

  if(left_side != right_side)
  {
    return -1;
  }

  return right_side+1;
}

bool equalPaths(Node * root)
{
    // Add your code below

    //tree is empty
    if(root == nullptr)
    {
      return true;
    }

    int depth = checkDistance(root);
    if(depth != -1)
    {
      return true;
    }
    else
    {
      return false;
    }
}

