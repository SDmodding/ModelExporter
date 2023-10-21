#define _CRT_SECURE_NO_WARNINGS
#include <array>
#include <iostream>
#include <Windows.h>
#include <string>
#include <filesystem>
#include <unordered_map>

#include "3rdParty/umHalf.h"

// Defines
#define PROJECT_VERSION		"v1.0.1"
#define PROJECT_NAME		"Model Exporter " PROJECT_VERSION

// SDK Stuff
#define UFG_PAD_INSERT(x, y) x ## y
#define UFG_PAD_DEFINE(x, y) UFG_PAD_INSERT(x, y)
#define UFG_PAD(size) char UFG_PAD_DEFINE(padding_, __LINE__)[size] = { 0x0 }

namespace UFG
{
	struct ResourceData_t
	{
		uint32_t m_TypeUID = 0x0;
		uint32_t m_EntrySize[2] = { 0x0, 0x0 };
		uint32_t m_Offset = 0x0;

		UFG_PAD(0x18);

		uint32_t m_NameUID = 0x0;

		UFG_PAD(0x14);

		uint32_t m_ChunkUID = 0x0;

		char m_DebugName[36];
	};
}

namespace Illusion
{
	struct Buffer_t : UFG::ResourceData_t
	{
		uint32_t m_Type = 0x0;
		uint32_t m_Size = 0x0;
		uint64_t m_DataOffset = 0xD0;
		uint32_t m_ElementSize = 0x0;
		uint32_t m_NumElements = 0x0;

		uint8_t m_BufferPadding[192] = {
			0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x4D, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC6, 0x42,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x30, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		};

		void* GetData()
		{
			return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(&m_DataOffset) + m_DataOffset);
		}
	};

	struct Mesh_t
	{
		struct Handle_t
		{
			UFG_PAD(0x18);

			uint32_t m_NameUID = 0x0;

			UFG_PAD(0x4);
		};

		Handle_t m_MaterialHandle;
		Handle_t m_VertexDeclHandle;
		Handle_t m_IndexBufferHandle;
		Handle_t m_VertexBufferHandles[4];
		int m_PrimType = 0;
		int m_IndexStart = 0;
		uint32_t m_NumPrims = 0;
		uint32_t m_Pad = 0;
		const char* m_Description = nullptr;
	};

	struct MaterialParam_t
	{
		struct StateParam_t
		{
			uint32_t m_NameUID;
			uint32_t m_TypeUID;
			uint16_t m_ParamIndex;

			UFG_PAD(0x2);
		};

		StateParam_t m_StateParam;

		UFG_PAD(0x1C);

		uint32_t m_NameUID;

		UFG_PAD(0x4);

		uint32_t m_TypeUID;

		UFG_PAD(0x4);
	};

	struct MaterialUser_t
	{
		uint16_t m_VisibilityFlags = 0x1F;
		uint16_t mShadowFlags = 0x0;
		uint8_t m_Align[12] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	};

	struct Material_t : UFG::ResourceData_t
	{
		UFG_PAD(0x8);

		uint64_t m_StateBlockMask[2] = { 0x0, 0x0 };
		uint32_t m_NumParams;

		UFG_PAD(0x4);

		uint64_t m_MaterialUserOffset = 0;

		MaterialParam_t* GetParam(uint32_t p_NameUID)
		{
			MaterialParam_t* m_ParamTable = reinterpret_cast<MaterialParam_t*>(reinterpret_cast<uintptr_t>(this) + sizeof(Material_t));
			for (uint32_t i = 0; m_NumParams > i; ++i)
			{
				MaterialParam_t* m_Param = &m_ParamTable[i];
				if (m_Param->m_StateParam.m_NameUID == p_NameUID)
					return m_Param;
			}

			return nullptr;
		}
	};

	struct Model_t : UFG::ResourceData_t
	{
		float m_AABBMin[3];
		uint32_t m_NumPrims;
		float m_AABBMax[3];

		UFG_PAD(0x44);

		uint64_t m_MeshOffset;
		uint32_t m_NumMeshes;

		Mesh_t* GetMesh(uint32_t p_Index)
		{
			uintptr_t* m_TableOffset = reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(&m_MeshOffset) + m_MeshOffset);
			uintptr_t m_MeshTable = (reinterpret_cast<uintptr_t>(m_TableOffset) + *m_TableOffset);
			return reinterpret_cast<Mesh_t*>(m_MeshTable + (static_cast<uintptr_t>(p_Index) * 0x110));
		}
	};
}

