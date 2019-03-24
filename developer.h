#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <wincon.h>
#include <time.h>
#include <string.h>

#define SCR_ROW  25  /*��Ļ����*/
#define SCR_COL  80  /*��Ļ����*/


/*��������Ϣ�����ṹ*/ 
typedef struct developer_node{
  char name[30];                   /*����������*/
  int num;                         /*����������*/
  float area;                      /*���������*/
  struct developer_node * next;    /*ָ����һ���ָ��*/
  struct flat_node * fnext;        /*ָ��¥����Ϣ֧����ָ��*/
}DEVELOPER_NODE;


/*¥����Ϣ�����ṹ*/
typedef struct flat_node{
  char name[30];                  /*¥������*/
  char name_of_dev[30];           /*������*/
  char date_of_start[12];         /*��������*/
  char location[20];              /*¥�̵�ַ*/
  float floor_area;               /*ռ�����*/
  int num_of_building;            /*��������*/
  float structure_area;           /*�������*/
  float ave_price;                /*ÿƽ�׾���*/
  char person[20];                /*��ϵ��*/
  char tele_num[20];              /*��ϵ�绰*/
  struct flat_node * next;        /*ָ����һ����ָ��*/
  struct sale_node * snext;       /*ָ�򷿲�������Ϣ֧����ָ��*/
}FLAT_NODE;

/*����������Ϣ�����*/
typedef struct sale_node{
  char name[30];                 /*�ͻ�����*/
  char id_num[20];               /*���֤��*/
  char date[12];                 /*��������*/
  char type[12];                 /*����*/
  char name_of_flat[30];         /*����¥��*/
  char build_id[10];             /*¥����*/
  float structure_area;          /*�������*/
  float price;                   /*ÿƽ�׵���*/
  float dec;                     /*�ۿ�*/
  float real_price;              /*ʵ�ʽ��׽��*/
  char saleman[30];              /*����Ա*/
  struct sale_node * next;       /*ָ����һ����ָ��*/
}SALE_NODE;

typedef struct sum_node{
	char name[100];
	int isum1;
	int isum2;
	int isum3;
	float fsum1;
	float fsum2;
	float fsum3;
	float fsum4;
	struct sum_node * next;
} SUM_NODE;

/**<��Ļ������Ϣ�����ṹ*/
typedef struct layer_node {
    char LayerNo;              /*�������ڲ���*/
    SMALL_RECT  rcArea;     /*����������������*/
    CHAR_INFO *pContent;     /*�������������ַ���Ԫԭ��Ϣ�洢������*/
    char *pScrAtt;             /*�������������ַ���Ԫԭ����ֵ�洢������*/
    struct layer_node *next;     /*ָ����һ����ָ��*/
} LAYER_NODE;

/**<��ǩ���ṹ*/
typedef struct label_bundle {
    char **ppLabel;        /*��ǩ�ַ��������׵�ַ*/
    COORD *pLoc;        /*��ǩ��λ�����׵�ַ*/
    int num;               /*��ǩ����*/
} LABEL_BUNDLE;

/**<�����ṹ */
typedef struct hot_area {
    SMALL_RECT *pArea;     /*������λ�����׵�ַ*/
    char *pSort;           /*�������(�������ı���ѡ���)�����׵�ַ*/
    char *pTag;            /*������������׵�ַ*/
    int num;               /*��������*/
} HOT_AREA;
 
/**<���˵�������*/
char *ga_main_menu[] = {"�ļ�(F)", "�༭(E)","��ѯ(I)","ͳ��(S)","����(H)"};

/**<�Ӳ˵�������*/
char *ga_sub_menu[]= {"[S] ���ݱ���", "[L] ���ݼ���","[X] �˳� Alt+X"
     , "[1]���뿪����","[2]�޸Ŀ�����","[3]ɾ��������",""
     , "[4]����¥��","[5]�޸�¥��","[6]ɾ��¥��",""
     , "[7]����������Ϣ","[8]�޸�������Ϣ","[9]ɾ��������Ϣ"
     , "[F] �����̲�ѯ", "[A] ¥�̲�ѯ", "[B] ���۲�ѯ", "[C] ģ����ѯ"
     , "[S] ������ͳ��",   "[P] ���ͳ��" ,"[B]����Աͳ��","[T]ռ�����ͳ��" 
     ,"[A] ����ϵͳ",  "[C] ���� Alt+C"};

char * gp_scr_att = NULL;   /*�����Ļ���ַ���Ԫ����ֵ�Ļ�����*/
int    ga_sub_menu_count[] = {3, 11, 4, 4, 2}; /*�����˵������Ӳ˵��ĸ���*/
int    gi_sel_menu = 1;     /*��ѡ�е����˵����,��ʼΪ1*/
int    gi_sel_sub_menu = 0; /*��ѡ�е��Ӳ˵����,��ʼΪ0,��ʾδѡ��*/
char * FileName[3]={"message1.txt","message2.txt","message3.txt"};/*������,¥��,���ۻ�����Ϣ�����ļ�*/

HANDLE       gh_std_out;   /*��׼����豸���*/
HANDLE       gh_std_in;    /*��׼�����豸���*/
DEVELOPER_NODE* gp_head = NULL;      /*����ͷָ��*/
LAYER_NODE * gp_top_layer = NULL;  /*����������Ϣ����ͷ*/
CHAR_INFO  * gp_buff_menubar_info = NULL;  /*��Ų˵�����Ļ���ַ���Ϣ�Ļ�����*/
DWORD        len;           /*ָ�������ָ��,��������ַ���ʵ����Ŀ*/

