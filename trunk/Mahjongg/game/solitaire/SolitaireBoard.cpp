/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: SolitaireBoard.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 12.03.04 0:46 $
*
*================================================================================================*/

#include "stdafx.h"
#include "SolitaireBoard.h"
#include "..\..\Service\random.h"


CSolitaireBoard::CSolitaireBoard()
{
}


CSolitaireBoard::~CSolitaireBoard()
{
}


void CSolitaireBoard::PlaceTiles()
{
	if (g_AppSettings.m_bSolvableBoards)
		PlaceTilesSolvable();
	else
		PlaceTilesSimple();
}


void CSolitaireBoard::PlaceTilesForDemo(CSimpleArray<CSolitairePos>& vecRemove)
{
	PlaceTilesSolvable(&vecRemove);
}


void CSolitaireBoard::PlaceTilesSolvable(CSimpleArray<CSolitairePos>* pvecRemove)
{
	QPROFILE_FUN(_T("CSolitaireBoard::PlaceTilesSolvable"));

	// count tiles
	unsigned int nTiles = CountTiles();

	if (nTiles != 144)
		return;

	CTile aTiles[144];

	unsigned int idx = 0;

	for (unsigned int i = 1; i <= 34; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			aTiles[idx++] = i;
		}
	}

	for ( int i = 35; i <= 42; i++)
	{
		aTiles[idx++] = i;
	}

	while (!FindPlaceable(aTiles, pvecRemove))
	{
	}
}


unsigned int CSolitaireBoard::CollectPos(CSimpleArray<CSolitairePos>& vecPos)
{
	vecPos.RemoveAll();

	for (int i = 0; i < m_nWidth; i++)
	{
		for (int j = 0; j < m_nHeight; j++)
		{
			for (int k = 0; k < m_nDepth; k++)
			{
				CSolitairePos pos(i, j, k);

				if (GetTile(pos) == EMPTY_TILE &&
						!CheckBlockLeft(pos, false) && !CheckBlockRight(pos, false) && !CheckBlockDown(pos, false))
				{
					vecPos.Add(pos);
				}
			}
		}
	}

	return vecPos.GetSize();
}


bool CSolitaireBoard::FindPlaceable(CTile* pTiles, CSimpleArray<CSolitairePos>* pvecRemove, int nPlaced)
{
	if (nPlaced == 144)
		return true;

	CSimpleArray<CSolitairePos> vecPos;

	CollectPos(vecPos);

	// get collected postions count
	int nPositions = vecPos.GetSize();

	if (nPositions < 2) // not enough free place
		return false;

	unsigned int idx1, idx2;

	int nTries = 1;

	int i = 0;

	for (i = 0; i < nTries; i++)
	{
		// get tiles to place
		unsigned int iTile1, iTile2;

		do
		{
			iTile1 = randomMT(144);
		}
		while (pTiles[iTile1] == EMPTY_TILE);

		do
		{
			iTile2 = randomMT(144);
		}
		while (iTile2 == iTile1 || pTiles[iTile2] == EMPTY_TILE || !TileEqual(pTiles[iTile1], pTiles[iTile2]));

		//
		CTile objTile1 = pTiles[iTile1];

		CTile objTile2 = pTiles[iTile2];

		pTiles[iTile1] = EMPTY_TILE;

		pTiles[iTile2] = EMPTY_TILE;

		// get positions
		idx1 = randomMT(nPositions);

		do
		{
			idx2 = randomMT(nPositions);
		}
		while (idx2 == idx1);

		//
		GetTile(vecPos[idx1]) = objTile1;

		GetTile(vecPos[idx2]) = objTile2;

		if (FindPlaceable(pTiles, pvecRemove, nPlaced + 2))
		{
			break;
		}

		// rollback
		pTiles[iTile1] = objTile1;

		pTiles[iTile2] = objTile2;

		GetTile(vecPos[idx1]) = EMPTY_TILE;

		GetTile(vecPos[idx2]) = EMPTY_TILE;
	}

	if (i == nTries)
		return false; // failed to place

	if (pvecRemove != NULL)
	{
		pvecRemove->Add(vecPos[idx1]);
		pvecRemove->Add(vecPos[idx2]);
	}

	return true;
}


bool CSolitaireBoard::CheckBlockLeft(CSolitairePos pos, bool bHasEmpty)
{
	// left most tile does not block anything
	if (pos.i <= 1)
		return false;

	for (signed char j0 = -1; j0 <= 1; j0++)
	{
		if ((unsigned)(pos.j + j0) >= m_nHeight)
			continue;

		if (GetTile(pos.i - 2, pos.j + j0, pos.k) == EMPTY_TILE)
		{
			// if we block something with empty tile
			// return true
			if (CheckBlockLeft(CSolitairePos(pos.i - 2, pos.j + j0, pos.k), true))
				return true;
		}
		else if (GetTile(pos.i - 2, pos.j + j0, pos.k) != NO_TILE)
		{
			// if we had empty tiles on this track and met not empty
			// so we have block line. return true
			if (bHasEmpty)
				return true;
		}
	}

	return false;
}
bool CSolitaireBoard::CheckBlockRight(CSolitairePos pos, bool bHasEmpty)
{
	// right most tile does not block anything
	if (pos.i >= m_nWidth - 2)
		return false;

	for (signed char j0 = -1; j0 <= 1; j0++)
	{
		if ((unsigned)(pos.j + j0) >= m_nHeight)
			continue;

		if (GetTile(pos.i + 2, pos.j + j0, pos.k) == EMPTY_TILE)
		{
			// if we block something with empty tile
			// return true
			if (CheckBlockRight(CSolitairePos(pos.i + 2, pos.j + j0, pos.k), true))
				return true;
		}
		else if (GetTile(pos.i + 2, pos.j + j0, pos.k) != NO_TILE)
		{
			// if we had empty tiles on this track and met not empty
			// so we have block line. return true
			if (bHasEmpty)
				return true;
		}
	}

	return false;
}
bool CSolitaireBoard::CheckBlockDown(CSolitairePos pos, bool bHasEmpty)
{
	// tiles from lowest level do not block
	if (pos.k == 0)
		return false;

	for (signed char i0 = -1; i0 <= 1; i0++)
	{
		for (signed char j0 = -1; j0 <= 1; j0++)
		{
			if ((unsigned)(pos.i + i0) >= m_nWidth || (unsigned)(pos.j + j0) >= m_nHeight)
				continue;

			if (GetTile(pos.i + i0, pos.j + j0, pos.k - 1) == EMPTY_TILE)
				return true;
		}
	}

	return false;
}


