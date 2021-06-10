#include <iostream>
#include <codecvt>
#include <fstream>
#include <string>

#include "S3MBReader.h"
#include "S3MBWriter.h"
#include "ROGroupLite.h"
//#include "cpl_config.h"
#include <iosfwd>

#include "S3MBSCP.h"
using namespace std;
using namespace S3MB;

int main(int argc, char* argv[])
{

	///************************************************************************/
	//*          1.����һ�����������ݲ�д��S3MB�ļ�
	///************************************************************************/

	RenderOperationGroup* pGroup = ROGroupLite::CreateROGroupLite(0, L"./1.png");
	S3MBWriter m_S3MBWriter;
	m_S3MBWriter.SetROGroup(pGroup);
	m_S3MBWriter.SetIsChangeTexture(true,true);//DXTѹ��
	wstring strS3mbFile = L"./������0.s3mb";
	m_S3MBWriter.SetFilePath(strS3mbFile);
	m_S3MBWriter.Save(0);
	delete pGroup;
	m_S3MBWriter.Clear();

	pGroup = ROGroupLite::CreateROGroupLite(1, L"./2.jpg");
	pGroup->ReComputeBoundingBox();//����Χ��/�򲻶Ի��ް�Χ��/��ʱ�ɵ���
	m_S3MBWriter.SetROGroup(pGroup);
	m_S3MBWriter.SetIsChangeTexture(true,true);//DXTѹ��
	strS3mbFile = L"./������1.s3mb";
	m_S3MBWriter.SetFilePath(strS3mbFile);
	m_S3MBWriter.Save(0);
	delete pGroup;
	m_S3MBWriter.Clear();

	pGroup = ROGroupLite::CreateROGroupLite(2, L"./3.jpg");
	pGroup->ReComputeBoundingBox();
	m_S3MBWriter.SetROGroup(pGroup);
	m_S3MBWriter.SetIsChangeTexture(true, true);//DXTѹ��
	strS3mbFile = L"./������2.s3mb";
	m_S3MBWriter.SetFilePath(strS3mbFile);
	m_S3MBWriter.Save(0);
	delete pGroup;
	m_S3MBWriter.Clear();

	pGroup = ROGroupLite::CreateROGroupLite(3, L"./5.jpg");
	pGroup->ReComputeBoundingBox();
	m_S3MBWriter.SetROGroup(pGroup);
	m_S3MBWriter.SetIsChangeTexture(true, true);//DXTѹ��
	strS3mbFile = L"./������3.s3mb";
	m_S3MBWriter.SetFilePath(strS3mbFile);
	m_S3MBWriter.Save(0);
	m_S3MBWriter.Clear();
	///************************************************************************/
	//*        2:ͨ��ָ����s3mb�ļ�����ȡ����Ro��Ϣ�����������                                                                     */
   ///************************************************************************/


	S3MBReader m_S3MBReaderRo;

	wstring strS3MBFileSrc = L"./������0.s3mb";
	m_S3MBReaderRo.SetFilePath(strS3MBFileSrc);
	m_S3MBReaderRo.ReadS3MBAsROGroup();
	pGroup = m_S3MBReaderRo.GetRenderOperationGroup();
	///************************************************************************/
	//*       3:ͨ��ָ����s3mb�����ļ��У�������ȡs3md�����                                                                */
   ///************************************************************************/


	//S3MBReader m_S3MBReaderAtt;

	//string strS3MBFolderSrc = "..\\SampleData\\CBD_part";
	//m_S3MBReaderAtt.SetFilePath(strS3MBFolderSrc);
	//m_S3MBReaderAtt.ReadS3MBAttribute();



	//////////////////////����SCP�ļ�//////////////////
	//�����
	Point3D m_Position(118, 39, 0);
	Rect2D geoBounds(0, 0, 0, 0);
	int epsgCode = 4326;
	bool isDegree = true;

	BoundingBox m_Box;
	m_Box.SetNULL();
	for (int i = 0; i < pGroup->GetNumChildren(); i++)
	{
		if (NULL != pGroup->GetChild(i)->AsGeode())
		{
			m_Box.Merge(((RenderOperationGeode*)(pGroup->GetChild(i)))->GetBoundingBox());
		}
	}
	///����SCP�ļ�//////////////////
	S3MBSCP* pS3MBSCP = new S3MBSCP;
	pS3MBSCP->m_strAsset = L"SuperMap";

	pS3MBSCP->m_pntPosition = m_Position;
	pS3MBSCP->m_rcGeoBounds = geoBounds;
	pS3MBSCP->m_prjCoordEPSG = epsgCode;
	pS3MBSCP->m_bIsDegree = isDegree;
	pS3MBSCP->m_dbHeightMax = m_Box.GetMax().z + m_Position.z;
	pS3MBSCP->m_dbHeightMin = m_Box.GetMin().z + m_Position.z;
	//pS3MBSCP->m_enDataType = SDT_ObliquePhoto;

	pS3MBSCP->m_mapExtensions[L"s3m:FileType"] = L"OSGBCacheFile";
	pS3MBSCP->m_mapExtensions[L"s3m:RenderMode"] = L"Normal";
	pS3MBSCP->m_mapExtensions[L"s3m:TileSplitType"] = L"LOCAL";

	std::vector<wstring> vecRootFiles;//�����
	std::vector<BoundingBox> vecRootBoundboxs;//������ӦBoundingBox
	vecRootFiles.push_back(L"������0.s3mb");
	vecRootBoundboxs.push_back(m_Box);

	pS3MBSCP->SetTiles(vecRootFiles, vecRootBoundboxs);
	wstring strOutputFilePath = StringHelper::GetAbsolutePath(strS3MBFileSrc);
	wstring strFilePatch = StringHelper::GetAbsolutePath(strOutputFilePath, L"../sample.scp");
	pS3MBSCP->SaveToJsonFile(strFilePatch);
	delete pS3MBSCP;
	pS3MBSCP = NULL;
	system("pause");
	return 0;
}