// Actual Project
struct PermFile_t
{
	uint8_t* m_Data = nullptr;
	size_t m_Size = 0;

	std::string m_Name;
	std::vector<UFG::ResourceData_t*> m_Resources;

	~PermFile_t()
	{
		if (m_Data)
			delete[] m_Data;
	}

	PermFile_t() { }

	UFG::ResourceData_t* GetResourceData(uint32_t p_NameUID)
	{
		for (UFG::ResourceData_t* m_ResourceData : m_Resources)
		{
			if (m_ResourceData->m_NameUID == p_NameUID)
				return m_ResourceData;
		}
		
		return nullptr;
	}

	void ParseData()
	{
		size_t m_Offset = 0;
		while (m_Size > m_Offset)
		{
			UFG::ResourceData_t* m_ResourceData = reinterpret_cast<UFG::ResourceData_t*>(&m_Data[m_Offset]);
			if (m_ResourceData->m_TypeUID && m_ResourceData->m_EntrySize[0])
			{
				if (m_ResourceData->m_Offset)
				{
					UFG::ResourceData_t* m_ResourceDataOffset = reinterpret_cast<UFG::ResourceData_t*>(&m_Data[m_Offset + m_ResourceData->m_Offset]);
					m_Resources.emplace_back(m_ResourceDataOffset);
				}
				else
					m_Resources.emplace_back(m_ResourceData);
			}

			m_Offset += static_cast<size_t>(m_ResourceData->m_EntrySize[0]) + 0x10;
		}
	}

	bool LoadFile(const char* p_FilePath)
	{
		FILE* m_File = fopen(p_FilePath, "rb");
		if (!m_File)
			return false;

		m_Name = std::filesystem::path(p_FilePath).filename().string();
		m_Name = m_Name.substr(0, m_Name.find_first_of('.'));

		fseek(m_File, 0, SEEK_END);

		m_Size = static_cast<size_t>(ftell(m_File));
		m_Data = new uint8_t[m_Size];

		fseek(m_File, 0, SEEK_SET);

		size_t m_ReadRes = fread(m_Data, 1, m_Size, m_File);

		fclose(m_File);

		if (m_ReadRes != m_Size)
			return false;

		ParseData();
		return true;
	}
};

struct MtlFile_t
{
	std::string m_Name;
	float m_Ka[3];
	float m_Kd[3];
	float m_Ks[3];
	float m_D;
	int m_Illum;

	std::string m_MapKd;
	std::string m_MapBump;
	std::string m_MapKs;

	void WriteToFile(FILE* p_File)
	{
		fprintf(p_File, "newmtl %s\n", &m_Name[0]);
		fprintf(p_File, "Ka %f %f %f\n", m_Ka[0], m_Ka[1], m_Ka[2]);
		fprintf(p_File, "Kd %f %f %f\n", m_Kd[0], m_Kd[1], m_Kd[2]);
		fprintf(p_File, "Ks %f %f %f\n", m_Ks[0], m_Ks[1], m_Ks[2]);
		fprintf(p_File, "d %f\n", m_D);
		fprintf(p_File, "illum %d\n", m_Illum);

		if (!m_MapKd.empty())
			fprintf(p_File, "map_Kd %s\n", &m_MapKd[0]);

		if (!m_MapBump.empty())
			fprintf(p_File, "map_bump %s\n", &m_MapBump[0]);

		if (!m_MapKs.empty())
			fprintf(p_File, "map_Ks %s\n", &m_MapKs[0]);
	}
};

