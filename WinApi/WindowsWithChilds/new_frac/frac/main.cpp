#include <windows.h>
#include <set>
#include <new>
#include "main.h"
#include "resource2.h"
#include <commctrl.h>       // must have
#pragma comment( lib, "comctl32.lib" )  // must have
//#include "resource.h"
#include <d3d9.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#pragma comment (lib, "d3d9.lib")
//#pragma comment (lib, "d3dx9.lib")
#include <strsafe.h>
#pragma warning( default : 4996 )
#include<vector>
#include "fractal.h"
#include <cmath>
//#include "bpoint.h"
#define PI 3.14159265
#define ID_BUTTON 3001
#define IDC_DRAW 3002
#define DELETE_MSG WM_USER + 1
#define DRAW_MSG WM_USER + 2


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; // Buffer to hold Vertices

// A structure for our custom vertex type
struct CUSTOMVERTEX
{
	FLOAT x, y, z, rhw; // The transformed position for the vertex
	DWORD color;        // The vertex color
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

//std::vector<std::pair<float, float>> GraphicsPoints ;
int GraphicsPointsSize;
//std::vector<float> Tensions;
CUSTOMVERTEX* Vertices = new CUSTOMVERTEX[2];

HWND hMainDlg, TempChild, hWndDirectX;
HINSTANCE hInst;
LPRECT lpMainDialogRect = new RECT;
LPRECT lpMainDialogRectChild = new RECT;
LPRECT lpDesktopWindow = new RECT;



typedef int (*ColorFunc)(float, float, float, float, int);
typedef float (*AlphaFunc)(float, float, float);
//int redraw = 1;
int size = 0;
int nCmdSh = 0 ;
int switcherAlpha = 0;
wchar_t buff[256];
char cbuff[256];
std::set<HWND> basicPointsWindows;
int temp_background = 0;
float temp_plitecolor = 0;
//Fractal f;

float toAlpha1(float x, float max_temp, float min_temp)
{
	return 1.0f;
	//return min_temp*min_temp/(x*x);
}

int toColor1(float x, float max_temp, float min_temp, float gamma, int shift)
{
	float temp = min_temp/x;
	return temp_plitecolor*(int)((0xFF-shift)*pow(temp, gamma)+shift);
}

int toColor2(float x, float max_temp, float min_temp, float gamma, int shift)
{
	float temp = x/max_temp;
	return temp_plitecolor*(int)((0xFF-shift)*pow(temp, gamma)+shift);
}

int toColor3(float x, float max_temp, float min_temp, float gamma, int shift)
{
	float temp = min_temp/x;
	return 0x010101*(int)((0xFF-shift)*pow(temp, gamma)+shift);
}

int toColor4(float x, float max_temp, float min_temp, float gamma, int shift)
{
	float temp = x/max_temp;
	return 0x010101*(int)((0xFF-shift)*pow(temp, gamma)+shift);
}

int toColor5(float x, float max_temp, float min_temp, float gamma, int shift)
{
	float temp = min_temp/x;
	return 0x010101*(int)((0xFF-shift)*pow(temp, gamma)+shift);
}

int toColor6(float x, float max_temp, float min_temp, float gamma, int shift)
{
	float temp = min_temp/x;
	return  0x010000 * (int)((0xFF-shift)*floor((2*atan(1/(x*x))/PI)*0xFF)+shift);
}

int toColor7(float x, float max_temp, float min_temp, float gamma, int shift)
{
	float temp = min_temp/x;
	return 0x010000*(int)((0xFF-shift)*pow(temp, gamma)+shift);
}

int toColor8(float x, float max_temp, float min_temp, float gamma, int shift)
{
	float temp = x/max_temp;
	return 0x010000 * (int)(((0xFF-shift)*pow(temp, gamma))+shift);
}



ColorFunc toColor=toColor1;
//AlphaFunc toAlpha=toAlpha1;

void SetBufferPoints( const std::vector<std::pair<float, float>> &GraphicsPoints)
{
	GraphicsPointsSize = GraphicsPoints.size();
	delete[] Vertices;
	Vertices = new CUSTOMVERTEX[GraphicsPointsSize];
	for(int i=0; i<GraphicsPointsSize; i++)
	{
		Vertices[i].x = GraphicsPoints[i].first ;
		Vertices[i].y = GraphicsPoints[i].second ;
		Vertices[i].z = 0.5f;
		Vertices[i].rhw = 1.0 ;
		Vertices[i].color = 0xFFFFFF ;
	}
};

void SetBufferPointsW( const std::vector<std::pair<float, float>> &GraphicsPoints, const std::vector< float> &Tensions)
{
	GraphicsPointsSize = GraphicsPoints.size();
	float temp_max = *max_element(Tensions.begin(),Tensions.end());
	float temp_min = *min_element(Tensions.begin(),Tensions.end());
	int shift = SendDlgItemMessage(hMainDlg, IDC_COLORINTENSE, TBM_GETPOS, 0,0);
	GetDlgItemText(hMainDlg, IDC_GAMMACOEF, buff, 256);
	float gamma = _wtof(buff);
	
	delete[] Vertices;
	labelVerticesW:
	try{
	Vertices = new CUSTOMVERTEX[GraphicsPointsSize];
	}
	catch(...)
	{
		goto labelVerticesW;
	}
	for(int i=0; i<GraphicsPointsSize; i++)
	{
		Vertices[i].x = GraphicsPoints[i].first ;
		Vertices[i].y = GraphicsPoints[i].second ;
		Vertices[i].z = 0.5f;

		Vertices[i].rhw =  1.0f;
		Vertices[i].color = toColor(Tensions[i], temp_max, temp_min, gamma, shift);
	}
}

void deleteBufferPoints()
{
	delete[] Vertices;
}



//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D( HWND hWnd )
{
	// Create the D3D object.
	if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return E_FAIL;

	// Set up the structure used to create the D3DDevice
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	// Create the D3DDevice
	if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice ) ) )
	{
		return E_FAIL;
	}

	// Device state would normally be set here

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: InitVB()
// Desc: Creates a vertex buffer and fills it with our Vertices. The vertex
//       buffer is basically just a chuck of memory that holds Vertices. After
//       creating it, we must Lock()/Unlock() it to fill it. For indices, D3D
//       also uses index buffers. The special thing about vertex and index
//       buffers is that they can be created in device memory, allowing some
//       cards to process them in hardware, resulting in a dramatic
//       performance gain.
//-----------------------------------------------------------------------------
HRESULT InitVB()
{
	//Initialize three Vertices for rendering a triangle
	//CUSTOMVERTEX Vertices[] =
	//{
	//    { 150.0f,  50.0f, 0.5f, 1.0f, 0xffff0000, }, // x, y, z, rhw, color
	//    { 250.0f, 250.0f, 0.5f, 1.0f, 0xff00ff00, },
	//    {  50.0f, 250.0f, 0.5f, 1.0f, 0xff00ffff, },
	//};

	// Create the vertex buffer. Here we are allocating enough memory
	// (from the default pool) to hold all our 3 custom Vertices. We also
	// specify the FVF, so the vertex buffer knows what data it contains.

	if( FAILED( g_pd3dDevice->CreateVertexBuffer( GraphicsPointsSize  * sizeof( CUSTOMVERTEX ),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL ) ) )
	{
		return E_FAIL;
	}

	// Now we fill the vertex buffer. To do this, we need to Lock() the VB to
	// gain access to the Vertices. This mechanism is required becuase vertex
	// buffers may be in device memory.
	VOID* pVertices;
	if( FAILED( g_pVB->Lock( 0, GraphicsPointsSize * sizeof( CUSTOMVERTEX ), ( void** )&pVertices, 0 ) ) )
		return E_FAIL;
	memcpy( pVertices, Vertices, GraphicsPointsSize * sizeof( CUSTOMVERTEX ) );
	int temp = sizeof( Vertices );
	g_pVB->Unlock();

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Cleanup()
{
	if( g_pVB != NULL )
		g_pVB->Release();

	if( g_pd3dDevice != NULL )
		g_pd3dDevice->Release();

	if( g_pD3D != NULL )
		g_pD3D->Release();
}




//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------


VOID Render()
{
	// Clear the backbuffer to a blue color
	g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, (D3DCOLOR)(int)temp_background, 1.0f, 0 );

	// Begin the scene
	if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
	{
		// Draw the triangles in the vertex buffer. This is broken into a few
		// steps. We are passing the Vertices down a "stream", so first we need
		// to specify the source of that stream, which is our vertex buffer. Then
		// we need to let D3D know what vertex shader to use. Full, custom vertex
		// shaders are an advanced topic, but in most cases the vertex shader is
		// just the FVF, so that D3D knows what type of Vertices we are dealing
		// with. Finally, we call DrawPrimitive() which does the actual rendering
		// of our geometry (in this case, just one triangle).
		g_pd3dDevice->SetStreamSource( 0, g_pVB, 0, sizeof( CUSTOMVERTEX ) );
		g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
		g_pd3dDevice->DrawPrimitive( D3DPT_POINTLIST, 0, GraphicsPointsSize  );

		// End the scene
		g_pd3dDevice->EndScene();
	}

	// Present the backbuffer contents to the display
	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

void convertfloatToWchar(float input, wchar_t* output)
{
	//memset(buff,0,sizeof(wchar_t)*256);
	//char cbuff[256];
	_gcvt(input, 7, cbuff);
	MultiByteToWideChar(
		CP_UTF8,
		0,
		cbuff,
		256,
		output,
		256
		);
	//memset(buff,0,sizeof(wchar_t)*256);
}

void SetBasicPoint(HWND &hWnd, float a11,float a12,float a21,float a22,float xCoord,float yCoord )
{
	//wchar_t buff[256];
	
	//char cbuff[256];
	//memset(buff,0,sizeof(wchar_t)*256);
	//sprintf(a11,buff,256);
	//sprintf( buff, "%e", a11 );
	//wcstof( const wchar_t* str, wchar_t** str_end )

	//FormatFloat
	//memset(buff,0,sizeof(wchar_t)*256);
	convertfloatToWchar(a11, buff);
	SetDlgItemText(hWnd,IDC_A11, buff);
	//memset(buff,0,sizeof(wchar_t)*256);
	convertfloatToWchar(a12, buff);
	SetDlgItemText(hWnd,IDC_A12,buff);
	convertfloatToWchar(a21, buff);
	SetDlgItemText(hWnd,IDC_A21,buff);
	convertfloatToWchar(a22, buff);
	SetDlgItemText(hWnd,IDC_A22,buff);
	convertfloatToWchar(xCoord, buff);
	SetDlgItemText(hWnd,IDC_X,buff);
	convertfloatToWchar(yCoord, buff);
	SetDlgItemText(hWnd,IDC_Y,buff);
	
}

void GetBasicPoint(const HWND &hWnd, BasicPoint &bpoint )
{
	//wchar_t buff[256];
	float a11, a12, a21, a22, xCoord, yCoord;
	Extpair extpair;
	GetDlgItemText(hWnd,
		IDC_A11,
		buff,
		256);
	a11 = _wtof(buff);
	GetDlgItemText(hWnd,
		IDC_A12,
		buff,
		256);
	a12 = _wtof(buff);
	GetDlgItemText(hWnd,
		IDC_A21,
		buff,
		256);
	a21 = _wtof(buff);
	GetDlgItemText(hWnd,
		IDC_A22,
		buff,
		256);
	a22 = _wtof(buff);
	GetDlgItemText(hWnd,
		IDC_X,
		buff,
		256);
	xCoord = _wtof(buff);
	GetDlgItemText(hWnd,
		IDC_Y,
		buff,
		256);
	yCoord = _wtof(buff);

	extpair.xKoord = xCoord;
	extpair.yKoord = yCoord;
	bpoint.point = extpair;
	bpoint.transform.oo = a11;
	bpoint.transform.ot = a12;
	bpoint.transform.to = a21;
	bpoint.transform.tt = a22;			
}


void AddBasicPoint(const HWND &hWnd, Fractal &fractal )
{
	//wchar_t buff[256];
	float a11, a12, a21, a22, xCoord, yCoord;
	Extpair extpair;
	GetDlgItemText(hWnd,
		IDC_A11,
		buff,
		256);
	a11 = _wtof(buff);
	GetDlgItemText(hWnd,
		IDC_A12,
		buff,
		256);
	a12 = _wtof(buff);
	GetDlgItemText(hWnd,
		IDC_A21,
		buff,
		256);
	a21 = _wtof(buff);
	GetDlgItemText(hWnd,
		IDC_A22,
		buff,
		256);
	a22 = _wtof(buff);
	GetDlgItemText(hWnd,
		IDC_X,
		buff,
		256);
	xCoord = _wtof(buff);
	GetDlgItemText(hWnd,
		IDC_Y,
		buff,
		256);
	yCoord = _wtof(buff);

	extpair.xKoord = xCoord;
	extpair.yKoord = yCoord;
	fractal.addpoint(xCoord, yCoord, a11, a12, a21, a22);
	fractal.fracset.push_back(extpair);

}




BOOL CALLBACK ChildDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM) {
	//wchar_t buff[256];
	double pos;
	static double  temp_pos = 100.0; 
	int randSize = 7;
	double temp;
	double temp_ten;
	double a11, a12, a21, a22, xCoord, yCoord;
	BasicPoint bpoint;
	switch(uMsg) {
	case WM_ACTIVATE:
		TempChild = hWnd;
		temp_pos = (100.0 - SendDlgItemMessage(hWnd, IDC_TENSION, TBM_GETPOS, 0,0));
		break;
	case WM_VSCROLL:

		switch(LOWORD(wParam))
		{

			//При любом изменении положения движка, меняем громкость
		case TB_LINEUP:
		case TB_LINEDOWN:
		case TB_PAGEUP:
		case TB_PAGEDOWN:
		case TB_TOP:
		case TB_BOTTOM:
		case TB_THUMBPOSITION:
			pos= (100.0 - SendDlgItemMessage(hWnd, IDC_TENSION, TBM_GETPOS, 0,0));
			GetBasicPoint(hWnd , bpoint);
			SetBasicPoint(hWnd, bpoint.transform.oo*pos/temp_pos ,bpoint.transform.ot*pos/temp_pos, bpoint.transform.to*pos/temp_pos, bpoint.transform.tt*pos/temp_pos, bpoint.point.xKoord, bpoint.point.yKoord );
			GetBasicPoint(hWnd , bpoint);
			temp_ten = bpoint.transform.SetTension();
	        convertfloatToWchar(temp_ten, buff);
	        SetDlgItemText(hWnd,IDC_TENSIONCOEFF,buff);
			temp_pos = (100.0 - SendDlgItemMessage(hWnd, IDC_TENSION, TBM_GETPOS, 0,0));
			//SetDlgItemInt(hWnd, IDC_EDIT1, 100 - (int)pos*10,0);
			break;
		}


		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)) {
	case IDC_MATRIXTYPE1:
		GetBasicPoint(hWnd, bpoint);
		SetBasicPoint(hWnd, 1, 1, 1, 1, bpoint.point.xKoord, bpoint.point.yKoord);
		SendMessage(hWnd, WM_COMMAND, (WPARAM)IDC_CALIBRATE, 0);
		break;

	case IDC_MATRIXTYPE2:
		GetBasicPoint(hWnd, bpoint);
		SetBasicPoint(hWnd, 1, 1, 0, 1, bpoint.point.xKoord, bpoint.point.yKoord);
		SendMessage(hWnd, WM_COMMAND, (WPARAM)IDC_CALIBRATE, 0);
		break;

	case IDC_MATRIXTYPE3:
		GetBasicPoint(hWnd, bpoint);
		SetBasicPoint(hWnd, 1, 1, 1, 0, bpoint.point.xKoord, bpoint.point.yKoord);
		SendMessage(hWnd, WM_COMMAND, (WPARAM)IDC_CALIBRATE, 0);
		break;

	case IDC_MATRIXTYPE4:
		GetBasicPoint(hWnd, bpoint);
		SetBasicPoint(hWnd, 0, 1, 1, 1, bpoint.point.xKoord, bpoint.point.yKoord);
		SendMessage(hWnd, WM_COMMAND, (WPARAM)IDC_CALIBRATE, 0);
		break;

	case IDC_MATRIXTYPE5:
		GetBasicPoint(hWnd, bpoint);
		SetBasicPoint(hWnd, 1, 1, 1, 0, bpoint.point.xKoord, bpoint.point.yKoord);
		SendMessage(hWnd, WM_COMMAND, (WPARAM)IDC_CALIBRATE, 0);
		break;

	case IDC_MATRIXTYPE6:
		GetBasicPoint(hWnd, bpoint);
		SetBasicPoint(hWnd, 1, 0, 0, 1, bpoint.point.xKoord, bpoint.point.yKoord);
		SendMessage(hWnd, WM_COMMAND, (WPARAM)IDC_CALIBRATE, 0);
		break;

	case IDC_MATRIXTYPE7:
		GetBasicPoint(hWnd, bpoint);
		SetBasicPoint(hWnd, 0, 1, 1, 0, bpoint.point.xKoord, bpoint.point.yKoord);
		SendMessage(hWnd, WM_COMMAND, (WPARAM)IDC_CALIBRATE, 0);
		break;

		case IDC_CALIBRATE:
			SendMessage(GetDlgItem(hWnd, IDC_TENSION), TBM_SETPOS, TRUE, 30);
			GetBasicPoint(hWnd, bpoint);
			temp = bpoint.transform.SetTension();
			pos= 100.0 - SendDlgItemMessage(hWnd, IDC_TENSION, TBM_GETPOS, 0,0);
			temp_pos = pos;
			SetBasicPoint(hWnd, 0.7*bpoint.transform.oo/temp, 0.7*bpoint.transform.ot/temp, 0.7*bpoint.transform.to/temp, 0.7*bpoint.transform.tt/temp, bpoint.point.xKoord, bpoint.point.yKoord);
			GetBasicPoint(hWnd, bpoint);
			temp_ten = bpoint.transform.SetTension();
	        convertfloatToWchar(temp_ten, buff);
	        SetDlgItemText(hWnd,IDC_TENSIONCOEFF,buff);
			break;
		case IDC_RANDOM:
			GetBasicPoint(hWnd, bpoint);
			SetBasicPoint(hWnd, (rand() % randSize) - (randSize/2), (rand() % randSize) - (randSize/2), (rand() % randSize) - (randSize/2), (rand() % randSize) - (randSize/2), bpoint.point.xKoord, bpoint.point.yKoord);
			break;
		case IDOK:
		case IDCANCEL:
			EndDialog(hWnd, 0);
			basicPointsWindows.erase(hWnd);
			SendMessage(hMainDlg,DELETE_MSG,NULL,NULL);
			break;
		}
		break;

	case WM_CLOSE:
		EndDialog(hWnd, 0);
		basicPointsWindows.erase(hWnd);
		SendMessage(hMainDlg,DELETE_MSG,NULL,NULL);
		break;
	}

	return false;
}


