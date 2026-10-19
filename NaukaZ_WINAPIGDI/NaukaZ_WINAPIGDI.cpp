# include <windows.h>
# include <string>

#define ROZMIAR_KOTA 200

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
    static int mysz_x = 255, mysz_y = 255;
    static int rozmiar_oka = 25;

    switch (wParam) {
        case VK_UP: {
            rozmiar_oka += 5;
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }
        case VK_DOWN: {
            rozmiar_oka -= 5;
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }
    }

    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // inicjalizacja zmiennych pozycji kota
            int srodek_kota_x = mysz_x, srodek_kota_y = mysz_y;
            int srodek_oka_lewego_x = srodek_kota_x - ROZMIAR_KOTA / 2, srodek_oka_lewego_y = srodek_kota_y + ROZMIAR_KOTA / 8;
            int srodek_oka_prawego_x = srodek_kota_x + ROZMIAR_KOTA / 2, srodek_oka_prawego_y = srodek_kota_y + ROZMIAR_KOTA / 8;

            // inicjalizacja pedzli
            HPEN pioro = CreatePen(1, 5, RGB(255, 255, 0));
            HBRUSH pedzelCzerwony = CreateSolidBrush(RGB(255, 0, 0));
            HBRUSH pedzelZolty = CreateSolidBrush(RGB(255, 255, 0));

            POINT uszy[5] = {
                {srodek_kota_x - ROZMIAR_KOTA, srodek_kota_y}, // lewy bok
                {srodek_kota_x - ROZMIAR_KOTA/2, srodek_kota_y - ROZMIAR_KOTA / 2}, // wierzcholek lewego ucha
                {srodek_kota_x, srodek_kota_y}, // srodek
                {srodek_kota_x + ROZMIAR_KOTA / 2, srodek_kota_y - ROZMIAR_KOTA / 2}, // wierzcholek prawego ucha
                {srodek_kota_x + ROZMIAR_KOTA, srodek_kota_y}, // prawy bok
            };

            // WYPELNIENIE KOTA

            SelectObject(hdc, GetStockObject(NULL_PEN));
            SelectObject(hdc, pedzelCzerwony);

            Polygon(hdc, uszy, 5);
            
            Chord(hdc,
                srodek_kota_x - ROZMIAR_KOTA, srodek_kota_y - ROZMIAR_KOTA,   // Lewy-górny róg prostokąta opisującego elipsę
                srodek_kota_x + ROZMIAR_KOTA, srodek_kota_y + ROZMIAR_KOTA * 3 / 4,   // Prawy-dolny róg
                srodek_kota_x - ROZMIAR_KOTA, srodek_kota_y,   // Punkt początkowy łuku (po prawej)
                srodek_kota_x + ROZMIAR_KOTA, srodek_kota_y    // Punkt końcowy łuku (po lewej)
            );


            // PYSK KOTA

            SelectObject(hdc, pedzelZolty);

            // lewe oko
            Ellipse(hdc, 
                srodek_oka_lewego_x - rozmiar_oka,
                srodek_oka_lewego_y - rozmiar_oka,
                srodek_oka_lewego_x + rozmiar_oka,
                srodek_oka_lewego_y + rozmiar_oka
            );

            // prawe oko
            Ellipse(hdc,
                srodek_oka_prawego_x - rozmiar_oka,
                srodek_oka_prawego_y - rozmiar_oka,
                srodek_oka_prawego_x + rozmiar_oka,
                srodek_oka_prawego_y + rozmiar_oka
            );

            // morda kota
            POINT morda[3] = {
                {srodek_kota_x - ROZMIAR_KOTA/6, srodek_kota_y + ROZMIAR_KOTA / 3}, // lewy gorny
                {srodek_kota_x, srodek_kota_y + (ROZMIAR_KOTA / 3) * 1.5}, // srodek dol
                {srodek_kota_x + ROZMIAR_KOTA / 6, srodek_kota_y + ROZMIAR_KOTA / 3} // prawy gorny 
            };

            Polygon(hdc, morda, 3);

            // OBRYS KOTA
            
            SelectObject(hdc, pioro);

            Polyline(hdc, uszy, 5);

            Arc(hdc,
                srodek_kota_x - ROZMIAR_KOTA, srodek_kota_y - ROZMIAR_KOTA,   // Lewy-górny róg prostokąta opisującego elipsę
                srodek_kota_x + ROZMIAR_KOTA, srodek_kota_y + ROZMIAR_KOTA * 3 / 4,   // Prawy-dolny róg
                srodek_kota_x - ROZMIAR_KOTA, srodek_kota_y,   // Punkt początkowy łuku (po prawej)
                srodek_kota_x + ROZMIAR_KOTA, srodek_kota_y    // Punkt końcowy łuku (po lewej)
            );

            DeleteObject(pioro);
            DeleteObject(pedzelCzerwony);
            DeleteObject(pedzelZolty);

            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_MOUSEMOVE: {
            mysz_x = LOWORD(lParam);
            mysz_y = HIWORD(lParam);

            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}