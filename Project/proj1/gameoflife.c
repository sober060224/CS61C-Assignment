/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

// Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
// Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
// and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	// YOUR CODE HERE
	Color *color = (Color *)malloc(sizeof(Color));
	if (color == NULL)
		return NULL;
	*color = image->image[row][col];

	int dx[8] = {-1, 1, 0, 0, -1, -1, 1, 1}, dy[8] = {0, 0, -1, 1, -1, 1, -1, 1}; // 上下左右，左右的上下

	// 任何存活细胞，如果其周围八个单元格中有两个或三个存活邻居，则会存活到下一代。
	// 类似地，任何死亡细胞，如果具有三个存活邻居，则会在下一代复活为存活细胞。
	// 所有其他存活细胞将在下一代死亡，而所有其他死亡细胞会保持不活跃状态。

	int encodeBits = colorToBits(color);

	// 判断指定位置色素下一代状态
	// 每个位比特
	for (int i = 0; i < 24; i++)
	{
		int sum = 0;
		// 当前位存活下一代可能死亡
		if (getBit(encodeBits, i))
			setBit(&encodeBits, i, 0);

		// 每个颜色（单元格）
		for (int j = 0; j < 8; j++)
		{
			// 最左和最右，最上和最下互通
			int newRow = (row + dx[j] + image->rows) % image->rows, newCol = (col + dy[j] + image->cols) % image->cols;
			Color adjColor = image->image[newRow][newCol]; // 邻居颜色
			int encodeAdjBits = colorToBits(&adjColor);
			// 每个细胞是每个颜色像素的每个位比特
			// 拿每个像素的每个位比特

			// 颜色邻居位是活的
			if (getBit(encodeAdjBits, i))
				sum++;
		}

		// 存活
		if (getBit(encodeBits, i) && (rule >> (9 + sum)))
			setBit(&encodeBits, i, 1);
		// 复活
		if (!getBit(encodeBits, i) && (rule >> sum))
			setBit(&encodeBits, i, 1);
		// // 存活
		// if (getBit(mixBit, i) && (sum == 2 || sum == 3))
		// 	setBit(&mixBit, i, 1);
		// // 复活
		// if (!getBit(mixBit, i) && sum == 3)
		// 	setBit(&mixBit, i, 1);
	}

	applyBitsToColor(encodeBits, color);
	return color;
}

// The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
// You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	// YOUR CODE HERE
	Image *newImage = (Image *)malloc(sizeof(Image));
	if (newImage == NULL)
		return NULL;
	newImage->rows = image->rows, newImage->cols = image->cols;

	Color **imageColor = (Color **)malloc(sizeof(Color *) * image->rows);
	if (imageColor == NULL)
		return NULL;

	for (int i = 0; i < image->rows; i++)
	{
		imageColor[i] = (Color *)malloc(sizeof(Color) * image->cols);
		if (imageColor[i] == NULL)
		{
			for (int j = 0; j < i; j++)
				free(imageColor[j]);
			free(imageColor);
			return NULL;
		}
	}

	for (int i = 0; i < image->rows; i++)
	{
		for (int j = 0; j < image->cols; j++)
		{
			Color *t = evaluateOneCell(image, i, j, rule);
			imageColor[i][j] = *t;
			free(t);
		}
	}

	newImage->image = imageColor;
	return newImage;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	// YOUR CODE HERE
	if (argc <= 2)
	{
		printf("usage: ./gameOfLife filename rule\n"
			   "filename is an ASCII PPM file (type P3) with maximum value 255.\n"
			   "rule is a hex number beginning with 0x; Life is 0x1808.\n");
		exit(-1);
	}

	Image *image = readData(argv[1]);
	if (image == NULL)
		return -1;
	uint32_t rule = 0;

	char *endptr;
	rule = strtol(argv[2], endptr, 16);
	if (rule != '\0' || rule < 0x0 || rule > 0x3FFFF)
	{
		printf("usage: ./gameOfLife filename rule\n"
			   "filename is an ASCII PPM file (type P3) with maximum value 255.\n"
			   "rule is a hex number beginning with 0x; Life is 0x1808.\n");
		exit(-1);
	}

	Image *secret = life(image, rule);
	if (secret == NULL)
		return -1;
	writeData(secret);

	freeImage(secret);
	freeImage(image);
	return 0;
}