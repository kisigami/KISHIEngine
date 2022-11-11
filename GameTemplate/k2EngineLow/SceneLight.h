#pragma once

namespace nsK2EngineLow
{
	/// <summary>
	/// ディレクションライトの構造体
	/// </summary>
	struct Directionlight
	{
		Vector3 ligDirection;     //ライトの方向
		float pad;                //パディング
		Vector3 ligColor;         //ライトのカラー
		float pad3;               //パディング
	};
	/// <summary>
	/// ライトの構造体
	/// </summary>
	struct Light
	{
		Directionlight directionlight;      //ディレクションライト

		Vector3 eyePos;                     //視点
		float pad1;                         //パディング
		Vector3 ambientLight;               //環境光
		float pad2;
	};

	/// <summary>
	/// シーンライトクラス
	/// </summary>
	class SceneLight
	{
	public:
		SceneLight() {};
		~SceneLight() {};
		/// <summary>
		/// 初期化
		/// </summary>
		void Init();
		/// <summary>
		/// 更新
		/// </summary>
		void Update();

		Light& GetSceneLight()
		{
			return m_light;
		}

		/// <summary>
		/// ディレクションライトの方向の設定
		/// </summary>
		/// <param name="lig">方向</param>
		void SetLigDirection(const Vector3& lig)
		{
			m_light.directionlight.ligDirection = lig;
		}
		/// <summary>
		/// ディレクションライトのカラーの設定
		/// </summary>
		/// <param name="color">カラー</param>
		void SetLigColor(const Vector3& color)
		{
			m_light.directionlight.ligColor = color;
		}
		/// <summary>
		/// 視点の設定
		/// </summary>
		/// <param name="lig">視点</param>
		void SetEyePos(const Vector3& lig)
		{
			m_light.eyePos = lig;
		}
		/// <summary>
		/// 環境光の設定
		/// </summary>
		/// <param name="lig">強さ</param>
		void SetAmbientLihgt(const Vector3& lig)
		{
			m_light.ambientLight = lig;
		}

	private:
		Light          m_light;   //ライト
	};

	//グローバル
	extern SceneLight g_sceneLight;
};