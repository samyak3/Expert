#if 1
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>
#include<iostream>
using namespace std;
extern void encode(unsigned char bitmap[256][32], unsigned char data[4096]);
extern void decode(unsigned char data[4096], unsigned char bitmap[256][32]);
int calls = 0;
static void setpixel(unsigned char bitmap[256][32], int x, int y, int pixel)
{
	calls++;
	//cout << "x :: " << x << " y :: " << y << endl;
	unsigned char mask = 0x80 >> (x % 8);//depending upon value of x we identify which bit to set or reset
	if (pixel == 0) bitmap[y][x / 8] &= ~mask;//the divide by 8 operation means that range of values of x are scaled to x/8
	else bitmap[y][x / 8] |= mask;
}

static int getpixel(unsigned char bitmap[256][32], int x, int y)
{
	unsigned char mask = 0x80 >> (x % 8);
	if ((bitmap[y][x / 8] & mask) == 0) return 0;
	else return 1;
}

static void noise(unsigned char bitmap[256][32])
{
	for (register int y = 0; y < 256; y += 2)
		for (register int x = 0; x < 256; x += 2)
		{
			setpixel(bitmap, x + rand() % 2, y + rand() % 2, 0);
			setpixel(bitmap, x + rand() % 2, y + rand() % 2, 1);
		}
}

static void rotate(unsigned char bitmap[256][32], int angle)
{
	static unsigned char buffer[256][32];
	if (angle != 0)
	{
		switch (angle)
		{
		case 1: // 90
			for (register int y = 0; y < 256; y++)
				for (register int x = 0; x < 256; x++)
					setpixel(buffer, x, y, getpixel(bitmap, y, 255 - x));
			break;

		case 2: // 180
			for (register int y = 0; y < 256; y++)
				for (register int x = 0; x < 256; x++)
					setpixel(buffer, x, y, getpixel(bitmap, 255 - x, 255 - y));
			break;

		case 3: // 270
			for (register int y = 0; y < 256; y++)
				for (register int x = 0; x < 256; x++)
					setpixel(buffer, x, y, getpixel(bitmap, 255 - y, x));
			break;
		}

		for (register int y = 0; y < 256; y++)
			for (register int x = 0; x < 256; x++)
				setpixel(bitmap, x, y, getpixel(buffer, x, y));
	}
}

static void flip(unsigned char bitmap[256][32], int mode)
{
	static unsigned char buffer[256][32];
	if (mode != 0)
	{
		switch (mode)
		{
		case 1: // horizontal
			for (register int x = 0; x < 256; x++)
				for (register int y = 0; y < 256; y++)
					setpixel(buffer, x, y, getpixel(bitmap, x, 255 - y));
			break;

		case 2: // vertical
			for (register int y = 0; y < 256; y++)
				for (register int x = 0; x < 256; x++)
					setpixel(buffer, x, y, getpixel(bitmap, 255 - x, y));
			break;
		}

		for (register int y = 0; y < 256; y++)
			for (register int x = 0; x < 256; x++)
				setpixel(bitmap, x, y, getpixel(buffer, x, y));
	}
}

