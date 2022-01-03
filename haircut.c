#include "stdlib.h"
#include "stdio.h"
#include "malloc.h"

#define ChairNum 7  //total number of the chairs
#define OpenTime 0      //开门营业时间
#define CloseTime 480   //休业时间
#define R rand()       //random number

/*链接队列：类型和函数声明 */
typedef struct {
	int inTime;
	int servTime;
	int waitTime;
} DataType;

struct  Node;
typedef  struct  Node  *PNode;
struct Node			/* 结点结构 */
{
	DataType     info;
	PNode        link;
};
struct  LinkQueue 		/* 链接队列类型定义 */
{
    PNode  f;  		/* 头指针 */
    PNode  r;  		/* 尾指针 */
};
typedef  struct LinkQueue  *PLinkQueue;		/*链接队列类型的指针类型*/
typedef DataType Customer;

PLinkQueue  createEmptyQueue_link( void );
int  isEmptyQueue_link( PLinkQueue plqu );
void  enQueue_link( PLinkQueue plqu, DataType x );
void  deQueue_link( PLinkQueue plqu );
DataType  frontQueue_link( PLinkQueue plqu );
int iniCustomQueqe(PLinkQueue cusQu);  //creat an random queue of customers


/**********************************************************************************
/理发店问题模拟仿真程序。设置椅子（服务员）数目ChairNum,每位顾客的信息由三个部
/分组成：到店时间、需要服务时间、等待时间，由三个整数类型字段表示。到店时间和需
/要服务时间由一个随机数来代替。最后求出顾客等待的平均时间的统计值。
/**********************************************************************************/

int  main()
{
	PLinkQueue cusQu = createEmptyQueue_link();
	PLinkQueue waitQu = createEmptyQueue_link();

//	int durtime, servTime,aveTime,aveServTime;
	int freeNum = ChairNum;          //number of the chairs available
	int i, j=0, curTime,totalWaitTime=0,totalStayTime=0;
	int chair[ChairNum]={0};       //其中存放当前顾客所需要的服务时间
	Customer cst;
	int num = iniCustomQueqe(cusQu);

	/*假定到关门时间，等待队列里还有1个顾客，则继续服务直到完成*/
	for (curTime = 0; curTime < CloseTime ; curTime++)
	{
		cst = frontQueue_link(cusQu);               //观察顾客流头部的顾客的信息
		if (cst.inTime <= curTime)
		{
			enQueue_link(waitQu,cst);               //enter a customer
			deQueue_link(cusQu);
		}

		for (i = 0; i <ChairNum; i++) //依次检测每一把理发椅的状态。
		{
			if (chair[i] > 1)         //not empty, still need i minutes
				chair[i]--;
			else                       //have an empty chair
			{
				if (chair[i] == 1)
					chair[i] --;       //free this chair

				if (!isEmptyQueue_link(waitQu))      //有顾客在等待
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


/*随机产生一个顾客流，间隔时间为1-20分钟，需要服务时间为15-100分钟*/
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
	while ( t < CloseTime);          //休业前都还可以进入

	printf("Total custom num =%d\n",num);
	return num;            //return the total number of customers in the queue
}


/********************************************************************
  创建一个空队列，返回指向空队列的指针
********************************************************************/
PLinkQueue createEmptyQueue_link( )
{
    PLinkQueue plqu;
	/* 申请队列结构 */
   plqu = (struct LinkQueue *)malloc(sizeof(struct LinkQueue));
    if (plqu != NULL)
    {
        plqu->f = NULL;    /* 头尾指针皆指向空 */
        plqu->r = NULL;
    }
   else printf("Out of space!! \n");
   return (plqu);
}


/*********************************************************************
  判断plqu所指的队列是否为空队列，当plqu所指的队列为空队列时，返回1，
否则返回0
*********************************************************************/
int isEmptyQueue_link( PLinkQueue plqu )
{

    /* 队头指针指向空为空队列 */
    return (plqu->f == NULL);
}


/*********************************************************************
进队列运算, 表示往plqu所指的队列中插入一个值为ｘ的元素
*********************************************************************/
void enQueue_link( PLinkQueue plqu, DataType x )
{
	PNode p;
	/* 申请新结点 */
	p = (PNode)malloc( sizeof( struct Node ) );
    if ( p == NULL ) printf("空间不够!");
    else
    {
        /* 最后一个元素，其无后继 */
        p->info = x;
        p->link = NULL;
        if (plqu->f == NULL)   /* 原来为空队列 */
        {
            /* 头尾指针皆指向p */
            plqu->f = p;
            plqu->r = p;
        }
        else
        {
            /* 在尾指针所指结点后插入，并修改尾指针 */
            plqu->r->link = p;
            plqu->r = p;
        }
    }
}


/********************************************************************
  表示从plqu所指的队列中删除一个元素
********************************************************************/
void deQueue_link( PLinkQueue plqu )
{
	PNode p;
	/* 首先判断队列是否为空，空队列不能删除 */
	if( plqu->f == NULL ) printf( "Empty queue.\n " );
    else
    {
        /* 删除头指针指向的结点，并且修改头指针 */
        p = plqu->f;
        plqu->f = plqu->f->link;
        free(p);
    }
}



/********************************************************************
  当plqu所指的队列不空时，求队列头部元素的值，队列保持不变
********************************************************************/
DataType frontQueue_link( PLinkQueue plqu )
{
	DataType a={0,0,0};
    /* 只取队头指针指向结点的值，不改变头指针 */
	if( plqu->f == NULL )
	{
		printf( "Empty queue.\n " );
		return (a);
	}
    else return (plqu->f->info);
}



