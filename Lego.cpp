#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
#define MAX 30000
//#define MAX 2

#define DBG //printf

extern int test(int modules[][4][4]);
struct info
{
	int maxx;
	int index;
};
typedef struct mInfo
{
	int index;
	int maxx;
	int surplus;
	int minn;
};
bool comp(mInfo a, mInfo b)
{
	if (a.maxx > b.maxx)
		return true;
	return false;
}
int nodeCount = 0; 
int nodeCountResused = 0;
bool used[MAX]; 
struct node
{
	int data;
	node* arr[9];
	vector<info>v;
	node()
	{
		for (int i = 0; i < 9; i++)
			arr[i] = 0;
		data = 0;
		nodeCount++;
	}
};
node* root[200];

#if 1
int main()
{
	static int modules[MAX][4][4];

	int base, tc, i, j, k;
	srand(3);
	for (tc = 0; tc < 10; tc++)
	{

		for (i = 0; i < MAX; i++)
		{
			base = 1 + rand() % 6;
			for (j = 0; j < 4; j++)
			{
				for (k = 0; k < 4; k++)
				{
					modules[i][j][k] = base + rand() % 3;
				}
			}
		}
		//if (tc == 9)			
		printf("%d\n", test(modules));
	}
	return 0;
}
#else
int main()
{
	static int modules[MAX][4][4];

	int base, tc, i, j, k;
	srand(3);
	for (tc = 0; tc < 1; tc++)
	{
		modules[0][0][0] = 4;
		modules[0][0][1] = 3;
		modules[0][0][2] = 4;
		modules[0][0][3] = 3;
		modules[0][1][0] = 3;
		modules[0][1][1] = 3;
		modules[0][1][2] = 3;
		modules[0][1][3] = 3;
		modules[0][2][0] = 4;
		modules[0][2][1] = 3;
		modules[0][2][2] = 3;
		modules[0][2][3] = 3;
		modules[0][3][0] = 3;
		modules[0][3][1] = 3;
		modules[0][3][2] = 4;
		modules[0][3][3] = 3;


		modules[1][0][0] = 4;
		modules[1][0][1] = 4;
		modules[1][0][2] = 3;
		modules[1][0][3] = 4;
		modules[1][1][0] = 3;
		modules[1][1][1] = 4;
		modules[1][1][2] = 4;
		modules[1][1][3] = 4;
		modules[1][2][0] = 4;
		modules[1][2][1] = 4;
		modules[1][2][2] = 4;
		modules[1][2][3] = 4;
		modules[1][3][0] = 3;
		modules[1][3][1] = 4;
		modules[1][3][2] = 3;
		modules[1][3][3] = 4;
		

		/*modules[1][0][0] = 3;
		modules[1][0][1] = 4;
		modules[1][0][2] = 3;
		modules[1][0][3] = 4;
		modules[1][1][0] = 4;
		modules[1][1][1] = 4;
		modules[1][1][2] = 4;
		modules[1][1][3] = 4;
		modules[1][2][0] = 3;
		modules[1][2][1] = 4;
		modules[1][2][2] = 4;
		modules[1][2][3] = 4;
		modules[1][3][0] = 4;
		modules[1][3][1] = 4;
		modules[1][3][2] = 3;
		modules[1][3][3] = 4;*/
		printf("%d\n", test(modules));
	}
	return 0;
}

#endif


int down[MAX][4][4];

