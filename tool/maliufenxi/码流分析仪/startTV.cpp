#include"stdafx.h"
#include"startTV.h"

SDL_Rect rect;
void startTVinit(LPVOID lpParam)
{
	CCTsDlg * dlg;

	char name[256] = { 0 };

	dlg = (CCTsDlg *)lpParam;


	
	CWnd* pWnd = dlg->GetDlgItem(IDC_pic);
	sprintf(name, "SDL_WINDOWID=0x%lx", pWnd->GetSafeHwnd());
	
	SDL_putenv(name);

	RECT    rc;
	pWnd->GetWindowRect(&rc);
	rect.x = rc.left;
	rect.y = rc.top;
	rect.w = rc.right - rc.left;
	rect.h = rc.bottom - rc.top;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER))
	{
		AfxMessageBox(L"Could not initialize SDL ");
		exit(1);
	}

	SDL_Surface* pScreen = 0;
	pScreen = SDL_SetVideoMode(rect.w, rect.h, 0, 0);    // Creat a SDL window, and get the window's surface.
	SDL_Surface* pShownBMP = 0;
	pShownBMP = SDL_LoadBMP("TV.bmp"); // Load a BMP file, and convert it as a surface.
	SDL_BlitSurface(pShownBMP, 0, pScreen, 0);
	SDL_Flip(pScreen);//	SDL_FreeSurface(pScreen);
//	Sleep(100);
	
}

