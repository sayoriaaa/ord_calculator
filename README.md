# 计算器

一个基于语法制导翻译的计算器

## Getting Started

*ord* requires:

* flex & bison
* gcc
* make
* graphviz

you can build *ord* from repository by executing the following command:

```
make
```

## Commands

- -h：帮助
- -v：版本
- -pre：输出前序遍历
- -post：输出后序遍历
- -d：绘制表达树，在当前目录下产生ast.png

## Usage

不附加参数作为普通计算器，添加参数输出参数信息