#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW -1
#define SAVE 1
#define WITHDRAW -1
typedef int Status;
typedef int CustomerType;

//客户结构体
typedef struct Customer {
	//存钱或取钱的数目
	int money;
	//存钱还是取钱
	CustomerType type;
	//处理时间
	int time;
}Customer;
//队列结点结构体
typedef struct LQNode {
	//客户数据
	Customer customer;
	//指向下一个结点
	struct LQNode *next;
}LQNode, *QueuePtr;
//队列结构体
typedef struct LQueue{
	//队头指针
	QueuePtr front;
	//队尾指针
	QueuePtr rear;
}LQueue;
//银行结构体
typedef struct Bank {
	//银行剩余资金
	int money;
	//窗口1
	LQueue window_1;
	//窗口2
	LQueue window_2;
	//时间
	int time;
}Bank;

//输出边框
void PrintLine() {
	printf("------------------------------------\n");
}

//初始化队列
void InitQueue_LQ(LQueue &Q) {
	Q.front = NULL;
	Q.rear = NULL;
}
//队列判空
Status QueueEmpty_LQ(LQueue Q) {
	if (NULL == Q.front)return TRUE;
	return FALSE;
}
//获取队头元素
Status GetHead_LQ(LQueue Q, Customer &e) {
	if (TRUE == QueueEmpty_LQ(Q)) return ERROR;
	e = Q.front->customer;
	return OK;
}
//入队
Status EnQueue_LQ(LQueue &Q, Customer e) {
	LQNode *p;
	p = (LQNode*)malloc(sizeof(LQNode));
	if (NULL == p)return OVERFLOW;
	p->customer = e; p->next = NULL;
	if (TRUE == QueueEmpty_LQ(Q)) Q.front = p;
	else Q.rear->next = p;
	Q.rear = p;
	return OK;
}
//出队
Status DeQueue_LQ(LQueue &Q, Customer &e) {
	LQNode *p;
	if (TRUE == QueueEmpty_LQ(Q)) return ERROR;
	p = Q.front;
	e = p->customer;
	Q.front = p->next;
	if (Q.rear == p) Q.rear = NULL;
	free(p);
	return OK;
}
//销毁队列
void DestroyQueue_LQ(LQueue &Q) {
	Customer e;
	while (FALSE == QueueEmpty_LQ) {
		DeQueue_LQ(Q, e);
	}
}
//输出队列
void DisPlay(LQueue Q) {
	if (TRUE == QueueEmpty_LQ(Q)) {
		printf("队列为空\n");
		PrintLine();
	}
	else {
		LQNode *p;
		p = Q.front;
		while (NULL != p) {
			if (SAVE == p->customer.type)printf("|存%d|", p->customer.money);
			else printf("|取%d|", p->customer.money);
			p = p->next;
		}
		printf("\n");
		PrintLine();
	}
}