int up[MAX][4][4];
int test(int modules[][4][4])
{
	mInfo amInfo[MAX];
	for (int i = 0; i < 200; i++)
		root[i] = 0;
	for (int k = 0; k < MAX; k++)
		used[k] = false;
	for (int k = 0; k < MAX; k++)
	{
		int maxx = 0;
		int minn = 10;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (maxx < modules[k][i][j])
					maxx = modules[k][i][j];
				if (minn > modules[k][i][j])
					minn = modules[k][i][j];
			}
		}
		amInfo[k].index = k;
		amInfo[k].maxx = maxx;
		amInfo[k].minn = minn;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				down[k][i][j] = maxx - modules[k][i][j];
				up[k][i][j] = modules[k][i][j] - minn;
			}
		}

		//deficit
		int deficit = 0;
		int surplus = 0;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				deficit += maxx - modules[k][i][j];
				surplus += modules[k][i][j] - minn;
			}

		}
		//cout << "deficit :: " << deficit << endl;
		//cout << "surplus :: " << surplus << endl;
		amInfo[k].surplus = surplus;
		if (root[deficit] == 0)
		{
			node* temp = new node;
			root[deficit] = temp;
		}
		//upper left
		{
			node* lroot = root[deficit];
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					int elm = down[k][i][j];
					if (lroot->arr[elm])
					{
						nodeCountResused++;
						lroot = lroot->arr[elm];
					}
					else
					{
						node* temp = new node;
						lroot->arr[elm] = temp;
						temp->data = elm;
						lroot = temp;
					}

				}
			}
			info inode;
			inode.index = k;
			inode.maxx = maxx;
			lroot->v.push_back(inode);
			int xsxs = 0;
		}
	//upper right
	{
		node* lroot = root[deficit];
		for (int j = 3; j >= 0; j--)
		{
			for (int i = 0; i < 4; i++)
			{
				int elm = down[k][i][j];
				if (lroot->arr[elm])
				{
					nodeCountResused++;
					lroot = lroot->arr[elm];
				}
				else
				{
					node* temp = new node;
					lroot->arr[elm] = temp;
					temp->data = elm;
					lroot = temp;
				}

			}
		}
		info inode;
		inode.index = k;
		inode.maxx = maxx;
		lroot->v.push_back(inode);
		int xsxs = 0;
	}
	//lower right
	{
		node* lroot = root[deficit];
		for (int i = 3; i >= 0; i--)
		{
			for (int j = 3; j >= 0; j--)
			{
				int elm = down[k][i][j];
				if (lroot->arr[elm])
				{
					nodeCountResused++;
					lroot = lroot->arr[elm];
				}
				else
				{
					node* temp = new node;
					lroot->arr[elm] = temp;
					temp->data = elm;
					lroot = temp;
				}

			}
		}
		info inode;
		inode.index = k;
		inode.maxx = maxx;
		lroot->v.push_back(inode);
		int xsxs = 0;
	}

	//lower left
	{
		node* lroot = root[deficit];
		for (int j = 0; j <= 3; j++)
		{
			for (int i = 3; i >= 0; i--)
			{
				int elm = down[k][i][j];
				if (lroot->arr[elm])
				{
					nodeCountResused++;
					lroot = lroot->arr[elm];
				}
				else
				{
					node* temp = new node;
					lroot->arr[elm] = temp;
					temp->data = elm;
					lroot = temp;
				}

			}
		}
		info inode;
		inode.index = k;
		inode.maxx = maxx;
		lroot->v.push_back(inode);
		int xsxs = 0;
	}
		
	}
	sort(amInfo, amInfo + MAX, comp);

	int k = 0;
	int ans = 0;
	for (int x = 0; x < MAX; x++)
	{
		k = amInfo[x].index;
		if (used[k])
			continue;
		int surplus = amInfo[x].surplus;
		node* lroot = root[surplus];
		if (lroot == 0)
			continue;
		bool bLeave = false;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 3; j >=0; j--)
			{
				int elm = up[k][i][j];
				if (lroot->arr[elm])
				{
					//nodeCountResused++;
					lroot = lroot->arr[elm];
				}
				else
				{
					bLeave = true;
					break;
				}
			}
			if(bLeave)
				break;
		}
		if (bLeave)
			continue;
		int maxBlockLenth = 0;
		int uindex = -1;
		int msize = lroot->v.size();
		//if (msize > 1)
//			cout << "msize :: " << msize << endl;
		for (int vv = 0; vv < msize; vv++)
		{
			if (maxBlockLenth < lroot->v[vv].maxx && used[lroot->v[vv].index] == false && lroot->v[vv].index != k)
			{
				maxBlockLenth = lroot->v[vv].maxx;
				uindex = lroot->v[vv].index;
			}
		}
		if (maxBlockLenth)
		{
			used[uindex] = true;
			used[k] = true;
			ans += maxBlockLenth + amInfo[x].minn;
		}
	}
	
	return ans;
}
