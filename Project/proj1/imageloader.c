/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**				sober060224
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

// Opens a .ppm P3 image file, and constructs an Image object.
// You may find the function fscanf useful.
// Make sure that you close the file with fclose before returning.
Image *readData(char *filename)
{
	// YOUR CODE HERE
	FILE *fp = fopen(filename, "r");
	if (fp == NULL)
		return NULL;
	char buffer[256];

	fscanf(fp, "%s", buffer);

	Image *imageData = (Image *)malloc(sizeof(Image));
	if (imageData == NULL)
		return NULL;
	fscanf(fp, "%d %d", &imageData->cols, &imageData->rows);

	fscanf(fp, "%s", buffer);

	Color **imageColor = (Color **)malloc(sizeof(Color *) * imageData->rows);
	if (imageColor == NULL)
		return NULL;
	for (int i = 0; i < imageData->rows; i++)
	{
		imageColor[i] = (Color *)malloc(sizeof(Color) * imageData->cols);
		if (imageColor[i] == NULL)
			return NULL;
	}

	for (int i = 0; i < imageData->rows; i++)
		for (int j = 0; j < imageData->cols; j++)
			fscanf(fp, "%hhd %hhd %hhd", &imageColor[i][j].R, &imageColor[i][j].G, &imageColor[i][j].B);
	imageData->image = imageColor;

	fclose(fp);
	return imageData;
}

// Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	// YOUR CODE HERE
	printf("P3\n");
	printf("%d %d\n", image->cols, image->rows);
	printf("255\n");
	for (int i = 0; i < image->rows; i++)
	{
		for (int j = 0; j < image->cols; j++)
		{
			printf("%d %d %d", image->image[i][j].R, image->image[i][j].G, image->image[i][j].B);
			if (j != image->cols - 1)
				printf("   ");
		}
		printf("\n");
	}
}

// Frees an image
void freeImage(Image *image)
{
	// YOUR CODE HERE
	for (int i = 0; i < image->rows; i++)
		free(image->image[i]);
	free(image->image);
	free(image);
}

int colorToBits(Color *color)
{
	int a = color->R, b = color->G, c = color->B;
	int res = a;
	int countBit = 8;
	res <<= countBit;
	res += b;

	res <<= countBit;
	res += c;

	return res;
}

void applyBitsAssist(int *num, int *x)
{
	*x = (*num & ((1 << 8) - 1));
	*num >>= 8;
}

void applyBitsToColor(int num, Color *color)
{
	applyBitsAssist(&num, &color->B);
	applyBitsAssist(&num, &color->G);
	color->R = num;
}

int getBit(int num, int n)
{
	if (num & (1 << n))
		return 1;
	return 0;
}

void setBit(int *num, int n, int bit)
{
#error "等待实现"
}