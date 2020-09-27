#if!defined daniudfhiadshfi_165asd68vcads
#define daniudfhiadshfi_165asd68vcads
#include<iostream>
#include<graphics.h>
#include<conio.h>
#include<windows.h>
#include<time.h>
#include<queue>
#include <fstream>
#include<math.h>
#include<string>
using namespace std;
int datacollector_a = 0;//datacollector_a为检测datacollector对象数的全局变量
class Node
{
public:
	Node() {}
	void virtual drawnode() = 0;
	int virtual get_x() = 0;
	int virtual get_y() = 0;
};
class Datacollector :public Node
{
public:
	Datacollector() :Node() { datacollector_a = 1; }
	Datacollector(int xx, int yy) :Node() { x = xx; y = yy; datacollector_a = 1; }
private:
	int x = 0, y = 0;
	void  drawnode() {
		setfillcolor(RED);
		solidcircle(x, y, 5);
	}
	int get_x() { return x; }
	int get_y() { return y; }
};
class sensor :public Node
{
public:
	sensor(int xx, int yy) :Node() { x = xx; y = yy; }
	sensor() :Node() {}
private:
	int x = 0, y = 0;
	void  drawnode() {
		setfillcolor(BLUE);
		solidcircle(x, y, 5);
	}
	int get_x() { return x; }
	int get_y() { return y; }
};

