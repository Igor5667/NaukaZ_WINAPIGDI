# include <windows.h>
# include <string>

using namespace std;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "MojaKlasaOkna";

    // struktura klasy okna
    WNDCLASS klasaOkna = { 0 };

    // wypelnianie standardowych informacji
    klasaOkna.lpfnWndProc = WindowProc;
    klasaOkna.hInstance = hInstance;
    klasaOkna.lpszClassName = CLASS_NAME;

    // kwesie wygladowe
    klasaOkna.hCursor = LoadCursor(NULL, IDC_ARROW);  // kursor
    klasaOkna.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);  // tlo

    // sprawdzamy czy rejestracja sie powiodla
    if (!RegisterClass(&klasaOkna)) return 0;


    // tworzenie naszego glownego okna
    HWND glowneOkno = CreateWindow(
        CLASS_NAME,             // Nazwa klasy
        "Laboratorium WinAPI",  // Tytuł okna
        WS_OVERLAPPEDWINDOW,    // Styl okna
        1500, 500,              // Pozycja X, Y
        800, 800,               // Szerokość, wysokość
        NULL,                   // Rodzic
        NULL,                   // Menu
        hInstance,              // Instancja
        NULL
    );

    if (glowneOkno == NULL) return 0;
    ShowWindow(glowneOkno, nCmdShow);

    // glowna petla przechowujaca meldunki (komunikaty od usera klawiatura, mysz, zmiana okna)
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static int mouse_x = -1, mouse_y = -1;

    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);



            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_LBUTTONDOWN: {
            // lParam odnośnie pozycji kursora
            mouse_x = LOWORD(lParam);
            mouse_y = HIWORD(lParam);

            /*char bufor[100];
            wsprintf(bufor, "x: %d, y: %d", mouse_x, mouse_y);
            MessageBox(hwnd, bufor, "Pozycja myszy", MB_OK);*/

            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        }
        case WM_RBUTTONUP: {
            string msg = "Kliknales przycisk ";
            if (wParam & MK_CONTROL) msg += "z controlem";
            else msg += "bez controla";
            msg += ". Czy to prawda?";

            int decyzja = MessageBox(hwnd, msg.c_str(), "Prawy klawisz nacisniety", MB_YESNO);

            if (decyzja != IDYES) MessageBox(hwnd, "Klamiesz", "Klamca", MB_OK);

            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}