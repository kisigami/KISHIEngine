#pragma once

namespace nsK2EngineLow
{
	/// <summary>
	/// �f�B���N�V�������C�g�̍\����
	/// </summary>
	struct Directionlight
	{
		Vector3 ligDirection;     //���C�g�̕���
		float pad;                //�p�f�B���O
		Vector3 ligColor;         //���C�g�̃J���[
		float pad3;               //�p�f�B���O
	};
	/// <summary>
	/// ���C�g�̍\����
	/// </summary>
	struct Light
	{
		Directionlight directionlight;      //�f�B���N�V�������C�g

		Vector3 eyePos;                     //���_
		float pad1;                         //�p�f�B���O
		Vector3 ambientLight;               //����
		float pad2;
	};

	/// <summary>
	/// �V�[�����C�g�N���X
	/// </summary>
	class SceneLight
	{
	public:
		SceneLight() {};
		~SceneLight() {};
		/// <summary>
		/// ������
		/// </summary>
		void Init();
		/// <summary>
		/// �X�V
		/// </summary>
		void Update();

		Light& GetSceneLight()
		{
			return m_light;
		}

		/// <summary>
		/// �f�B���N�V�������C�g�̕����̐ݒ�
		/// </summary>
		/// <param name="lig">����</param>
		void SetLigDirection(const Vector3& lig)
		{
			m_light.directionlight.ligDirection = lig;
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̃J���[�̐ݒ�
		/// </summary>
		/// <param name="color">�J���[</param>
		void SetLigColor(const Vector3& color)
		{
			m_light.directionlight.ligColor = color;
		}
		/// <summary>
		/// ���_�̐ݒ�
		/// </summary>
		/// <param name="lig">���_</param>
		void SetEyePos(const Vector3& lig)
		{
			m_light.eyePos = lig;
		}
		/// <summary>
		/// �����̐ݒ�
		/// </summary>
		/// <param name="lig">����</param>
		void SetAmbientLihgt(const Vector3& lig)
		{
			m_light.ambientLight = lig;
		}

	private:
		Light          m_light;   //���C�g
	};

	//�O���[�o��
	extern SceneLight g_sceneLight;
};