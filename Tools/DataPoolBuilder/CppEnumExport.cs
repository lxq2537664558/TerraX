using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataPoolBuilder
{
    public class CppEnumExport
    {
        public static string OutputPath = @".\DataDefine.h";
        public static string NameSpace = "TerraX";
        public static void Export2CppFile(List<string> listFiles)
        {
            using (FileStream fs = new FileStream(OutputPath, FileMode.OpenOrCreate, FileAccess.Write))
            {
                fs.Lock(0, fs.Length);
                StreamWriter sw = new StreamWriter(fs);
                sw.WriteLine(@"#pragma once");
                sw.WriteLine();
                sw.WriteLine(@"namespace TerraX");
                sw.WriteLine(@"{");

                foreach (string str in listFiles)
                {
                    sw.WriteLine("\tenum {");
                    DoWrite(str, sw);
                    sw.WriteLine("\t};");
                    sw.WriteLine();
                }
                sw.WriteLine(@"}");
                fs.Unlock(0, fs.Length);//一定要用在Flush()方法以前，否则抛出异常。
                sw.Flush();
            }
        }

        private static void DoWrite(string path, StreamWriter sw)
        {
            string file_name = Path.GetFileNameWithoutExtension(path);
            JObject jo = JObject.Parse(File.ReadAllText(path));
            foreach (JProperty jp in jo.Properties())
            {
                sw.WriteLine("\t\t" + file_name.ToLower() + "_" + jp.Name.ToLower() + "_index,");
            }
            sw.WriteLine("\t\t" + file_name.ToLower() + "_" + "index_size,");
        }
    }
}
