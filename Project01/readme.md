# README
> BY: 新雅62/CDIE6 项雨桐

## 1. 开发环境
* 操作系统: 
  * ``Mac OS X 10.15.1 build:19B88``
  * ``Darwin Kernel Version: 19.0.0``
* 编译器:
	```sh
	Configured with: --prefix=/Library/Developer/CommandLineTools/usr --with-gxx-include-dir=/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/4.2.1
	Apple clang version 11.0.0 (clang-1100.0.33.12)#macOS中的g++并没有被改造和重写，只是指向clang++编译器。
	Target: x86_64-apple-darwin19.0.0
	Thread model: posix
	InstalledDir: /Library/Developer/CommandLineTools/usr/bin
	```
* ``OpenCV``:``Version 4.1.2``
* 文本编辑器:``Microsoft Visual Studio Code Version: 1.40.0``
* ``cmake``:``Version 3.15.5``

## 2. 文件说明
* ``result/ : 运行结果存放路径``
  * ``畸变：[distort_插值算法_正/负畸变_畸变半径.png]``
  * ``旋转扭曲：[rotate_插值算法_最大角度.png]``
* ``c_cpp_properties.json : MacOS VSCode C++开发配置文件``
* ``interpolation.cpp : 源代码文件``
* ``makefile : makefile文件``
* ``interpolation : 二进制可执行文件( 限Macintosh系统, 在当前目录下通过./interpolation 运行 )``