/*=================================================================================================
*
*  Copyright (c) 1998-2004 Thinking Thing Software
*  All rights reserved.
*
*  Project: Amazing Mahjongg 3D
*  $Workfile: Fireworks.h $
*  $Author: andrey-ovcharov $
*  $Revision: 1.1 $
*  $Modtime: 9.03.04 23:44 $
*
*================================================================================================*/

#pragma once

#define INITIALSPD 21000
#define EXPLODEHEIGHT 200

#define CONST_G (0.6f)

enum FW_STATE
{
	FW_READY,
	FW_FIRED,
	FW_EXPLODED,
	FW_STOPPED
};

#define SPARKS 300
#define BULLETS 5

class CFireWorks
{

protected:

	class CFWSpark
	{

	public:
		GLfloat x;
		GLfloat y;
		GLfloat vx;
		GLfloat vy;

		long time;

	public:
		CFWSpark()
		{
			x = y = vx = vy = 0.0f;
			time = 0;
		}

		void Draw();
	};

	class CFWBullet
	{

	public:
		CFWSpark Spark[SPARKS];

		GLfloat ExplodeHeight;
		int color;
		int upcount;
		int readytoexplode;
		FW_STATE state;

		CFireWorks* m_pFireworks;

	public:
		CFWBullet()
		{
			ExplodeHeight = 0.0f;
			color = upcount = readytoexplode = 0;
		}

		void Init();
		void MoveFired();
		void MoveExploded();
		void Explode();

		void Draw();
	};

public:
	CFWBullet Bullet[BULLETS];

	GLfloat m_sin[3600];
	GLfloat m_cos[3600];

public:
	CFireWorks();

	void Init();
	void Step();

	void Draw();
};