BOOL CALLBACK MainDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM) {
	InitCommonControls();
	static Fractal f;
	int funcSwitcher = 0;
	void (*funcTemp)(std::list<ExtpairWT> &input);
	std::vector<std::pair<float, float>> GraphicsPoints;
	std::vector<float> Tensions;
	int xSizeChild = 100;
	int ySizeChild = 100;
	int xSizeCounter = 0;
	int ySizeCounter = 0;
	int temp_shift = 0;
	float temp_gamma = 1;
	int pos = 0;
	static int counter = 0;
	std::set<HWND>::iterator at;
	float a11, a12, a21, a22;
	float xCoord, yCoord;
	BasicPoint bpoint;
	Extpair extpair;
	Matrix matrix;
	int iteration;

	
	/*CheckRadioButton(
	hMainDlg,
	IDC_COMPLETE,
	IDC_BUILDING,
	IDC_COMPLETE);
	CheckRadioButton(
	hMainDlg,
	IDC_COMPLETEEPS,
	IDC_BUILDINGEPS,
	IDC_COMPLETEEPS);*/
	switch(uMsg) {
		case WM_CREATE:
		SetDlgItemText(hWnd,IDC_ITERATIONNUMBER,L"1");
		SendDlgItemMessage(hWnd, IDC_COLORINTENSE, TBM_SETRANGE, (WPARAM)1, (LPARAM)MAKELONG(0,100));
		SendDlgItemMessage(hWnd, IDC_GAMMA, TBM_SETRANGE, (WPARAM)1, (LPARAM)MAKELONG(0,100));
        SendMessage(GetDlgItem(hWnd, IDC_COLORINTENSE), TBM_SETPOS, TRUE, 0);
		SendMessage(GetDlgItem(hWnd, IDC_GAMMA), TBM_SETPOS, TRUE, 50);

		CheckDlgButton(
				hWnd,
				IDC_REDRAW,
				BST_CHECKED
				);

		/*temp_shift = SendDlgItemMessage(hWnd, IDC_COLORINTENSE, TBM_GETPOS, 0,0);
	        convertfloatToWchar(temp_shift, buff);
	        SetDlgItemText(hWnd,IDC_SHIFTCOEFF,buff);

			temp_gamma = pow((float)1.3, (float)( SendDlgItemMessage(hWnd, IDC_GAMMA, TBM_GETPOS, 0,0) - 50) / 10 );
	        convertfloatToWchar(temp_gamma, buff);
	        SetDlgItemText(hWnd,IDC_GAMMACOEF,buff);*/
		//CreateWindow(L"BUTTON",L"Draw",WS_CHILD|BS_DEFPUSHBUTTON,0,0,90,20,hWnd,(HMENU)IDC_DRAW,GetModuleHandle(NULL),NULL);
		break;
		case WM_HSCROLL:
		switch(LOWORD(wParam))
		{

			//При любом изменении положения движка, меняем громкость
		case TB_LINEUP:
		case TB_LINEDOWN:
		case TB_PAGEUP:
		case TB_PAGEDOWN:
		case TB_TOP:
		case TB_BOTTOM:
		case TB_THUMBPOSITION:
			
			temp_shift = SendDlgItemMessage(hWnd, IDC_COLORINTENSE, TBM_GETPOS, 0,0);
	        convertfloatToWchar(temp_shift, buff);
	        SetDlgItemText(hWnd,IDC_SHIFTCOEFF,buff);

			temp_gamma = pow((float)1.3, (float)( SendDlgItemMessage(hWnd, IDC_GAMMA, TBM_GETPOS, 0,0) - 50) / 10 );
	        convertfloatToWchar(temp_gamma, buff);
	        SetDlgItemText(hWnd,IDC_GAMMACOEF,buff);

			temp_background = (SendDlgItemMessage(hWnd, IDC_BACKGROUND, TBM_GETPOS, 0,0)*0xFFFFFF)/100;
	        convertfloatToWchar(temp_background, buff);
	        SetDlgItemText(hWnd,IDC_EDIT_BACKGROUND,buff);

			temp_plitecolor = (SendDlgItemMessage(hWnd, IDC_PLITECOLOR, TBM_GETPOS, 0,0)*0xFFFFFF)/100;
	        convertfloatToWchar(temp_plitecolor, buff);
	        SetDlgItemText(hWnd,IDC_EDIT_PLITECOLOR ,buff);
			
			break;
		}


		break;
		case DRAW_MSG:
				CheckDlgButton(
				hMainDlg,
				IDC_CHECKEPS,
				BST_UNCHECKED
				);

		   if(switcherAlpha == 0){ funcTemp = floorListExtWTStrong;}
		   else{funcTemp = floorListExtWTStrongInv;}
			
			if(IsDlgButtonChecked(hWnd, IDC_REDRAW))
			{
			iteration = GetDlgItemInt(hMainDlg, IDC_ITERATIONNUMBER, NULL, FALSE );
			f.fracsetW.clear();
			f.basicpoints.clear();
			//at = basicPointsWindows.begin();
			for(at = basicPointsWindows.begin(); at != basicPointsWindows.end(); at++)
			{
				AddBasicPoint( *at , f );
			}
			labelW:
			try{
			f.buildW(iteration, switcherAlpha);
			}
			//buildFractalPolygone(f);
			catch(...)
			{
				iteration -= 1;
				goto labelW;
			}
			}
	        funcTemp(f.fracsetW);
			wConvertFractalToPairVector(f, GraphicsPoints, Tensions);
			SetBufferPointsW( GraphicsPoints, Tensions);
			
			// Initialize Direct3D
			if( SUCCEEDED( InitD3D( hWndDirectX ) ) )
			{
				// Create the vertex buffer
				if( SUCCEEDED( InitVB() ) )
				{
					// Show the window
					Render();
					UpdateWindow( hWndDirectX );	
				}
			}
			CheckDlgButton(
				hMainDlg,
				IDC_CHECKEPS,
				BST_CHECKED
				);
				break;

	case WM_COMMAND:
		switch(LOWORD(wParam)) {
		case IDC_GAMMACALIBRATE:
			SendMessage(GetDlgItem(hWnd, IDC_GAMMA), TBM_SETPOS, TRUE, 50);
			temp_shift = SendDlgItemMessage(hWnd, IDC_COLORINTENSE, TBM_GETPOS, 0,0);
	        convertfloatToWchar(temp_shift, buff);
	        SetDlgItemText(hWnd,IDC_SHIFTCOEFF,buff);

			SendMessage(GetDlgItem(hWnd, IDC_COLORINTENSE), TBM_SETPOS, TRUE, 0);
			temp_gamma = pow((float)1.3, (float)( SendDlgItemMessage(hWnd, IDC_GAMMA, TBM_GETPOS, 0,0) - 50) / 10 );
	        convertfloatToWchar(temp_gamma, buff);
	        SetDlgItemText(hWnd,IDC_GAMMACOEF,buff);
			break;

		case IDC_PRINTEPS:
			CheckDlgButton(
				hMainDlg,
				IDC_CHECKEPS,
				BST_UNCHECKED
				);
			f.writetoEps();
			CheckDlgButton(
				hMainDlg,
				IDC_CHECKEPS,
				BST_CHECKED
				);
			break;
		case IDC_ADD:
			basicPointsWindows.insert(TempChild = CreateDialog(hInst, MAKEINTRESOURCE(IDD_CHILDDLG), (HWND)hMainDlg, ChildDlgProc));
			SendDlgItemMessage(TempChild, IDC_TENSION, TBM_SETRANGE, (WPARAM)1, (LPARAM)MAKELONG(0,90));
			//SendDlgItemMessage(TempChild, IDC_TENSION, TBM_SETPOS, (WPARAM)1, (LPARAM)5);
			GetWindowRect( TempChild, lpMainDialogRectChild );
			xSizeChild = lpMainDialogRectChild->right - lpMainDialogRectChild->left;
			ySizeChild = lpMainDialogRectChild->bottom - lpMainDialogRectChild->top;


			/*xSizeCounter = (lpDesktopWindow->right - lpMainDialogRect->right) / xSizeChild;
			ySizeCounter = lpDesktopWindow->bottom / ySizeChild;*/
			xSizeCounter = (lpMainDialogRect->right - lpMainDialogRect->left) / xSizeChild + 1;
			ySizeCounter = (lpDesktopWindow->bottom - lpMainDialogRect->bottom) / ySizeChild ;

			SetWindowPos( 
				TempChild, 
				HWND_BOTTOM,
				xSizeChild * ((counter / ySizeCounter ) % xSizeCounter), 
				ySizeChild *(counter % ySizeCounter )+lpMainDialogRect->bottom -  lpMainDialogRect->top, 
				xSizeChild  , 
				ySizeChild  , 
				SWP_SHOWWINDOW
				); 

			size = basicPointsWindows.size();
			SetDlgItemText(hWnd,IDC_POINTSNUMBER,_itow(size,buff,256));

			//ShowWindow(TempChild,nCmdSh);
			++counter;

			SetDlgItemText(TempChild,IDC_A11,L"0.5");
			SetDlgItemText(TempChild,IDC_A12,L"0");
			SetDlgItemText(TempChild,IDC_A21,L"0");
			SetDlgItemText(TempChild,IDC_A22,L"0.5");
			SetDlgItemText(TempChild,IDC_X,L"100.0");
			SetDlgItemText(TempChild,IDC_Y,L"100.0");
		    SendMessage(TempChild, WM_COMMAND, (WPARAM)IDC_CALIBRATE, 0);
			//DialogBox(hInst, MAKEINTRESOURCE(IDD_CHILDDLG), hMainDlg, ChildDlgProc);
			break;
		case IDC_COLOR1:
			toColor = toColor1;
			switcherAlpha = 0;
			SendMessage(hWnd,DRAW_MSG,NULL,NULL);
			break;
			case IDC_COLOR2:
			toColor = toColor2;
			switcherAlpha = 0;
			SendMessage(hWnd,DRAW_MSG,NULL,NULL);
			break;
			case IDC_COLOR3:
			toColor = toColor3;
			switcherAlpha = 1;
			SendMessage(hWnd,DRAW_MSG,NULL,NULL);
			break;
			case IDC_COLOR4:
			toColor = toColor4;
			switcherAlpha = 1;
			SendMessage(hWnd,DRAW_MSG,NULL,NULL);
			break;
			case IDC_COLOR5:
			toColor = toColor7;
			switcherAlpha = 0;
			SendMessage(hWnd,DRAW_MSG,NULL,NULL);
			break;
			case IDC_COLOR6:
			toColor = toColor8;
			switcherAlpha = 0;
			SendMessage(hWnd,DRAW_MSG,NULL,NULL);
			break;
			case IDC_COLOR7:
			toColor = toColor7;
			switcherAlpha = 1;
			SendMessage(hWnd,DRAW_MSG,NULL,NULL);
			break;
			case IDC_COLOR8:
			toColor = toColor8;
			switcherAlpha = 1;
			SendMessage(hWnd,DRAW_MSG,NULL,NULL);
			break;
			
		case IDC_BUILDING:
			/*SendMessage(GetDlgItem(hMainDlg,IDC_COMPLETE),BM_SETCHECK,1,0); 
			SendMessage(GetDlgItem(hMainDlg,IDC_BUILDING),BM_SETCHECK,0,0); */
			CheckRadioButton(
				hMainDlg,
				IDC_COMPLETE,
				IDC_BUILDING,
				IDC_BUILDING);

			iteration = GetDlgItemInt(hMainDlg, IDC_ITERATIONNUMBER, NULL, FALSE );
			f.fracset.clear();
			f.basicpoints.clear();
			//at = basicPointsWindows.begin();
			for(at = basicPointsWindows.begin(); at != basicPointsWindows.end(); at++)
			{
				AddBasicPoint( *at , f );
			}

			label1:
			try{
			f.build(iteration);
			//buildFractalPolygone(f);
			convertFractalToPairVector(f, GraphicsPoints);
			SetBufferPoints( GraphicsPoints);}
			catch(...)
			{
				iteration -= 1;
				goto label1;
			}
			// Initialize Direct3D
			if( SUCCEEDED( InitD3D( hWndDirectX ) ) )
			{
				// Create the vertex buffer
				if( SUCCEEDED( InitVB() ) )
				{
					// Show the window
					Render();
					UpdateWindow( hWndDirectX );	
				}
			}
			//SendMessage(hWnd, WM_PAINT, NULL, NULL);
			CheckRadioButton(
				hMainDlg,
				IDC_COMPLETE,
				IDC_BUILDING,
				IDC_COMPLETE);
			break;
		case IDC_COMPLETE:
			CheckRadioButton(
				hMainDlg,
				IDC_COMPLETE,
				IDC_BUILDING,
				IDC_COMPLETE);
			/*CheckRadioButton(
			hMainDlg,
			IDC_COMPLETE,
			IDC_BUILDING,
			IDC_COMPLETE);*/
			break;
		
		}	// switch(LOWORD(wParam))
		break;
	case DELETE_MSG:
		size = basicPointsWindows.size();
		SetDlgItemText(hWnd,IDC_POINTSNUMBER,_itow(size,buff,256));
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		PostQuitMessage(0);
		break;
	}

	return false;
}

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
HWND hConnectButton;
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	//HWND hw;
	switch( msg )
	{
	case WM_CREATE:
		hMainDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_MAINDLG), (HWND)hWnd, MainDlgProc);
		SetDlgItemText(hMainDlg,IDC_ITERATIONNUMBER,L"0");
		GetWindowRect( hMainDlg, lpMainDialogRect );
		SetWindowPos( 
			hMainDlg, 
			HWND_BOTTOM,
			0, 
			0, 
			lpMainDialogRect->right - lpMainDialogRect->left, 
			lpMainDialogRect->bottom - lpMainDialogRect->top  , 
			SWP_SHOWWINDOW
			); 
		ShowWindow(hMainDlg, nCmdSh);
		break;
		//hConnectButton=CreateWindow(L"BUTTON",L"Connect to ",WS_CHILD|WS_VISIBLE| BS_DEFPUSHBUTTON,0,0,90,20,hWnd,(HMENU)ID_BUTTON,hInst,NULL);
		//ShowWindow(hConnectButton, nCmdSh);
		/*	break;
		case WM_PAINT:

		break;*/
	case WM_LBUTTONDOWN:
		SetDlgItemInt(TempChild,IDC_X, LOWORD(lParam) , FALSE);
		SetDlgItemInt(TempChild,IDC_Y, HIWORD(lParam) , FALSE);
		break;
	case WM_DESTROY:
		Cleanup();
		PostQuitMessage( 0 );
		return 0;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

