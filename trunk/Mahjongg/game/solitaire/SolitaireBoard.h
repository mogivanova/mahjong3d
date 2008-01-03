/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: SolitaireBoard.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 9.03.04 23:44 $
*
*================================================================================================*/

#pragma once

#include "Board.h"
#include "SolitaireLayout.h"

class CSolitaireBoard :
			public CBoard
{

public:
	CSolitaireBoard();
	~CSolitaireBoard();

	bool HaveMoreMoves();
	unsigned char CountTiles();

	bool IsFree(unsigned char i, unsigned char j, unsigned char k);
	bool IsFree(CSolitairePos pos);

	void PlaceTiles();
	void PlaceTilesForDemo(CSimpleArray<CSolitairePos>& vecRemove);

protected:
	void PlaceTilesSimple();
	void PlaceTilesSolvable(CSimpleArray<CSolitairePos>* pvecRemove = NULL);

	bool FindPlaceable(CTile* pTiles, CSimpleArray<CSolitairePos>* pvecRemove, int nPlaced = 0);

	bool CheckBlockLeft(CSolitairePos pos, bool bHasEmpty);
	bool CheckBlockRight(CSolitairePos pos, bool bHasEmpty);
	bool CheckBlockDown(CSolitairePos pos, bool bHasEmpty);

	unsigned int CollectPos(CSimpleArray<CSolitairePos>& vecPos);
};

