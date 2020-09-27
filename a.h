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
int datacollector_a = 0;//datacollector_aΪ���datacollector��������ȫ�ֱ���
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
	queue<Node*>node_0;//����ڵ���Ϣ
	int MAX = 100+1;//�ڵ���Ŀ����
	int HIGH = 600, WIDE = 1000, LONG = 400, SHORT = 300;
	//HIGH,WIDE��Ӧ���泤��LONG��SHORT��Ӧ��Բ����
	double Space = 3.14 * LONG*SHORT;//��Բ���
	int S_radio = 40, L_radio = 70;//��Χ��9-100֮��,S_radio��С�����Χ��L_radio��󴫲���Χ
	int datacoller_num = 1;//Datacollector������λ��
	int **node_xy = new int*[MAX];//��¼���ڵ������λ����Ϣ
	int **path_length = new int*[MAX];//��¼�����ľ��룬��INT_MAX�������  
	queue<int>path_1;//path_1��¼������·������Ľڵ�
	queue<int>path_2;//path_2���ڼ�¼δ�������Ľڵ�
	string *path = new string[MAX]; //path[i]��¼��0��i���·����·����Ϣ��Ȼ��ͨ��delete��new�������·��
									//���������sensor��datacollector·�����ɿ�����datacollector��ÿ��sensor·�������Խ���һά���鼴��
	int *arr = NULL;//������ʾ��ɾ��·��
	int Period = 2;
	int K = 0;//�����
	Insterestarea() {//���캯��
		srand(time(NULL));
		K = rand();
		Initial_arr();
	}
	Insterestarea(int MAX,int Period) {//���캯��
		this->MAX = MAX;
		this->Period = Period;
		srand(time(NULL));
		K = rand();
		Initial_arr();
	}
	void Drawmap() {//���ƻ�������
		initgraph(WIDE, HIGH);
		ellipse(0, 0, 2 * LONG, 2 * SHORT);
		line(2 * LONG, 0, 2 * LONG, 2 * SHORT);
		outtextxy(2 * LONG + 5, 0, L"����ʾdatacollector");
		outtextxy(2 * LONG + 5, 20, L"�����ʾsensor");
		outtextxy(2 * LONG + 5, 40, 48 + S_radio / 10);
		outtextxy(2 * LONG + 15, 40, 48 + S_radio % 10);
		outtextxy(2 * LONG + 25, 40, L"�ڽ���һ���ڵ�");
		outtextxy(2 * LONG + 5, 60, 48 + L_radio / 10);
		outtextxy(2 * LONG + 15, 60, 48 + L_radio % 10);
		outtextxy(2 * LONG + 25, 60, L"Ϊ������Χ");
		outtextxy(2 * LONG + 5, 80, L"�Զ����ɽڵ�");
	}
	int Show_nodenum()
	{
		settextcolor(RED);
		for (int i = 0; i < MAX; i++)//��ÿ���ڵ�ֱ����ʾ�ڵ���
		{
			Show_numxy(i, node_xy[i][0] + 5, node_xy[i][1] - 5);
		}
		settextcolor(WHITE);
		return 0;
	}
	void Delay(int a) {//�ӳ�i���ʱ��
		for (int i = 0; i < a; i++) {
			Sleep(1000);
			outtextxy(2 * LONG + 5, 140, L"����    �����һ�νڵ�");
			Show_numxy(a-i, 2 * LONG + 20, 140);
		}
		Sleep(1000);
	}
	bool Node_lega(int x, int y)//�ж��Ƿ�����Բ��
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
	bool Tnear_text(int x, int y) {//�ж����ڵ�λ���Ƿ�������߹�Զ,���������򷵻�true
		if (node_0.size() == 0)return false;
		Node* p = NULL;
		int j = 0;
		for (int i = 0; i < node_0.size(); i++) {
			p = node_0.front();
			node_0.pop();
			node_0.push(p);
			if ((x - p->get_x())*(x - p->get_x()) + (y - p->get_y())*(y - p->get_y()) < S_radio * S_radio)
			{
				for (int j = i + 1; j<node_0.size(); j++) {//���ֽڵ��ڶ�����λ�õ��ȶ�
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
	string Num_string(int i) {//����������װ��Ϊ��Ӧ�ַ���
		string str = "->";
		if (i < 10) { char a = i + 48; str += a; }
		else if (i < 100) {
			char a = i / 10 + 48; str += a;
			char aa = i % 10 + 48; str += aa;
		}
		return str;
	}
	int String_num(int a) {//����Ӧ���ַ���·��ת��Ϊ���鴢��,���ؽڵ���Ŀ
		int x = 1, y = 0;//x��¼λ����y��¼·���ڵ����
		for (int i = 0; path[a][i] != '\0'; i++) {
			if (path[a][i]>('0' - 1) && path[a][i] < ('9' + 1)) {//��ȡ�ڵ���Ŀ���㽨������
				if (x == 1)y++; x *= 10;
			}
			else { x = 1; }
		}
		x = 1;
		arr = new int[y];
		for (int i = 0, j = 0; path[a][i] != '\0'; i++) {//j����ͳ�����ת���Ľڵ���Ŀ
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
				path_length[i][j] = INT_MAX;//INT_MAX��������ı�ʾ
			}
		}//�ɹ���������ʼ������������
	}
	void Update_arr() {
		Node *p = NULL;
		for (int i = 0; i < MAX; i++) {
			p = node_0.front(); node_0.pop(); node_0.push(p);
			node_xy[i][0] = p->get_x(); node_xy[i][1] = p->get_y();
		}//�ȸ����е����긳ֵ��
		for (int i = 0; i < MAX; i++) {
			for (int j = 0; j < MAX; j++) {
				int x = node_xy[j][0], y = node_xy[j][1];
				double path_xxyy = (node_xy[i][0] - x)*(node_xy[i][0] - x) + (node_xy[i][1] - y)*(node_xy[i][1] - y);//��¼��������ƽ��
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
	void Delete_arr() {//�������������Ϣ
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
	void Show_numxy(int x, int x_xy, int y_xy) {//ָ��λ�������ֵ
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
	int Random() {//����������㷨
		return K = (7393 * K + 5939) % 1000000000;
	}
	void Drawnode() {
		outtextxy(2 * LONG + 5, 100, L"��ʼ�Զ����ɽڵ�");
		int x = 0, y = 0, n = 0;
		//x���ڼ�¼����һ���ڵ㸽����y��¼����
		//n����̽�����ڵ�
		int node_x = 0, node_y = 0;//��¼ÿ���ڵ����Ϣ
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
		outtextxy(2 * LONG + 5, 100, L"�ڵ����ɽ���                    ");
		Update_arr();
		Show_path();
		Delete_arr();
	}
	void Get_closepath() {//��ɽڵ�����·�����㷨�����ڽڵ���Ŀ�϶����Բ����㷨ΪDijkstra�㷨
		for (int i = 0; i < MAX; i++) {
			path_2.push(i);
		}
		path_1.push(0); path_2.pop();//��������path_1��
		for (int i = 1; i <MAX; i++) {//����ֱ�Ӻ�̽ڵ��·��
			if (path_length[0][i] != INT_MAX) {//�ж��Ƿ�Ϊֱ�Ӻ��
				path[i] = "0" + Num_string(i);//����·��,���������������������¾���
			}
		}//��һ���ڵ�ɹ�����path_1
		while (path_2.size() != 0) {//��ʼ�������·��������
			int a = path_2.front();//a������¼��̽ڵ�ڵ���
			for (int i = 0; i < path_2.size(); i++) {//Ѱ�ҵ�����Ǹ��ڵ�
				if (path_length[0][path_2.front()] < path_length[0][a]) {
					a = path_2.front();
				}
				path_2.push(path_2.front()); path_2.pop();//����ֱ��Ѱ������˳����Ҫ,�˴���Ϊ�˱������ݶ�ʧ
			}
			while (path_2.front() != a) {//����ѡ�еĽڵ�,����path_2
				path_2.push(path_2.front()); path_2.pop();
			}path_2.pop();
			if (path_length[0][path_1.back()] + path_length[path_1.back()][a]
				< path_length[0][a] && path_length[path_1.back()][a] != INT_MAX) {//�ж�·���Ƿ���Ҫ����
				path[a] = path[path_1.back()] + Num_string(a);//����·��
				path_length[0][a] = path_length[0][path_1.back()] + path_length[path_1.back()][a];
				//���³���
			}
			path_1.push(a); //����path_1
			for (int i = 0; i < path_2.size(); i++) {//����ֱ�Ӻ�̽ڵ��·��
				if (path_length[path_1.back()][path_2.front()] != INT_MAX&&path_length[0][path_1.back()] +
					path_length[path_1.back()][path_2.front()]< path_length[0][path_2.front()]) {
					//�ж��Ƿ�Ϊֱ�Ӻ��,����һ���ڵ�����Ƕ���ڵ��ֱ�Ӻ����������Ҫ�ԱȾ���
					path[path_2.front()] = path[path_1.back()] + Num_string(path_2.front());//����·��
					path_length[0][path_2.front()] = path_length[0][path_1.back()] + path_length[path_1.back()][path_2.front()];
					//���³���
				}
				path_2.push(path_2.front()); path_2.pop();//����path_2����������ѭ����˳��ߵ�
			}
		}
	}
	void Show_path(int i) {//ͨ��ֱ��չʾ·��
		int num = String_num(i);//num���ڼ�¼�ڵ���Ŀ
		for (int j = 0; j < num - 1; j++) {
			setlinecolor(WHITE);
			line(node_xy[arr[j]][0], node_xy[arr[j]][1], node_xy[arr[j + 1]][0], node_xy[arr[j + 1]][1]);
		}
	}
	void Delete_path(int i) {//����չʾ·����ֱ�ߣ����ڵ����ɫ
		int num = String_num(i);//num���ڼ�¼�ڵ���Ŀ
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
	void Choose_node() {//return 0��ʾ�˳��˽ڵ�ѡ����,���ڸ������·����ʾ���ܵ���ȷ��,Sensor���������Բ�
		int i = 0;
		while (1) {
			i = (abs(Random())) % (MAX - 1);
			setfillcolor(WHITE);
			solidcircle(node_xy[i][0], node_xy[i][1], 5);
			Show_path(i);
			outtextxy(800, 200, L"·��Ϊ��ͼ����ʾ");
			Delay(Period);
			Delete_path(i);
		}
	}
	void Show_path() {
		Show_nodenum();
		Get_closepath();
		ofstream record("C:\\Users\\������\\Desktop\\ģ��sensornetwork���·������.txt");//�����������·���Ľ����ɹ����
		for (int i = 0; i < MAX; i++) {//���·��
			if (path[i] != "0") {
				record << path[i] << endl;
			}
		}
		record.close();
		Choose_node();
	}
};
#endif
//����������Ϣ