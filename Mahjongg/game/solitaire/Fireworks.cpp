/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: Fireworks.cpp $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 12.03.04 0:49 $
*
*================================================================================================*/

#include "stdafx.h"

#include "..\..\Service\random.h"
#include "fireworks.h"
#include "..\..\resource.h"

/////////////////////////////////////////////////////////////////////////

CRect scrRect;

/////////////////////////////////////////////////////////////////////////
//
void CFireWorks::CFWSpark::Draw(COpenGLDriver* pDriver)
{
	GLfloat sx = x / 150.0f;
	GLfloat sy = y / 150.0f - 2.1f;

	pDriver->Vertex(sx, sy, 0);
}

/////////////////////////////////////////////////////////////////////////
//
void CFireWorks::CFWBullet::Init()
{
	int nX = randomMT(16000) - 8000;

	GLfloat vx = (GLfloat)(nX) / 1000.0f;
	GLfloat vy = (GLfloat)(INITIALSPD + randomMT(INITIALSPD / 2)) / 1000.0f;

	for (int i = 0; i < SPARKS; i++)
	{
		Spark[i].x = 0.0f;
		Spark[i].y = 0.0f;
		Spark[i].vx = vx;
		Spark[i].vy = vy;
	}

	color = randomMT(4);

	ExplodeHeight = (GLfloat)(randomMT(100000) + 350000) / 1000.0f;
	state = FW_FIRED;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
void CFireWorks::CFWBullet::MoveFired()
{
	readytoexplode = 0;

	upcount = 0;

	for (int i = 0; i < SPARKS; i++)
	{
		Spark[i].vy -= CONST_G;

		Spark[i].x = Spark[i].x + Spark[i].vx;
		Spark[i].y = Spark[i].y + Spark[i].vy;

		if (Spark[i].y > ExplodeHeight || Spark[i].vy < 0.0f)
			readytoexplode = 1;

		if (Spark[i].y > 0.0f)
			upcount++;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
void CFireWorks::CFWBullet::Explode()
{
	for (int i = 0; i < SPARKS; i++)
	{
		GLfloat spd = (GLfloat)randomMT(8000) / 1000.0f;

		int nAngle = randomMT(3600);

		Spark[i].vx = (spd * m_pFireworks->m_cos[nAngle]);
		Spark[i].vy = (spd * m_pFireworks->m_sin[nAngle]);

		Spark[i].time = 32;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
void CFireWorks::CFWBullet::MoveExploded()
{
	upcount = 0;

	for (int i = 0; i < SPARKS; i++)
	{
		Spark[i].vy -= CONST_G;

		Spark[i].x = Spark[i].x + Spark[i].vx;
		Spark[i].y = Spark[i].y + Spark[i].vy;

		if (Spark[i].y > 0.0f && Spark[i].time > 0.0f)
			upcount++;

		if (Spark[i].time > 0)
			Spark[i].time--;
	}
}


void CFireWorks::CFWBullet::Draw(COpenGLDriver* pDriver)
{
	for (int i = 0; i < SPARKS; i++)
	{
		switch (state)
		{

		case FW_FIRED:
			pDriver->Color(255, 255, 255);
			Spark[i].Draw(pDriver);
			break;

		case FW_EXPLODED:

			switch (color)
			{

			case 0:
				pDriver->Color((GLubyte)Spark[i].time << 3, (GLubyte)Spark[i].time << 3, (GLubyte)Spark[i].time << 3);
				Spark[i].Draw(pDriver);
				break;

			case 1:
				pDriver->Color(0, (GLubyte)Spark[i].time << 3, 0);
				Spark[i].Draw(pDriver);
				break;

			case 2:
				pDriver->Color((GLubyte)Spark[i].time << 3, 0, 0);
				Spark[i].Draw(pDriver);
				break;

			case 3:
				pDriver->Color((GLubyte)Spark[i].time << 3, (GLubyte)Spark[i].time << 3, 0);
				Spark[i].Draw(pDriver);
				break;
			}

			break;
		}
	}
}

CFireWorks::CFireWorks()
{
	for (int i = 0; i < 3600; i++)
	{
		GLfloat a = (GLfloat)i;
		m_sin[i] = sin(2.0f  * 3.14159f * a / 3600.0f);
		m_cos[i] = cos(2.0f  * 3.14159f * a / 3600.0f);
	}
}

void CFireWorks::Init()
{
	for (int i = 0; i < BULLETS; i++)
	{
		Bullet[i].Init();
		Bullet[i].m_pFireworks = this;
	}
}

void CFireWorks::Step()
{
	int upcount = 0;

	for (int i = 0; i < BULLETS; i++)
	{
		switch (Bullet[i].state)
		{

		case FW_READY:
			Bullet[i].Init();
			break;

		case FW_FIRED:
			Bullet[i].MoveFired();

			if (Bullet[i].readytoexplode)
			{
				Bullet[i].Explode();
				Bullet[i].state = FW_EXPLODED;
				Sound(IDR_EXPLODE);
			}

			break;

		case FW_EXPLODED:
			Bullet[i].MoveExploded();
			break;

		case FW_STOPPED:
			Bullet[i].state = FW_READY;
			break;
		}

		upcount += Bullet[i].upcount;
	}

	if (upcount == 0)
	{
		for (int i = 0; i < BULLETS; i++)
		{
			Bullet[i].Init();
			Sound(IDR_FIRE);
		}
	}
}

void CFireWorks::Draw(COpenGLDriver* pDriver)
{
	QPROFILE_FUN(_T("CFireWorks::Draw"));
	/*
	*/
	pDriver->NewList(800, GL_COMPILE);
	pDriver->Begin(GL_POINTS);
	{
		for (int i = 0; i < BULLETS; i++)
		{
			Bullet[i].Draw(pDriver);
		}
	}

	pDriver->End();
	pDriver->EndList();

	pDriver->CallList(800);
}

/*
void FireWork::Draw(HDC hdc, CRect &rect)
{
	for(int i=0; i<BULLETS; i++)
	{
		Bullet[i].Draw(hdc, rect);
	}
}

void FireWork::Clear(HDC hdc, CRect &rect)
{
	for(int i=0; i<BULLETS; i++)
	{
		Bullet[i].Clear(hdc, rect);
	}
}
*/