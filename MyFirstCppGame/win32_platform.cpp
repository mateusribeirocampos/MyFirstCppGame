#include <windows.h>

// Variável global para controlar o estado do loop principal
bool running = true;

// Ponteiro para a memória do buffer de pixels
void* buffer_memory;

// Largura do buffer de pixels
int buffer_width;

// Altura do buffer de pixels
int buffer_height;

// Estrutura que contém informações sobre o bitmap
BITMAPINFO buffer_bitmap_info;

// Função de retorno de chamada da janela
LRESULT CALLBACK WindowCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Variável para armazenar o resultado da mensagem
    LRESULT results = 0;

    // Switch case para lidar com diferentes tipos de mensagens
    switch (message) {
        // Caso a janela seja fechada ou destruída
    case WM_CLOSE:
    case WM_DESTROY:
    {
        // Definir a variável global 'running' como false para sair do loop
        running = false;
    } break;

    // Caso a janela seja redimensionada
    case WM_SIZE:
    {
        // Obtém o tamanho do cliente da janela
        RECT rect;
        GetClientRect(hwnd, &rect);
        buffer_width = rect.right - rect.left;
        buffer_height = rect.bottom - rect.top;

        // Calcula o tamanho do buffer de pixels
        int bufferSize = buffer_width * buffer_height * sizeof(unsigned int);

        // Libera a memória do buffer anterior, se existir
        if (buffer_memory) VirtualFree(buffer_memory, 0, MEM_RELEASE);

        // Aloca memória para o novo buffer de pixels
        buffer_memory = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        // Configura as informações do bitmap
        buffer_bitmap_info.bmiHeader.biSize = sizeof(buffer_bitmap_info.bmiHeader);
        buffer_bitmap_info.bmiHeader.biWidth = buffer_width;
        buffer_bitmap_info.bmiHeader.biHeight = buffer_height;
        buffer_bitmap_info.bmiHeader.biPlanes = 1;
        buffer_bitmap_info.bmiHeader.biBitCount = 32;
        buffer_bitmap_info.bmiHeader.biCompression = BI_RGB;
    } break;

    // Caso padrão para todas as outras mensagens
    default:
    {
        // Chama a função de processo de janela padrão do sistema operacional
        results = DefWindowProc(hwnd, message, wParam, lParam);
    } break;
    }

    // Retorna o resultado da mensagem
    return results;
}

// Função principal do Windows
int __stdcall WinMain(
    _In_ HINSTANCE hInstance, 
    _In_opt_ HINSTANCE hPrevInstance, 
    _In_ LPSTR lpCmdLine, 
    _In_ int nCmdShow)
{
    // Cria uma classe de janela
    WNDCLASS window_class = {};
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpszClassName = L"Colision Line Game"; // Nome da classe da janela
    window_class.lpfnWndProc = WindowCallback; // Função de retorno de chamada da janela

    // Registra a classe da janela
    RegisterClass(&window_class);

    // Cria a janela
    HWND window = CreateWindowEx(
        0,
        window_class.lpszClassName,
        L"Colision Line Game", // Título da janela
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, // Estilo da janela
        CW_USEDEFAULT, // Posição X da janela
        CW_USEDEFAULT, // Posição Y da janela
        1280, // Largura da janela
        720, // Altura da janela
        0,
        0,
        hInstance,
        0);

    // Obtém o contexto de dispositivo da janela
    HDC hdc = GetDC(window);

    // Loop principal do programa
    while (running) {
        // Trata as mensagens da fila de mensagens
        MSG message;
        while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }

        // Dizer um olá grande na tela
        RECT textRect;
        textRect.left = 0;
        textRect.top = 0;
        textRect.right = buffer_width;
        textRect.bottom = buffer_height;

        DrawText(hdc, L"Welcome to the Game", -1, &textRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        SIZE size;
        GetTextExtentPoint32(hdc, L"Welcome to the Game", wcslen(L"Welcome to the Game"), &size);
        COLOR16 color = RGB(255, 0, 0);
        SetTextColor(hdc, color);
        HFONT hFont = CreateFont(50, 0, 30, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
        SelectObject(hdc, hFont);

        // Preenche o buffer de pixels
        unsigned int* pixel = (unsigned int*)buffer_memory;
        for (int y = 0; y < buffer_height; y++)
        {
            for (int x = 0; x < buffer_width; x++)
            {
                // Verifica se o pixel está na borda de 20 pixels
                if (x < 20 || x >= buffer_width - 20 || y < 20 || y >= buffer_height - 20)
                {
                    *pixel++ = 0xFF0000; // Preenche o pixel com a cor vermelha (0xFF0000)
                }
                else
                {
                    *pixel++ = 0x000000; // Preenche o pixel com a cor preta (0x000000)
                }
            }
        }

        // Renderiza a janela
        StretchDIBits(
            hdc,
            0,
            0,
            buffer_width,
            buffer_height,
            0,
            0,
            buffer_width,
            buffer_height,
            buffer_memory,
            &buffer_bitmap_info,
            DIB_RGB_COLORS,
            SRCCOPY);
    }

    // Retorna 0 para indicar que o programa foi encerrado com sucesso
    return 0;
}
