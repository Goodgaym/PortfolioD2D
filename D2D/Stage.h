#pragma once

#define MAX_STAGE 4

UINT stage[MAX_STAGE][BUBBLE_LINE_SIZE_Y][BUBBLE_LINE_SIZE_X] = { {
	{3,3,2,2,1,1,4,4},
	{3,3,2,2,1,1,4,0},
	{1,1,4,4,3,3,2,2},
	{1,4,4,3,3,2,2,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0}
},{
	{0,0,0,7,7,0,0,0},
	{0,0,0,1,0,0,0,0},
	{0,0,0,3,0,0,0,0},
	{0,0,0,1,0,0,0,0},
	{0,0,0,4,0,0,0,0},
	{0,0,0,5,0,0,0,0},
	{0,0,0,4,0,0,0,0},
	{0,0,0,1,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0}
},{
	{4,0,0,0,0,0,0,4},
	{3,4,1,2,3,4,1,0},
	{2,0,0,0,0,0,0,2},
	{1,2,3,4,1,2,3,0},
	{0,0,0,3,0,0,0,0},
	{0,0,0,4,0,0,0,0},
	{0,0,0,3,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0}
},{
	{0,7,7,0,0,1,1,0},
	{0,1,0,0,0,1,0,0},
	{0,3,0,0,0,5,0,0},
	{0,5,0,0,0,5,0,0},
	{0,4,0,0,0,4,0,0},
	{0,3,0,0,0,3,0,0},
	{0,3,0,0,0,1,0,0},
	{0,5,0,0,0,5,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0}
} };