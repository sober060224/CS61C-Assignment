#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

typedef struct Color
{
	uint8_t R;
	uint8_t G;
	uint8_t B;
} Color;
typedef struct Image
{
	Color **image;
	uint32_t rows;
	uint32_t cols;
} Image;

// Loads an image from a ppm file, and converts to an Image object
extern Image *readData(char *filename);

// Writes an image to stdout
extern void writeData(Image *image);

// Frees an image and all its associated data
extern void freeImage(Image *image);

// 将红绿蓝色素按二进制从左到右编码成一个新数字
int colorToBits(Color *color);

// 将一个数字从左到右解码成红绿蓝色素
void applyBitsToColor(int num, Color *color);

// 获取第n位二进制数index-0
int getBit(int num, int n);

// 设置数字第n位进制数为bit
void setBit(int *num, int n, int bit);