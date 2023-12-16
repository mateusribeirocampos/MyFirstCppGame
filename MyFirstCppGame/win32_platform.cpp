#include <windows.h>

bool running = true;

LRESULT CALLBACK WindowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT results = 0;
	switch (message) {
		case WM_CLOSE:
		case WM_DESTROY:
		{
			running = false;
		} break;

		default:
		{
			results = DefWindowProc(window, message, wParam, lParam);
		} break;
	}
	return results;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// crate a window class
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = L"MyFirstCppGameWindowClass";
	window_class.lpfnWndProc = WindowCallback;

	//Register the window class
	RegisterClass(&window_class);

	// Create the window
	HWND window = CreateWindowEx(
		0,
		window_class.lpszClassName,
		L"My First Cpp Game",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1280,
		720,
		0,
		0,
		hInstance,
		0);

	//loop
	while (running) {
		// input
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

	}

}