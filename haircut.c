#include "stdlib.h"
#include "stdio.h"
#include "malloc.h"

#define ChairNum 7  //total number of the chairs
#define OpenTime 0      //����Ӫҵʱ��
#define CloseTime 480   //��ҵʱ��
#define R rand()       //random number

/*���Ӷ��У����ͺͺ������� */
typedef struct {
	int inTime;
	int servTime;
	int waitTime;
} DataType;

struct  Node;
typedef  struct  Node  *PNode;
struct Node			/* ���ṹ */
{
	DataType     info;
	PNode        link;
};
struct  LinkQueue 		/* ���Ӷ������Ͷ��� */
{
    PNode  f;  		/* ͷָ�� */
    PNode  r;  		/* βָ�� */
};
typedef  struct LinkQueue  *PLinkQueue;		/*���Ӷ������͵�ָ������*/
typedef DataType Customer;

PLinkQueue  createEmptyQueue_link( void );
int  isEmptyQueue_link( PLinkQueue plqu );
void  enQueue_link( PLinkQueue plqu, DataType x );
void  deQueue_link( PLinkQueue plqu );
DataType  frontQueue_link( PLinkQueue plqu );
int iniCustomQueqe(PLinkQueue cusQu);  //creat an random queue of customers


/**********************************************************************************
/��������ģ���������������ӣ�����Ա����ĿChairNum,ÿλ�˿͵���Ϣ��������
/����ɣ�����ʱ�䡢��Ҫ����ʱ�䡢�ȴ�ʱ�䣬���������������ֶα�ʾ������ʱ�����
/Ҫ����ʱ����һ������������档�������˿͵ȴ���ƽ��ʱ���ͳ��ֵ��
/**********************************************************************************/

int  main()
{
	PLinkQueue cusQu = createEmptyQueue_link();
	PLinkQueue waitQu = createEmptyQueue_link();

//	int durtime, servTime,aveTime,aveServTime;
	int freeNum = ChairNum;          //number of the chairs available
	int i, j=0, curTime,totalWaitTime=0,totalStayTime=0;
	int chair[ChairNum]={0};       //���д�ŵ�ǰ�˿�����Ҫ�ķ���ʱ��
	Customer cst;
	int num = iniCustomQueqe(cusQu);

	/*�ٶ�������ʱ�䣬�ȴ������ﻹ��1���˿ͣ����������ֱ�����*/
	for (curTime = 0; curTime < CloseTime ; curTime++)
	{
		cst = frontQueue_link(cusQu);               //�۲�˿���ͷ���Ĺ˿͵���Ϣ
		if (cst.inTime <= curTime)
		{
			enQueue_link(waitQu,cst);               //enter a customer
			deQueue_link(cusQu);
		}

		for (i = 0; i <ChairNum; i++) //���μ��ÿһ�����ε�״̬��
		{
			if (chair[i] > 1)         //not empty, still need i minutes
				chair[i]--;
			else                       //have an empty chair
			{
				if (chair[i] == 1)
					chair[i] --;       //free this chair

				if (!isEmptyQueue_link(waitQu))      //�й˿��ڵȴ�
				{
					cst = frontQueue_link(waitQu);
					deQueue_link(waitQu);      //get a customer
					chair[i] = cst.servTime;    //set the time needd
					cst.waitTime = curTime - cst.inTime;
					totalWaitTime += cst.waitTime;
					if ((curTime+cst.servTime)<=CloseTime) totalStayTime = totalStayTime+cst.waitTime+cst.servTime;
					else totalStayTime = totalStayTime+cst.waitTime;
					printf("Customer: %4d; in:%4d. Wait %4d; chair:%4d; serve %4d min.\n",
						                   j++,cst.inTime,cst.waitTime,i+1,cst.servTime);
				}
			}
		}//end for i
	}//end for curTime
    printf("averageWaitTime:%d min\naverageStayTime:%d min\nactual usage rate:%d%%\n",
           totalWaitTime/num,totalStayTime/num,100*(totalStayTime-totalWaitTime)/(ChairNum*(CloseTime-OpenTime)));

	return 0;
}


