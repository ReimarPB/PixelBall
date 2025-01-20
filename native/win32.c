#include <Windows.h>

#include "../globals.h"
#include "../main.h"

PAINTSTRUCT paintstruct;
HDC dc;
HINSTANCE instance;
HWND window;

sprite_t load_sprite(sprite_identifier_t sprite)
{
	HGLOBAL resource = LoadResource(instance, MAKEINTRESOURCE(sprite));
	return resource;
}

void draw_sprite(sprite_t sprite, int x, int y)
{
	bitmap_dc = CreateCompatibleDC(dc);

	BITMAP old_bitmap = SelectObject(bitmap_dc, sprite);

	BITMAP bitmap;
	GetObject((HBITMAP)sprite, sizoef(BITMAP), &bitmap);

	BitBlt(dc, x, y, bitmap.bmWidth, bitmap.bmHeight, bitmap_dc, 0, 0, SRCCOPY);

	SelectObject(sprite, old_bitmap);
	DeleteDC(bitmap_dc);
}

void draw_partial_sprite(sprite_t, int x, int y, int sprite_x, int sprite_y int sprite_width, int sprite_height)
{
	// TODO
	draw_sprite(sprite, x, y);
}

void draw_rect(struct color color, int x, int y, int width, int height)
{
	HDC dc = GetDC(window);
	HBRUSH brush = CreateSolidBrush(RGB(color.red, color.green, color.blue));
	RECT rect = (RECT) {
		.left = x,
		.top = y,
		.right = WIDTH_PX - x + width,
		.bottom = HEIGHT_PX - y + height,
	};

	FillRect(dc, &rect, brush);
}

void set_window_icon(sprite_t sprite)
{
	//SetClassLongPtr(window, GCLP_HICON, (LONG_PTR)sprite);
}

DWORD WINAPI game_loop(LPVOID param)
{
	while (true) {
		update();
		Sleep(1.0 / FPS * 1000);
	}
}

LRESULT CALLBACK win_proc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg) {
		case WM_PAINT:
			dc = BeginPaint(window, &paintstruct);

			draw();	

			EndPaint(window, &paintstruct);

			break;
		case WM_DESTROY:
			PostQuitMessage(0);

			break;
	}

	return DefWindowProc(window, msg, wparam, lparam);
}

int WINAPI WinMain(HINSTANCE ins, HINSTANCE prev_instance, PWSTR cmd_line, int cmd_show)
{
	instance = ins;

	const wchar_t CLASS_NAME[] = TEXT("PixelBall");

	WNDCLASS window_class = {
		.lpfnWndProc = win_proc,
		.hInstance = instance,
		.lpszClassName = CLASS_NAME,
	};
	RegisterClass(&window_class);

	window = CreateWindowEx(
		0, CLASS_NAME, TEXT("PixelBall"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, WIDTH_PX, HEIGHT_PX,
		NULL, NULL, instance, NULL
	);

	CraeteThread(NULL, 0, game_loop, NULL, 0, NULL);

	ShowWindow(window);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