namespace Helper
{
	void GetUV(uint16_t* p_UVBytes, float* p_UV)
	{
		HalfFloat m_HalfFloat;
		{
			m_HalfFloat.bits = p_UVBytes[0];
			p_UV[0] = m_HalfFloat;

			m_HalfFloat.bits = p_UVBytes[1];
			p_UV[1] = (1.f - m_HalfFloat.operator float());
		}
	}

	void GetNormal(uint8_t* p_NormalBytes, float* p_Normal)
	{
		for (int i = 0; 3 > i; ++i)
			p_Normal[i] = (static_cast<float>(p_NormalBytes[i]) / 255.f) * 2.f - 1.f;
	}

	void GetColor(uint8_t* p_ColorBytes, float* p_Colors)
	{
		for (int i = 0; 3 > i; ++i)
			p_Colors[i] = (static_cast<float>(p_ColorBytes[i]) / 255.f);
	}

	bool GetTextureColor(uint32_t p_NameUID, float* p_Array)
	{
		static std::unordered_map<uint32_t, std::array<float, 3>> m_Map =
		{
			{ 0xCCBEDA91, { 0.95f, 0.95f, 0.95f } } // V_PAINT01_D
		};

		auto m_Find = m_Map.find(p_NameUID);
		if (m_Find == m_Map.end())
			return false;

		memcpy(p_Array, (*m_Find).second.data(), (sizeof(float) * 3));
		return true;
	}

	std::string GetPermFilePath()
	{
		OPENFILENAMEA m_OpenFileName = { 0 };
		ZeroMemory(&m_OpenFileName, sizeof(OPENFILENAMEA));

		m_OpenFileName.lStructSize = sizeof(OPENFILENAMEA);

		static char m_FilePath[MAX_PATH] = { '\0' };
		m_OpenFileName.lpstrFile = m_FilePath;
		m_OpenFileName.nMaxFile = sizeof(m_FilePath);
		m_OpenFileName.lpstrTitle = "Select Perm File";
		m_OpenFileName.lpstrFilter = "(Perm File)\0*.bin\0";
		m_OpenFileName.Flags = (OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST);

		if (GetOpenFileNameA(&m_OpenFileName) == 0)
			m_OpenFileName.lpstrFile[0] = '\0';

		return m_OpenFileName.lpstrFile;
	}
}

int g_Argc = 0;
char** g_Argv = nullptr;
void InitArgParam(int p_Argc, char** p_Argv)
{
	g_Argc = p_Argc;
	g_Argv = p_Argv;
}

std::string GetArgParam(const char* p_Arg)
{
	for (int i = 0; g_Argc > i; ++i)
	{
		if (_stricmp(p_Arg, g_Argv[i]) != 0)
			continue;

		int m_ParamIndex = (i + 1);
		if (m_ParamIndex >= g_Argc)
			break;

		return g_Argv[m_ParamIndex];
	}

	return "";
}

bool HasArgSet(const char* p_Arg)
{
	for (int i = 0; g_Argc > i; ++i)
	{
		if (_stricmp(p_Arg, g_Argv[i]) == 0)
			return true;
	}

	return false;
}

void ReturnKeyWait()
{
	if (HasArgSet("-silent"))
		return;

	int m_Dummy = getchar();
}

