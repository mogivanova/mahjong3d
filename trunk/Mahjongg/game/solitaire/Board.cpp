/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: Board.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 9.03.04 23:44 $
*
*================================================================================================*/

#include "stdafx.h"
#include "Board.h"
#include "SolitaireLayout.h"


CBoard::CBoard()
{
	m_pBoard = NULL;
	m_nDepth = 0;
	m_nWidth = 0;
	m_nHeight = 0;
}


CBoard::~CBoard()
{
	if (m_pBoard != NULL)
		delete [] m_pBoard;
}


void CBoard::Set(CSolitaireLayout& objLayout)
{
	SetDimensions(objLayout.GetWidth(), objLayout.GetHeight(), objLayout.GetDepth());

	for (unsigned int i = 0; i < objLayout.Size(); i++)
	{
		m_pBoard[i] = objLayout.m_pBoard[i];
	}
}


void CBoard::Clear()
{
	if (m_pBoard == NULL)
		return;

	for (UINT i = 0; i < Size(); i++)
		m_pBoard[i] = NO_TILE;
}
void CBoard::SetDimensions(unsigned char width, unsigned char height, unsigned char depth)
{
	_ASSERT(width > 0 && height > 0 && depth > 0);

	m_nWidth = width;
	m_nHeight = height;
	m_nDepth = depth;

	if (m_pBoard != NULL)
		delete [] m_pBoard;

	m_pBoard = new CTile[Size()];

	Clear();
}


CBoard& CBoard::operator=(const CBoard & brd)
{
	SetDimensions(brd.m_nWidth, brd.m_nHeight, brd.m_nDepth);

	for (unsigned int i = 0; i < Size(); i++)
	{
		m_pBoard[i] = brd.m_pBoard[i];
	}

	return *this;
}


bool CBoard::Save(CMJCryptStream& strOut)
{
	strOut.Write(m_nWidth);
	strOut.Write(m_nHeight);
	strOut.Write(m_nDepth);

	for (unsigned int i = 0; i < Size(); i++)
	{
		strOut.Write((unsigned char)m_pBoard[i]);
	}

	return true;
}


bool CBoard::Load(CMJCryptStream& strIn)
{
	unsigned char w, h, d;
	strIn.Read(&w);
	strIn.Read(&h);
	strIn.Read(&d);
	SetDimensions(w, h, d);

	for (unsigned int i = 0; i < Size(); i++)
	{
		unsigned char ch;
		strIn.Read(&ch);
		m_pBoard[i] = ch;
	}

	return true;
}

