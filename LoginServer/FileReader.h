#pragma once

#include <io.h>
#include <fstream>
#include <direct.h>
#include <vector>

#define TableFileExName	"json"

namespace TerraX
{
	struct FileInfo_t
	{
		char szFilePath[256];
		char szFileName[32];
		char szFileExt[8];
	};
	class FileReader
	{
	public:
		FileReader(const std::string& szFilePath)
		{
			ReadFiles(szFilePath);
		}
		std::vector<FileInfo_t>& GetFileNames() { return m_FileInfoVec; }
	private:
		void ReadFiles(const std::string& szFilePath)
		{
			//文件句柄  
			long   hFile = 0;
			FileInfo_t fi;
			//文件信息  
			struct _finddata_t fileinfo;  //很少用的文件信息读取结构
			std::string szFilePathTmp = szFilePath;
			if ((hFile = _findfirst(szFilePathTmp.assign(szFilePath).append("\\*").c_str(), &fileinfo)) != -1)
			{
				do
				{
					if ((fileinfo.attrib &  _A_SUBDIR))  //比较文件类型是否是文件夹
					{
						if (strncmp(fileinfo.name, ".", 1) != 0 && strncmp(fileinfo.name, "..", 2) != 0)
						{
							//m_FileNameVec.push_back(szFilePathTmp.assign(szFilePath).append("\\").append(fileinfo.name) );
							ReadFiles(szFilePathTmp.assign(szFilePath).append("\\").append(fileinfo.name));
						}
					}
					else
					{
						ExtractFileInfo(fileinfo.name, fi.szFileName, fi.szFileExt);
						if (strncmp(fi.szFileExt, TableFileExName, sizeof(TableFileExName)) == 0)
						{
							szFilePathTmp.assign(szFilePath).append("\\").append(fileinfo.name);
							strcpy(fi.szFilePath, szFilePathTmp.c_str());
							m_FileInfoVec.push_back(fi);
						}
					}
				} while (_findnext(hFile, &fileinfo) == 0);  //寻找下一个，成功返回0，否则-1
				_findclose(hFile);
			}
		}
		bool ExtractFileInfo(char* full_name, char* file_name, char* file_ext)
		{
			//unsafe, but does not matter
			char *ext = strrchr(full_name, '.');
			if (ext)
			{
				*ext = '\0';
				strcpy(file_name, full_name);
				strcpy(file_ext, ext + 1);
				*ext = '.';
				return true;
			}
			assert(0);
			return false;
		}
	private:
		std::vector<FileInfo_t> m_FileInfoVec;
	};
}