//初始化银行
void InitBank(Bank &bank, int money, int t) {
	LQueue window_1, window_2;
	InitQueue_LQ(window_1);
	InitQueue_LQ(window_2);
	bank.money = money;
	bank.window_1 = window_1;
	bank.window_2 = window_2;
	bank.time = t;
}
//初始化客户
void InitCustomer(Customer &customer, int rand) {
	customer.money = rand % 5000;
	customer.type = rand % 2;
	customer.time = rand % 12 + 3;
	printf("生成客户：money = %d，time = %d，", customer.money, customer.time);
	if (SAVE == customer.type)printf("type = 存钱\n");
	else printf("type = 取钱\n");
	PrintLine();
}
//银行操作
void BankOperation(Bank &bank, int &num, int window_name) {
	Customer customer_out;
	if (1 == window_name) {
		//窗口一操作
		printf("窗口一：");
		//获取队头元素
		GetHead_LQ(bank.window_1, customer_out);
		//窗口一判空
		if (TRUE == QueueEmpty_LQ(bank.window_1)) {
			//更新银行时间
			bank.time = (int)time(NULL);
			printf("等待客户中...\n");
			PrintLine();
		}
		else {
			//判断存钱还是取钱
			if (SAVE == customer_out.type) {
				//判断是否达到处理时间
				if ((int)time(NULL) - bank.time > customer_out.time) {
					//更新银行时间
					bank.time += customer_out.time;
					//出队，处理业务
					DeQueue_LQ(bank.window_1, customer_out);
					bank.money += customer_out.money;
					printf("客户存钱：%d，花费时间%d\n", customer_out.money, customer_out.time);
					num++;
				}
				else {
					printf("业务处理中...\n");
				}
				PrintLine();
			}
			else {
				//判断剩余资金是否充足
				if (bank.money > customer_out.money) {
					if ((int)time(NULL) - bank.time > customer_out.time) {
						//更新银行时间
						bank.time += customer_out.time;
						//出队，处理业务
						DeQueue_LQ(bank.window_1, customer_out);
						bank.money -= customer_out.money;
						printf("客户取钱：%d，花费时间%d\n", customer_out.money, customer_out.time);
						num++;
					}
					else {
						printf("业务处理中...\n");
					}
					PrintLine();
				}
				else {
					//从窗口一出队，入队窗口二
					DeQueue_LQ(bank.window_1, customer_out);
					EnQueue_LQ(bank.window_2, customer_out);
					printf("资金不足，客户进入窗口二\n");
					PrintLine();
				}
			}
		}
	}
	else {
		//窗口二操作
		printf("窗口二：");
		GetHead_LQ(bank.window_2, customer_out);
		//判断资金是否充足
		if (bank.money > customer_out.money) {
			if ((int)time(NULL) - bank.time > customer_out.time) {
				//更新银行时间
				bank.time += customer_out.time;
				//出队，处理业务
				DeQueue_LQ(bank.window_2, customer_out);
				bank.money -= customer_out.money;
				printf("客户取钱：%d，花费时间%d\n", customer_out.money, customer_out.time);
				num++;
			}
			else {
				printf("业务处理中...\n");
			}
			PrintLine();
		}
		else {
			printf("资金不足，客户继续等待\n");
			PrintLine();
		}
		//处理完转到窗口一操作
		BankOperation(bank, num, 1);
	}
}

void main() {
	Bank bank;
	Customer customer_in;
	int money = 10000;
	int num = 0;
	int open_time = (int)time(NULL);
	int input = 0;

	printf("输入要运行的时间（秒）：");
	scanf_s("%d", &input);

	//初始化银行
	InitBank(bank, money, open_time);
	//初始化随机数种子
	srand((int)time(NULL));

	while (1) {
		Sleep(2000);
		system("cls");
		PrintLine();
		//随机生成客户
		if (0 == rand() % 3) {
			InitCustomer(customer_in, rand());
			//用户进入第一个窗口
			EnQueue_LQ(bank.window_1, customer_in);
		}
		//银行业务处理
		if (TRUE == QueueEmpty_LQ(bank.window_2)) {
			//窗口二为空，处理窗口一
			printf("窗口二：等待客户中...\n");
			PrintLine();
			BankOperation(bank, num, 1);
		}
		else {
			//窗口二不为空，处理窗口二
			BankOperation(bank, num, 2);
		}
		//显示当前队列情况
		printf("窗口一队列：");
		DisPlay(bank.window_1);
		printf("窗口二队列：");
		DisPlay(bank.window_2);
		//显示银行剩余资金
		printf("当前银行剩余资金：%d\n", bank.money);
		PrintLine();
		printf("接待人数：%d，接待时间：%d\n", num, bank.time - open_time);
		PrintLine(); printf("\n");
		//判断时间到了没有
		if (input <= (int)time(NULL) - open_time)break;
	}
	DestroyQueue_LQ(bank.window_1);
	DestroyQueue_LQ(bank.window_2);
	system("pause");
}