int main(void)
{
	static const int TCSIZE = 1024;
	static unsigned char data[TCSIZE][4096] = { 0 };
	static unsigned char org[TCSIZE][4096] = { 0 };
	static unsigned char result[TCSIZE][4096] = { 0 };
	static unsigned char bitmap[TCSIZE][256][32] = { 0 };

	srand(3); // The seed will be changed.

	for (register int TC = 0; TC < TCSIZE; TC++)
		for (register int c = 0; c < 4096; c++)
			org[TC][c] = data[TC][c] = rand() % 15;

	int PERFORMANCE = 10000;
	time_t start = clock();

	for (register int TC = 0; TC < TCSIZE; TC++)
		encode(bitmap[TC], data[TC]);

	PERFORMANCE -= (clock() - start) / (CLOCKS_PER_SEC / 1000);

	for (register int TC = 0; TC < TCSIZE; TC++)
	{
		noise(bitmap[TC]);
		//rotate(bitmap[TC], rand() % 4);
		//flip(bitmap[TC], rand() % 3);
	}

	start = clock();
	for (register int TC = 0; TC < TCSIZE; TC++)
		decode(result[TC], bitmap[TC]);
	PERFORMANCE -= (clock() - start) / (CLOCKS_PER_SEC / 1000);

	int PASS = 0;
	for (register int TC = 0; TC < TCSIZE; TC++)
	{
		for (register int i = 0; i < 1024; i++)
		{
			if (org[TC][i] != result[TC][i])
			{
				cout << "mismatch occurred at index :: " << i << endl;
				cout << "org data :: " << (int)org[TC][i] << endl;
				cout << "dec data :: " << (int)result[TC][i] << endl;
			}
		}
	}
	for (register int TC = 0; TC < TCSIZE; TC++)
	{
		if (memcmp(org[TC], result[TC], 4096) == 0)
			PASS++;
		else
			cout << "falied for TC :: " << TC << endl;
	}

	PERFORMANCE = PASS * 10000 + PERFORMANCE;

	if (PASS < 1024)
		printf("0\n");
	else
		printf("1\n");
	return 0;
}
void encode(unsigned char bitmap[256][32], unsigned char data[4096])
{
	int row = 0;
	int col = 0;
	for (int i = 0; i < 4096; i++)
	{
		int iuse = data[i];
		//cout << " enc :: " << iuse << endl;
		row = i / 32 * 2;
		col = i % 32;
		//now transform data for redundancy
		unsigned char enc = 255;
		int m8 = iuse & 8;
		if (m8 == 0)
		{
			enc = enc & (255-128-64);
		}
		int m4 = iuse & 4; 
		if (m4 == 0)
		{
			enc = enc & (255-32-16);
		}
		int m2 = iuse & 2;
		if (m2 == 0)
		{
			enc = enc & (255-8-4);
		}
		int m1 = iuse & 1; 
		if (m1 == 0)
		{
			enc = enc & (255 - 2 - 1);
		}
		//cout << " enc :: " << (int)enc << endl;
		bitmap[row][col] = enc;
		bitmap[row+1][col] = enc;
	}
}

void decode(unsigned char data[4096], unsigned char bitmap[256][32])
{	
	int index = 0;
	for (int row = 0; row < 256; row = row + 2)
	{
		for (int col = 0; col < 32; col++)
		{
			int sum = 0;
			index = row * 16 + col;
			sum = sum + getpixel(bitmap, col * 8, row);
			sum = sum + getpixel(bitmap, col * 8, row+1);
			sum = sum + getpixel(bitmap, col * 8 + 1, row);
			sum = sum + getpixel(bitmap, col * 8 + 1, row+1);
			int act = 15;
			if (sum < 3)
			{
				act = act & 7;
			}
			sum = 0;

			sum = sum + getpixel(bitmap, col * 8 + 2, row);
			sum = sum + getpixel(bitmap, col * 8 + 2, row + 1);
			sum = sum + getpixel(bitmap, col * 8 + 1 + 2, row);
			sum = sum + getpixel(bitmap, col * 8 + 1 + 2, row + 1);
			if (sum < 3)
			{
				act = act & 11;
			}
			sum = 0;
			sum = sum + getpixel(bitmap, col * 8 + 4, row);
			sum = sum + getpixel(bitmap, col * 8 + 4, row + 1);
			sum = sum + getpixel(bitmap, col * 8 + 1 + 4, row);
			sum = sum + getpixel(bitmap, col * 8 + 1 + 4, row + 1);
			if (sum < 3)
			{
				act = act & 13;
			}
			sum = 0;
			sum = sum + getpixel(bitmap, col * 8 + 6, row);
			sum = sum + getpixel(bitmap, col * 8 + 6, row + 1);
			sum = sum + getpixel(bitmap, col * 8 + 1 + 6, row);
			sum = sum + getpixel(bitmap, col * 8 + 1 + 6, row + 1);
			if (sum < 3)
			{
				act = act & 14;
			}
			data[index] = act;
		}
	}
	/*cout <<" 0 0 :: "<< getpixel(bitmap, 0, 0) << endl;
	cout << " 0 1 :: " << getpixel(bitmap, 1, 0) << endl;
	cout << " 1 0 :: " << getpixel(bitmap, 0, 1) << endl;
	cout << " 1 1 :: " << getpixel(bitmap, 1, 1) << endl;*/
}
#endif
