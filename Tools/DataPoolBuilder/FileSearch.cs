using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataPoolBuilder
{
    public class FileSearch
    {
        private static List<string> m_listFiles = new List<string>();
        public static List<string> SearchFolder(string folder_path, string file_extension = ".*")
        {
            m_listFiles.Clear();
            DoSearch(folder_path, file_extension);
            return m_listFiles;
        }
        private static void DoSearch(string folder_path, string file_extension)
        {
            DirectoryInfo root = new DirectoryInfo(folder_path);
            DirectoryInfo[] dirs = root.GetDirectories();
            FileInfo[] files = root.GetFiles();
            foreach (FileInfo fi in files)
            {
                if (file_extension.Equals(".*") || fi.Extension.Equals(file_extension))
                {
                    //Console.WriteLine(fi.FullName);
                    m_listFiles.Add(fi.FullName);
                }
            }
            foreach (DirectoryInfo di in dirs)
            {
                //Console.WriteLine(di.FullName);
                DoSearch(di.FullName, file_extension);
            }
        }
    }
}
