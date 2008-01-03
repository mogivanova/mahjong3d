/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: Board.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 9.03.04 23:44 $
*
*================================================================================================*/

#pragma once

#include "Service\MJObjects.h"
#include "Service\MJCrypt.h"

class CSolitaireLayout;

///////////////////////////////////////////////////////////////////////////////////////////////////
//
inline bool TileEqual(CTile Tile1, CTile Tile2)
{
	// number tiles
	if (Tile1 < 35 && Tile2 < 35)
		return (Tile1 == Tile2);

	// wind tiles
	if (Tile1 >= 35 && Tile1 <= 38 && Tile2 >= 35 && Tile2 <= 38)
		return true;

	// flower tiles
	if (Tile1 >= 39 && Tile1 <= 42 && Tile2 >= 39 && Tile2 <= 42)
		return true;

	return false;
}

/////////////////////////////////////////////////////////////////////////////
//

class CSolitairePos
{

public:
	unsigned char i, j, k;

	CSolitairePos()
	{
		i = j = k = 0xFF;
	}

	CSolitairePos(unsigned char _i, unsigned char _j, unsigned char _k)
	{
		i = _i;
		j = _j;
		k = _k;
	}

	void Clear()
	{
		i = j = k = 0xFF;
	}

	bool IsClear()
	{
		return i == 0xFF;
	}

	CSolitairePos& operator=(const CSolitairePos& pos)
	{
		i = pos.i;
		j = pos.j;
		k = pos.k;
		return *this;
	}

	friend int operator ==(CSolitairePos& pos1, CSolitairePos& pos2);
	friend int operator !=(CSolitairePos& pos1, CSolitairePos& pos2);
};

inline int operator ==(CSolitairePos& pos1, CSolitairePos& pos2)
{
	return (pos1.i == pos2.i && pos1.j == pos2.j && pos1.k == pos2.k);
}

inline int operator !=(CSolitairePos& pos1, CSolitairePos& pos2)
{
	return !(pos1 == pos2);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//

class CSolitaireMove
{

public:
	CSolitairePos m_pos1;
	CTile m_Tile1;
	CSolitairePos m_pos2;
	CTile m_Tile2;

public:
	inline CSolitaireMove();
	inline CSolitaireMove(CSolitairePos& pos1, CTile tile1, CSolitairePos pos2, CTile tile2);
	inline CSolitaireMove& operator=(const CSolitaireMove& rec);

	inline bool Save(CMJCryptStream& stream);
	inline bool Load(CMJCryptStream& stream);
};

inline CSolitaireMove::CSolitaireMove()
{
	m_Tile1 = m_Tile2 = 0xFE;
}

inline CSolitaireMove::CSolitaireMove(CSolitairePos& pos1, CTile tile1, CSolitairePos pos2, CTile tile2)
{
	m_pos1 = pos1;
	m_Tile1 = tile1;
	m_pos2 = pos2;
	m_Tile2 = tile2;
}

inline CSolitaireMove& CSolitaireMove::operator=(const CSolitaireMove & rec)
{
	m_pos1 = rec.m_pos1;
	m_pos2 = rec.m_pos2;
	m_Tile1 = rec.m_Tile1;
	m_Tile2 = rec.m_Tile2;
	return *this;
}

inline bool CSolitaireMove::Save(CMJCryptStream& stream)
{
	stream.Write(m_pos1.i);
	stream.Write(m_pos1.j);
	stream.Write(m_pos1.k);
	stream.Write(m_Tile1);

	stream.Write(m_pos2.i);
	stream.Write(m_pos2.j);
	stream.Write(m_pos2.k);
	stream.Write(m_Tile2);

	return true;
}

inline bool CSolitaireMove::Load(CMJCryptStream& stream)
{
	stream.Read(&m_pos1.i);
	stream.Read(&m_pos1.j);
	stream.Read(&m_pos1.k);
	stream.Read(&m_Tile1);

	stream.Read(&m_pos2.i);
	stream.Read(&m_pos2.j);
	stream.Read(&m_pos2.k);
	stream.Read(&m_Tile2);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//

class CBoard
{

protected:
	CTile* m_pBoard;
	unsigned char m_nWidth, m_nHeight, m_nDepth;

public:
	CBoard();
	~CBoard();

	void Clear();

	unsigned char GetWidth()
	{
		return m_nWidth;
	}

	unsigned char GetHeight()
	{
		return m_nHeight;
	}

	unsigned char GetDepth()
	{
		return m_nDepth;
	}

	CTile& GetTile(unsigned char i, unsigned char j, unsigned char k);
	CTile& GetTile(CSolitairePos& pos);

	virtual void Set(CSolitaireLayout& objLayout);

	bool Save(CMJCryptStream& strOut);
	bool Load(CMJCryptStream& strIn);

	CBoard& operator=(const CBoard& brd);

protected:
	unsigned int Size()
	{
		return ((unsigned int)m_nWidth) * ((unsigned int)m_nHeight) * ((unsigned int)m_nDepth);
	}

	void SetDimensions(unsigned char width, unsigned char height, unsigned char depth);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//
inline CTile& CBoard::GetTile(CSolitairePos& pos)
{
	return GetTile(pos.i, pos.j, pos.k);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
inline CTile& CBoard::GetTile(unsigned char i, unsigned char j, unsigned char k)
{
	_ASSERT( i < m_nWidth && j < m_nHeight && k < m_nDepth);

	return m_pBoard[ k * m_nWidth * m_nHeight + j * m_nWidth + i];
}
