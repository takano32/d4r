#include <windows.h>
#include "InputSystem.h"
#include "JoystickDevice.h"
#include "KeyboardDevice.h"

using namespace base;

// �v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);       // �E�C���h�E�v���V�[�W��


// �O���[�o���ϐ�
BOOL end = FALSE;                                           // �I������


//==============================================================================================
// ���C������
//==============================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // �E�C���h�E�N���X�\����
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(WNDCLASS));
    wc.style         = CS_HREDRAW | CS_VREDRAW;             // �E�C���h�E�X�^�C��
    wc.lpfnWndProc   = (WNDPROC)WndProc;                    // �E�C���h�E�v���V�[�W���A�h���X
    wc.cbClsExtra    = 0;                                   // �⏕�̈�T�C�Y
    wc.cbWndExtra    = 0;                                   // �⏕�̈�T�C�Y
    wc.hInstance     = hInstance;                           // �C���X�^���X�n���h��
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);     // �A�C�R��
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);         // �}�E�X�J�[�\��
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);          // �N���C�A���g�̈�w�i�F
    wc.lpszMenuName  = NULL;                                // �E�C���h�E���j���[
    wc.lpszClassName = "Windows";                           // �E�C���h�E�N���X��

    // �E�C���h�E�N���X�̓o�^
    if(!RegisterClass(&wc)) return FALSE;

    // �E�C���h�E�̍쐬
    HWND hWnd = CreateWindow(
        wc.lpszClassName,                                   // �E�C���h�E�N���X��
        "�^�C�g��",                                         // �E�C���h�E�^�C�g��
        WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,        // �E�C���h�E�X�^�C��
        CW_USEDEFAULT,                                      // �\���w���W
        CW_USEDEFAULT,                                      // �\���x���W
        640,                                              // ��
        480,                                             // ����
        NULL,                                               // �e�E�C���h�E
        NULL,                                               // �E�C���h�E���j���[
        hInstance,                                          // �C���X�^���X�n���h��
        NULL);                                              // WM_CREATE���

    // DirectInput �̏�����
    //if(!InitDInput(hWnd, hInstance)) return FALSE;
	InputSystem* pSystem= new InputSystem( hWnd );
	//KeyboardDevice* pDevice = new KeyboardDevice( pSystem );
	JoystickDevice* pDevice = new JoystickDevice( pSystem );

    // �E�C���h�E�̕\��
    ShowWindow(hWnd, nCmdShow);                             // �\����Ԃ̐ݒ�
    UpdateWindow(hWnd);                                     // �N���C�A���g�̈�̍X�V

    // �f�o�C�X�R���e�L�X�g�̎擾
    HDC hDC = GetDC(hWnd);

    // ���b�Z�[�W���[�v
    MSG msg;
    while(!end)
    {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
            TranslateMessage(&msg);                         // ���z�L�[���b�Z�[�W�̕ϊ�
            DispatchMessage(&msg);                          // �E�C���h�E�v���V�[�W���֓]��
        } else{
            PatBlt(hDC, 0, 0, 640, 480, WHITENESS);         // ��ʂ̃N���A
            pDevice->DumpStatus(hDC);                             // �W���C�X�e�B�b�N���͏���
            Sleep(60);                                      // �ҋ@
        }
    }

    // �f�o�C�X�R���e�L�X�g�̉��
    ReleaseDC(hWnd, hDC);

    return msg.wParam;
}

//==============================================================================================
// �E�C���h�E�v���V�[�W��
//==============================================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_DESTROY:
        // �E�C���h�E���j�����ꂽ�Ƃ��̏���
        end = TRUE;
        PostQuitMessage(0);
        return 0;

    default:
        // �f�t�H���g����
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}
 