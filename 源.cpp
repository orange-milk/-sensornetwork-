/*
��a.h��ͷ�ļ���¼������������Ҫ�õ���ͷ�ļ��������ռ��ʹ��
��������������

����̨��ʾ��Χ���÷�ʽ����
initgraph(600, 600);


�ڵ�İ뾶����Ϊ5��sensorΪ��ɫԲ�㣬datacollectorΪ��ɫԲ��,�趨�����뾶Ϊ50���ۺ�ʵ�ʿ��ǹ涨�뾶Ϊ30�����ҽ���һ���ڵ�
�ڵ����÷�ʽ����
setfillcolor(BLUE);
solidcircle(300, 300, 5);


//����㷨�������ַhttps://www.cnblogs.com/forget406/p/5294143.html,����
// x[i] = ( a * x[i-1] + b ) % m �ڴ˴δ�����a=937,b=691,mԽ�������Խ�á�

����datacollector�Ķ���ʱע��ֻ�ܽ���һ��������a.hͷ�ļ���
datacollector_aΪ���datacollector��������ȫ�ֱ���

Sleep(1000);�ӳ�1s

�а�ɫ�߿���insterestarea��Ե��
*/
#include"a.h"
int main()
{
	cout << "Ĭ��������sensor����������0,��������1"<<endl;
	int num = 10;
	cin >> num;
	while (num != 0 && num != 1) {
		cout << "Ĭ��������sensor����������0,��������1" << endl;
		cin >> num;
	}
	Insterestarea *a=NULL;
	if(num==0){
		a=new Insterestarea();
	}
	else {
		cout << "������sensor����������"<<endl;
		int max,  period;
		cin >> max >> period;
		a = new Insterestarea(max+1,period);
	}
	a->Drawmap();
	a->Drawnode();
	return 0;
}