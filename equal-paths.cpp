#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

bool check_paths(Node* root, int &height){
  //is tree empty
  if(root == nullptr){
    height = -1;
    return true;
  }
  //checks for leaves
  if(root->left == nullptr && root->right == nullptr){
    height = 0;
    return true;
  }

  int left_h = -1;
  int right_h = -1;
  bool left_g = true;
  bool right_g = true;
  if(root->left != nullptr){
    left_g = check_paths(root->left, left_h);
  }
  if(root->right != nullptr){
    right_g = check_paths(root->right, right_h);
  }

  // if invalid
  if(!left_g || !right_g){
    return false;
  }


  if(root->left != nullptr && root->right != nullptr){
    if(left_h != right_h){
      return false;
    }
  }
  //calculates the height
  if(root->left != nullptr){
    height = left_h + 1;
  }else{
    height = right_h + 1;
  }
  return true;
}

bool equalPaths(Node * root)
{
    // Add your code below
    int height = -1;
    return check_paths(root, height);

}