int main(int p_Argc, char** p_Argv)
{
#ifdef _DEBUG
	if (!IsDebuggerPresent())
		int m_DebugKey = getchar();
#endif

	char m_CurrentDirectory[MAX_PATH] = { '\0' };
	GetCurrentDirectoryA(sizeof(m_CurrentDirectory), m_CurrentDirectory);

	SetConsoleTitleA(PROJECT_NAME);
	InitArgParam(p_Argc, p_Argv);

	std::string m_PermPath	= GetArgParam("-perm");
	std::string m_OutputDir = GetArgParam("-outdir");

	if (m_PermPath.empty())
	{
		printf("[ ! ] No '-perm' defined, opening file explorer to select file.\n");
		m_PermPath = Helper::GetPermFilePath();

		if (m_PermPath.empty())
		{
			printf("[ ERROR ] You didn't select perm file!\n"); ReturnKeyWait();
			return 1;
		}
	}

	if (m_PermPath.find(".perm.bin") == std::string::npos)
	{
		printf("[ ERROR ] This is not perm file!\n"); ReturnKeyWait();
		return 1;
	}

	PermFile_t m_PermFile;
	if (!m_PermFile.LoadFile(&m_PermPath[0]))
	{
		printf("[ ERROR ] Failed to load perm file!\n"); ReturnKeyWait();
		return 1;
	}
	
	if (m_PermFile.m_Resources.empty())
	{
		printf("[ ERROR ] This perm file doesn't seems to have any resources!\n"); ReturnKeyWait();
		return 1;
	}

	std::vector<Illusion::Model_t*> m_Models;
	for (UFG::ResourceData_t* m_ResourceData : m_PermFile.m_Resources)
	{
		if (m_ResourceData->m_TypeUID != 0x6DF963B3) // Is 'ModelData'?
			continue;

		m_Models.emplace_back(reinterpret_cast<Illusion::Model_t*>(m_ResourceData));
	}

	if (m_Models.empty())
	{
		printf("[ ERROR ] This perm file doesn't seems to have any models!\n"); ReturnKeyWait();
		return 1;
	}

	// Setup Output Directory
	{
		if (m_OutputDir.empty())
			m_OutputDir = m_PermFile.m_Name;

		if (m_CurrentDirectory[0] != '\0')
			SetCurrentDirectoryA(m_CurrentDirectory); // Restore working directory, just in case...

		std::filesystem::create_directory(m_OutputDir);
	}

	size_t m_ModelIndex = 0;
	size_t m_ModelsExported = 0;

	for (Illusion::Model_t* m_Model : m_Models)
	{
		++m_ModelIndex;
		if (!m_Model->m_NumMeshes)
			continue; // What the fuck?

		printf("\n[%zu/%zu] Exporting: %s\n", m_ModelIndex, m_Models.size(), m_Model->m_DebugName);
		printf("\tNumMeshes: %u\n", m_Model->m_NumMeshes);

		std::string m_MtlFilePath = (m_OutputDir + "\\" + m_Model->m_DebugName + ".mtl");
		FILE* m_MtlFile = fopen(&m_MtlFilePath[0], "w");
		if (m_MtlFile)
		{
			for (uint32_t m = 0; m_Model->m_NumMeshes > m; ++m)
			{
				Illusion::Mesh_t* m_Mesh = m_Model->GetMesh(m);
				Illusion::Material_t* m_Material = reinterpret_cast<Illusion::Material_t*>(m_PermFile.GetResourceData(m_Mesh->m_MaterialHandle.m_NameUID));

				if (!m_Material)
				{
					printf("\t\t[ Error ] Material at index %u couldn't be found.\n", m);
					continue;
				}

				printf("\t\t[ Material ] %s\n", m_Material->m_DebugName);

				fprintf(m_MtlFile, "# Material: %s\n", m_Material->m_DebugName);

				char m_Format[256];
				sprintf_s(m_Format, sizeof(m_Format), "0x%X", m_Material->m_NameUID);

				MtlFile_t m_MtlBase;
				{
					m_MtlBase.m_Name = m_Format;
					m_MtlBase.m_Ka[0] = m_MtlBase.m_Ka[1] = m_MtlBase.m_Ka[2] = 1.f;
					m_MtlBase.m_Kd[0] = m_MtlBase.m_Kd[1] = m_MtlBase.m_Kd[2] = 0.5f;
					m_MtlBase.m_Ks[0] = m_MtlBase.m_Ks[1] = m_MtlBase.m_Ks[2] = 0.5f;
					m_MtlBase.m_D = 1.f;
					m_MtlBase.m_Illum = 0;
				}

				Illusion::MaterialParam_t* m_Diffuse = m_Material->GetParam(0xDCE06689); // texDiffuse
				{
					if (!m_Diffuse)
						m_Diffuse = m_Material->GetParam(0x19410F73); // texDiffuse2

					if (!m_Diffuse)
						m_Diffuse = m_Material->GetParam(0x5F95AF1); // texDiffuseWorld
				}

				if (m_Diffuse)
				{
					UFG::ResourceData_t* m_DiffuseResource = m_PermFile.GetResourceData(m_Diffuse->m_NameUID);
					if (m_DiffuseResource)
					{
						printf("\t\t\tDiffuse: %s\n", m_DiffuseResource->m_DebugName);
						fprintf(m_MtlFile, "# Diffuse Name: %s\n", m_DiffuseResource->m_DebugName);
					}

					if (!Helper::GetTextureColor(m_Diffuse->m_NameUID, m_MtlBase.m_Kd))
					{
						sprintf_s(m_Format, sizeof(m_Format), "../Textures/0x%X.png", m_Diffuse->m_NameUID);
						m_MtlBase.m_MapKd = m_Format;
					}
				}

				Illusion::MaterialParam_t* m_Normal = m_Material->GetParam(0xECADC789); // texNormal
				if (m_Normal)
				{
					UFG::ResourceData_t* m_NormalResource = m_PermFile.GetResourceData(m_Normal->m_NameUID);
					if (m_NormalResource)
					{
						printf("\t\t\tNormal: %s\n", m_NormalResource->m_DebugName);
						fprintf(m_MtlFile, "# Normal Name: %s\n", m_NormalResource->m_DebugName);
					}

					sprintf_s(m_Format, sizeof(m_Format), "../Textures/0x%X.png", m_Normal->m_NameUID);
					m_MtlBase.m_MapBump = m_Format;
				}

				Illusion::MaterialParam_t* m_Specular = m_Material->GetParam(0xCB460EC7); // texSpecular
				if (m_Specular)
				{
					UFG::ResourceData_t* m_SpecularResource = m_PermFile.GetResourceData(m_Specular->m_NameUID);
					if (m_SpecularResource)
					{
						printf("\t\t\tSpecular: %s\n", m_SpecularResource->m_DebugName);
						fprintf(m_MtlFile, "# Specular Name: %s\n", m_SpecularResource->m_DebugName);
					}

					sprintf_s(m_Format, sizeof(m_Format), "../Textures/0x%X.png", m_Specular->m_NameUID);
					m_MtlBase.m_MapKs = m_Format;
				}

				m_MtlBase.WriteToFile(m_MtlFile);
				fprintf(m_MtlFile, "\n");
			}

			fclose(m_MtlFile);
		}

		std::string m_ObjFilePath = (m_OutputDir + "\\" + m_Model->m_DebugName + ".obj");
		FILE* m_ObjFile = fopen(&m_ObjFilePath[0], "w");
		if (m_ObjFile)
		{
			fprintf(m_ObjFile, "mtllib %s.mtl\n", m_Model->m_DebugName);
			fprintf(m_ObjFile, "s 1\n");

			uint32_t m_VertexIndex = 0;
			uint32_t m_UVIndex = 0;
			uint32_t m_NormalIndex = 0;
			for (uint32_t m = 0; m_Model->m_NumMeshes > m; ++m)
			{
				Illusion::Mesh_t* m_Mesh = m_Model->GetMesh(m);
				Illusion::Material_t* m_Material	= reinterpret_cast<Illusion::Material_t*>(m_PermFile.GetResourceData(m_Mesh->m_MaterialHandle.m_NameUID));
				Illusion::Buffer_t* m_IndexBuffer	= reinterpret_cast<Illusion::Buffer_t*>(m_PermFile.GetResourceData(m_Mesh->m_IndexBufferHandle.m_NameUID));
				Illusion::Buffer_t* m_VertexBuffer	= reinterpret_cast<Illusion::Buffer_t*>(m_PermFile.GetResourceData(m_Mesh->m_VertexBufferHandles[0].m_NameUID));
				Illusion::Buffer_t* m_UVBuffer		= reinterpret_cast<Illusion::Buffer_t*>(m_PermFile.GetResourceData(m_Mesh->m_VertexBufferHandles[1].m_NameUID));

				if (!m_IndexBuffer || !m_VertexBuffer || !m_UVBuffer)
				{
					printf("\t\t[ Error ] Mesh at index %u is missing some buffer.\n", m);
					continue;
				}

				printf("\t\t[ Mesh ] Index %u, Vertexes: %u, Prims: %u\n", m, m_VertexBuffer->m_ElementSize, m_Mesh->m_NumPrims);

				enum eVertexDecl : int
				{
					eVertexDecl_Unknown = 0,
					eVertexDecl_UVN,
					eVertexDecl_UVNT,
					eVertexDecl_UVNTC,
					eVertexDecl_UV2NTC,
					eVertexDecl_VehicleUVNTC,
					eVertexDecl_SlimUV,
					eVertexDecl_Skinned,
					eVertexDecl_SkinnedUVNT,
				};
				eVertexDecl m_VertexDecl = eVertexDecl_Unknown;

				switch (m_Mesh->m_VertexDeclHandle.m_NameUID)
				{
					case 0xF2700F96: m_VertexDecl = eVertexDecl_UVN; break;
					case 0x9BA68DBC: m_VertexDecl = eVertexDecl_UVNT; break;
					case 0x911E1A51: m_VertexDecl = eVertexDecl_UVNTC; break;
					case 0x78921EA0: m_VertexDecl = eVertexDecl_UV2NTC; break;
					case 0x276B9567: 
					{
						m_VertexDecl = eVertexDecl_Skinned;
						if (m_UVBuffer->m_ElementSize != 0x4)
							m_UVBuffer = reinterpret_cast<Illusion::Buffer_t*>(m_PermFile.GetResourceData(m_Mesh->m_VertexBufferHandles[2].m_NameUID));
					}
					break;
					case 0xE234EF7A: 
					{
						m_VertexDecl = eVertexDecl_VehicleUVNTC;
						if (m_UVBuffer->m_ElementSize != 0x4)
							m_UVBuffer = reinterpret_cast<Illusion::Buffer_t*>(m_PermFile.GetResourceData(m_Mesh->m_VertexBufferHandles[2].m_NameUID));
					}
					break;
					case 0x7E0D7533: m_VertexDecl = eVertexDecl_SlimUV; break;
					case 0xAC5D89E2:
					{
						m_VertexDecl = eVertexDecl_SkinnedUVNT;
						if (m_UVBuffer->m_ElementSize != 0x4)
							m_UVBuffer = reinterpret_cast<Illusion::Buffer_t*>(m_PermFile.GetResourceData(m_Mesh->m_VertexBufferHandles[2].m_NameUID));
					}
					break;
				}

				if (m_VertexDecl == eVertexDecl_Unknown)
				{
					printf("\t\t[ Error ] Mesh at index %u using unsupported vertex decl: 0x%X.\n", m, m_Mesh->m_VertexDeclHandle.m_NameUID);
					continue; // Unsupported Decl
				}

				fprintf(m_ObjFile, "o %s_%u\n", m_Model->m_DebugName, m);
				if (m_Material)
					fprintf(m_ObjFile, "usemtl 0x%X\n", m_Material->m_NameUID);

				uintptr_t m_VertexData = reinterpret_cast<uintptr_t>(m_VertexBuffer->GetData());
				uintptr_t m_UVData = reinterpret_cast<uintptr_t>(m_UVBuffer->GetData());

				// Vertices
				{
					switch (m_VertexDecl)
					{
						default:
						{
							for (uint32_t i = 0; m_VertexBuffer->m_NumElements > i; ++i)
							{
								float* m_Vertices = reinterpret_cast<float*>(m_VertexData + static_cast<uintptr_t>(m_VertexBuffer->m_ElementSize * i));
								fprintf(m_ObjFile, "v %f %f %f\n", m_Vertices[0], m_Vertices[1], m_Vertices[2]);
							}
						}
						break;
						case eVertexDecl_VehicleUVNTC:
						{
							struct VehicleUVNTC_t
							{
								float m_Vertices[4];
								uint8_t m_Normal[4];
								uint8_t m_Tangent[4];
							};

							for (uint32_t i = 0; m_VertexBuffer->m_NumElements > i; ++i)
							{
								VehicleUVNTC_t* m_VehicleUVNTC = reinterpret_cast<VehicleUVNTC_t*>(m_VertexData + static_cast<uintptr_t>(m_VertexBuffer->m_ElementSize * i));
								fprintf(m_ObjFile, "v %f %f %f\n", m_VehicleUVNTC->m_Vertices[0], m_VehicleUVNTC->m_Vertices[1], m_VehicleUVNTC->m_Vertices[2]);

								float m_Normal[3];
								Helper::GetNormal(m_VehicleUVNTC->m_Normal, m_Normal);

								fprintf(m_ObjFile, "vn %f %f %f\n", m_Normal[0], m_Normal[1], m_Normal[2]);
							}
						}
						break;
						case eVertexDecl_Skinned:
						case eVertexDecl_SkinnedUVNT:
						{
							struct Skinned_t
							{
								float m_Vertices[4];
								uint8_t m_Normal[4];
								uint8_t m_Tangent[4];
							}; 
							
							for (uint32_t i = 0; m_VertexBuffer->m_NumElements > i; ++i)
							{
								Skinned_t* m_Skinned = reinterpret_cast<Skinned_t*>(m_VertexData + static_cast<uintptr_t>(m_VertexBuffer->m_ElementSize * i));
								fprintf(m_ObjFile, "v %f %f %f\n", m_Skinned->m_Vertices[0], m_Skinned->m_Vertices[1], m_Skinned->m_Vertices[2]);

								float m_Normal[3];
								Helper::GetNormal(m_Skinned->m_Normal, m_Normal);

								fprintf(m_ObjFile, "vn %f %f %f\n", m_Normal[0], m_Normal[1], m_Normal[2]);
							}
						}
						break;
					}
				}

				// UV
				if (m_UVBuffer)
				{
					switch (m_VertexDecl)
					{
						case eVertexDecl_UVN:
						case eVertexDecl_UVNT:
						{
							struct UVN_t
							{
								uint16_t m_UV[2];
								uint8_t m_Normal[4];
							};

							for (uint32_t i = 0; m_UVBuffer->m_NumElements > i; ++i)
							{
								UVN_t* m_UVN = reinterpret_cast<UVN_t*>(m_UVData + static_cast<uintptr_t>(m_UVBuffer->m_ElementSize * i));
								float m_UV[2];
								Helper::GetUV(m_UVN->m_UV, m_UV);

								float m_Normal[3];
								Helper::GetNormal(m_UVN->m_Normal, m_Normal);

								fprintf(m_ObjFile, "vt %f %f\n", m_UV[0], m_UV[1]);
								fprintf(m_ObjFile, "vn %f %f %f\n", m_Normal[0], m_Normal[1], m_Normal[2]);
							}
						}
						break;
						case eVertexDecl_UVNTC:
						{
							struct UVNTC_t
							{
								uint16_t m_UV[2];
								uint8_t m_Normal[4];
								uint8_t m_Tangent[4];
								uint8_t m_Color[4];
							};

							for (uint32_t i = 0; m_UVBuffer->m_NumElements > i; ++i)
							{
								UVNTC_t* m_UVNTC = reinterpret_cast<UVNTC_t*>(m_UVData + static_cast<uintptr_t>(m_UVBuffer->m_ElementSize * i));

								float m_UV[2];
								Helper::GetUV(m_UVNTC->m_UV, m_UV);

								float m_Normal[3];
								Helper::GetNormal(m_UVNTC->m_Normal, m_Normal);

								fprintf(m_ObjFile, "vt %f %f\n", m_UV[0], m_UV[1]);
								fprintf(m_ObjFile, "vn %f %f %f\n", m_Normal[0], m_Normal[1], m_Normal[2]);
							}
						}
						break;
						case eVertexDecl_UV2NTC:
						{
							struct UV2NTC_t
							{
								uint16_t m_UV[2];
								uint16_t m_UV2[2];
								uint8_t m_Normal[4];
								uint8_t m_Tangent[4];
								uint8_t m_Color[4];
							};

							for (uint32_t i = 0; m_UVBuffer->m_NumElements > i; ++i)
							{
								UV2NTC_t* m_UV2NTC = reinterpret_cast<UV2NTC_t*>(m_UVData + static_cast<uintptr_t>(m_UVBuffer->m_ElementSize * i));

								float m_UV[2];
								Helper::GetUV(m_UV2NTC->m_UV, m_UV);

								float m_Normal[3];
								Helper::GetNormal(m_UV2NTC->m_Normal, m_Normal);

								fprintf(m_ObjFile, "vt %f %f\n", m_UV[0], m_UV[1]);
								fprintf(m_ObjFile, "vn %f %f %f\n", m_Normal[0], m_Normal[1], m_Normal[2]);
							}
						}
						break;
						case eVertexDecl_VehicleUVNTC:
						case eVertexDecl_SlimUV:
						case eVertexDecl_Skinned:
						case eVertexDecl_SkinnedUVNT:
						{
							struct SlimUV_t
							{
								uint16_t m_UV[2];
							};

							if (m_UVBuffer->m_ElementSize != 0x4)
								break;

							for (uint32_t i = 0; m_UVBuffer->m_NumElements > i; ++i)
							{
								SlimUV_t* m_SlimUV = reinterpret_cast<SlimUV_t*>(m_UVData + static_cast<uintptr_t>(m_UVBuffer->m_ElementSize * i));
								float m_UV[2];
								Helper::GetUV(m_SlimUV->m_UV, m_UV);

								fprintf(m_ObjFile, "vt %f %f\n", m_UV[0], m_UV[1]);
							}
						}
						break;
					}
				}

				// Faces
				{
					uint16_t* m_FacesArray = &reinterpret_cast<uint16_t*>(m_IndexBuffer->GetData())[m_Mesh->m_IndexStart];
					for (uint32_t i = 0; m_Mesh->m_NumPrims > i; ++i)
					{
						switch (m_Mesh->m_PrimType)
						{
							case 3: // Triangles
							{
								uint16_t* m_Faces = &m_FacesArray[i * 3];
								uint32_t m_Face1 = static_cast<uint32_t>(m_Faces[0] + 1);
								uint32_t m_Face2 = static_cast<uint32_t>(m_Faces[1] + 1);
								uint32_t m_Face3 = static_cast<uint32_t>(m_Faces[2] + 1);
								fprintf(m_ObjFile, "f %u/%u/%u %u/%u/%u %u/%u/%u\n", 
									m_Face1 + m_VertexIndex, m_Face1 + m_UVIndex, m_Face1,
									m_Face2 + m_VertexIndex, m_Face2 + m_UVIndex, m_Face2,
									m_Face3 + m_VertexIndex, m_Face3 + m_UVIndex, m_Face3);
							}
							break;
						}
					}
				}

				m_VertexIndex += m_VertexBuffer->m_NumElements;
				m_UVIndex += m_UVBuffer->m_NumElements;
			}

			fclose(m_ObjFile);
		}

		++m_ModelsExported;
	}

	printf("\n[ ~ ] Exported %zu models out of %zu.\n", m_ModelsExported, m_Models.size()); ReturnKeyWait();

	return 0;
}