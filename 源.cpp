/*
由a.h的头文件记录整个代码所需要用到的头文件及命名空间的使用
和三个基本的类

控制台显示范围设置方式如下
initgraph(600, 600);


节点的半径设置为5，sensor为蓝色圆点，datacollector为红色圆点,设定传播半径为50，综合实际考虑规定半径为30内有且仅有一个节点
节点设置方式如下
setfillcolor(BLUE);
solidcircle(300, 300, 5);


//随机算法借鉴与网址https://www.cnblogs.com/forget406/p/5294143.html,质数
// x[i] = ( a * x[i-1] + b ) % m 在此次代码中a=937,b=691,m越大随机性越好。

构造datacollector的对象时注意只能建立一个对象，在a.h头文件中
datacollector_a为检测datacollector对象数的全局变量

Sleep(1000);延迟1s

有白色边框标记insterestarea边缘处
*/
#include"a.h"
int main()
{
	cout << "默认周期与sensor个数请输入0,配置输入1"<<endl;
	int num = 10;
	cin >> num;
	while (num != 0 && num != 1) {
		cout << "默认周期与sensor个数请输入0,配置输入1" << endl;
		cin >> num;
	}
	Insterestarea *a=NULL;
	if(num==0){
		a=new Insterestarea();
	}
	else {
		cout << "请输入sensor个数和周期"<<endl;
		int max,  period;
		cin >> max >> period;
		a = new Insterestarea(max+1,period);
	}
	a->Drawmap();
	a->Drawnode();
	return 0;
}