class Insterestarea
{
public:
	queue<Node*>node_0;//储存节点信息
	int MAX = 100+1;//节点数目上限
	int HIGH = 600, WIDE = 1000, LONG = 400, SHORT = 300;
	//HIGH,WIDE对应界面长宽，LONG，SHORT对应椭圆长宽。
	double Space = 3.14 * LONG*SHORT;//椭圆面积
	int S_radio = 40, L_radio = 70;//范围在9-100之间,S_radio最小间隔范围，L_radio最大传播范围
	int datacoller_num = 1;//Datacollector创建的位置
	int **node_xy = new int*[MAX];//记录各节点的坐标位置信息
	int **path_length = new int*[MAX];//记录两点间的距离，以INT_MAX做无穷大  
	queue<int>path_1;//path_1记录完成最短路径运算的节点
	queue<int>path_2;//path_2用于记录未完成运算的节点
	string *path = new string[MAX]; //path[i]记录从0到i最近路径的路径信息，然后通过delete和new创建最短路径
									//由于是求个sensor到datacollector路径，可看做求datacollector到每个sensor路径，所以建立一维数组即可
	int *arr = NULL;//辅助显示和删除路径
	int Period = 2;
	int K = 0;//随机数
	Insterestarea() {//构造函数
		srand(time(NULL));
		K = rand();
		Initial_arr();
	}
	Insterestarea(int MAX,int Period) {//构造函数
		this->MAX = MAX;
		this->Period = Period;
		srand(time(NULL));
		K = rand();
		Initial_arr();
	}
	void Drawmap() {//绘制基本界面
		initgraph(WIDE, HIGH);
		ellipse(0, 0, 2 * LONG, 2 * SHORT);
		line(2 * LONG, 0, 2 * LONG, 2 * SHORT);
		outtextxy(2 * LONG + 5, 0, L"红点表示datacollector");
		outtextxy(2 * LONG + 5, 20, L"蓝点表示sensor");
		outtextxy(2 * LONG + 5, 40, 48 + S_radio / 10);
		outtextxy(2 * LONG + 15, 40, 48 + S_radio % 10);
		outtextxy(2 * LONG + 25, 40, L"内仅有一个节点");
		outtextxy(2 * LONG + 5, 60, 48 + L_radio / 10);
		outtextxy(2 * LONG + 15, 60, 48 + L_radio % 10);
		outtextxy(2 * LONG + 25, 60, L"为传播范围");
		outtextxy(2 * LONG + 5, 80, L"自动生成节点");
	}
	int Show_nodenum()
	{
		settextcolor(RED);
		for (int i = 0; i < MAX; i++)//对每个节点直接显示节点数
		{
			Show_numxy(i, node_xy[i][0] + 5, node_xy[i][1] - 5);
		}
		settextcolor(WHITE);
		return 0;
	}
	void Delay(int a) {//延迟i秒的时间
		for (int i = 0; i < a; i++) {
			Sleep(1000);
			outtextxy(2 * LONG + 5, 140, L"还有    秒更新一次节点");
			Show_numxy(a-i, 2 * LONG + 20, 140);
		}
		Sleep(1000);
	}
	bool Node_lega(int x, int y)//判断是否在椭圆内
	{
		int a = 0, b = 0;
		if (x < LONG) { a = LONG - x; }
		else { a = x - LONG; }
		if (y < SHORT) { b = SHORT - y; }
		else { b = y - SHORT; }
		if (a*a *(SHORT / 100) * (SHORT / 100) + b*b *(LONG / 100) *(LONG / 100)
			< (LONG / 100)  * (LONG / 100) *SHORT * SHORT) {
			return true;
		}
		return false;
	}
	bool Tnear_text(int x, int y) {//判断两节点位置是否过近或者过远,若不合适则返回true
		if (node_0.size() == 0)return false;
		Node* p = NULL;
		int j = 0;
		for (int i = 0; i < node_0.size(); i++) {
			p = node_0.front();
			node_0.pop();
			node_0.push(p);
			if ((x - p->get_x())*(x - p->get_x()) + (y - p->get_y())*(y - p->get_y()) < S_radio * S_radio)
			{
				for (int j = i + 1; j<node_0.size(); j++) {//保持节点在队列中位置的稳定
					p = node_0.front();
					node_0.pop();
					node_0.push(p);
				}
				return true;
			}
			else if ((x - p->get_x())*(x - p->get_x()) + (y - p->get_y())*(y - p->get_y()) > L_radio * L_radio) {
				j++;
			}
		}
		if (j == node_0.size() && j != 0) { return true; }
		return false;
	}
	string Num_string(int i) {//辅助将数组装换为对应字符串
		string str = "->";
		if (i < 10) { char a = i + 48; str += a; }
		else if (i < 100) {
			char a = i / 10 + 48; str += a;
			char aa = i % 10 + 48; str += aa;
		}
		return str;
	}
	int String_num(int a) {//将对应的字符串路径转换为数组储存,返回节点数目
		int x = 1, y = 0;//x记录位数，y记录路径节点个数
		for (int i = 0; path[a][i] != '\0'; i++) {
			if (path[a][i]>('0' - 1) && path[a][i] < ('9' + 1)) {//获取节点数目方便建立数组
				if (x == 1)y++; x *= 10;
			}
			else { x = 1; }
		}
		x = 1;
		arr = new int[y];
		for (int i = 0, j = 0; path[a][i] != '\0'; i++) {//j用于统计完成转换的节点数目
			if (path[a][i]>('0' - 1) && path[a][i] < ('9' + 1)) {
				if (x == 1) { arr[j++] = path[a][i] - '0'; }
				else if (x == 10) { arr[j - 1] = (path[a][i] - '0') + arr[j - 1] * x; }
				x = 10;
			}
			else { x = 1; }
		}
		return y;
	}
	void Initial_arr()
	{
		for (int i = 0; i < MAX; i++) {
			node_xy[i] = new int[2];
			path_length[i] = new int[MAX];
			path[i] = "0";
		}
		for (int i = 0; i < MAX; i++) {
			node_xy[i][0] = 0; node_xy[i][1] = 0;
			for (int j = 0; j < MAX; j++) {
				path_length[i][j] = INT_MAX;//INT_MAX看做无穷的表示
			}
		}//成功构建并初始化了三个数组
	}
	void Update_arr() {
		Node *p = NULL;
		for (int i = 0; i < MAX; i++) {
			p = node_0.front(); node_0.pop(); node_0.push(p);
			node_xy[i][0] = p->get_x(); node_xy[i][1] = p->get_y();
		}//先给所有点坐标赋值好
		for (int i = 0; i < MAX; i++) {
			for (int j = 0; j < MAX; j++) {
				int x = node_xy[j][0], y = node_xy[j][1];
				double path_xxyy = (node_xy[i][0] - x)*(node_xy[i][0] - x) + (node_xy[i][1] - y)*(node_xy[i][1] - y);//记录两点间距离平方
				if (path_xxyy< L_radio*L_radio) {
					path_length[i][j] = sqrt(path_xxyy);
				}
			}
		}
		for (int i = 0; i < MAX; i++) {
			if (path_length[0][i] != INT_MAX) {
				path[i] += Num_string(i);
			}
		}
		delete p;
	}
	void Delete_arr() {//清楚各个数组信息
		for (int i = 0; i < MAX; i++) {
			delete  path_length[i]; delete node_xy[i];
		}
		delete path_length; delete node_xy;
		while (node_0.size() != 0)
		{
			node_0.pop();
		}
		while (path_1.size() != 0)
		{
			path_1.pop();
		}
		while (path_2.size() != 0)
		{
			path_2.pop();
		}
	}
	void Show_numxy(int x, int x_xy, int y_xy) {//指定位置输出数值
		if ((x > 0 || x == 0) && x < 10) {
			int y = textwidth(48);
			outtextxy(x_xy, y_xy, 48);
			outtextxy(x_xy + y, y_xy, 48);
			outtextxy(x_xy + y + y, y_xy, 48 + x % 10);
		}
		else if (9 < x&&x < 100) {
			outtextxy(x_xy, y_xy, 48);
			int y = textwidth(48);
			outtextxy(x_xy + y, y_xy, 48 + x / 10);
			int yy = textwidth(48 + x / 10);
			outtextxy(x_xy + y + yy, y_xy, 48 + x % 10);
		}
		else if (99 < x&&x < 1000) {
			outtextxy(x_xy, y_xy, 48 + x / 100);
			int y = textwidth(48 + x / 100);
			outtextxy(x_xy + y, y_xy, 48 + (x / 10) % 10);
			int yy = textwidth(48 + (x / 10) % 10);
			outtextxy(x_xy + y + yy, y_xy, 48 + x % 10);
		}
		else { outtextxy(x_xy, y_xy, ' '); }
	}
	int Random() {//随机数生成算法
		return K = (7393 * K + 5939) % 1000000000;
	}
	void Drawnode() {
		outtextxy(2 * LONG + 5, 100, L"开始自动生成节点");
		int x = 0, y = 0, n = 0;
		//x用于记录在哪一个节点附近，y记录距离
		//n辅助探测死节点
		int node_x = 0, node_y = 0;//记录每个节点的信息
		Node *p1 = new Datacollector(200, 150);
		p1->drawnode();
		node_0.push(p1);
		int j = 0;
		for (; node_0.size() < MAX; ) {
			j = node_0.size();
			node_x = Random() % 800, node_y = Random() % 600;
			while (Tnear_text(node_x, node_y) || !Node_lega(node_x, node_y)) {
				node_x = Random() % 800;
				node_y = Random() % 600;
			}
			Node *p2 = new sensor(node_x, node_y);
			p2->drawnode();
			node_0.push(p2);
		}
		outtextxy(2 * LONG + 5, 100, L"节点生成结束                    ");
		Update_arr();
		Show_path();
		Delete_arr();
	}
	void Get_closepath() {//完成节点间最短路径的算法，由于节点数目较多所以采用算法为Dijkstra算法
		for (int i = 0; i < MAX; i++) {
			path_2.push(i);
		}
		path_1.push(0); path_2.pop();//将起点放入path_1中
		for (int i = 1; i <MAX; i++) {//更新直接后继节点的路径
			if (path_length[0][i] != INT_MAX) {//判断是否为直接后继
				path[i] = "0" + Num_string(i);//更新路径,由于是起点后继所以无需更新距离
			}
		}//第一个节点成功进入path_1
		while (path_2.size() != 0) {//开始进行最短路径的运算
			int a = path_2.front();//a辅助记录最短节点节点数
			for (int i = 0; i < path_2.size(); i++) {//寻找到最短那个节点
				if (path_length[0][path_2.front()] < path_length[0][a]) {
					a = path_2.front();
				}
				path_2.push(path_2.front()); path_2.pop();//由于直接寻找所以顺序不重要,此处是为了避免数据丢失
			}
			while (path_2.front() != a) {//弹出选中的节点,更新path_2
				path_2.push(path_2.front()); path_2.pop();
			}path_2.pop();
			if (path_length[0][path_1.back()] + path_length[path_1.back()][a]
				< path_length[0][a] && path_length[path_1.back()][a] != INT_MAX) {//判断路径是否需要更新
				path[a] = path[path_1.back()] + Num_string(a);//更新路径
				path_length[0][a] = path_length[0][path_1.back()] + path_length[path_1.back()][a];
				//更新长度
			}
			path_1.push(a); //更新path_1
			for (int i = 0; i < path_2.size(); i++) {//更新直接后继节点的路径
				if (path_length[path_1.back()][path_2.front()] != INT_MAX&&path_length[0][path_1.back()] +
					path_length[path_1.back()][path_2.front()]< path_length[0][path_2.front()]) {
					//判断是否为直接后继,由于一个节点可能是多个节点的直接后继所以依旧要对比距离
					path[path_2.front()] = path[path_1.back()] + Num_string(path_2.front());//更新路径
					path_length[0][path_2.front()] = path_length[0][path_1.back()] + path_length[path_1.back()][path_2.front()];
					//更新长度
				}
				path_2.push(path_2.front()); path_2.pop();//避免path_2队列在做完循环后顺序颠倒
			}
		}
	}
	void Show_path(int i) {//通过直线展示路径
		int num = String_num(i);//num用于记录节点数目
		for (int j = 0; j < num - 1; j++) {
			setlinecolor(WHITE);
			line(node_xy[arr[j]][0], node_xy[arr[j]][1], node_xy[arr[j + 1]][0], node_xy[arr[j + 1]][1]);
		}
	}
	void Delete_path(int i) {//消除展示路径的直线，及节点的颜色
		int num = String_num(i);//num用于记录节点数目
		for (int j = 0; j < num - 1; j++) {
			setlinecolor(BLACK);
			line(node_xy[arr[j]][0], node_xy[arr[j]][1], node_xy[arr[j + 1]][0], node_xy[arr[j + 1]][1]);
		}
		setfillcolor(RED);
		solidcircle(node_xy[arr[0]][0], node_xy[arr[0]][1], 5);
		for (int j = 0; j < num - 1; j++) {
			setfillcolor(BLUE);
			solidcircle(node_xy[arr[j + 1]][0], node_xy[arr[j + 1]][1], 5);
		}
		Show_nodenum();
	}
	void Choose_node() {//return 0表示退出了节点选择功能,用于辅助检测路径显示功能的正确性,Sensor进行周期性采
		int i = 0;
		while (1) {
			i = (abs(Random())) % (MAX - 1);
			setfillcolor(WHITE);
			solidcircle(node_xy[i][0], node_xy[i][1], 5);
			Show_path(i);
			outtextxy(800, 200, L"路径为如图所显示");
			Delay(Period);
			Delete_path(i);
		}
	}
	void Show_path() {
		Show_nodenum();
		Get_closepath();
		ofstream record("C:\\Users\\李宇鹏\\Desktop\\模拟sensornetwork最短路径测试.txt");//辅助测试最短路径的建立成功与否
		for (int i = 0; i < MAX; i++) {//输出路径
			if (path[i] != "0") {
				record << path[i] << endl;
			}
		}
		record.close();
		Choose_node();
	}
};
#endif
//更新数组信息