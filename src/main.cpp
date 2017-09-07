//============================================================================
// Name        : Matrix.cpp
// Author      : Mateusz Fraszczynski
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "../include/Matrix.h"
#include "../include/MatrixTest.h"
#include "../include/DHparam.h"
#include "../include/Timer.h"
#include <windows.h>

using namespace std;

void dh( void );

string AppTitle = "Denavit-Hartenberg notation";
string SubWindowName = "Add Joint";
#define mainWinSizeX 800
#define mainWinSizeY 500
#define childWinSizeX 330
#define childWinSizeY 150

#define ID_BUTTON_ADD_JOINT 	101
#define ID_BUTTON_SHOW_CHAIN 	102
#define ID_BUTTON_REMOVE_JOINT 	103
#define ID_BUTTON_ADD 			104

#define SIG_SUCCESS				500
#define SIG_FAILED				501

#define DRAW_ROW				700
#define DRAW_TAB_X				200
#define DRAW_TAB_Y				 30

HINSTANCE hInst;
HWND hwnd_main;
HWND hwnd_child;
HWND button_add_joint;
HWND button_show_chain;
HWND button_remove_joint;
HWND button_add;

#define INPUT_BOXES 8
HWND input[ INPUT_BOXES ];

DHparam * dhp = NULL;
unsigned int no_of_joints;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
LRESULT CALLBACK WindowProcChild(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
int initMainWindow( );
void fillMainWindow( );
int initChildWindow( );
void fillChildWindow( );
int createChildWindow( );
bool verifyAndAddValues( );
double getValues( unsigned int );
void drawRow( HDC , int , int );
void fillInitRow( HDC );

int WinMain( HINSTANCE hInst_l , HINSTANCE , LPSTR , int nCmdShow ) {

	#ifdef DEBUG
	    printf("RUNNING DEBUG BUILD\n");
	    MatrixTest start;
	#else
	    printf("Running... this is a release build.");
	#endif

	if ( ! initMainWindow( ) ) return 0;
	if ( ! initChildWindow( ) ) return 0;
	fillMainWindow( );

	dhp = new DHparam();

	ShowWindow( hwnd_main , nCmdShow );
	UpdateWindow( hwnd_main );

	MSG msg;
	while ( GetMessage( & msg , NULL , 0 , 0 ) > 0 ) {
		TranslateMessage( & msg );
		DispatchMessage( & msg );
	}
	delete dhp;
	return msg.wParam;
}

LRESULT CALLBACK WindowProc( HWND hwnd , UINT msg , WPARAM wparam , LPARAM lparam ) {
	switch ( msg ) {
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC dc;
			RECT r;
			GetClientRect( hwnd , & r );
			dc = BeginPaint( hwnd , & ps );
			DrawText( dc , "Welcom in DH" , -1 , & r , DT_SINGLELINE | DT_CENTER | DT_VCENTER );
			int y_offset = DRAW_TAB_Y;
			// TODO Printing just once !
			for ( unsigned int i = 0 ; i < no_of_joints + 1 ; i++ , y_offset += 30 ) {
				drawRow( dc , DRAW_TAB_X , y_offset  );
			}
			fillInitRow( dc );
			EndPaint( hwnd , & ps );
			break;
		}
		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;
		case WM_COMMAND: {
			switch( wparam ) {
				case ID_BUTTON_ADD_JOINT: {
					if ( ! createChildWindow( ) ) break;
					ShowWindow( hwnd_child , SW_SHOW );
				    UpdateWindow( hwnd_child );
					break;
				}
				case ID_BUTTON_SHOW_CHAIN:
					MessageBox( hwnd , "Test", "..." , MB_ICONINFORMATION );
					break;
				case ID_BUTTON_REMOVE_JOINT:
					MessageBox( hwnd , "Test", "..." , MB_ICONINFORMATION );
					break;
				}
			break;
		}
		case WM_PARENTNOTIFY: {
			switch( wparam ) {
				case SIG_SUCCESS:
					//MessageBox( hwnd , "Successful added joint ! " , "Success !" , MB_ICONINFORMATION );
					break;
				case SIG_FAILED:
					//MessageBox( hwnd , "Failed to add joint ! ", "Failed !" , MB_ICONINFORMATION );
					break;
				default:
					break;
			}
			break;
		}
		default:
			return DefWindowProc( hwnd , msg , wparam , lparam );
	}
	return 0;
}

