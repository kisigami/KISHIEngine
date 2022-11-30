#include "stdafx.h"
#include "system/system.h"
#include "Game.h"

// K2EngineLow�̃O���[�o���A�N�Z�X�|�C���g�B
K2EngineLow* g_k2EngineLow = nullptr;

/// <summary>
/// ���C���֐�
/// </summary>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// �Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	// k2EngineLow�̏������B
	g_k2EngineLow = new K2EngineLow();
	g_k2EngineLow->Init(g_hWnd, FRAME_BUFFER_W, FRAME_BUFFER_H);

	g_sceneLight.Init();
	//�����_�����O�G���W���̏�����
	g_renderingEngine.Init();

	//�Q�[���쐬
	auto game = NewGO<Game>(0, "game");

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		auto& renderContext = g_graphicsEngine->GetRenderContext();
		// �t���[���̊J�n���ɌĂяo���K�v�����鏈�������s
		g_k2EngineLow->BeginFrame();
	
		//�����_�����O�^�[�Q�b�g�̐ݒ�
		g_renderingEngine.SetMainRenderTarget(renderContext);

		//�X�V����
		g_k2EngineLow->ExecuteUpdate();

		g_k2EngineLow->ExecuteRender();

		//�|�X�g�G�t�F�N�g���s
		g_renderingEngine.Excute(renderContext);

		// �t���[���̏I�����ɌĂяo���K�v�����鏈�������s�B
		g_k2EngineLow->EndFrame();
	}

	delete g_k2EngineLow;

	return 0;
}
