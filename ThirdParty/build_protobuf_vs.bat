::参考文章 https://github.com/google/protobuf/blob/master/cmake/README.md
::默认当前操作系统已安装 git 和 cmake,并配置好了环境变量
echo off & color 0A

::设置所需要的Protobuf版本,最新版本可以在github上查到 https://github.com/google/protobuf
::必须与下载的版本一致
set PROTOBUF_VESION="3.0.2"
echo %PROTOBUF_VESION%
set PROTOBUF_PATH="protobuf-%PROTOBUF_VESION%"
echo %PROTOBUF_PATH%
cd %PROTOBUF_PATH%

::设置VS工具集,相当于指定VS版本,取决于VS的安装路径
set VS_DEV_CMD="C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\Tools\VsDevCmd.bat"
::设置工程文件夹名字,用来区分不同的VS版本
set BUILD_PATH="build_VS2015"
::设置编译版本 Debug Or Release
set MODE="Release"


cd cmake
if not exist %BUILD_PATH% md %BUILD_PATH%

cd %BUILD_PATH%
if not exist %MODE% md %MODE%
cd %MODE%

::开始构建和编译
call %VS_DEV_CMD%
cmake ../../ -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=%MODE%
call extract_includes.bat
nmake /f Makefile

echo %cd%
pause