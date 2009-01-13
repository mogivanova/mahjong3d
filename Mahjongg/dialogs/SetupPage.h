#pragma once

class CSetupPage
{

public:
	CSetupPage(void) {}

	~CSetupPage(void) {}

public:
	virtual void Show(UINT nCommand) = 0;
	virtual void Move(CRect& rect) = 0;
};