void CSolitaireBoard::PlaceTilesSimple()
{
	QPROFILE_FUN(_T("CSolitaireBoard::PlaceTilesSimple"));

	unsigned int i, j, k;
	unsigned int N;

	unsigned int nTiles = CountTiles();

	if (nTiles != 144)
		return;

	unsigned int nPlaced = 0;

	for (N = 1; N <= 34; N++)
	{
		for (int idx = 0; idx < 4; idx++)
		{
			do
			{
				i = (int)randomMT(m_nWidth);
				j = (int)randomMT(m_nHeight);
				k = (int)randomMT(m_nDepth);

			}
			while (GetTile(i, j, k) != EMPTY_TILE);

			GetTile(i, j, k) = N;

			nPlaced++;

			if (nPlaced == nTiles)
				return;
		}
	}

	for (N = 35; N <= 42; N++)
	{
		do
		{
			i = (int)randomMT(m_nWidth);
			j = (int)randomMT(m_nHeight);
			k = (int)randomMT(m_nDepth);

		}
		while (GetTile(i, j, k) != EMPTY_TILE);

		GetTile(i, j, k) = N;

		nPlaced++;

		if (nPlaced == nTiles)
			return;
	}
}


bool CSolitaireBoard::IsFree(CSolitairePos pos)
{
	return IsFree(pos.i, pos.j, pos.k);
}
bool CSolitaireBoard::IsFree(unsigned char i, unsigned char j, unsigned char k)
{
	if (GetTile(i, j, k) == NO_TILE)
		return false;

	signed char i0, j0;

	// check upper layer
	if (k < m_nDepth - 1)
	{
		for (i0 = -1; i0 <= 1; i0++)
		{
			for (j0 = -1; j0 <= 1; j0++)
			{
				if ((unsigned char)(i + i0) >= m_nWidth || (unsigned char)(j + j0) >= m_nHeight)
					continue;

				if (GetTile(i + i0, j + j0, k + 1) != NO_TILE)
					return false;
			}
		}
	}

	bool bClosedLeft = false;

	bool bClosedRight = false;

	for (j0 = -1; j0 <= 1; j0++)
	{
		if ((unsigned char)(j + j0) >= m_nHeight)
			continue;

		// check tiles on the left side
		if ((unsigned char)(i - 2) < m_nWidth && GetTile(i - 2, j + j0, k) != NO_TILE)
			bClosedLeft = true;

		// check tiles on the right side
		if ((unsigned char)(i + 2) < m_nWidth && GetTile(i + 2, j + j0, k) != NO_TILE)
			bClosedRight = true;
	}

	if (bClosedLeft && bClosedRight)
		return false;

	return true;
}


unsigned char CSolitaireBoard::CountTiles()
{
	unsigned char nReturn = 0;

	for (unsigned char i = 0; i < m_nWidth; i++)
	{
		for (unsigned char j = 0; j < m_nHeight; j++)
		{
			for (unsigned char k = 0; k < m_nDepth; k++)
			{
				if (GetTile(i, j, k) != NO_TILE)
					nReturn++;
			}
		}
	}

	return nReturn;
}


bool CSolitaireBoard::HaveMoreMoves()
{
	for (UINT i1 = 0; i1 < m_nWidth; i1++)
	{
		for (UINT j1 = 0; j1 < m_nHeight; j1++)
		{
			for (UINT k1 = 0; k1 < m_nDepth; k1++)
			{
				// check there is tile and it's free
				if (GetTile(i1, j1, k1) == NO_TILE)
					continue;

				if (!IsFree(i1, j1, k1))
					continue;
				// find a pair
				for (UINT i2 = 0; i2 < m_nWidth; i2++)
				{
					for (UINT j2 = 0; j2 < m_nHeight; j2++)
					{
						for (UINT k2 = 0; k2 < m_nDepth; k2++)
						{
							// there is a tile
							if (GetTile(i2, j2, k2) == NO_TILE)
								continue;

							// same position
							if (i1 == i2 && j1 == j2 && k1 == k2)
								continue;

							if (!IsFree(i2, j2, k2))
								continue;

							// have free pair of tiles
							if (TileEqual(GetTile(i1, j1, k1), GetTile(i2, j2, k2)))
								return true;
						}
					}
				}

			}
		}
	}

	return false;
}

