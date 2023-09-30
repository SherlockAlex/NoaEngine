#include <iostream>
#include <string>
#include <Windows.h>

// ����ӿ� Platform
class Platform {
public:
    virtual void Create(int w, int h, const char* title, int flag) = 0;
    virtual ~Platform() {}
};

// ���� Platform_windows
class Platform_windows : public Platform {
public:
    void Create(int w, int h, const char* title, int flag) override {
        WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, Platform_windows::WindowProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"MyWindow", NULL };
        RegisterClassEx(&wc);

        DWORD windowStyle = WS_OVERLAPPEDWINDOW;
        if (flag == 1) {
            windowStyle = WS_POPUP;
        }

        RECT windowRect = { 0, 0, w, h };
        AdjustWindowRect(&windowRect, windowStyle, FALSE);

        HWND hwnd = CreateWindow(wc.lpszClassName, L"MyWindow", windowStyle, CW_USEDEFAULT, CW_USEDEFAULT,
            windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, NULL, NULL, wc.hInstance, NULL);

        ShowWindow(hwnd, SW_SHOWDEFAULT);
        UpdateWindow(hwnd);

        MSG msg;
        while (true) {
            if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
                if (msg.message == WM_QUIT) {
                    std::cout <<"quit" << std::endl;
                    break;
                }
            }
            else {
                //�¼�ִ��
            }
        }

        UnregisterClass(wc.lpszClassName, wc.hInstance);
    }

public:
    static LRESULT WINAPI WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        switch (uMsg) {
        case WM_CLOSE:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
        return 0;
    }
};

int main() {
    int width = 800;
    int height = 600;
    const char* title = "My Window";
    int flag = 0; // 0��ʾ����ģʽ��1��ʾȫ��ģʽ

    // ���� Platform_windows ʵ��
    Platform_windows platform;

    // ���� CreateWindow ������������
    platform.Create(width, height, title, flag);

    return 0;
}
