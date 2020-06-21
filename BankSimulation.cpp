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

//�ͻ��ṹ��
typedef struct Customer {
	//��Ǯ��ȡǮ����Ŀ
	int money;
	//��Ǯ����ȡǮ
	CustomerType type;
	//����ʱ��
	int time;
}Customer;
//���н��ṹ��
typedef struct LQNode {
	//�ͻ�����
	Customer customer;
	//ָ����һ�����
	struct LQNode *next;
}LQNode, *QueuePtr;
//���нṹ��
typedef struct LQueue{
	//��ͷָ��
	QueuePtr front;
	//��βָ��
	QueuePtr rear;
}LQueue;
//���нṹ��
typedef struct Bank {
	//����ʣ���ʽ�
	int money;
	//����1
	LQueue window_1;
	//����2
	LQueue window_2;
	//ʱ��
	int time;
}Bank;

//����߿�
void PrintLine() {
	printf("------------------------------------\n");
}

//��ʼ������
void InitQueue_LQ(LQueue &Q) {
	Q.front = NULL;
	Q.rear = NULL;
}
//�����п�
Status QueueEmpty_LQ(LQueue Q) {
	if (NULL == Q.front)return TRUE;
	return FALSE;
}
//��ȡ��ͷԪ��
Status GetHead_LQ(LQueue Q, Customer &e) {
	if (TRUE == QueueEmpty_LQ(Q)) return ERROR;
	e = Q.front->customer;
	return OK;
}
//���
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
//����
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
//���ٶ���
void DestroyQueue_LQ(LQueue &Q) {
	Customer e;
	while (FALSE == QueueEmpty_LQ) {
		DeQueue_LQ(Q, e);
	}
}
//�������
void DisPlay(LQueue Q) {
	if (TRUE == QueueEmpty_LQ(Q)) {
		printf("����Ϊ��\n");
		PrintLine();
	}
	else {
		LQNode *p;
		p = Q.front;
		while (NULL != p) {
			if (SAVE == p->customer.type)printf("|��%d|", p->customer.money);
			else printf("|ȡ%d|", p->customer.money);
			p = p->next;
		}
		printf("\n");
		PrintLine();
	}
}

//��ʼ������
void InitBank(Bank &bank, int money, int t) {
	LQueue window_1, window_2;
	InitQueue_LQ(window_1);
	InitQueue_LQ(window_2);
	bank.money = money;
	bank.window_1 = window_1;
	bank.window_2 = window_2;
	bank.time = t;
}
//��ʼ���ͻ�
void InitCustomer(Customer &customer, int rand) {
	customer.money = rand % 5000;
	customer.type = rand % 2;
	customer.time = rand % 12 + 3;
	printf("���ɿͻ���money = %d��time = %d��", customer.money, customer.time);
	if (SAVE == customer.type)printf("type = ��Ǯ\n");
	else printf("type = ȡǮ\n");
	PrintLine();
}
//���в���
void BankOperation(Bank &bank, int &num, int window_name) {
	Customer customer_out;
	if (1 == window_name) {
		//����һ����
		printf("����һ��");
		//��ȡ��ͷԪ��
		GetHead_LQ(bank.window_1, customer_out);
		//����һ�п�
		if (TRUE == QueueEmpty_LQ(bank.window_1)) {
			//��������ʱ��
			bank.time = (int)time(NULL);
			printf("�ȴ��ͻ���...\n");
			PrintLine();
		}
		else {
			//�жϴ�Ǯ����ȡǮ
			if (SAVE == customer_out.type) {
				//�ж��Ƿ�ﵽ����ʱ��
				if ((int)time(NULL) - bank.time > customer_out.time) {
					//��������ʱ��
					bank.time += customer_out.time;
					//���ӣ�����ҵ��
					DeQueue_LQ(bank.window_1, customer_out);
					bank.money += customer_out.money;
					printf("�ͻ���Ǯ��%d������ʱ��%d\n", customer_out.money, customer_out.time);
					num++;
				}
				else {
					printf("ҵ������...\n");
				}
				PrintLine();
			}
			else {
				//�ж�ʣ���ʽ��Ƿ����
				if (bank.money > customer_out.money) {
					if ((int)time(NULL) - bank.time > customer_out.time) {
						//��������ʱ��
						bank.time += customer_out.time;
						//���ӣ�����ҵ��
						DeQueue_LQ(bank.window_1, customer_out);
						bank.money -= customer_out.money;
						printf("�ͻ�ȡǮ��%d������ʱ��%d\n", customer_out.money, customer_out.time);
						num++;
					}
					else {
						printf("ҵ������...\n");
					}
					PrintLine();
				}
				else {
					//�Ӵ���һ���ӣ���Ӵ��ڶ�
					DeQueue_LQ(bank.window_1, customer_out);
					EnQueue_LQ(bank.window_2, customer_out);
					printf("�ʽ��㣬�ͻ����봰�ڶ�\n");
					PrintLine();
				}
			}
		}
	}
	else {
		//���ڶ�����
		printf("���ڶ���");
		GetHead_LQ(bank.window_2, customer_out);
		//�ж��ʽ��Ƿ����
		if (bank.money > customer_out.money) {
			if ((int)time(NULL) - bank.time > customer_out.time) {
				//��������ʱ��
				bank.time += customer_out.time;
				//���ӣ�����ҵ��
				DeQueue_LQ(bank.window_2, customer_out);
				bank.money -= customer_out.money;
				printf("�ͻ�ȡǮ��%d������ʱ��%d\n", customer_out.money, customer_out.time);
				num++;
			}
			else {
				printf("ҵ������...\n");
			}
			PrintLine();
		}
		else {
			printf("�ʽ��㣬�ͻ������ȴ�\n");
			PrintLine();
		}
		//������ת������һ����
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

	printf("����Ҫ���е�ʱ�䣨�룩��");
	scanf_s("%d", &input);

	//��ʼ������
	InitBank(bank, money, open_time);
	//��ʼ�����������
	srand((int)time(NULL));

	while (1) {
		Sleep(2000);
		system("cls");
		PrintLine();
		//������ɿͻ�
		if (0 == rand() % 3) {
			InitCustomer(customer_in, rand());
			//�û������һ������
			EnQueue_LQ(bank.window_1, customer_in);
		}
		//����ҵ����
		if (TRUE == QueueEmpty_LQ(bank.window_2)) {
			//���ڶ�Ϊ�գ�������һ
			printf("���ڶ����ȴ��ͻ���...\n");
			PrintLine();
			BankOperation(bank, num, 1);
		}
		else {
			//���ڶ���Ϊ�գ������ڶ�
			BankOperation(bank, num, 2);
		}
		//��ʾ��ǰ�������
		printf("����һ���У�");
		DisPlay(bank.window_1);
		printf("���ڶ����У�");
		DisPlay(bank.window_2);
		//��ʾ����ʣ���ʽ�
		printf("��ǰ����ʣ���ʽ�%d\n", bank.money);
		PrintLine();
		printf("�Ӵ�������%d���Ӵ�ʱ�䣺%d\n", num, bank.time - open_time);
		PrintLine(); printf("\n");
		//�ж�ʱ�䵽��û��
		if (input <= (int)time(NULL) - open_time)break;
	}
	DestroyQueue_LQ(bank.window_1);
	DestroyQueue_LQ(bank.window_2);
	system("pause");
}