LRESULT CALLBACK WindowProcChild( HWND hwnd , UINT msg , WPARAM wparam , LPARAM lparam ) {
	switch ( msg ) {
		case WM_PAINT: {
			break;
		}
		case WM_COMMAND: {
			switch( wparam ) {
				case ID_BUTTON_ADD: {
					if ( verifyAndAddValues( ) ) {
						no_of_joints++;
						SendMessage( hwnd_main , WM_PAINT , DRAW_ROW , 0 );
						//MessageBox( NULL , "Successful added joint ! " , "Success !" , MB_ICONINFORMATION );
						//SendMessage( hwnd_main , WM_PARENTNOTIFY , 0 , 0 );
					} else {
						//SendMessage( hwnd_main , WM_PARENTNOTIFY , 0 , 0 );
					}
					DestroyWindow( hwnd_child );
					break;
				}
				default:
					break;
			}
			break;
		}
		case WM_DESTROY: {
			DestroyWindow( hwnd_child );
			break;
		}
		default:
			return DefWindowProc( hwnd , msg , wparam , lparam );
	}
	return 0;
}

int initMainWindow( ) {
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon( NULL , IDI_WINLOGO );
	wc.hCursor = LoadCursor( NULL , IDC_ARROW );
	wc.hbrBackground = ( HBRUSH ) COLOR_WINDOWFRAME;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = AppTitle.c_str();
	if ( ! RegisterClass( & wc ) ) return 0;
	hwnd_main = CreateWindow( AppTitle.c_str() , AppTitle.c_str() , WS_OVERLAPPEDWINDOW , CW_USEDEFAULT , CW_USEDEFAULT , mainWinSizeX , mainWinSizeY , NULL , NULL , hInst , NULL );
	if ( ! hwnd_main ) return 0;
	return 1;
}

int initChildWindow( ) {
	WNDCLASSEX wcx;
	wcx.cbSize = sizeof( wcx );
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WindowProcChild;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInst;
	wcx.hIcon = LoadIcon( NULL , IDI_APPLICATION );
	wcx.hCursor = LoadCursor( NULL , IDC_ARROW );
	wcx.hbrBackground = ( HBRUSH ) COLOR_WINDOWFRAME;
	wcx.lpszMenuName =  NULL;
	wcx.lpszClassName = SubWindowName.c_str();
	wcx.hIconSm = ( HICON ) LoadImage( hInst , MAKEINTRESOURCE( 5 ) , IMAGE_ICON , GetSystemMetrics( SM_CXSMICON ) , GetSystemMetrics( SM_CYSMICON ) , LR_DEFAULTCOLOR );
	if ( ! RegisterClassEx( & wcx ) ) return 0;
	return 1;
}

int createChildWindow( ) {
	hwnd_child = CreateWindowEx( WS_EX_APPWINDOW , SubWindowName.c_str() , SubWindowName.c_str() ,  WS_OVERLAPPEDWINDOW | WS_EX_TOPMOST | WS_POPUP , CW_USEDEFAULT , CW_USEDEFAULT , childWinSizeX , childWinSizeY , hwnd_main , NULL , hInst , NULL );
	if ( ! hwnd_child ) return 0;
	fillChildWindow( );
	return 1;
}

void fillMainWindow( ) {
	button_add_joint = CreateWindowEx( WS_EX_CLIENTEDGE , "BUTTON" , "Add joint" , WS_CHILD | WS_VISIBLE | WS_BORDER , 0 , 0 , 150 , 30 , hwnd_main , ( HMENU ) ID_BUTTON_ADD_JOINT , hInst , NULL );
	button_show_chain = CreateWindowEx( WS_EX_CLIENTEDGE , "BUTTON" , "Show chain" , WS_CHILD | WS_VISIBLE | WS_BORDER , 0 , 30 , 150 , 30 , hwnd_main , ( HMENU ) ID_BUTTON_SHOW_CHAIN , hInst , NULL );
	button_remove_joint = CreateWindowEx( WS_EX_CLIENTEDGE , "BUTTON" , "Remove joint" , WS_CHILD | WS_VISIBLE | WS_BORDER , 0 , 60 , 150 , 30 , hwnd_main , ( HMENU ) ID_BUTTON_REMOVE_JOINT , hInst , NULL );
}

