using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace excel2json
{
    public class FileSearch
    {
        private static List<string> m_listFiles = new List<string>();
        private static List<string> m_listFolders = new List<string>();

        public static List<string> SearchFoldersInFolder(string root_path)
        {
            m_listFolders.Clear();
            DoSearchFolders(root_path, "");
            return m_listFolders;
        }
        private static void DoSearchFolders(string root_path, string child_path)
        {
            DirectoryInfo folder = new DirectoryInfo(root_path + child_path);
            DirectoryInfo[] dirs = folder.GetDirectories();
            foreach (DirectoryInfo di in dirs)
            {
                //Console.WriteLine(di.FullName);
                child_path = child_path + di.Name + "\\";
                m_listFolders.Add(child_path);
                DoSearchFolders(root_path, child_path);
            }
        }

        public static List<string> SearchFilesInFolder(string root_path, string file_extension = ".*")
        {
            m_listFiles.Clear();
            DoSearchFiles(root_path, "", file_extension);
            return m_listFiles;
        }
        private static void DoSearchFiles(string root_path, string child_path, string file_extension)
        {
            DirectoryInfo folder = new DirectoryInfo(root_path + child_path);
            FileInfo[] files = folder.GetFiles();
            foreach (FileInfo fi in files)
            {
                if (file_extension.Equals(".*") || fi.Extension.Equals(file_extension))
                {
                    //Console.WriteLine(fi.FullName);
                    m_listFiles.Add(child_path + Path.GetFileNameWithoutExtension(fi.Name));
                }
            }
            DirectoryInfo[] dirs = folder.GetDirectories();
            foreach (DirectoryInfo di in dirs)
            {
                //Console.WriteLine(di.FullName);
                child_path = child_path + di.Name + "\\";
                DoSearchFiles(root_path, child_path, file_extension);
            }
        }
    }
}
