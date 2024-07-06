#include"windows.h"
#include "thread"//多线程库
#include"framework.h"
#include"fastclicker.h"
#include"commctrl.h"
using namespace std;
constexpr auto MAX_LOADSTRING = 1000;
//ID定义：
//按钮：0
constexpr auto ID_BUTTON1 = 001;
constexpr auto ID_BUTTON2 = 002;
//选项卡：1
constexpr auto IDC_TABCTRL = 101;
//编辑控件：2
constexpr auto ID_EDIT1 = 201;
constexpr auto ID_EDIT2 = 202;
constexpr auto ID_EDIT3 = 203;
//文本：3
constexpr auto ID_TEXT0 = 300;
constexpr auto ID_TEXT1 = 301;
constexpr auto ID_TEXT2 = 302;
constexpr auto ID_TEXT3 = 303;
// 全局变量
HINSTANCE hInst;// 当前实例
WCHAR szTitle[MAX_LOADSTRING];// 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];// 主窗口类名
HWND hwndTab = nullptr;
HWND Button1 = nullptr;
HWND Button2 = nullptr;
HWND hwndEdit1 = nullptr;
HWND hwndEdit2 = nullptr;
HWND hwndEdit3 = nullptr;
HWND tip0 = nullptr;
HWND tip1 = nullptr;
HWND tip2 = nullptr;
HWND tip3 = nullptr;
int width;//窗口宽度
int height;//窗口高度
int num1 = -1;
int num2 = -1;
int num3 = -1;
thread m[1] = {};
// 函数的前向声明:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
static void Fastclicker(int num, int time, int delay) {//点击次数，点击间隔时间(ms) ，延迟启动时间(ms)
    Sleep(delay);
    if (num == 0) {
        while (1) {
            if (GetAsyncKeyState(0x50))break;
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            Sleep(time);
        }
    }
    else {
        for (int i = 1;i <= num;i++) {
            if (GetAsyncKeyState(0x50))break;
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            Sleep(time);
        }
    }
}
static void TAB_SH_ALL(BOOL TF) {
    if (TF) {
        ShowWindow(Button1, SW_SHOW);
        ShowWindow(tip0, SW_SHOW);
        ShowWindow(hwndEdit1, SW_SHOW);
        ShowWindow(tip1, SW_SHOW);
        ShowWindow(hwndEdit2, SW_SHOW);
        ShowWindow(tip2, SW_SHOW);
        ShowWindow(hwndEdit3, SW_SHOW);
        ShowWindow(tip3, SW_SHOW);
        ShowWindow(Button2, SW_HIDE);
    }
    else {
        ShowWindow(Button1, SW_HIDE);
        ShowWindow(tip0, SW_HIDE);
        ShowWindow(hwndEdit1, SW_HIDE);
        ShowWindow(tip1, SW_HIDE);
        ShowWindow(hwndEdit2, SW_HIDE);
        ShowWindow(tip2, SW_HIDE);
        ShowWindow(hwndEdit3, SW_HIDE);
        ShowWindow(tip3, SW_HIDE);
        ShowWindow(Button2, SW_SHOW);
    }
}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    //初始化
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FASTCLICKER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    if (!InitInstance(hInstance, nCmdShow)) {
        return 0;
    }
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FASTCLICKER));
    MSG msg;
    // 主消息循环:
    while(GetMessage(&msg, nullptr, 0, 0)){
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int)msg.wParam;
}
ATOM MyRegisterClass(HINSTANCE hInstance) {//注册窗口类
    WNDCLASSEXW wcex{};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FASTCLICKER));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_FASTCLICKER);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    return RegisterClassExW(&wcex);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {//保存实例句柄并创建主窗口
    hInst = hInstance;// 将实例句柄存储在全局变量中
    HWND hWnd = CreateWindowW(szWindowClass, L"一款免费高效简洁的连点器", WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);
    if (!hWnd)return FALSE;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return 1;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {//处理主窗口的消息
    wchar_t tabTitle0[] = L"连点器";
    wchar_t tabTitle1[] = L"关于作者";
    //提前初始化
    TCITEM tie = {};
    WCHAR buffer1[256]{};
    WCHAR buffer2[256]{};
    WCHAR buffer3[256]{};
    num1 = -1;num2 = -1;num3 = -1;
    switch (message) {
    case WM_COMMAND: {
        int wmId = LOWORD(wParam);
        switch (wmId) {// 分析菜单选择:
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case ID_BUTTON1:
            GetWindowText(GetDlgItem(hWnd, ID_EDIT1), buffer1, 256);
            GetWindowText(GetDlgItem(hWnd, ID_EDIT2), buffer2, 256);
            GetWindowText(GetDlgItem(hWnd, ID_EDIT3), buffer3, 256);
            if (((swscanf_s(buffer1, L"%d", &num1) <= 0) && (num1 < 0)) || ((swscanf_s(buffer2, L"%d", &num2) <= 0) && (num2 < 0)) || ((swscanf_s(buffer3, L"%d", &num3) <= 0) && (num3 < 0))) {
                MessageBox(hWnd, L"请输入非负整数！", L"提示", MB_OK);
                break;
            }
            m[1] = thread(Fastclicker, num1, num2, num3);
            m[1].detach();
            break;
        case ID_BUTTON2:
            system("start https://space.bilibili.com/367492433");
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    }
    case WM_NOTIFY: {
        LPNMHDR lpnmhdr = (LPNMHDR)lParam;
        if (lpnmhdr->idFrom == IDC_TABCTRL && lpnmhdr->code == TCN_SELCHANGE) {
            // 切换选项卡处理
            int iSelectedTab = TabCtrl_GetCurSel(lpnmhdr->hwndFrom);
            switch (iSelectedTab) {
            case 0:// 第一个选项卡被选中
                TAB_SH_ALL(1);
                break;
            case 1:// 第二个选项卡被选中
                TAB_SH_ALL(0);
                break;
            }
        }
        break;
    }
    case WM_SIZE: {// 实时获取窗口宽高
        width = LOWORD(lParam);
        height = HIWORD(lParam);
        SetWindowPos(hwndTab, NULL, 0, 0, width, height, SWP_NOZORDER);
        SetWindowPos(Button1, NULL, width - 230, height - 42, 220, 30, SWP_NOZORDER);
        break;
    }
    case WM_CREATE:
        hwndTab = CreateWindow(WC_TABCONTROL, L"", WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE, 0, 0, 800, 600, hWnd, (HMENU)IDC_TABCTRL, hInst, NULL);//创建Tab 控件
        tie.mask = TCIF_TEXT;// 为 Tab 控件添加选项卡项
        tie.pszText = tabTitle0;//选项卡1
        TabCtrl_InsertItem(hwndTab, 0, &tie);
        tie.pszText = tabTitle1;//选项卡2
        TabCtrl_InsertItem(hwndTab, 1, &tie);
        //添加所有页面的元素
        tip0 = CreateWindow(L"STATIC", L"使用注意事项：\n1.停止按键为P!!!\n2.如果间隔时间不为0（点击速度较慢）,一按P就会很快停止点击;否则速度会很快,但会很卡,需要长按P且等待一会才会停止点击。\n3.点击启动按钮后,请在设定的延迟时间内将鼠标指针移到需要点击的位置（别忘了移导致反复启动）", WS_CHILD | WS_VISIBLE, 10, 200, 400, 200, hWnd, (HMENU)ID_TEXT0, hInst, NULL);// 创建总提示文本
        Button1 = CreateWindow(L"BUTTON", L"启动!（点击并经过设定延迟后）", WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE, 0, 0, 0, 0, hWnd, (HMENU)ID_BUTTON1, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);//创建按钮1
        Button2 = CreateWindow(L"BUTTON", L"点击前往作者的b站主页", WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON, 30, 50, 180, 30, hWnd, (HMENU)ID_BUTTON2, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);//创建按钮2
        hwndEdit1 = CreateWindow(L"EDIT", L"", WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | WS_VISIBLE, 10, 50, 100, 20, hWnd, (HMENU)ID_EDIT1, hInst, NULL);// 创建编辑控件1
        tip1 = CreateWindow(L"STATIC", L"请输入点击次数（0为持续点击）:", WS_CHILD | WS_VISIBLE, 10, 50 - 20, 230, 20, hWnd, (HMENU)ID_TEXT1, hInst, NULL);// 创建编辑控件1的提示文本
        hwndEdit2 = CreateWindow(L"EDIT", L"", WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | WS_VISIBLE, 10, 100, 100, 20, hWnd, (HMENU)ID_EDIT2, hInst, NULL);// 创建编辑控件2
        tip2 = CreateWindow(L"STATIC", L"请输入点击间隔时间ms（0为无间隔）:", WS_CHILD | WS_VISIBLE, 10, 100 - 20, 300, 20, hWnd, (HMENU)ID_TEXT2, hInst, NULL);// 创建编辑控件2的提示文本
        hwndEdit3 = CreateWindow(L"EDIT", L"", WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | WS_VISIBLE, 10, 150, 100, 20, hWnd, (HMENU)ID_EDIT3, hInst, NULL);// 创建编辑控件3
        tip3 = CreateWindow(L"STATIC", L"按下启动按钮到开始点击的时间延迟（ms）:", WS_CHILD | WS_VISIBLE, 10, 150 - 20, 300, 20, hWnd, (HMENU)ID_TEXT3, hInst, NULL);// 创建编辑控件3的提示文本
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {// “关于”框的消息处理程序
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}