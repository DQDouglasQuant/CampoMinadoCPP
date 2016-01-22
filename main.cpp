#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GRID_SIZE 10
#define NUM_MINES 10
#define ID_BTN_START 1000

HWND hButtons[GRID_SIZE][GRID_SIZE];
int board[GRID_SIZE][GRID_SIZE]; // -1 = mina, 0-8 = quantidade de minas ao redor
int state[GRID_SIZE][GRID_SIZE]; // 0 = oculto, 1 = revelado, 2 = com bandeira
bool gameOver = false;

WNDPROC oldButtonProc;

// Subclass procedure: Isso permite capturar o clique com o botao direito nos botoes!
LRESULT CALLBACK ButtonSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_RBUTTONDOWN) {
        int id = GetWindowLong(hWnd, GWL_ID) - ID_BTN_START;
        int r = id / GRID_SIZE;
        int c = id % GRID_SIZE;
        if (!gameOver && state[r][c] != 1) {
            if (state[r][c] == 0) {
                state[r][c] = 2;
                SetWindowText(hWnd, "B"); // 'B' de Bandeira
            } else if (state[r][c] == 2) {
                state[r][c] = 0;
                SetWindowText(hWnd, "");
            }
        }
        return 0;
    }
    return CallWindowProc(oldButtonProc, hWnd, uMsg, wParam, lParam);
}

void PlaceMines() {
    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++) {
            board[i][j] = 0;
            state[i][j] = 0;
            SetWindowText(hButtons[i][j], "");
            EnableWindow(hButtons[i][j], TRUE);
        }
    
    int minesPlaced = 0;
    while (minesPlaced < NUM_MINES) {
        int r = rand() % GRID_SIZE;
        int c = rand() % GRID_SIZE;
        if (board[r][c] != -1) {
            board[r][c] = -1;
            minesPlaced++;
        }
    }
    
    // Calcula numeros ao redor
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (board[i][j] == -1) continue;
            int count = 0;
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    int nr = i + dr, nc = j + dc;
                    if (nr >= 0 && nr < GRID_SIZE && nc >= 0 && nc < GRID_SIZE && board[nr][nc] == -1) {
                        count++;
                    }
                }
            }
            board[i][j] = count;
        }
    }
    gameOver = false;
}

void Reveal(int r, int c) {
    if (r < 0 || r >= GRID_SIZE || c < 0 || c >= GRID_SIZE) return;
    if (state[r][c] != 0 || gameOver) return;
    
    state[r][c] = 1;
    EnableWindow(hButtons[r][c], FALSE);
    
    if (board[r][c] == -1) {
        SetWindowText(hButtons[r][c], "*");
        gameOver = true;
        MessageBox(NULL, "BOOM! Voce explodiu uma mina!", "Fim de Jogo", MB_OK | MB_ICONERROR);
        PlaceMines(); // Reinicia
        return;
    } else if (board[r][c] > 0) {
        char txt[2];
        sprintf(txt, "%d", board[r][c]);
        SetWindowText(hButtons[r][c], txt);
    } else {
        // Flood fill (Revela zeros em volta)
        SetWindowText(hButtons[r][c], "");
        for (int dr = -1; dr <= 1; dr++) {
            for (int dc = -1; dc <= 1; dc++) {
                Reveal(r + dr, c + dc);
            }
        }
    }
    
    // Checa vitoria
    int hidden = 0;
    for (int i=0; i<GRID_SIZE; i++)
        for (int j=0; j<GRID_SIZE; j++)
            if (state[i][j] != 1) hidden++;
    
    if (hidden == NUM_MINES && !gameOver) {
        gameOver = true;
        MessageBox(NULL, "Parabens! Voce encontrou todas as minas!", "Vitoria", MB_OK | MB_ICONINFORMATION);
        PlaceMines();
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            srand((unsigned)time(NULL));
            for (int i = 0; i < GRID_SIZE; i++) {
                for (int j = 0; j < GRID_SIZE; j++) {
                    int id = ID_BTN_START + (i * GRID_SIZE + j);
                    hButtons[i][j] = CreateWindow("BUTTON", "",
                        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                        10 + j * 30, 10 + i * 30, 30, 30,
                        hwnd, (HMENU)(INT_PTR)id, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
                        
                    // Subclassing button to catch right-clicks (Truque classico de Win32!)
                    oldButtonProc = (WNDPROC)SetWindowLongPtr(hButtons[i][j], GWLP_WNDPROC, (LONG_PTR)ButtonSubclassProc);
                }
            }
            PlaceMines();
            return 0;
        }
        case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            if (wmId >= ID_BTN_START && wmId < ID_BTN_START + (GRID_SIZE * GRID_SIZE)) {
                int id = wmId - ID_BTN_START;
                int r = id / GRID_SIZE;
                int c = id % GRID_SIZE;
                
                // Segurar SHIFT + Clique esquerdo tambem coloca bandeira, para facilitar no notebook
                if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
                    if (!gameOver && state[r][c] != 1) {
                        if (state[r][c] == 0) {
                            state[r][c] = 2;
                            SetWindowText(hButtons[r][c], "B");
                        } else if (state[r][c] == 2) {
                            state[r][c] = 0;
                            SetWindowText(hButtons[r][c], "");
                        }
                    }
                } else {
                    if (state[r][c] == 0) {
                        Reveal(r, c);
                    }
                }
            }
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "MinesweeperClass";
    
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1); // Fundo cinza padrao do Windows

    RegisterClass(&wc);

    // Ajusta o tamanho da janela
    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "Campo Minado - Edicao 2012",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 335, 360,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