/**<��ܺ���������*/
void  TextInterface(void);/**<ϵͳ�����ʼ��*/
void  ClearScreen(void);  /**<����*/
void  ShowMenu(void);   /**<��ʾ���˵���*/
void  PopMenu(int num); /**<���������˵�*/
void  PopOn(SMALL_RECT *, WORD, LABEL_BUNDLE *, HOT_AREA *); /**<����������Ļ��Ϣά��*/
void  PopOff(void); /**<�رյ�������*/
void  DrawBox(SMALL_RECT *parea);   /**<�����Ӳ˵��߿�*/
void  LocateSubMenu(int num, SMALL_RECT *parea);  /**< �����˵���λ*/
void  MarkMainMenu(int num);   /**< ��Ǳ�ѡ�е����˵���*/
void  MarkSubMenu(int num);  /**< ��Ǳ�ѡ�е��Ӳ˵���*/
void  SetMainHotArea(void);  /**< �����˵�����������*/
void  RunSystem(void);  /**< ϵͳ����ģ���ѡ�������*/
void  CloseSys(DEVELOPER_NODE *hd); 	/**< �ر�ϵͳ���˳�*/
inline void Gotoxy(COORD pos);		/**< �������������pos��*/
inline void Delay(void);		/**< ʱ������0.05s*/
inline void ClearScreen(void);		/**< ˢ������*/
int   MyStrstr(char *s,char *t);	/**< ��s���в���t��*/
/**<���ܺ���������*/
void InquireDeveloper(DEVELOPER_NODE *head, char *s,int max,int min);/**<��ѯ������*/ 
void InquireFlat(DEVELOPER_NODE *head, char *s,int max,int min);/**<��ѯ¥��*/ 
void InquireSale2(DEVELOPER_NODE *head, char *n, char *ad, char *bd);/**<��ѯ�������1*/ 
void InquireSale1(DEVELOPER_NODE *head, char *id,char * nafl,char * bid);/**<��ѯ�������2*/ 
void AutoInquire(DEVELOPER_NODE *phead, char kw[]);/**<ģ����ѯ*/ 
BOOL LoadList(DEVELOPER_NODE  **phead,char *fn[]); /**<������������Ϣ*/
void SaveList(DEVELOPER_NODE *phead,char *fn[]);  /**<����������Ϣ*/
BOOL YearStatistics(void);/**<�����Ϣͳ��*/
void StatisticalYear(DEVELOPER_NODE *phead);
BOOL DevStatistics(void); /**<��������Ϣͳ��*/
void StatisticalDev(DEVELOPER_NODE *phead);
BOOL SmanStatistics(void);/**<������Աͳ��*/
void StatisticalSman(DEVELOPER_NODE *phead);
BOOL AreaStatistics(void);/**<ռ�����ͳ��*/
void StatisticalArea(DEVELOPER_NODE *phead);
void TraverseList(DEVELOPER_NODE * phead); /**<���������������Ϣ*/
void SetQuitHotArea(HOT_AREA *pHotArea, int iHot);/**<�����˳�ϵͳʱ���ȼ�*/
int  DealQuitInput(HOT_AREA *pHotArea, int *piHot);/**<�����˳�ϵͳ���������Ϣ*/

/**<���ܺ���ģ�麯��������*/
BOOL ExeFunction(int main_menu_num, int sub_menu_num);
BOOL SaveData(void);
BOOL LoadData(void);
BOOL BackupData(void);
BOOL RestoreData(void);
BOOL ExitSys(void);

/**��������Ϣά��*/
BOOL InsertDeveloper(void); 
int InsertDeveloperNode(DEVELOPER_NODE **phead);
int  DelDevNode(DEVELOPER_NODE **phead,char *dev_name);
BOOL DelDeveloper(void);
BOOL ModifyDeveloper(void);
int  ModifyDevNode(DEVELOPER_NODE *phead,char *dev_name,DEVELOPER_NODE *pdev_node);
/**<¥����Ϣά��*/
BOOL InsertFlat(void);
int InsertflatNode(DEVELOPER_NODE *phead);
FLAT_NODE *SeekFlatNode(DEVELOPER_NODE *phead, char *findname);
BOOL ModifyFlat(void);
int ModifyFlatNode(DEVELOPER_NODE *phead,char *name,FLAT_NODE *pfla_node);
BOOL DelFlat(void);
int DelFlatNode(DEVELOPER_NODE *phead,char *fal_name);
/**<���������Ϣά��*/
BOOL InsertSale(void);
int InsertSaleNode(DEVELOPER_NODE *phead, SALE_NODE *psal_node);
SALE_NODE *SeekSaleNode(DEVELOPER_NODE *phead,char *name1, char *e_num);
BOOL ModifySale(void);
int ModifySaleNode(DEVELOPER_NODE *phead,char *flatname, char *e_num, SALE_NODE *psal_node);
BOOL DelSale(void);
int DelSaleNode(DEVELOPER_NODE *phead,char *flatname, char *e_num);
int rightyear(char *a,char *b);
int diff(char **s,char * m,int flag);

BOOL FlatInquire(void);
BOOL DeveloperInquire(void);
BOOL SaleInquire(void);
BOOL InquireAuto(void);
BOOL DevStatistics(void);
BOOL PriceStatistics(void);
BOOL TraverseData(void);
BOOL AboutSys(void);
BOOL Clear(void);
