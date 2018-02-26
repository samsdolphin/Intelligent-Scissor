#include <stdio.h>
#include <algorithm>
#include <cstring>
#include <vector>
#include <math.h>
#include "mex.h"
#include "fibheap.h"

#define INITIAL  0
#define ACTIVE   1
#define EXPANDED 2

using namespace std;

struct imgNode
{
	int state;
	double totalcost;
	imgNode *prenode;
	int col, row;
};

bool comp(double val, imgNode const& node)
{
	return (node.totalcost>val);
}

void minPath(double costGraph[], double* out, int w, int h, int sx, int sy)
{
	vector<vector<imgNode> > allNode;
	for(int i = 0; i < h; i++)
	{
		allNode.push_back(vector<imgNode>());
		for(int j = 0; j < w; j++)
		{
			imgNode n;
			n.state = INITIAL;
			n.totalcost = 1e8;
			n.prenode = NULL;
			n.col = j;
			n.row = i;
			allNode[i].push_back(n);
		}
	}

	vector<imgNode> pq;
	imgNode seed;
	seed.col = sy;
	seed.row = sx;
	allNode[sx][sy].totalcost = 0;
	pq.push_back(seed);

	while(!pq.empty())
	{
		imgNode* min = &allNode[pq.front().row][pq.front().col];
		pq.erase(pq.begin());
		int x = min->row;
		int y = min->col;
		//mexPrintf("%s %d %d\n", "seed: ", x, y);
		min->state = EXPANDED;
		//min->totalcost = 0;
		vector<imgNode>::iterator it;

		if(y+1<w && allNode[x][y+1].state==INITIAL)
		{
			//mexPrintf("%s %d %d\n", "new: ", x, y+1);
			allNode[x][y+1].prenode = &allNode[x][y];
			allNode[x][y+1].totalcost = min->totalcost+costGraph[h*(3*y-1)+3*x-2];
			allNode[x][y+1].state = ACTIVE;
			it = upper_bound(pq.begin(), pq.end(), allNode[x][y+1].totalcost, comp);
			pq.insert(it, allNode[x][y+1]);
		}
		else if(y+1<w && allNode[x][y+1].state==ACTIVE)
			if(allNode[x][y+1].totalcost > min->totalcost+costGraph[h*(3*y-1)+3*x-2])
			{
				//mexPrintf("%s %d %d\n", "modify: ", x, y+1);
				allNode[x][y+1].totalcost = min->totalcost+costGraph[h*(3*y-1)+3*x-2];
				allNode[x][y+1].prenode = &allNode[x][y];
			}
		
		if(y+1<w && x>0 && allNode[x-1][y+1].state==INITIAL)
		{
			//mexPrintf("%s %d %d\n", "new: ", x-1, y+1);
			allNode[x-1][y+1].prenode = &allNode[x][y];
			allNode[x-1][y+1].totalcost = min->totalcost+costGraph[h*(3*y-1)+3*x-3];
			allNode[x-1][y+1].state = ACTIVE;
			it = upper_bound(pq.begin(), pq.end(), allNode[x-1][y+1].totalcost, comp);
			pq.insert(it, allNode[x-1][y+1]);
		}
		else if(y+1<w && x>0 && allNode[x-1][y+1].state==ACTIVE)
			if(allNode[x-1][y+1].totalcost > min->totalcost+costGraph[h*(3*y-1)+3*x-3])
			{
				//mexPrintf("%s %d %d\n", "modify: ", x-1, y+1);
				allNode[x-1][y+1].totalcost = min->totalcost+costGraph[h*(3*y-1)+3*x-3];
				allNode[x-1][y+1].prenode = &allNode[x][y];
			}

		if(x>0 && allNode[x-1][y].state==INITIAL)
		{
			//mexPrintf("%s %d %d\n", "new: ", x-1, y);
			allNode[x-1][y].prenode = &allNode[x][y];
			allNode[x-1][y].totalcost = min->totalcost+costGraph[h*(3*y-2)+3*x-3];
			allNode[x-1][y].state = ACTIVE;
			it = upper_bound(pq.begin(), pq.end(), allNode[x-1][y].totalcost, comp);
			pq.insert(it, allNode[x-1][y]);
		}
		else if(x>0 && allNode[x-1][y].state==ACTIVE)
			if(allNode[x-1][y].totalcost > min->totalcost+costGraph[h*(3*y-2)+3*x-3])
			{
				//mexPrintf("%s %d %d\n", "modify: ", x-1, y);
				allNode[x-1][y].totalcost = min->totalcost+costGraph[h*(3*y-2)+3*x-3];
				allNode[x-1][y].prenode = &allNode[x][y];
			}

		if(y>0 && x>0 && allNode[x-1][y-1].state==INITIAL)
		{
			//mexPrintf("%s %d %d\n", "new: ", x-1, y-1);
			allNode[x-1][y-1].prenode = &allNode[x][y];
			allNode[x-1][y-1].totalcost = min->totalcost+costGraph[h*(3*y-3)+3*x-3];
			allNode[x-1][y-1].state = ACTIVE;
			it = upper_bound(pq.begin(), pq.end(), allNode[x-1][y-1].totalcost, comp);
			pq.insert(it, allNode[x-1][y-1]);
		}
		else if(y>0 && x>0 && allNode[x-1][y-1].state==ACTIVE)
			if(allNode[x-1][y-1].totalcost > min->totalcost+costGraph[h*(3*y-3)+3*x-3])
			{
				//mexPrintf("%s %d %d\n", "modify: ", x-1, y-1);
				allNode[x-1][y-1].totalcost = min->totalcost+costGraph[h*(3*y-3)+3*x-3];
				allNode[x-1][y-1].prenode = &allNode[x][y];
			}

		if(y>0 && allNode[x][y-1].state==INITIAL)
		{
			//mexPrintf("%s %d %d\n", "new: ", x, y-1);
			allNode[x][y-1].prenode = &allNode[x][y];
			allNode[x][y-1].totalcost = min->totalcost+costGraph[h*(3*y-3)+3*x-2];
			allNode[x][y-1].state = ACTIVE;
			it = upper_bound(pq.begin(), pq.end(), allNode[x][y-1].totalcost, comp);
			pq.insert(it, allNode[x][y-1]);
		}
		else if(y>0 && allNode[x][y-1].state==ACTIVE)
			if(allNode[x][y-1].totalcost > min->totalcost+costGraph[h*(3*y-3)+3*x-2])
			{
				//mexPrintf("%s %d %d\n", "modify: ", x, y-1);
				allNode[x][y-1].totalcost = min->totalcost+costGraph[h*(3*y-3)+3*x-2];
				allNode[x][y-1].prenode = &allNode[x][y];
			}

		if(y>0 && x+1<h && allNode[x+1][y-1].state==INITIAL)
		{
			//mexPrintf("%s %d %d\n", "new: ", x+1, y-1);
			allNode[x+1][y-1].prenode = &allNode[x][y];
			allNode[x+1][y-1].totalcost = min->totalcost+costGraph[h*(3*y-3)+3*x-1];
			allNode[x+1][y-1].state = ACTIVE;
			it = upper_bound(pq.begin(), pq.end(), allNode[x+1][y-1].totalcost, comp);
			pq.insert(it, allNode[x+1][y-1]);
		}
		else if(y>0 && x+1<h && allNode[x+1][y-1].state==ACTIVE)
			if(allNode[x+1][y-1].totalcost > min->totalcost+costGraph[h*(3*y-3)+3*x-1])
			{
				//mexPrintf("%s %d %d\n", "modify: ", x+1, y-1);
				allNode[x+1][y-1].totalcost = min->totalcost+costGraph[h*(3*y-3)+3*x-1];
				allNode[x+1][y-1].prenode = &allNode[x][y];
			}

		if(x+1<h && allNode[x+1][y].state==INITIAL)
		{
			//mexPrintf("%s %d %d\n", "new: ", x+1, y);
			allNode[x+1][y].prenode = &allNode[x][y];
			allNode[x+1][y].totalcost = min->totalcost+costGraph[h*(3*y-2)+3*x-1];
			allNode[x+1][y].state = ACTIVE;
			it = upper_bound(pq.begin(), pq.end(), allNode[x+1][y].totalcost, comp);
			pq.insert(it, allNode[x+1][y]);
		}
		else if(x+1<h && allNode[x+1][y].state==ACTIVE)
			if(allNode[x+1][y].totalcost > min->totalcost+costGraph[h*(3*y-2)+3*x-1])
			{
				//mexPrintf("%s %d %d\n", "modify: ", x+1, y);
				allNode[x+1][y].totalcost = min->totalcost+costGraph[h*(3*y-2)+3*x-1];
				allNode[x+1][y].prenode = &allNode[x][y];
			}

		if(y+1<w && x+1<h && allNode[x+1][y+1].state==INITIAL)
		{
			//mexPrintf("%s %d %d\n", "new: ", x+1, y+1);
			allNode[x+1][y+1].prenode = &allNode[x][y];
			allNode[x+1][y+1].totalcost = min->totalcost+costGraph[h*(3*y-1)+3*x-1];
			allNode[x+1][y+1].state = ACTIVE;
			it = upper_bound(pq.begin(), pq.end(), allNode[x+1][y+1].totalcost, comp);
			pq.insert(it, allNode[x+1][y+1]);
		}
		else if(y+1<w && x+1<h && allNode[x+1][y+1].state==ACTIVE)
			if(allNode[x+1][y+1].totalcost > min->totalcost+costGraph[h*(3*y-1)+3*x-1])
			{
				//mexPrintf("%s %d %d\n", "modify: ", x+1, y+1);
				allNode[x+1][y+1].totalcost = min->totalcost+costGraph[h*(3*y-1)+3*x-1];
				allNode[x+1][y+1].prenode = &allNode[x][y];
			}
	}
	mexPrintf("%s\n", "prepare for out");
	imgNode* mouse = &allNode[50][99];
	while(mouse->prenode!=NULL)
	{
		mexPrintf("%d %d\n", mouse->row, mouse->col);
		mouse=mouse->prenode;
	}
/*
	for(int i = 0; i < h; i++)
		for(int j = 0; j < w; j++)
			if(allNode[i][j].prenode!=NULL)
				out[i+j*h] = double((allNode[i][j].prenode->col)*h+allNode[i][j].prenode->row+1);
*/
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) 
{
	double* costGraph =mxGetPr(prhs[0]);
    int sx = (int)mxGetScalar(prhs[1]);
    int sy = (int)mxGetScalar(prhs[2]);
    const int* dim = mxGetDimensions(prhs[0]);
    int h = dim[0];
    int w = dim[1];
    int dims[2] = {h/3, w/3};

    plhs[0] = mxCreateNumericArray(2, dims, mxDOUBLE_CLASS, mxREAL);
    double *arr = (double*)malloc(w/3*h/3*sizeof(double));
    minPath(costGraph, arr, w/3, h/3, sx, sy);
    memcpy((double*)mxGetPr(plhs[0]), arr, w/3*h/3*mxGetElementSize(plhs[0]));
    free(arr);
}