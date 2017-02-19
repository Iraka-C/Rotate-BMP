/*
  By Iraka (iraka@pku.edu.cn)
  written in C++, accept a command param. as filename
*/

#define _CRT_SECURE_NO_DEPRECATE

#include<fstream>
#include<string.h>
using namespace std;
unsigned char map[1800][1200][3];//[w][h][0:R 1:G 2:B]

int main(int argc,char **argv){
	if(argc<3)return 0;
	ifstream bmpi(argv[1],ios::binary|ios::in);
	ofstream bmpo(argv[2],ios::binary|ios::out);

	unsigned char bmphead[54];//BMP Head
	for(int i=0;i<=53;i++)bmphead[i]=bmpi.get();//...Various Check Can Be Applied

	int width=0,height=0;
	for(int i=21;i>=18;i--)width=(width<<8)+bmphead[i];
	for(int i=25;i>=22;i--)height=(height<<8)+bmphead[i];

	for(int i=height-1;i>=0;i--){//Down to Up
		for(int j=0;j<width;j++)//Left to Right
			for(int color=2;color>=0;color--)//BGR
				map[j][i][color]=bmpi.get();
		for(int t=1;t<=width%4;t++)bmpi.get();//Make it 4X
	}

	int ds=width*(height*3+height%4);//data size
	int fs=54+ds;//file size
	for(int i=18,th=height;i<=21;i++){bmphead[i]=th&255;th>>=8;}//Exchange
	for(int i=22,tw=width;i<=25;i++){bmphead[i]=tw&255;tw>>=8;}
	for(int i=34;i<=37;i++){bmphead[i]=ds&255;ds>>=8;}
	for(int i=2;i<=5;i++){bmphead[i]=fs&255;fs>>=8;}
	
	
	for(int i=0;i<=53;i++)bmpo.put(bmphead[i]);//Write Head
	for(int i=width-1;i>=0;i--){//Write Data
		for(int j=height-1;j>=0;j--)
			for(int color=2;color>=0;color--)
				bmpo.put(map[i][j][color]);
		for(int t=1;t<=height%4;t++)bmpo.put('\0');
	}

	bmpi.close();bmpo.close();
}
