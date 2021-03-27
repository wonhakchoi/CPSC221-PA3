/**
 *
 * shifty quadtree (pa3)
 * sqtree.cpp
 * This file will be used for grading.
 *
 */

#include "sqtree.h"

// First Node constructor, given.
SQtree::Node::Node(pair<int,int> ul, int w, int h, RGBAPixel a, double v)
  :upLeft(ul),width(w),height(h),avg(a),var(v),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
{}

// Second Node constructor, given
SQtree::Node::Node(stats & s, pair<int,int> ul, int w, int h)
  :upLeft(ul),width(w),height(h),NW(NULL),NE(NULL),SE(NULL),SW(NULL) {
  avg = s.getAvg(ul,w,h);
  var = s.getVar(ul,w,h);
}

// SQtree destructor, given.
SQtree::~SQtree() {
  clear();
}

// SQtree copy constructor, given.
SQtree::SQtree(const SQtree & other) {
  copy(other);
}

// SQtree assignment operator, given.
SQtree & SQtree::operator=(const SQtree & rhs) {
  if (this != &rhs) {
    clear();
    copy(rhs);
  }
  return *this;
}

/**
 * SQtree constructor given tolerance for variance.
 */
SQtree::SQtree(PNG & imIn, double tol) {
  // Your code here.
  stats s = stats(imIn);
  pair<int, int> ul = make_pair(0,0);
  root = buildTree(s, ul, imIn.width(), imIn.height(), tol);
}


/**
 * Helper for the SQtree constructor.
 */
SQtree::Node * SQtree::buildTree(stats & s, pair<int,int> & ul,
				 int w, int h, double tol) {

  // Your code here.
  //create node for current child
  Node *curr = new Node(ul, w, h, s.getAvg(ul, w, h), s.getVar(ul, w, h));

  //base case: check if the average colour of pixel is at most, prescribed
  //tolerance, stop splitting if equal or more than tolerance
  //or size of rectangle is 1x1
  if(tol >= s.getVar(ul, w, h) || (w == 1 && h == 1)) {
    return curr;
  }
  
  //set initial min to the var of the current image; all vars of children
  //should be smaller than this value, since there's less pixels
  double min_of_max_var = s.getVar(ul, w, h);
  double max_of_kids;
  double kid_vars[4];
  pair<int, int> split = make_pair(ul.first, ul.second);

  //check every combination for splitting; compare and find the min of the
  //max of variances, bounds don't include other set of edges
  for(int x = 0; x < w; x++) {
    for(int y = 0; y < h; y++) {
      //this is the original image; continue through loop
      if(x == 0 && y == 0) {
        continue;
      }
      else {
        //split in 4s, find max var
        //if we are splitting at the edge, split into 2 kids
        max_of_kids = 0;
        if(x == 0) {
          //check variability of the 2 kids
          if(s.getVar(ul, w, y) < s.getVar(make_pair(ul.first, ul.second + y), w, h - y)) {
            max_of_kids = s.getVar(make_pair(ul.first, ul.second + y), w, h - y);
          }
          else {
            max_of_kids = s.getVar(ul, w, y);
          }

          //do a check to see if it's the minimum of maximums
          if(max_of_kids < min_of_max_var) {
            min_of_max_var = max_of_kids;
            split.first = ul.first;
            split.second = ul.second + y;
          }
        }
        else if(y == 0) {
          if(s.getVar(ul, x, h) < s.getVar(make_pair(ul.first + x, ul.second), w - x, h)) {
            max_of_kids = s.getVar(make_pair(ul.first + x, ul.second), w - x, h);
          }
          else {
            max_of_kids = s.getVar(ul, x, h);
          }

          if(max_of_kids < min_of_max_var) {
            min_of_max_var = max_of_kids;
            split.first = ul.first + x;
            split.second = ul.second;
          }
        }

        //else, we are splitting in the middle, will have 4 kids
        else {
          //calculate the var for all 4 kids, find maximum of them
          kid_vars[0] = s.getVar(ul, x, y);
          kid_vars[1] = s.getVar(make_pair(ul.first, ul.second + y), x, h - y);
          kid_vars[2] = s.getVar(make_pair(ul.first + x, ul.second), w - x, y);
          kid_vars[3] = s.getVar(make_pair(ul.first + x, ul.second + y), w - x, h - y);

          for(int i = 0; i < 4; i++) {
            if(max_of_kids < kid_vars[i]) {
              max_of_kids = kid_vars[i];
            }
          }

          if(max_of_kids < min_of_max_var) {
            min_of_max_var = max_of_kids;
            split.first = ul.first + x;
            split.second = ul.second + y;
          }
        }
      }
    }
  }

  //recursive call to split into 2 or 4 kids, check if the min is at the edge
  
  //2 kids
  if(split.first == ul.first) {
    curr -> NW = buildTree(s, ul, w, split.second - ul.second, tol);
    curr -> SW = buildTree(s, split, w, h - (split.second - ul.second), tol);
  }
  else if(split.second == ul.second) {
    curr -> NW = buildTree(s, ul, split.first - ul.first, h, tol);
    curr -> SW = buildTree(s, split, w - (split.first - ul.first), h, tol);
  }

  //4 kids
  else {
    pair<int, int> ne_ul = make_pair(split.first, ul.second);
    pair<int, int> sw_ul = make_pair(ul.first, split.second);
    curr -> NW = buildTree(s, ul, split.first - ul.first, split.second - ul.second, tol);
    curr -> NE = buildTree(s, ne_ul, w - (split.first - ul.first), split.second - ul.second, tol);
    curr -> SW = buildTree(s, sw_ul, split.first - ul.first, h - (split.second - ul.second), tol);
    curr -> SE = buildTree(s, split, w - (split.first - ul.first), h - (split.second - ul.second), tol);
  }

  return curr;
}



