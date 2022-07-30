#pragma once
#include<iostream>
#include<cmath>
#include<stack>
using namespace std;

struct ConPnt
{
	int x;
	int y;
	float coszhi;
	ConPnt(int xx = 0, int yy = 0, double cosV = 0)
	{
		x = xx;
		y = yy;
		coszhi = cosV;
	};
};

ConPnt* finalpoints;
ConPnt* rawpoint;
stack<ConPnt>resultpoint;
int Ptotal;

//复制函数
void copyConPnt() {
	memcpy(finalpoints, rawpoint, sizeof(struct ConPnt) * Ptotal);
}

//找到Y值最小的点
int SortY(ConPnt* points) {
	int  Min = 0;
	for (int i = 1; i < Ptotal; i++) {
		if (points[i].y < points[Min].y)
			Min = i;
		if (points[i].y == points[Min].y)
		{
			if (points[i].x < points[Min].x)
				Min = i;
		}
	}
	return Min;
}

//计算余弦值
void Calculate(ConPnt* points, int Min) {
	for (int i = 0; i < Ptotal; i++) {
		if (i == Min) {
			points[i].coszhi = 1;
		}
		else {
			//与X轴组成的直角三角形
			double xfang = pow((points[i].x - points[Min].x), 2);
			double yfang = pow((points[i].y - points[Min].y), 2);
			points[i].coszhi = (points[i].x - points[Min].x) / sqrt(xfang + yfang);
		}
	}
}

//按照cos值排序,使用选择排序
void sortpoint(ConPnt* points) {
	ConPnt temp;
	for (int i = 0; i < Ptotal; i++) {
		//max变量保存该趟比较过程中，最大元素所对应的索引，
		//先假设前面的元素为最大元素
		int maxIndex = i;
		/*每趟比较，将前面的元素与其后的元素逐个比较*/
		for (int j = i + 1; j < Ptotal; j++) {
			//如果后面的元素大，将后面元素的索引极为最大值的索引
			if (points[j].coszhi > points[maxIndex].coszhi) {
				maxIndex = j;
			}
			if (points[j].coszhi == points[maxIndex].coszhi) {
				if (points[j].x < points[maxIndex].x)
				{
					maxIndex = j;
				}
				if (points[j].x == points[maxIndex].x)
				{
					if (points[j].y > points[maxIndex].y)
					{
						maxIndex = j;
					}
				}
			}
		}
		//然后交换此次查找到的最大值和原始的最大值
		if (points[maxIndex].coszhi >= points[i].coszhi) {
			temp = points[i];
			points[i] = points[maxIndex];
			points[maxIndex] = temp;
		}
	}
	int a = 0;
}

/*判断是否为逆时针旋转
area = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x)
area > 0，A - B - C逆时针旋转；
area < 0，A - B - C顺时针旋转；
area = 0，A - B - C在一条直线上。*/
int Counterclockwise(ConPnt a, ConPnt b, ConPnt c) {
	int area = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
	return area;
}

//一条直线上是同向还是异向
int tongyixiang(ConPnt a, ConPnt b, ConPnt c)
{
	int Direction = (b.x - a.x) * (c.x - a.x) + (b.y - a.y) * (c.y - a.y);
	return Direction;
}

//Graham扫描法
void Graham(ConPnt* points)
{
	resultpoint.push(points[0]);
	resultpoint.push(points[1]);
	for (int i = 2; i < Ptotal;)
	{
		ConPnt temp = resultpoint.top();
		resultpoint.pop();
		//若是在同一侧那么压到结果数组中
		if (Counterclockwise(resultpoint.top(), temp, points[i]) > 0)
		{
			resultpoint.push(temp);
			resultpoint.push(points[i]);
			i++;
		}
		if (Counterclockwise(resultpoint.top(), temp, points[i]) == 0)
		{
			if (tongyixiang(resultpoint.top(), temp, points[i]) > 0)
			{
				resultpoint.push(temp);
				resultpoint.push(points[i]);
				i++;
			}
		}
	}
}

//判断是否重复
bool twice(ConPnt a, ConPnt b[], int m)
{
	for (int i = 0; i < m; i++)
	{
		if (a.x == b[i].x && a.y == b[i].y)
		{
			return true;
		}
	}
	return false;
}

void clearall()
{
	delete rawpoint;
	delete finalpoints;
	while (!resultpoint.empty())
	{
		resultpoint.pop();
	}
}