/*�������һ���˿��������ʱ��Ϊ1-20���ӣ���Ҫ����ʱ��Ϊ15-100����*/
int iniCustomQueqe(PLinkQueue cusQu)
{
	int interval = 20;
	int minServeTime = 15, maxServeTime = 100;

	int num = 0, t = 0;    //starting point
	Customer cst;

	do
	{
		t = t + (R % interval)+1;
		cst.inTime = t;                 //customer in time
		cst.servTime = minServeTime + R % (maxServeTime - minServeTime);

		enQueue_link(cusQu,cst);          //add one into customer queue
		num ++;
	}
	while ( t < CloseTime);          //��ҵǰ�������Խ���

	printf("Total custom num =%d\n",num);
	return num;            //return the total number of customers in the queue
}


/********************************************************************
  ����һ���ն��У�����ָ��ն��е�ָ��
********************************************************************/
PLinkQueue createEmptyQueue_link( )
{
    PLinkQueue plqu;
	/* ������нṹ */
   plqu = (struct LinkQueue *)malloc(sizeof(struct LinkQueue));
    if (plqu != NULL)
    {
        plqu->f = NULL;    /* ͷβָ���ָ��� */
        plqu->r = NULL;
    }
   else printf("Out of space!! \n");
   return (plqu);
}


/*********************************************************************
  �ж�plqu��ָ�Ķ����Ƿ�Ϊ�ն��У���plqu��ָ�Ķ���Ϊ�ն���ʱ������1��
���򷵻�0
*********************************************************************/
int isEmptyQueue_link( PLinkQueue plqu )
{

    /* ��ͷָ��ָ���Ϊ�ն��� */
    return (plqu->f == NULL);
}


/*********************************************************************
����������, ��ʾ��plqu��ָ�Ķ����в���һ��ֵΪ����Ԫ��
*********************************************************************/
void enQueue_link( PLinkQueue plqu, DataType x )
{
	PNode p;
	/* �����½�� */
	p = (PNode)malloc( sizeof( struct Node ) );
    if ( p == NULL ) printf("�ռ䲻��!");
    else
    {
        /* ���һ��Ԫ�أ����޺�� */
        p->info = x;
        p->link = NULL;
        if (plqu->f == NULL)   /* ԭ��Ϊ�ն��� */
        {
            /* ͷβָ���ָ��p */
            plqu->f = p;
            plqu->r = p;
        }
        else
        {
            /* ��βָ����ָ������룬���޸�βָ�� */
            plqu->r->link = p;
            plqu->r = p;
        }
    }
}


/********************************************************************
  ��ʾ��plqu��ָ�Ķ�����ɾ��һ��Ԫ��
********************************************************************/
void deQueue_link( PLinkQueue plqu )
{
	PNode p;
	/* �����ж϶����Ƿ�Ϊ�գ��ն��в���ɾ�� */
	if( plqu->f == NULL ) printf( "Empty queue.\n " );
    else
    {
        /* ɾ��ͷָ��ָ��Ľ�㣬�����޸�ͷָ�� */
        p = plqu->f;
        plqu->f = plqu->f->link;
        free(p);
    }
}



/********************************************************************
  ��plqu��ָ�Ķ��в���ʱ�������ͷ��Ԫ�ص�ֵ�����б��ֲ���
********************************************************************/
DataType frontQueue_link( PLinkQueue plqu )
{
	DataType a={0,0,0};
    /* ֻȡ��ͷָ��ָ�����ֵ�����ı�ͷָ�� */
	if( plqu->f == NULL )
	{
		printf( "Empty queue.\n " );
		return (a);
	}
    else return (plqu->f->info);
}



