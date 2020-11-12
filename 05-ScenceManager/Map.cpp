#include <fstream>
#include <iostream>
#include "Map.h"
#include"Utils.h"
Map::Map(int idMap, int nTitleCols, int nTitleRows, int nMapCols, int nMapRows, int nTotalTiles)
{
	this->TileSet = CTextures::GetInstance()->Get(idMap);
	this->TotalColsOfMap = nMapCols;
	this->TotalRowsOfMap = nMapRows;
	this->TotalRowsOfTileSet = nTitleRows;
	this->TotalColsOfTitleSet = nTitleCols;
	this->TotalTiles = nTotalTiles;
}
Map::~Map()
{

}
void Map::CreateTilesFromTileSet()
{
	int left, bottom, right, top;
	for (int tileNum = 0; tileNum < TotalTiles; tileNum++) {
		left = tileNum % TotalColsOfTitleSet * TILE_WIDTH;
		top = tileNum / TotalColsOfTitleSet * TILE_HEIGHT;
		bottom = top + TILE_HEIGHT;
		right = left + TILE_WIDTH;
		LPSPRITE Tile = new CSprite(tileNum, left, top, right, bottom, TileSet);
		this->Tiles.push_back(Tile);
	}
}
void Map::LoadMatrix(LPCWSTR path)
{
	ifstream file;

	file.open(path);
	this->Matrix = new int* [TotalRowsOfMap];
	for (int iRow = 0; iRow < TotalRowsOfMap; iRow++)
	{
		this->Matrix[iRow] = new int[TotalColsOfMap];
		for (int iColumn = 0; iColumn < TotalColsOfMap; iColumn++)
		{
			file >> this->Matrix[iRow][iColumn];
		}

	}
	file.close();
}
void Map::Render(int x,int y)
{
	x = x / 16;
	y = y / 16;
	

	/*for (int iRow = 0; iRow < TotalRowsOfMap; iRow++)
	{
		for (int iColumn = 0; iColumn < TotalColsOfMap; iColumn++)
		{
			this->Tiles[this->Matrix[iRow][iColumn] - 1]->Draw(iColumn * TILE_WIDTH, iRow * TILE_HEIGHT,255);
		}
	}*/
	if (x - 16 < 0)
		x = 17;
	if (y - 16 < 0)
		y = 17;
	
		for (int iRow = y - 16; iRow < y + 16; iRow++)
		{
			if (iRow >= TotalRowsOfMap - 1) break;
			for (int iColumn = x - 16; iColumn < x + 16; iColumn++)
			{
				try
				{
					this->Tiles[this->Matrix[iRow][iColumn] - 1]->Draw(iColumn * TILE_WIDTH, iRow * TILE_HEIGHT, 255);
				}
				catch (const std::exception&)
				{
				}
				if (iColumn >= TotalColsOfMap-1)	break;
			}
			
			
		}
	
	
 	
}
int Map::GetMapHeiht()
{
	if (Height == 0)
		Height = TotalRowsOfMap * TILE_HEIGHT;
	return Height;
}
int Map::GetMapWidth()
{
	if (Width == 0)
	{
		Width = TotalColsOfMap * TILE_WIDTH;
	}
	return Width;
}
