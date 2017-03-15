using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Data;
using Excel;

namespace excel2json
{
    public static class Exporter
    {
        private static string FormatPath(string path, bool IsFolder = false)
        {
            if(path == "" || path == null)
            {
                return path;
            }
            string str = path.Replace('/', '\\');
            if (IsFolder && str[str.Length - 1] != '\\')
            {
                str += "\\";
            }
            return str;
        }
        private static string GenerateFileName(string folder, string file, string ext)
        {
            if(folder == null || file == null || ext == null)
            {
                return null;
            }
            return folder + file + ext;
        }

        private static void CreateFolders(string root, string child)
        {
            if (root == null || root == "")
            {
                return;
            }
            if(child == null)
            {
                return;
            }
            string folder = FormatPath(root + child, true);
            if (!Directory.Exists(folder))
            {
                Directory.CreateDirectory(folder);
            }
        }

        public static void Export2Folder(string excelPath, int header, string encoding, string jsonPath, string sqlPath, bool lowcase)
        {
            excelPath = FormatPath(excelPath, true);
            List<string> folders = FileSearch.SearchFoldersInFolder(excelPath);

            if(jsonPath != null && jsonPath != "")
            {
                string jsonFolder = FormatPath(jsonPath, true);
                if (!Directory.Exists(jsonFolder + "Client\\"))
                {
                    Directory.CreateDirectory(jsonFolder + "Client\\");
                }
                if (!Directory.Exists(jsonFolder + "Server\\"))
                {
                    Directory.CreateDirectory(jsonFolder + "Server\\");
                }
            }
            foreach (string str in folders)
            {
                CreateFolders(jsonPath, "Client\\" + str);
                CreateFolders(jsonPath, "Server\\" + str);
                CreateFolders(sqlPath, str);
            }

            List<string> files = FileSearch.SearchFilesInFolder(excelPath, ".xlsx");
            foreach (string str in files)
            {
                Export2Json(GenerateFileName(excelPath, str, ".xlsx"), header, encoding, GenerateFileName(jsonPath, "Client\\" + str, ".json"), lowcase, "c");
                Export2Json(GenerateFileName(excelPath, str, ".xlsx"), header, encoding, GenerateFileName(jsonPath, "Server\\" + str, ".json"), lowcase, "s");
                Export2Sql(GenerateFileName(sqlPath, str, ".xlsx"), header, encoding, GenerateFileName(sqlPath, str, ".sql"));
            }
        }

        public static void Export2File(string excelPath, int header, string encoding, string jsonPath, string sqlPath, bool lowcase)
        {
            excelPath = FormatPath(excelPath, true);
            Export2Json(excelPath, header, encoding, jsonPath, lowcase, "c");
            Export2Json(excelPath, header, encoding, jsonPath, lowcase, "s");
            Export2Sql(excelPath, header, encoding, sqlPath);
        }

        private static DataTable ReadExcel(string path)
        {
            path = FormatPath(path);
            using (FileStream excelFile = File.Open(path, FileMode.Open, FileAccess.Read))
            {
                // Reading from a OpenXml Excel file (2007 format; *.xlsx)
                IExcelDataReader excelReader = ExcelReaderFactory.CreateOpenXmlReader(excelFile);

                // The result of each spreadsheet will be created in the result.Tables
                excelReader.IsFirstRowAsColumnNames = true;
                DataSet book = excelReader.AsDataSet();

                // 数据检测
                if (book.Tables.Count < 1)
                {
                    throw new Exception("Excel文件中没有找到Sheet: " + path);
                }

                // 取得数据
                DataTable sheet = book.Tables[0];
                if (sheet.Rows.Count <= 0)
                {
                    throw new Exception("Excel Sheet中没有数据: " + path);
                }
                return sheet;
            }
            return null;
        }
        private static Encoding GetEncoding(string encoding)
        {
            //-- 确定编码
            Encoding cd = new UTF8Encoding(false);
            if (encoding != "utf8-nobom")
            {
                foreach (EncodingInfo ei in Encoding.GetEncodings())
                {
                    Encoding e = ei.GetEncoding();
                    if (e.EncodingName == encoding)
                    {
                        cd = e;
                        break;
                    }
                }
            }
            return cd;
        }
        private static void Export2Json(string excelPath, int header, string encoding, string jsonPath, bool lowcase, string filter)
        {
            if (jsonPath == null || jsonPath == "")
            {
                return;
            }
            DataTable dt = ReadExcel(excelPath);
            Encoding en = GetEncoding(encoding);
            //-- 导出JSON文件
            jsonPath = FormatPath(jsonPath);
            JsonExporter exporter = new JsonExporter(dt, header, lowcase, filter);
            exporter.SaveToFile(jsonPath, en);
        }
        private static void Export2Sql(string excelPath, int header, string encoding, string sqlPath)
        {
            if(sqlPath == null || sqlPath.Length <= 0)
            {
                return;
            }
            DataTable dt = ReadExcel(excelPath);
            Encoding en = GetEncoding(encoding);
            //-- 导出SQL文件
            sqlPath = FormatPath(sqlPath);
            SQLExporter exporter = new SQLExporter(dt, header);
            exporter.SaveToFile(sqlPath, en);
        }

    }
}
