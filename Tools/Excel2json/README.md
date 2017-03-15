excel2json
==========

- 把Excel表单转换成json对象，并保存到一个文本文件中。
- 表格格式见ExampleData.xlsx
- 导出的json对象的结构如下，每一行转换成一个json对象：
{
	ID:{...}
	ID:{...}
	ID:{...}
	...
}

Excel表单格式约定
-----------------
  - 第一行固定作为列名（用来构造json字段名称）；
  - 第一列固定作为对象的ID；
  - 读取Excel Workbook中的第一个sheet；
  - 对于SQL导出模式：第二行固定为字段类型
  - header倒数前一行为是否输出为client或者server或者不输出配置（目前对输出为json文件时有效）

命令行参数
---------
-  -e, --excel       Required. 输入的Excel文件或文件夹路径.
-  -j, --json        指定输出的json文件或文件夹路径.
-  -s, --sql         指定输出的SQL文件或文件夹路径.
-  -h, --header      Required. 表格中有几行是表头.
-  -c, --encoding    (Default: utf8-nobom) 指定编码的名称.
-  -l, --lowcase     (Default: false) 自动把字段名称转换成小写格式.

例如：“excel2json --excel test.xlsx --json test.json --header 3”，其中的输入和输出文件，都在当前目录下；
例如：“excel2json --excel ../testdata/ --json ../jsondata/ --header 3”，将testdata目录下所有的excel文件转换成json文件；