BOOL WINAPI MainDlgProc1( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	//HWND hw;
	switch( msg )
	{
		/*case WM_COMMAND:
		switch(LOWORD(wParam)) {
		case IDC_BUTTON1:
		break;
		}
		break;*/
	case WM_CREATE:
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		PostQuitMessage(0);
		break;

	}

	return false;
}



int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	Fractal f;
	std::vector<std::pair<float, float>> GraphicsPoints ;
	hInst = hInstance;
	nCmdSh = nCmdShow;
	HWND hMainDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_MAINDLG), (HWND)NULL, MainDlgProc1);
	GetWindowRect( hMainDlg, lpMainDialogRect );
	GetClientRect(GetDesktopWindow(), lpDesktopWindow);

	if(!hMainDlg) return false;
	//ShowWindow(hMainDlg,nCmdShow);

	//	std::pair<float, float> temp;
	//	for(int i = 0; i < 100; i++)
	//{ 
	//	temp.first = i;
	//	temp.second = i;
	//	GraphicsPoints.push_back(temp);
	//
	//}	

	buildFractalPolygone(f, 9, 0.4, 3 );

	convertFractalToPairVector(f, GraphicsPoints);
	SetBufferPoints( GraphicsPoints);
	// Register the window class
	WNDCLASSEX wc =
	{
		sizeof( WNDCLASSEX ), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle( NULL ), NULL, NULL, NULL, NULL,
		L"D3D Tutorial", NULL
	};
	RegisterClassEx( &wc );

	// Create the application's window
	/*HWND hWnd = CreateWindow( L"D3D Tutorial", L"D3D Tutorial 02: Vertices",
	WS_OVERLAPPEDWINDOW, lpMainDialogRect->right, 0, lpDesktopWindow->right - lpMainDialogRect->right , lpDesktopWindow->bottom,
	NULL, NULL, wc.hInstance, NULL );*/

	hWndDirectX = CreateWindow( L"D3D Tutorial", L"Self-Affine Plite",
		WS_OVERLAPPEDWINDOW, lpMainDialogRect->right, 0, lpDesktopWindow->right - lpMainDialogRect->right , lpDesktopWindow->bottom,
		NULL, NULL, wc.hInstance, NULL );

	// Initialize Direct3D
	if( SUCCEEDED( InitD3D( hWndDirectX ) ) )
	{
		// Create the vertex buffer
		if( SUCCEEDED( InitVB() ) )
		{
			// Show the window
			ShowWindow( hWndDirectX, SW_SHOWDEFAULT );
			UpdateWindow( hWndDirectX );

			// Enter the message loop
			MSG msg;
			ZeroMemory( &msg, sizeof( msg ) );
			while( msg.message != WM_QUIT )
			{
				if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
				{
					//UpdateWindow( hWndDirectX );
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}
				else
					Render();
			}
		}
	}


	//return 0;
	//	MSG msg;
	//	while (GetMessage(&msg, NULL, NULL, NULL)) {
	//		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
	//			{
	//		if (!IsWindow(hMainDlg) || !IsDialogMessage(hMainDlg, &msg)) {
	//			TranslateMessage(&msg);
	//			DispatchMessage(&msg);
	//		}	// if (!IsWindow(hMainDlg) || !IsDialogMessage(hMainDlg, &msg))
	//		}
	//		else
	//		{Render();}
	//	}	// while (GetMessage(&msg, NULL, NULL, NULL))

	deleteBufferPoints();
	UnregisterClass( L"D3D Tutorial", wc.hInstance );

	return true;
}

