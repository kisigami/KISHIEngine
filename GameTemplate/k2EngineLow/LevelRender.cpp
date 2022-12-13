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
		//tkl�t�@�C�������[�h����B
		m_tklFile.Load(filePath);

		MatrixTklToLevel();

		//���[�g�{�[���͂���Ȃ����߁Ai��1����n�߂邱�Ƃŏ��O�ł���B
		for (int i = 1; i < m_bonelist.size(); i++) {
			auto bone = m_bonelist[i].get();
			if (bone->GetParentBoneNo() == 0) {

				//�}�b�v�`�b�v���쐬����B
				//�s�񂩂�|�W�V�������쐬����B
				LevelObjectData levelObjData;
				bone->CalcWorldTRS(levelObjData.position, levelObjData.rotation, levelObjData.scale);

				//Zup��Yup�̕ύX�B
				float fix = levelObjData.position.y;
				levelObjData.position.y = levelObjData.position.z;
				levelObjData.position.z = -fix;

				fix = levelObjData.rotation.y;
				levelObjData.rotation.y = levelObjData.rotation.z;
				levelObjData.rotation.z = -fix;

				std::swap(levelObjData.scale.y, levelObjData.scale.z);
				//Zup��Yup�@�I���B



				levelObjData.name = m_bonelist[i]->GetName();
				std::wstring name = levelObjData.name;
				//@�����邩�ǂ����A��������B
				int pos = static_cast<int>(name.find('@'));
				//@�����݂��Ă���΁A@��̐������擾����B
				if (pos != std::wstring::npos)
				{
					std::wstring number = name.substr(static_cast<size_t>(pos) + 1);
					//wchar�^��int�^�ɕϊ����Ă���B
					levelObjData.number = std::stoi(number.c_str());
				}
				//�p�X�̍쐬
				wchar_t filePath[256];
				swprintf_s(filePath, L"Assets/modelData/%s.tkm", levelObjData.name);
				//�}���`�o�C�g�ɕϊ� wchar�� char
				//�x�[�X�̕�����B
				size_t origsize = wcslen(filePath) + 1;
				//�ϊ�����������
				size_t convertedChars = 0;
				//�}���`�o�C�g�i�[�p
				char strCon[] = "";
				//�T�C�Y
				size_t strConSize = (strlen(strCon) + 1) * 2;
				//�ϊ���̃T�C�Y�B
				const size_t newsize = origsize * 2;
				//�A����̃T�C�Y
				char* cFilePath = new char[newsize + strConSize];
				//�}���`�o�C�g�ɕϊ�����B���肫��Ȃ����̂͐؂肷��
				wcstombs_s(&convertedChars, cFilePath, newsize, filePath, _TRUNCATE);
				//������̘A��
				_mbscat_s((unsigned char*)cFilePath, newsize + strConSize, (unsigned char*)strCon);

				//Hook���o�^�ς݂Ȃ�΃}�b�v�`�b�v�͍쐬�s�v
				//false �̂܂܂Ȃ�쐬����B
				bool isHooked = false;
				//hookFunc�ɂȂɂ������Ă��Ȃ���Sobject�ɂ͉����Ȃ��B
				if (hookFunc != nullptr) {
					isHooked = hookFunc(levelObjData);
				}

				//hook��false�Ȃ܂܂Ȃ�΁B
				if (isHooked == false) {
					//�}�b�v�`�b�v�����_�[���쐬�B
					CreateMapChipRender(levelObjData, cFilePath);
				}

			}
		}
	}

	void LevelRender::Update()
	{
		for (auto& mapChipRender : m_mapChipRenderPtrs)
		{
			//�}�b�v�`�b�v�����_�[���X�V����B
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