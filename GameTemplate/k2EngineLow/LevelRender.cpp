#include "k2EngineLowPreCompile.h"
#include "LevelRender.h"
#include "level3D/TklFile.h"


namespace nsK2EngineLow
{
	LevelRender::~LevelRender()
	{

	}

	void LevelRender::Init(
		const char* filePath,
		std::function<bool(LevelObjectData& objData)> hookFunc
	)
	{
		//tklファイルをロードする。
		m_tklFile.Load(filePath);

		MatrixTklToLevel();

		//ルートボーンはいらないため、iを1から始めることで除外できる。
		for (int i = 1; i < m_bonelist.size(); i++) {
			auto bone = m_bonelist[i].get();
			if (bone->GetParentBoneNo() == 0) {

				//マップチップを作成する。
				//行列からポジションを作成する。
				LevelObjectData levelObjData;
				bone->CalcWorldTRS(levelObjData.position, levelObjData.rotation, levelObjData.scale);

				//ZupとYupの変更。
				float fix = levelObjData.position.y;
				levelObjData.position.y = levelObjData.position.z;
				levelObjData.position.z = -fix;

				fix = levelObjData.rotation.y;
				levelObjData.rotation.y = levelObjData.rotation.z;
				levelObjData.rotation.z = -fix;

				std::swap(levelObjData.scale.y, levelObjData.scale.z);
				//Zup⇔Yup　終わり。



				levelObjData.name = m_bonelist[i]->GetName();
				std::wstring name = levelObjData.name;
				//@があるかどうか、検索する。
				int pos = static_cast<int>(name.find('@'));
				//@が存在していれば、@後の数字を取得する。
				if (pos != std::wstring::npos)
				{
					std::wstring number = name.substr(static_cast<size_t>(pos) + 1);
					//wchar型をint型に変換している。
					levelObjData.number = std::stoi(number.c_str());
				}
				//パスの作成
				wchar_t filePath[256];
				swprintf_s(filePath, L"Assets/modelData/%s.tkm", levelObjData.name);
				//マルチバイトに変換 wchar→ char
				//ベースの文字列。
				size_t origsize = wcslen(filePath) + 1;
				//変換した文字列
				size_t convertedChars = 0;
				//マルチバイト格納用
				char strCon[] = "";
				//サイズ
				size_t strConSize = (strlen(strCon) + 1) * 2;
				//変換後のサイズ。
				const size_t newsize = origsize * 2;
				//連結後のサイズ
				char* cFilePath = new char[newsize + strConSize];
				//マルチバイトに変換する。入りきらないものは切りすて
				wcstombs_s(&convertedChars, cFilePath, newsize, filePath, _TRUNCATE);
				//文字列の連結
				_mbscat_s((unsigned char*)cFilePath, newsize + strConSize, (unsigned char*)strCon);

				//Hookが登録済みならばマップチップは作成不要
				//false のままなら作成する。
				bool isHooked = false;
				//hookFuncになにも入っていない＝Sobjectには何もない。
				if (hookFunc != nullptr) {
					isHooked = hookFunc(levelObjData);
				}

				//hookがfalseなままならば。
				if (isHooked == false) {
					//マップチップレンダーを作成。
					CreateMapChipRender(levelObjData, cFilePath);
				}

			}
		}
	}

	void LevelRender::Update()
	{
		for (auto& mapChipRender : m_mapChipRenderPtrs)
		{
			//マップチップレンダーを更新する。
			//mapChipRender.second->Update();
		}
	}

	void LevelRender::Draw(RenderContext& rc)
	{
		for (auto mapChipRenderPtr : m_mapChipRenderPtrs)
		{
			//mapChipRenderPtr.second->Draw(rc);
		}
	}
}