PNG SQtree::render() {
  PNG pic(root->width, root->height);
  renderHelper(pic, root);
  return pic;
}

void SQtree::renderHelper(PNG &png, Node *root) {


  // base case:
  if (root->NW == NULL && root->NE == NULL && root->SW == NULL && root->SE == NULL) {
    int x = root->upLeft.first;
    int y = root->upLeft.second;

    for (int i = x; i < root->width + x; i++) {
      for (int j = y; j < root->height + y; j++) {
          RGBAPixel *temp = png.getPixel(i, j);
          *temp = root -> avg;
      }
    }

  } else if (root->NE == NULL && root->SE == NULL) {
    renderHelper(png, root->NW);
    renderHelper(png, root->SW); 
  } else {
    renderHelper(png, root->NW); 
    renderHelper(png, root->NE);
    renderHelper(png, root->SW);
    renderHelper(png, root->SE);
  }
}

/**
 * Delete allocated memory.
 */
void SQtree::clear() {
  clearHelper(root);
}

void SQtree::clearHelper(Node * &root) {
  if (root != NULL) {
    clearHelper(root->NW);
    clearHelper(root->NE);
    clearHelper(root->SW);
    clearHelper(root->SE);
    delete root;
    root = NULL;
  }
}


void SQtree::copy(const SQtree & other) {
  root = copyHelper(other.root);
  root->height = other.root->height;
  root->width = other.root->width;

}

SQtree::Node* SQtree::copyHelper(Node *root) {
  if (root != NULL) {
    Node *newNode = new Node(root->upLeft, root->width, root->height, root->avg, root->var);
    newNode->NW = copyHelper(root->NW);
    newNode->NE = copyHelper(root->NE);
    newNode->SW = copyHelper(root->SW);
    newNode->SE = copyHelper(root->SE);
    return newNode;
  }
  return root;
}



int SQtree::size() {
  return sizeHelper(root);
}


int SQtree::sizeHelper(Node *root) {
  if (root != NULL) {
    int nodeCount = 1;
    nodeCount += sizeHelper(root->NW) + sizeHelper(root->NE) + sizeHelper(root->SW) + sizeHelper(root->SE);
    return nodeCount;
  } else {
    return 0;
  }

}

