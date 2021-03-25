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
  for(int x = ul.first; x < w; x++) {
    for(int y = ul.second; y < h; y++) {
      //this is the original image; continue through loop
      if(x == ul.first && y == ul.second) {
        continue;
      }
      else {
        //split in 4s, find max var
        //if we are splitting at the edge, split into 2 kids
        max_of_kids = 0;
        if(x == ul.first) {
          //check variability of the 2 kids
          if(s.getVar(ul, w, y) < s.getVar(make_pair(x, y), w, h - y)) {
            max_of_kids = s.getVar(make_pair(x, y), w, h - y);
          }
          else {
            max_of_kids = s.getVar(ul, w, y);
          }

          //do a check to see if it's the minimum of maximums
          if(max_of_kids < min_of_max_var) {
            min_of_max_var = max_of_kids;
            split.first = x;
            split.second = y;
          }
        }
        else if(y == ul.second) {
          if(s.getVar(ul, x, h) < s.getVar(make_pair(x, y), w - x, h)) {
            max_of_kids = s.getVar(make_pair(x, y), w - x, h);
          }
          else {
            max_of_kids = s.getVar(ul, x, h);
          }

          if(max_of_kids < min_of_max_var) {
            min_of_max_var = max_of_kids;
            split.first = x;
            split.second = y;
          }
        }

        //else, we are splitting in the middle, will have 4 kids
        else {
          //calculate the var for all 4 kids, find maximum of them
          kid_vars[0] = s.getVar(ul, x, y);
          kid_vars[1] = s.getVar(make_pair(ul.first, y), x, h - y);
          kid_vars[2] = s.getVar(make_pair(x, ul.second), w - x, y);
          kid_vars[3] = s.getVar(make_pair(x, y), w- x, h - y);

          for(int i = 0; i < 4; i++) {
            if(max_of_kids < kid_vars[i]) {
              max_of_kids = kid_vars[i];
            }
          }

          if(max_of_kids < min_of_max_var) {
            min_of_max_var = max_of_kids;
            split.first = x;
            split.second = y;
          }
        }
      }
    }
  }

  //recursive call to split into 2 or 4 kids, check if the min is at the edge
  
  //2 kids
  if(split.first == ul.first) {
    curr -> NW = buildTree(s, ul, w, split.second, tol);
    curr -> SW = buildTree(s, split, w, h - split.second, tol);
  }
  else if(split.second == ul.second) {
    curr -> NW = buildTree(s, ul, split.first, h, tol);
    curr -> SW = buildTree(s, split, w - split.first, h, tol);
  }

  //4 kids
  else {
    pair<int, int> ne_ul = make_pair(split.first, ul.second);
    pair<int, int> sw_ul = make_pair(ul.first, split.second);
    curr -> NW = buildTree(s, ul, split.first, split.second, tol);
    curr -> NE = buildTree(s, ne_ul, w - split.first, split.second, tol);
    curr -> SW = buildTree(s, sw_ul, split.first, h - split.second, tol);
    curr -> SE = buildTree(s, split, w - split.first, h - split.second, tol);
  }

  return curr;
}



/**
 * Render SQtree and return the resulting image.
 */
PNG SQtree::render() {

  // Your code here.
}

/**
 * Delete allocated memory.
 */
void SQtree::clear() {
  // Your code here.
}

void SQtree::copy(const SQtree & other) {
  // Your code here.
}

int SQtree::size() {
  // Your code here.
}