void fillChildWindow( ) {
	// Static
	HWND hStatic_d = CreateWindowEx( WS_EX_CLIENTEDGE , "STATIC" , NULL , WS_CHILD | WS_VISIBLE | SS_LEFT | ES_CENTER , 0 , 0 , 150 , 20 , hwnd_child , NULL , hInst , NULL );
	SetWindowText( hStatic_d , " Segment Offset " );
	HWND hStatic_theta = CreateWindowEx( WS_EX_CLIENTEDGE , "STATIC" , NULL , WS_CHILD | WS_VISIBLE | SS_LEFT | ES_CENTER , 0 , 20 , 150 , 20 , hwnd_child , NULL , hInst , NULL );
	SetWindowText( hStatic_theta , " Angle of Joint " );
	HWND hStatic_r = CreateWindowEx( WS_EX_CLIENTEDGE , "STATIC" , NULL , WS_CHILD | WS_VISIBLE | SS_LEFT | ES_CENTER , 0 , 40 , 150 , 20 , hwnd_child , NULL , hInst , NULL );
	SetWindowText( hStatic_r , " Segment Length " );
	HWND hStatic_alpha = CreateWindowEx( WS_EX_CLIENTEDGE , "STATIC" , NULL , WS_CHILD | WS_VISIBLE | SS_LEFT | ES_CENTER , 0 , 60 , 150 , 20 , hwnd_child , NULL , hInst , NULL );
	SetWindowText( hStatic_alpha , " Twist Angle " );
	// Add button
	button_add_joint = CreateWindowEx( WS_EX_CLIENTEDGE , "BUTTON" , "Add" , WS_CHILD | WS_VISIBLE | WS_BORDER , 0 , 80 , 310 , 30 , hwnd_child , ( HMENU ) ID_BUTTON_ADD , hInst , NULL );

	int x_offset, y_offset;
	for ( unsigned int i = 0 ; i < INPUT_BOXES ; i++ ) {
		i % 2 ? x_offset = 235 : x_offset = 150;
		input[ i ] = CreateWindowEx( WS_EX_CLIENTEDGE , "EDIT" , NULL , WS_CHILD | WS_VISIBLE | SS_LEFT | ES_CENTER | ES_NUMBER, x_offset , y_offset , 75 , 20 , hwnd_child , NULL , hInst , NULL );
		if ( x_offset == 235 ) y_offset += 20;
	}
	HWND dots [ INPUT_BOXES / 2 ];
	for ( unsigned int i = 0 ; i < INPUT_BOXES / 2 ; i++ ) {
		dots [ i ] = CreateWindowEx( WS_EX_CLIENTEDGE , "STATIC" , NULL , WS_CHILD | WS_VISIBLE | ES_CENTER , 225 , i * 20 , 10 , 20 , hwnd_child , NULL , hInst , NULL );
		SetWindowText( dots [ i ] , "." );
	}
}

bool verifyAndAddValues( ) {
	if ( dhp == NULL ) return false;
	jointParams jParams;
	for ( unsigned int i = 0 ; i < INPUT_BOXES ; i += 2 ) {
		double value = getValues( i );
		switch ( i ) {
				case 0:
					jParams.d = value;
					break;
				case 2: {
					if ( ! (value < 360 && value > -360) ) return false;
					jParams.theta = value;
					break;
				}
				case 4:
					jParams.r = value;
					break;
				case 6: {
					if ( ! (value < 360 && value > -360) ) return false;
					jParams.alpha = value;
					break;
				}
				default:
					break;
		}
	}
	dhp->addJointParams( jParams );
	return true;
}

double getValues( unsigned int index ) {
	TCHAR buff_h[ 20 ];
	TCHAR buff_l[ 20 ];
	GetWindowText( input[ index ] , buff_h , 20 );
	GetWindowText( input[ index + 1 ] , buff_l , 20 );
	string doubl_h = buff_h ;
	string doubl_l = buff_l ;
	string string_v = doubl_h + "." + doubl_l;
	double double_v = atof( string_v.c_str( ) );
	return double_v;
}

void drawRow( HDC dc , int x_start , int y_start ) {
	int x_current = x_start;
	int y_current = y_start;
	int column_width = 100;
	int column_high = 30;
	for ( unsigned int i = 0 ; i < 5 ; i++ ) {
		MoveToEx( dc , x_current , y_current , NULL );
		x_current += column_width;
		LineTo( dc , x_current , y_current );
		y_current += column_high;
		LineTo( dc , x_current , y_current );
		x_current -= column_width;
		LineTo( dc , x_current , y_current );
		y_current -= column_high;
		LineTo( dc , x_current , y_current );
		x_current += column_width;
	}
}

void fillInitRow( HDC dc ) {
	TextOut( dc , 230 , 38 , TEXT( "Joint" ) , 5 );
	TextOut( dc , 230 + 100 , 38 , TEXT( "  d  " ) , 5 );
	TextOut( dc , 230 + 200 , 38 , TEXT( "theta" ) , 5 );
	TextOut( dc , 230 + 300 , 38 , TEXT( "  r  " ) , 5 );
	TextOut( dc , 230 + 400 , 38 , TEXT( "alpha" ) , 5 );
}
