#include<iostream>
#include<algorithm>
using namespace std;
const int F = 100;
const int E = 3;
int dp[F + 1][E + 1];
int dpOpt[F + 1][E + 1];
int dpf[F + 1][E + 1];
int dpBS[F + 1][E + 1];
int operations = 0;
int operationsWithBinarySearch = 0;
int operationsLinear = 0;
int rsolve(int nFloors, int nEggsLeft)
{
	if (nEggsLeft == 1)
		return nFloors;
	if (nFloors <= 1)
		return nFloors;
	if (dp[nFloors][nEggsLeft])
		return dp[nFloors][nEggsLeft];
	int ans = 1 << 30;

	for (int f = 1; f <= nFloors; f++)
	{
		ans = min(ans, 1 + max(rsolve(f - 1, nEggsLeft - 1), rsolve(nFloors - f, nEggsLeft)));
	}
	dp[nFloors][nEggsLeft] = ans;
	return ans;
}

int solve()//O(N^2K) basic approach 
{
	for (int f = 1; f <= F; f++)
	{
		dp[f][1] = f;//if number of eggs are only 1 then number of floors trials are needed
	}
	for (int e = 1; e <= E; e++)
	{
		dp[1][e] = 1;
		dp[0][e] = 0;
	}
	for (int e = 2; e <= E; e++)
	{
		for (int f = 2; f <= F; f++)
		{
			dp[f][e] = 1 << 30;
			for (int dropFloor = 1; dropFloor <= f; dropFloor++)
			{
				int ans = 1 + max(dp[dropFloor - 1][e - 1], dp[f - dropFloor][e]);
				dp[f][e] = min(dp[f][e], ans);
				operations++;
			}
		}
	}
	return dp[F][E];
}
int solveWithOpt()//O(NK) linear approach 
{
	for (int f = 1; f <= F; f++)
	{
		dpOpt[f][1] = f;//if number of eggs are only 1 then number of floors trials are needed
		dpf[f][1] = 1;
	}
	for (int e = 1; e <= E; e++)
	{
		dpOpt[1][e] = 1;
		dpf[1][e] = 1;
		dpOpt[0][e] = 0;
	}
	for (int e = 2; e <= E; e++)
	{
		for (int f = 2; f <= F; f++)
		{
			int optf = dpf[f - 1][e];
			dpOpt[f][e] = 1 << 30;
			for (int dropFloor = optf; dropFloor <= f; dropFloor++)
			{
				operationsLinear++;
				int ans = 1 + max(dpOpt[dropFloor - 1][e - 1], dpOpt[f - dropFloor][e]);
				
				if (ans < dpOpt[f][e])
				{
					dpOpt[f][e] = min(dpOpt[f][e], ans);
					dpf[f][e] = dropFloor;
				}
				else if (ans == dpOpt[f][e])
				{
					dpf[f][e] = dropFloor;
					continue;
				}
				else
				{					
					break;					
				}
			}
		}
	}
	return dpOpt[F][E];
}
int solveWithBinarySearch()
{
	for (int f = 1; f <= F; f++)
	{
		dpBS[f][1] = f;//if number of eggs are only 1 then number of floors trials are needed
	}
	for (int e = 1; e <= E; e++)
	{
		dpBS[1][e] = 1;
		dpBS[0][e] = 0;
	}
	for (int e = 2; e <= E; e++)
	{
		for (int f = 2; f <= F; f++)
		{
			dpBS[f][e] = 1 << 30;
			int low = 1;
			int high = f;
			int mid = 0;
			int ans = 1 << 30;

			while (low <= high)
			{
				operationsWithBinarySearch++;
				mid = (low + high) / 2;
				int inc = dpBS[mid - 1][e - 1];
				int dec = dpBS[f - mid][e];
				ans = min(ans, 1 + max(inc, dec));
				if (inc < dec)
				{
					low = mid + 1;
				}
				else if (dec < inc)
				{
					high = mid - 1;
				}
				else
				{
					ans = 1 + dpBS[f - mid][e];
					break;
				}
			}
			dpBS[f][e] = ans;			
		}
	}
	return dpBS[F][E];

}
FILE* pOut = 0;
int main()
{
	freopen_s(&pOut, "output.txt", "w", stdout);

	cout << "Floors :: " << F << " Eggs ::" << E << endl;

	if (F <= 100)
		cout << "n^2k :: " << solve() << endl;

	cout << "nklogn ::" << solveWithBinarySearch() << endl;

	cout << "nk :: " << solveWithOpt() << endl;



	if (F <= 100)
		cout << "operations :: " << operations << endl;

	cout << "operationsWithBinarySearch :: " << operationsWithBinarySearch <<" factor :: "<<operationsWithBinarySearch/F/E<< endl;

	cout << "operationsLinear :: " << operationsLinear << " factor :: " << operationsLinear / F / E << endl;

	cout << "gain :: " << operationsWithBinarySearch / operationsLinear << endl;
	
	return 0;
}
