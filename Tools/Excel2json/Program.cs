using System;
using System.Text;
using System.IO;
using System.Data;

namespace excel2json
{
    /// <summary>
    /// 应用程序
    /// </summary>
    sealed partial class Program
    {
        /// <summary>
        /// 应用程序入口
        /// </summary>
        /// <param name="args">命令行参数</param>
        static void Main(string[] args)
        {
            System.DateTime startTime = System.DateTime.Now;

            //-- 分析命令行参数
            var options = new Options();
            var parser = new CommandLine.Parser(with => with.HelpWriter = Console.Error);

            if (parser.ParseArgumentsStrict(args, options, () => Environment.Exit(-1)))
            {
                //-- 执行导出操作
                try
                {
                    Run(options);
                }
                catch (Exception exp)
                {
                    Console.WriteLine("Error: " + exp.Message);
                }
            }

            //-- 程序计时
            System.DateTime endTime = System.DateTime.Now;
            System.TimeSpan dur = endTime - startTime;
            Console.WriteLine(
                string.Format("[{0}]：\t转换完成[{1}毫秒].",
                Path.GetFileName(options.ExcelPath),
                dur.Milliseconds)
                );

            Console.ReadKey();
        }

        /// <summary>
        /// 根据命令行参数，执行Excel数据导出工作
        /// </summary>
        /// <param name="options">命令行参数</param>
        private static void Run(Options options)
        {
            string excelPath = options.ExcelPath;
            int header = options.HeaderRows;

            if(Directory.Exists(excelPath))
            {
                Exporter.Export2Folder(excelPath, header, options.Encoding, options.JsonPath, options.SQLPath, options.Lowcase);
            }
            if(File.Exists(excelPath))
            {
                Exporter.Export2File(excelPath, header, options.Encoding, options.JsonPath, options.SQLPath, options.Lowcase);
            }
        }
    }
}
