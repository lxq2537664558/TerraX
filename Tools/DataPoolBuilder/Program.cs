using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataPoolBuilder
{
    class Program
    {
        static void Main(string[] args)
        {
            List<string> l = FileSearch.SearchFolder(@"E:\CodeSpace\TerraX\Tools\", ".json");

            CppEnumExport.Export2CppFile(l);

            Console.ReadKey();
        }
    }
}
