#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <wincon.h>
#include <time.h>
#include <string.h>

#define SCR_ROW  25  /*屏幕行数*/
#define SCR_COL  80  /*屏幕列数*/


/*开发商信息链结点结构*/ 
typedef struct developer_node{
  char name[30];                   /*开发商名称*/
  int num;                         /*房屋总套数*/
  float area;                      /*建筑总面积*/
  struct developer_node * next;    /*指向下一结点指针*/
  struct flat_node * fnext;        /*指向楼盘信息支链的指针*/
}DEVELOPER_NODE;


/*楼盘信息链结点结构*/
typedef struct flat_node{
  char name[30];                  /*楼盘名称*/
  char name_of_dev[30];           /*开发商*/
  char date_of_start[12];         /*开盘日期*/
  char location[20];              /*楼盘地址*/
  float floor_area;               /*占地面积*/
  int num_of_building;            /*房屋套数*/
  float structure_area;           /*建筑面积*/
  float ave_price;                /*每平米均价*/
  char person[20];                /*联系人*/
  char tele_num[20];              /*联系电话*/
  struct flat_node * next;        /*指向下一结点的指针*/
  struct sale_node * snext;       /*指向房产销售信息支链的指针*/
}FLAT_NODE;

/*房产销售信息链结点*/
typedef struct sale_node{
  char name[30];                 /*客户姓名*/
  char id_num[20];               /*身份证号*/
  char date[12];                 /*购房日期*/
  char type[12];                 /*房型*/
  char name_of_flat[30];         /*所属楼盘*/
  char build_id[10];             /*楼栋号*/
  float structure_area;          /*建筑面积*/
  float price;                   /*每平米单价*/
  float dec;                     /*折扣*/
  float real_price;              /*实际交易金额*/
  char saleman[30];              /*销售员*/
  struct sale_node * next;       /*指向下一结点的指针*/
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

/**<屏幕窗口信息链结点结构*/
typedef struct layer_node {
    char LayerNo;              /*弹出窗口层数*/
    SMALL_RECT  rcArea;     /*弹出窗口区域坐标*/
    CHAR_INFO *pContent;     /*弹出窗口区域字符单元原信息存储缓冲区*/
    char *pScrAtt;             /*弹出窗口区域字符单元原属性值存储缓冲区*/
    struct layer_node *next;     /*指向下一结点的指针*/
} LAYER_NODE;

/**<标签束结构*/
typedef struct label_bundle {
    char **ppLabel;        /*标签字符串数组首地址*/
    COORD *pLoc;        /*标签定位数组首地址*/
    int num;               /*标签个数*/
} LABEL_BUNDLE;

/**<热区结构 */
typedef struct hot_area {
    SMALL_RECT *pArea;     /*热区定位数组首地址*/
    char *pSort;           /*热区类别(按键、文本框、选项框)数组首地址*/
    char *pTag;            /*热区序号数组首地址*/
    int num;               /*热区个数*/
} HOT_AREA;
 
/**<主菜单名数组*/
char *ga_main_menu[] = {"文件(F)", "编辑(E)","查询(I)","统计(S)","帮助(H)"};

/**<子菜单名数组*/
char *ga_sub_menu[]= {"[S] 数据保存", "[L] 数据加载","[X] 退出 Alt+X"
     , "[1]插入开发商","[2]修改开发商","[3]删除开发商",""
     , "[4]插入楼盘","[5]修改楼盘","[6]删除楼盘",""
     , "[7]插入销售信息","[8]修改销售信息","[9]删除销售信息"
     , "[F] 开发商查询", "[A] 楼盘查询", "[B] 销售查询", "[C] 模糊查询"
     , "[S] 开发商统计",   "[P] 年份统计" ,"[B]销售员统计","[T]占地面积统计" 
     ,"[A] 关于系统",  "[C] 清屏 Alt+C"};

char * gp_scr_att = NULL;   /*存放屏幕上字符单元属性值的缓冲区*/
int    ga_sub_menu_count[] = {3, 11, 4, 4, 2}; /*各主菜单项下子菜单的个数*/
int    gi_sel_menu = 1;     /*被选中的主菜单项号,初始为1*/
int    gi_sel_sub_menu = 0; /*被选中的子菜单项号,初始为0,表示未选中*/
char * FileName[3]={"message1.txt","message2.txt","message3.txt"};/*开发商,楼盘,销售基础信息保存文件*/

HANDLE       gh_std_out;   /*标准输出设备句柄*/
HANDLE       gh_std_in;    /*标准输入设备句柄*/
DEVELOPER_NODE* gp_head = NULL;      /*主链头指针*/
LAYER_NODE * gp_top_layer = NULL;  /*弹出窗口信息链链头*/
CHAR_INFO  * gp_buff_menubar_info = NULL;  /*存放菜单条屏幕区字符信息的缓冲区*/
DWORD        len;           /*指向变量的指针,用来存放字符的实际数目*/

/**<框架函数的声明*/
void  TextInterface(void);/**<系统界面初始化*/
void  ClearScreen(void);  /**<清屏*/
void  ShowMenu(void);   /**<显示主菜单栏*/
void  PopMenu(int num); /**<弹出下拉菜单*/
void  PopOn(SMALL_RECT *, WORD, LABEL_BUNDLE *, HOT_AREA *); /**<弹出窗口屏幕信息维护*/
void  PopOff(void); /**<关闭弹出窗口*/
void  DrawBox(SMALL_RECT *parea);   /**<绘制子菜单边框*/
void  LocateSubMenu(int num, SMALL_RECT *parea);  /**< 弹出菜单定位*/
void  MarkMainMenu(int num);   /**< 标记被选中的主菜单项*/
void  MarkSubMenu(int num);  /**< 标记被选中的子菜单项*/
void  SetMainHotArea(void);  /**< 给主菜单栏设置热区*/
void  RunSystem(void);  /**< 系统功能模块的选择和运行*/
void  CloseSys(DEVELOPER_NODE *hd); 	/**< 关闭系统并退出*/
inline void Gotoxy(COORD pos);		/**< 将光标置于坐标pos处*/
inline void Delay(void);		/**< 时间暂延0.05s*/
inline void ClearScreen(void);		/**< 刷屏函数*/
int   MyStrstr(char *s,char *t);	/**< 在s串中查找t串*/
/**<功能函数的声明*/
void InquireDeveloper(DEVELOPER_NODE *head, char *s,int max,int min);/**<查询开发商*/ 
void InquireFlat(DEVELOPER_NODE *head, char *s,int max,int min);/**<查询楼盘*/ 
void InquireSale2(DEVELOPER_NODE *head, char *n, char *ad, char *bd);/**<查询销售情况1*/ 
void InquireSale1(DEVELOPER_NODE *head, char *id,char * nafl,char * bid);/**<查询销售情况2*/ 
void AutoInquire(DEVELOPER_NODE *phead, char kw[]);/**<模糊查询*/ 
BOOL LoadList(DEVELOPER_NODE  **phead,char *fn[]); /**<加载链表中信息*/
void SaveList(DEVELOPER_NODE *phead,char *fn[]);  /**<保存链表信息*/
BOOL YearStatistics(void);/**<年度信息统计*/
void StatisticalYear(DEVELOPER_NODE *phead);
BOOL DevStatistics(void); /**<开发商信息统计*/
void StatisticalDev(DEVELOPER_NODE *phead);
BOOL SmanStatistics(void);/**<销售人员统计*/
void StatisticalSman(DEVELOPER_NODE *phead);
BOOL AreaStatistics(void);/**<占地面积统计*/
void StatisticalArea(DEVELOPER_NODE *phead);
void TraverseList(DEVELOPER_NODE * phead); /**<遍历整个链表的信息*/
void SetQuitHotArea(HOT_AREA *pHotArea, int iHot);/**<设置退出系统时的热键*/
int  DealQuitInput(HOT_AREA *pHotArea, int *piHot);/**<处理退出系统是输入的信息*/

/**<功能函数模块函数的声明*/
BOOL ExeFunction(int main_menu_num, int sub_menu_num);
BOOL SaveData(void);
BOOL LoadData(void);
BOOL BackupData(void);
BOOL RestoreData(void);
BOOL ExitSys(void);

/**开发商信息维护*/
BOOL InsertDeveloper(void); 
int InsertDeveloperNode(DEVELOPER_NODE **phead);
int  DelDevNode(DEVELOPER_NODE **phead,char *dev_name);
BOOL DelDeveloper(void);
BOOL ModifyDeveloper(void);
int  ModifyDevNode(DEVELOPER_NODE *phead,char *dev_name,DEVELOPER_NODE *pdev_node);
/**<楼盘信息维护*/
BOOL InsertFlat(void);
int InsertflatNode(DEVELOPER_NODE *phead);
FLAT_NODE *SeekFlatNode(DEVELOPER_NODE *phead, char *findname);
BOOL ModifyFlat(void);
int ModifyFlatNode(DEVELOPER_NODE *phead,char *name,FLAT_NODE *pfla_node);
BOOL DelFlat(void);
int DelFlatNode(DEVELOPER_NODE *phead,char *fal_name);
/**<销售情况信息维护*/
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
