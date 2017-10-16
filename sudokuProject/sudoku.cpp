#include<iostream>
#include <fstream>  
#include <string>  
#include <stdlib.h> 
#include <time.h>  
using namespace std;
int row[10][10];//��¼ÿ�е�ȡֵ�������ֵΪ1��ʾ��ֵ��ȥ������row[2][1]==1��ʾ�ڶ�����1��ֵ�Ѿ���ȡ����
int column[10][10];//��¼ÿ�е�ȡֵ�����
int box[10][10];//��¼ÿ���Ź����ȡֵ���
int result[10][10];//�����������̵����ս��
int ran_num[10];
int num;//�ܹ���Ҫ���ɶ���������������
int nownum;// Ŀǰ���ɵ�������������

//�ѵ�ǰ����������������ļ���savefileΪ����ļ�����islast��ʾ��ǰ����������Ƿ�Ϊ���һ������
void outfile(ofstream &savefile, bool islast)
{
	string temp = "";//������ʱ������Ҫ���������
	for (int i = 1; i <= 9; i++)
	{
		for (int j = 1; j <= 9; j++)
		{
			if (j != 1) temp += " ";//ÿ������֮����Ҫһ���ո�
			temp += result[i][j] + '0';//��ÿ������ת�����ַ�
		}
		temp += "\r\n";//�����һ����Ҫ����
	}
	if (!islast) temp += "\r\n";//�������������һ�����̣����̼���Ҫ�ٴλ��С�
	savefile << temp;//���������뵽�ļ���
}

//�õݹ�����������nrow��ʾ��ǰ�У�ncol��ʾ��ǰ�У���ʾ��ǰ���ڴ����nrow�е�ncol�еĸ��ӡ�
void shudu(int nrow, int ncol, ofstream &savefile)
{
	//����ø������ĸ��Ź�����
	int nowbox = ((nrow - 1) / 3) * 3 + ((ncol - 1) / 3 + 1);
	//�Ըø��ӽ���1-9�����ֳ��ԣ����nownum==num��˵�������Ѿ����ˣ��Ͳ����ٽ�����ȥ�ˣ�ѭ���˳�
	for (int i = 1; i <= 9 && nownum<num; i++)
	{
		//�����ֵi�ڸ��и���û�г��ֹ����������һ������
		if (row[nrow][ran_num[i]] == 0 && column[ncol][ran_num[i]] == 0 && box[nowbox][ran_num[i]] == 0)//�ж���ֵi�ڸ��и��иþŹ�����û�г��ֹ�
		{
			result[nrow][ncol] = ran_num[i];//����ֵi��д����������
			row[nrow][ran_num[i]] = column[ncol][ran_num[i]] = box[nowbox][ran_num[i]] = 1;//��¼��i���ֵ�ڸ��и��к͸þŹ��������
			//���������������һ�������ˣ�˵���Ѿ��ҵ�һ����������
			if (nrow == 9 && ncol == 9)
			{
				nownum++;//���ҵ���������������+1
				if (nownum == num) outfile(savefile, true);//��������һ������
				else outfile(savefile, false);//�ⲻ����������
			}
			//���������Ӳ������һ�����ӣ������һ�����ӽ���ö��
			else
			{
				if (ncol == 9) shudu(nrow + 1, 1, savefile);//�����������Ǹ������һ�����ӣ�����
				else shudu(nrow, ncol + 1, savefile);//���������Ӳ��Ǹ������һ�����ӣ����������һ������
			}
			row[nrow][ran_num[i]] = column[ncol][ran_num[i]] = box[nowbox][ran_num[i]] = 0;//�ݹ����ԭ��Ϊ��һ�����ֵĳ�����׼��
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
		return -1;//�������ڶ�����������-c �����
	}
	if (atoi(argv[2]) == 0)
	{
		cout << "please enter a value greater than 0" << endl;
		return -1;//������������������������ �����
	}
	return atoi(argv[2]);
}

int main(int argc,char *argv[])
{
	//��ʼ���������顢�������
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			row[i][j] = column[i][j] = box[i][j] =0;
		}
		ran_num[i] = i;
	}
	int rantimes = 20,j,temp;//������Ҵ������������i��j��temp
	srand((unsigned)time(NULL));//��ʼ�����������
	//��������
	while (rantimes--)
	{
		i=rand() % 9 + 1;//��ȡһ��1-9�������
		j=rand() % 9 + 1;//��ȡһ��1-9�������
		//���i������j���򽻻�i��j��λ��
		if (i != j)
		{
			temp = ran_num[i];
			ran_num[i] = ran_num[j];
			ran_num[j] = temp;
		}
	}
	//��ʼ��nownum
	nownum = 0;
	num = input_check(argc, argv);
	if (num != -1)
	{
		//�򿪲������ļ�sudoku.txt
		ofstream savefile("sudoku.txt", ofstream::out);
		row[1][3] = column[1][3] = box[1][3] = 1;
		result[1][1] = 3;
		//�ݹ�����num����������
		shudu(1, 2, savefile);
		//�ر��ļ���
		savefile.close();
	}
	return 0;
}