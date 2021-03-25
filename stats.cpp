#include "stats.h"

stats::stats(PNG & im){

/* Your code here!! */

    int w = im.width();
    int h = im.height();

    // vector<vector<int>> M;
    // int m = number of rows, n = number of columns;
    // M.resize(m, vector<int>(n));

    sumRed.resize(w, vector<long>(h));
    sumGreen.resize(w, vector<long>(h));
    sumBlue.resize(w, vector<long>(h));

    sumsqRed.resize(w, vector<long>(h));
    sumsqGreen.resize(w, vector<long>(h));
    sumsqBlue.resize(w, vector<long>(h));


    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            
            RGBAPixel *pixel = im.getPixel(x, y);

            if (x == 0 && y == 0) {
                sumRed[x][y] = pixel->r;
                sumGreen[x][y] = pixel->g;
                sumBlue[x][y] = pixel->b;

                sumsqRed[x][y] = (pixel->r) * (pixel->r);
                sumsqGreen[x][y] = (pixel->g) * (pixel->g);
                sumsqBlue[x][y] = (pixel->b) * (pixel->b);


            } else if (x == 0 && y > 0) {
                sumRed[x][y] = sumRed[x][y-1] + pixel->r;
                sumGreen[x][y] = sumGreen[x][y-1] + pixel->g;
                sumBlue[x][y] = sumBlue[x][y-1] + pixel->b;

                sumsqRed[x][y] = sumsqRed[x][y-1] + (pixel->r) * (pixel->r);
                sumsqGreen[x][y] = sumsqGreen[x][y-1] + (pixel->g) * (pixel->g);
                sumsqBlue[x][y] = sumsqBlue[x][y-1] + (pixel->b) * (pixel->b);

    

            } else if (x > 0 && y == 0) {
                sumRed[x][y] = sumRed[x-1][y] + pixel->r;
                sumGreen[x][y] = sumGreen[x-1][y] + pixel->g;
                sumBlue[x][y] = sumBlue[x-1][y] + pixel->b;

                sumsqRed[x][y] = sumsqRed[x-1][y] + (pixel->r) * (pixel->r);
                sumsqGreen[x][y] = sumsqGreen[x-1][y] + (pixel->g) * (pixel->g);
                sumsqBlue[x][y] = sumsqBlue[x-1][y] + (pixel->b) * (pixel->b);

            

            } else {
                sumRed[x][y] = sumRed[x-1][y] + sumRed[x][y-1] - sumRed[x-1][y-1] + pixel->r;
                sumGreen[x][y] = sumGreen[x-1][y] + sumGreen[x][y-1] - sumGreen[x-1][y-1] + pixel->g;
                sumBlue[x][y] = sumBlue[x-1][y] + sumBlue[x][y-1] - sumBlue[x-1][y-1] + pixel->b;

                sumsqRed[x][y] = sumsqRed[x-1][y] + sumsqRed[x][y-1] - sumsqRed[x-1][y-1] + (pixel->r) * (pixel->r);
                sumsqGreen[x][y] = sumsqGreen[x-1][y] + sumsqGreen[x][y-1] - sumsqGreen[x-1][y-1] + (pixel->g) * (pixel->g);
                sumsqBlue[x][y] = sumsqBlue[x-1][y] + sumsqBlue[x][y-1] - sumsqBlue[x-1][y-1] + (pixel->b) * (pixel->b);

    
            }

        }
        

    }

}

