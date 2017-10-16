#include<iostream>
#include <fstream>  
#include <string>  
#include <stdlib.h> 
#include <time.h>  
using namespace std;
int row[10][10];//记录每行的取值情况，若值为1表示该值被去过。如row[2][1]==1表示第二行中1的值已经被取过了
int column[10][10];//记录每列的取值情况。
int box[10][10];//记录每个九宫格的取值情况
int result[10][10];//保存数独棋盘的最终结果
int ran_num[10];
int num;//总共需要生成多少数独棋盘数量
int nownum;// 目前生成的数独棋盘数量

//把当前的数独棋盘输出到文件，savefile为输出文件流，islast表示当前输出的棋盘是否为最后一个棋盘
void outfile(ofstream &savefile, bool islast)
{
	string temp = "";//用来临时保存需要输出的文字
	for (int i = 1; i <= 9; i++)
	{
		for (int j = 1; j <= 9; j++)
		{
			if (j != 1) temp += " ";//每个数字之间需要一个空格
			temp += result[i][j] + '0';//把每个数字转换成字符
		}
		temp += "\r\n";//输出完一行需要换行
	}
	if (!islast) temp += "\r\n";//如果这个不是最后一个棋盘，棋盘间需要再次换行。
	savefile << temp;//把文字输入到文件中
}

//用递归生成数独，nrow表示当前行，ncol表示当前列，表示当前正在处理第nrow行第ncol列的格子。
void shudu(int nrow, int ncol, ofstream &savefile)
{
	//计算该格子在哪个九宫格中
	int nowbox = ((nrow - 1) / 3) * 3 + ((ncol - 1) / 3 + 1);
	//对该格子进行1-9的数字尝试，如果nownum==num则说明棋盘已经够了，就不用再进行下去了，循环退出
	for (int i = 1; i <= 9 && nownum<num; i++)
	{
		//如果数值i在该行该列没有出现过，则进行下一步操作
		if (row[nrow][ran_num[i]] == 0 && column[ncol][ran_num[i]] == 0 && box[nowbox][ran_num[i]] == 0)//判定数值i在该行该列该九宫格有没有出现过
		{
			result[nrow][ncol] = ran_num[i];//把数值i填写入最终棋盘
			row[nrow][ran_num[i]] = column[ncol][ran_num[i]] = box[nowbox][ran_num[i]] = 1;//记录下i这个值在该行该列和该九宫格出现了
			//如果这个格子是最后一个格子了，说明已经找到一个数独棋盘
			if (nrow == 9 && ncol == 9)
			{
				nownum++;//已找到的数独棋盘数量+1
				if (nownum == num) outfile(savefile, true);//这个是最后一个棋盘
				else outfile(savefile, false);//这不是最后的棋盘
			}
			//如果这个格子不是最后一个格子，则对下一个格子进行枚举
			else
			{
				if (ncol == 9) shudu(nrow + 1, 1, savefile);//如果这个格子是该行最后一个格子，则换行
				else shudu(nrow, ncol + 1, savefile);//如果这个格子不是该行最后一个格子，则处理该行下一个格子
			}
			row[nrow][ran_num[i]] = column[ncol][ran_num[i]] = box[nowbox][ran_num[i]] = 0;//递归完后复原，为下一个数字的尝试做准备
		}
	}
}
int input_check(int argc, char *argv[])
{
	if (argc != 3)
	{
		cout << "invaild input" << endl;
		return -1;
	}
	if (argc == 3 && strcmp(argv[1], "-c") != 0)
	{
		cout << "please input -c" << endl;
		return -1;//如果输入第二个参数不是-c 不输出
	}
	if (atoi(argv[2]) == 0)
	{
		cout << "please enter a value greater than 0" << endl;
		return -1;//如果输入第三个参数不是数字 不输出
	}
	return atoi(argv[2]);
}

int main(int argc,char *argv[])
{
	//初始化行列数组、随机序列
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			row[i][j] = column[i][j] = box[i][j] =0;
		}
		ran_num[i] = i;
	}
	int rantimes = 20,j,temp;//定义打乱次数和随机变量i、j、temp
	srand((unsigned)time(NULL));//初始化随机生成器
	//打乱序列
	while (rantimes--)
	{
		i=rand() % 9 + 1;//获取一个1-9的随机数
		j=rand() % 9 + 1;//获取一个1-9的随机数
		//如果i不等于j，则交换i和j的位置
		if (i != j)
		{
			temp = ran_num[i];
			ran_num[i] = ran_num[j];
			ran_num[j] = temp;
		}
	}
	//初始化nownum
	nownum = 0;
	num = input_check(argc, argv);
	if (num != -1)
	{
		//打开并创建文件sudoku.txt
		ofstream savefile("sudoku.txt", ofstream::out);
		row[1][3] = column[1][3] = box[1][3] = 1;
		result[1][1] = 3;
		//递归生成num个数独棋盘
		shudu(1, 2, savefile);
		//关闭文件流
		savefile.close();
	}
	return 0;
}