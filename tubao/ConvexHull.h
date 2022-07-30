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

//���ƺ���
void copyConPnt() {
	memcpy(finalpoints, rawpoint, sizeof(struct ConPnt) * Ptotal);
}

//�ҵ�Yֵ��С�ĵ�
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

//��������ֵ
void Calculate(ConPnt* points, int Min) {
	for (int i = 0; i < Ptotal; i++) {
		if (i == Min) {
			points[i].coszhi = 1;
		}
		else {
			//��X����ɵ�ֱ��������
			double xfang = pow((points[i].x - points[Min].x), 2);
			double yfang = pow((points[i].y - points[Min].y), 2);
			points[i].coszhi = (points[i].x - points[Min].x) / sqrt(xfang + yfang);
		}
	}
}

//����cosֵ����,ʹ��ѡ������
void sortpoint(ConPnt* points) {
	ConPnt temp;
	for (int i = 0; i < Ptotal; i++) {
		//max����������˱ȽϹ����У����Ԫ������Ӧ��������
		//�ȼ���ǰ���Ԫ��Ϊ���Ԫ��
		int maxIndex = i;
		/*ÿ�˱Ƚϣ���ǰ���Ԫ��������Ԫ������Ƚ�*/
		for (int j = i + 1; j < Ptotal; j++) {
			//��������Ԫ�ش󣬽�����Ԫ�ص�������Ϊ���ֵ������
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
		//Ȼ�󽻻��˴β��ҵ������ֵ��ԭʼ�����ֵ
		if (points[maxIndex].coszhi >= points[i].coszhi) {
			temp = points[i];
			points[i] = points[maxIndex];
			points[maxIndex] = temp;
		}
	}
	int a = 0;
}

/*�ж��Ƿ�Ϊ��ʱ����ת
area = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x)
area > 0��A - B - C��ʱ����ת��
area < 0��A - B - C˳ʱ����ת��
area = 0��A - B - C��һ��ֱ���ϡ�*/
int Counterclockwise(ConPnt a, ConPnt b, ConPnt c) {
	int area = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
	return area;
}

//һ��ֱ������ͬ��������
int tongyixiang(ConPnt a, ConPnt b, ConPnt c)
{
	int Direction = (b.x - a.x) * (c.x - a.x) + (b.y - a.y) * (c.y - a.y);
	return Direction;
}

//Grahamɨ�跨
void Graham(ConPnt* points)
{
	resultpoint.push(points[0]);
	resultpoint.push(points[1]);
	for (int i = 2; i < Ptotal;)
	{
		ConPnt temp = resultpoint.top();
		resultpoint.pop();
		//������ͬһ����ôѹ�����������
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

//�ж��Ƿ��ظ�
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