//* Returns the sums of all pixel values in one color channel.
//* Useful in computing the average color of a rectangle.
long stats::getSum(char channel, pair<int,int> ul, int w, int h){

/* Your code here!! */

    int x = ul.first;
    int y = ul.second;

    long sum;

    if (channel == 'r') {


        if (x == 0 && y == 0) {
            sum = sumRed[w - 1][h - 1];
        } else if (x == 0 && y > 0) {
            sum = sumRed[w - 1][y + h - 1] - sumRed[w - 1][y - 1];
        } else if (x > 0 && y == 0) { 
            sum = sumRed[x + w - 1][h - 1] - sumRed[x - 1][h - 1];
        } else {
            sum = sumRed[x + w - 1][y + h - 1] - sumRed[x + w - 1][y -1] - sumRed[x - 1][y + h - 1] + sumRed[x - 1][y - 1];
        }
        
    } else if(channel == 'g') {
        if (x == 0 && y == 0) {
            sum = sumGreen[w - 1][h - 1];
        } else if (x == 0 && y > 0) {
            sum = sumGreen[w - 1][y + h - 1] - sumGreen[w - 1][y - 1];
        } else if (x > 0 && y == 0) { 
            sum = sumGreen[x + w - 1][h - 1] - sumGreen[x - 1][h - 1];
        } else {
            sum = sumGreen[x + w - 1][y + h - 1] - sumGreen[x + w - 1][y - 1] - sumGreen[x - 1][y + h - 1] + sumGreen[x - 1][y - 1];
        }
    } else {
        if (x == 0 && y == 0) {
            sum = sumBlue[w - 1][h - 1];
        } else if (x == 0 && y > 0) {
            sum = sumBlue[w - 1][y + h - 1] - sumBlue[w - 1][y - 1];
        } else if (x > 0 && y == 0) { 
            sum = sumBlue[x + w - 1][h - 1] - sumBlue[x - 1][h - 1];
        } else {
            sum = sumBlue[x + w - 1][y + h - 1] - sumBlue[x + w - 1][y - 1] - sumBlue[x - 1][y + h - 1] + sumBlue[x - 1][y - 1];
        }
    }

    return sum;

}

long stats::getSumSq(char channel, pair<int,int> ul, int w, int h){
/* Your code here!! */
    int x = ul.first;
    int y = ul.second;

    long sumsq;
    
    if (channel == 'r') {
        if (x == 0 && y == 0) {
            sumsq = sumsqRed[w - 1][h - 1];
        } else if (x == 0 && y > 0) {
            sumsq = sumsqRed[w - 1][y + h - 1] - sumsqRed[w - 1][y - 1];
        } else if (x > 0 && y == 0) { 
            sumsq = sumsqRed[x + w - 1][h - 1] - sumsqRed[x - 1][h - 1];
        } else {
            sumsq = sumsqRed[x + w - 1][y + h - 1] - sumsqRed[x + w - 1][y -1] - sumsqRed[x - 1][y + h - 1] + sumsqRed[x - 1][y - 1];
        }
        
    } else if (channel == 'g') {
        if (x == 0 && y == 0) {
            sumsq = sumsqGreen[w - 1][h - 1];
        } else if (x == 0 && y > 0) {
            sumsq = sumsqGreen[w - 1][y + h - 1] - sumsqGreen[w - 1][y - 1];
        } else if (x > 0 && y == 0) { 
            sumsq = sumsqGreen[x + w - 1][h - 1] - sumsqGreen[x - 1][h - 1];
        } else {
            sumsq = sumsqGreen[x + w - 1][y + h - 1] - sumsqGreen[x + w - 1][y -1] - sumsqGreen[x - 1][y + h - 1] + sumsqGreen[x - 1][y - 1];
        }
    } else {
        if (x == 0 && y == 0) {
            sumsq = sumsqBlue[w - 1][h - 1];
        } else if (x == 0 && y > 0) {
            sumsq = sumsqBlue[w - 1][y + h - 1] - sumsqBlue[w - 1][y - 1];
        } else if (x > 0 && y == 0) { 
            sumsq = sumsqBlue[x + w - 1][h - 1] - sumsqBlue[x - 1][h - 1];
        } else {
            sumsq = sumsqBlue[x + w - 1][y + h - 1] - sumsqBlue[x + w - 1][y - 1] - sumsqBlue[x - 1][y + h - 1] + sumsqBlue[x - 1][y - 1];
        }
    }

    return sumsq;
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int,int> ul, int w, int h){
    /* Your code here!! */

    double area = w * h;

    double redValue = getSumSq('r', ul, w, h) - (getSum('r', ul, w, h)) * (getSum('r', ul, w, h)) / area;
    double greenValue = getSumSq('g', ul, w, h) - (getSum('g', ul, w, h)) * (getSum('g', ul, w, h)) / area;
    double blueValue = getSumSq('b', ul, w, h) - (getSum('b', ul, w, h)) * (getSum('b', ul, w, h)) / area;

    return (double) redValue + greenValue + blueValue;
    
}
		


RGBAPixel stats::getAvg(pair<int,int> ul, int w, int h){
    /* Your code here!! */

    double area = w * h;
    double redValue = getSum('r', ul, w, h) / area;
    double greenValue = getSum('g', ul, w, h) / area;
    double blueValue = getSum('b', ul, w, h) / area;

    return RGBAPixel(redValue, greenValue, blueValue);

}
