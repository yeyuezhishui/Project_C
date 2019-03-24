#include "developer.h"

/**< Main函数.*/
int main(void)
{

    TextInterface();  /*文本菜单界面初始化*/ 
    LoadList(&gp_head,FileName);  /*加载信息*/
	RunSystem();    /*系统功能模块的选择及运行*/
    SaveList(gp_head,FileName);   /*保存信息*/
    CloseSys(gp_head);         /*关闭系统*/
	return 0;
}

/**< 将光标置于pos位置处*/
inline void Gotoxy(COORD pos)
{
    gh_std_out = GetStdHandle(STD_OUTPUT_HANDLE);  /*获取标准输出设备句柄*/
	SetConsoleCursorPosition(gh_std_out,pos);      /*设置光标位置在pos处*/
}

inline void Delay(void)
{
  int goal;
  goal = 50 + clock();
  while ( goal > clock() );
}

/**< 关闭系统,释放动态存储区.*/
void CloseSys(DEVELOPER_NODE *hd)
{
    DEVELOPER_NODE *p1 = hd,  *pp1;
    FLAT_NODE *p2, *pp2;
    SALE_NODE *p3, *pp3;
    /*释放十字交叉链表的动态存储区*/
    while (p1 != NULL)
    {
        pp1 = p1->next;
        p2 = p1->fnext;
        while (p2 != NULL)     /*释放楼盘支链的动态存储区*/
        {
            pp2 = p2->next;
            p3 = p2->snext;
            while (p3 != NULL) /*释放销售信息支链的动态存储区*/
            {
                pp3 = p3->next;
                free(p3);
                p3 = pp3;
            }
            free(p2);
            p2 = pp2;
        }
        free(p1);            /*释放开发商主链结点的动态存储区*/
        p1 = pp1;
    }
	ClearScreen();     /*清除屏幕信息*/
    /*释放存放菜单条、状态条等信息动态存储区*/
    free(gp_buff_menubar_info);
    /*关闭标准输入和输出设备句柄*/
    CloseHandle(gh_std_out);
    CloseHandle(gh_std_in);
    return;
}

/**< 文本菜单界面.*/
void TextInterface(void)
{
    COORD size = {SCR_COL, SCR_ROW};  /*窗口缓冲区大小.列:80, 行:25 */

    gh_std_out = GetStdHandle(STD_OUTPUT_HANDLE);  /* 获取标准输出设备句柄*/
    gh_std_in  = GetStdHandle(STD_INPUT_HANDLE);   /* 获取标准输入设备句柄*/

    SetConsoleTitle("【房产销售信息管理系统】"); /*设置窗口标题*/
    SetConsoleScreenBufferSize(gh_std_out, size); /*设置窗口缓冲区大小80*25*/
    ClearScreen(); /* 清屏*/

    /*创建弹出窗口信息堆栈，将初始化后的屏幕窗口当作第一层弹出窗口*/
    gp_scr_att = (char *)calloc(SCR_COL * SCR_ROW, sizeof(char));/*屏幕字符属性*/
    gp_top_layer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));/*动态创建gp_top_layer结点*/
    gp_top_layer->LayerNo = 0;      /*弹出窗口的层号为0*/
    gp_top_layer->rcArea.Left = 0;  /*弹出窗口的区域为整个屏幕窗口*/
    gp_top_layer->rcArea.Top = 0;   /*弹出窗口区域位置初始化*/
    gp_top_layer->rcArea.Right = SCR_COL - 1;
    gp_top_layer->rcArea.Bottom = SCR_ROW - 1;
    gp_top_layer->pContent = NULL;  /*弹出窗口区域字符单元原信息存储缓冲区初始值为NULL*/
    gp_top_layer->pScrAtt = gp_scr_att; /*gp_scr_att指向弹出窗口区域字符单元原属性值存储缓冲区信息*/
    gp_top_layer->next = NULL; /*指向的下一层窗口信息为NULL*/

    ShowMenu();   /*显示主菜单栏*/
    return ;
}

/**< 在屏幕上显示主菜单栏.*/
void ShowMenu(void)
{
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    CONSOLE_CURSOR_INFO lpCur;
    COORD size;
    COORD pos = {0, 0},pos1={1,0};
	SMALL_RECT rcMenu;
    int i;
    int PosA = 2;
    char ch;
    GetConsoleScreenBufferInfo( gh_std_out, &bInfo );  /*获取屏幕缓冲区信息，存入bInfo中*/
    size.X = bInfo.dwSize.X;
    size.Y = 1;
    SetConsoleCursorPosition(gh_std_out, pos); /* 光标设置到；(0,0) */
    for (i=0; i < 5; i++) /*在窗口第一行处输出主菜单项*/
        printf("【%s】", ga_main_menu[i]);
    /*申请动态存储区作为存放菜单条屏幕区字符信息的缓冲区*/
    gp_buff_menubar_info = (CHAR_INFO *)malloc(size.X * size.Y * sizeof(CHAR_INFO));
    /*设置缓冲区范围*/
	rcMenu.Left=0;
	rcMenu.Top=0;
	rcMenu.Right=size.X-1;
	rcMenu.Bottom=0;
    /*将窗口第一行的内容读入到存放菜单条屏幕区字符信息的缓冲区中*/
    ReadConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
    /*将这一行中英文字母置为红色，其他字符单元置为白底黑字*/
    for (i=0; i<size.X; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN| BACKGROUND_RED;/*白底黑字*/
        ch = (char)((gp_buff_menubar_info+i)->Char.AsciiChar);
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
            (gp_buff_menubar_info+i)->Attributes |= FOREGROUND_RED|FOREGROUND_INTENSITY;  /*英文字母为亮红色*/
    }
    /*修改后的菜单条字符信息回写到窗口的第一行*/
    WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
    MarkMainMenu(gi_sel_menu);  /*在选中主菜单项上做标记，gi_sel_menu初值为1*/
	printf("\n");
    return;
}

/**< 在指定主菜单项（num）上做选中标志.*/
void MarkMainMenu(int num)
{
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD size;
    COORD pos = {0, 0};
    int PosA = 2, PosB;
    SMALL_RECT rcMenu ;
    char ch;
    int i;
    if (num == 0)  /*num为0时，将会去除主菜单项选中标记*/
    {
        PosA = 0;
        PosB = 0;
    }
    else  /*否则，定位选中主菜单项的起止位置: PosA为起始位置, PosB为截止位置*/
    {
        for (i=1; i<num; i++)
             PosA += strlen(ga_main_menu[i-1]) + 4;
        PosB = PosA + strlen(ga_main_menu[num-1]);
    }
    GetConsoleScreenBufferInfo( gh_std_out, &bInfo ); /*获取屏幕缓冲区信息，存入bInfo中*/
    size.X = bInfo.dwSize.X;
    size.Y = 1;
    /*去除选中菜单项前面的菜单项选中标记*/
    for (i=0; i<PosA; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN| BACKGROUND_RED;/*白底黑字*/
        ch = (gp_buff_menubar_info+i)->Char.AsciiChar;
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
            (gp_buff_menubar_info+i)->Attributes |= FOREGROUND_RED|FOREGROUND_INTENSITY;/*字母为亮红色*/
    }
    /*在选中菜单项上做标记，亮青色底黑字*/
    for (i=PosA; i<PosB; i++)
        (gp_buff_menubar_info+i)->Attributes =BACKGROUND_GREEN|BACKGROUND_BLUE|BACKGROUND_INTENSITY;
    /*去除选中菜单项后面的菜单项选中标记*/
    for (i=PosB; i<bInfo.dwSize.X; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;/*白底黑字*/
        ch = (char)((gp_buff_menubar_info+i)->Char.AsciiChar);
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
            (gp_buff_menubar_info+i)->Attributes |= FOREGROUND_RED|FOREGROUND_INTENSITY;/*字母为亮红色*/
    }
    /*设置屏幕缓冲区范围大小*/
	rcMenu.Left=0;
	rcMenu.Top=0;
	rcMenu.Right=size.X-1;
	rcMenu.Bottom=0;
    /*将做好标记的菜单条信息写到窗口第一行*/
    WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
    return;
}

/**< 清除屏幕上的信息.*/
void ClearScreen(void)
{
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD home = {0, 1};
    unsigned long size;
    GetConsoleScreenBufferInfo( gh_std_out, &bInfo );/*取屏幕缓冲区信息*/
    size = bInfo.dwSize.X * bInfo.dwSize.Y;     /*计算屏幕缓冲区字符单元数*/
    /*将屏幕缓冲区所有单元的字符属性设置为当前屏幕缓冲区字符属性*/
    FillConsoleOutputAttribute(gh_std_out, bInfo.wAttributes, size, home, &len);
    /*将屏幕缓冲区所有单元填充为空格字符*/
    FillConsoleOutputCharacter(gh_std_out, ' ', size, home, &len);
	SetConsoleCursorPosition(gh_std_out,home);  /*将光标位置回放在{0,1}处*/
}

/**< 弹出指定编号为num的主菜单项对应的子菜单.*/
void PopMenu(int num)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT rcPop,*aArea;
    COORD pos,*aLoc;
    WORD att;
    int i, j, loc = 0;
	char *aSort, *aTag;/*aSort:热区类别 aTag:热区编号*/
	char *pch;
    if (num != gi_sel_menu)    /*如果指定主菜单不是已选中菜单*/
    {
        if (gp_top_layer->LayerNo != 0) /*如果此前已有子菜单弹出*/
        {
            PopOff();/*关闭该弹出窗口*/
            gi_sel_sub_menu = 0;/*将被选中的子菜单项号置为0*/
        }
    }
    else if (gp_top_layer->LayerNo != 0) /*若已弹出该子菜单，则返回*/
        return;
    gi_sel_menu = num;               /*将选中主菜单项置为指定的主菜单项*/
    MarkMainMenu(gi_sel_menu);        /*在选中的主菜单项上做标记*/
    LocateSubMenu(gi_sel_menu, &rcPop); /*计算弹出子菜单的区域位置, 存放在rcPop中*/
    /*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
    for (i=1; i<gi_sel_menu; i++)
        loc += ga_sub_menu_count[i-1];
    /*将该组子菜单项项名存入标签束结构变量*/
    labels.ppLabel = ga_sub_menu + loc;   /*标签束第一个标签字符串的地址*/
    labels.num = ga_sub_menu_count[gi_sel_menu-1]; /*标签束中标签字符串的个数*/
    /*定义一个坐标数组，存放每个标签字符串输出位置的坐标等价于:COORD aLoc[labels.num];*/
	aLoc=(COORD *)malloc(labels.num*sizeof(COORD));
    for (i=0; i<labels.num; i++) /*确定标签字符串的输出位置，存放在坐标数组中*/
    {
        aLoc[i].X = rcPop.Left + 2;
        aLoc[i].Y = rcPop.Top + i + 1;
    }
    labels.pLoc = aLoc; /*使标签束结构变量labels的成员pLoc指向坐标数组的首元素*/
    /*设置子菜单热区信息*/
    areas.num = labels.num;       /*热区的个数，等于标签的个数，即子菜单的项数*/
	aArea=(SMALL_RECT *)malloc(areas.num*sizeof(SMALL_RECT));/*定义数组存放所有热区位置等价于:SMALL_RECT aArea[areas.num];*/
	aSort=(char *)malloc(areas.num*sizeof(char));    /*定义数组存放所有热区对应类别等价于:char aSort[areas.num];*/
	aTag=(char *)malloc(areas.num*sizeof(char));    /*定义数组存放每个热区的编号:char aTag[areas.num];*/
    /*热区定位*/
	for (i=0; i<areas.num; i++)
    {/*各热区范围*/
        aArea[i].Left = rcPop.Left + 2;
        aArea[i].Top = rcPop.Top + i + 1;
        aArea[i].Right = rcPop.Right - 2;
        aArea[i].Bottom = aArea[i].Top;
        aSort[i] = 0;     /*热区类别都为0(按钮型)*/
        aTag[i] = i + 1; /*热区按顺序编号1,2,3,4...*/
    }
    areas.pArea = aArea;  /*使热区结构变量areas的成员pArea指向热区位置数组首元素*/
    areas.pSort = aSort;  /*使热区结构变量areas的成员pSort指向热区类别数组首元素*/
    areas.pTag = aTag;    /*使热区结构变量areas的成员pTag指向热区编号数组首元素*/
    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*子菜单字符属性:白底黑字*/
	PopOn(&rcPop, att, &labels, &areas);
    DrawBox(&rcPop);       /*给弹出窗口画边框*/
    pos.X = rcPop.Left + 2;
    for (pos.Y=rcPop.Top+1; pos.Y<rcPop.Bottom; pos.Y++)
    { /*此循环用来在空串子菜项位置画线形成分隔，并取消此菜单项的热区属性*/
        pch = ga_sub_menu[loc+pos.Y-rcPop.Top-1];
        if (strlen(pch)==0) /*串长为0，表明为空串*/
        {   /*首先画横线*/
            FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-3,pos, &len);
            for (j=rcPop.Left+2; j<rcPop.Right-1; j++)
            {   /*取消该区域字符单元的热区属性*/
                gp_scr_att[pos.Y*SCR_COL+j] &= 3; /*按位与的结果保留了低两位*/
            }
        }
    }
    /*将子菜单项的功能键设为白底红字*/
    pos.X = rcPop.Left + 3;
    att =  FOREGROUND_GREEN |FOREGROUND_INTENSITY;
    for (pos.Y=rcPop.Top+1; pos.Y<rcPop.Bottom; pos.Y++)
    {
        if (strlen(ga_sub_menu[loc+pos.Y-rcPop.Top-1])==0)
        {
            continue;  /*跳过空串*/
        }
        FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &len);
    }
    return;
}

/**< 在指定区域输出弹出窗口信息, 同时设置子菜单热区, 将弹出窗口位置信息存储入栈.*/
void PopOn(SMALL_RECT *pRc, WORD att, LABEL_BUNDLE *pLabel, HOT_AREA *pHotArea)
{
    LAYER_NODE *nextLayer;  /*弹出窗口信息结构变量指针*/
    COORD size;
    COORD pos = {0, 0};
    char  *pCh;
    int i, j, row;
    /*弹出窗口所在位置字符单元信息入栈*/
    size.X = pRc->Right - pRc->Left + 1;    /*弹出窗口的宽度*/
    size.Y = pRc->Bottom - pRc->Top + 1;    /*弹出窗口的高度*/
    /*申请存放弹出窗口相关信息的动态存储区*/
    nextLayer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
    nextLayer->next = gp_top_layer;
    nextLayer->LayerNo = gp_top_layer->LayerNo + 1;
    nextLayer->rcArea = *pRc;
    nextLayer->pContent = (CHAR_INFO *)malloc(size.X * size.Y * sizeof(CHAR_INFO));
    nextLayer->pScrAtt = (char *)malloc(size.X * size.Y * sizeof(char));
    pCh = nextLayer->pScrAtt;
    /*将弹出窗口覆盖区域的字符信息保存，用于在关闭弹出窗口时恢复原样*/
    ReadConsoleOutput(gh_std_out, nextLayer->pContent, size, pos, pRc);
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {   /*此二重循环将所覆盖字符单元的原先属性值存入动态存储区，便于以后恢复*/
        for (j=pRc->Left; j<=pRc->Right; j++)
        {
            *pCh = gp_scr_att[i*SCR_COL+j];  /*将pCh指向的内容放入gp_scr_att指向的字符单元属性值的缓冲区*/
            pCh++;
        }
    }
    gp_top_layer = nextLayer;  /*完成弹出窗口相关信息入栈操作,仍将gp_top_layer设置为弹出窗口信息链链头*/
    /*设置弹出窗口区域字符的新属性*/
    pos.X = pRc->Left;
    pos.Y = pRc->Top;
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {
        FillConsoleOutputAttribute(gh_std_out, att, size.X, pos, &len);
        pos.Y++;
    }
    /*将标签束中的标签字符串在设定的位置输出*/
    for (i=0; i<pLabel->num; i++)
    {
        pCh = pLabel->ppLabel[i];
        if (strlen(pCh) != 0)
            WriteConsoleOutputCharacter(gh_std_out, pCh, strlen(pCh),pLabel->pLoc[i], &len);
    }
    /*设置弹出窗口区域字符单元的新属性*/
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {   /*此二重循环设置字符单元的层号*/
        for (j=pRc->Left; j<=pRc->Right; j++)
            gp_scr_att[i*SCR_COL+j] = gp_top_layer->LayerNo;
    }
    /*此二重循环设置所有热区中字符单元的热区类型和热区编号*/
    for (i=0; i<pHotArea->num; i++)
    {
        row = pHotArea->pArea[i].Top;
        for (j=pHotArea->pArea[i].Left; j<=pHotArea->pArea[i].Right; j++)
            gp_scr_att[row*SCR_COL+j] |= (pHotArea->pSort[i] << 6)| (pHotArea->pTag[i] << 2);
    }
    return;
}

/**< 关闭弹出窗口,恢复覆盖区域原外观和字符单元原属性.*/
void PopOff(void)
{
    LAYER_NODE *nextLayer;
    COORD size;
    COORD pos = {0, 0};
    char *pCh;
    int i, j;
    /*栈底存放的主界面屏幕信息，不用关闭*/
    if ((gp_top_layer->next==NULL) || (gp_top_layer->pContent==NULL))
        return;
    nextLayer = gp_top_layer->next;
    /*恢复弹出窗口区域原外观*/
    size.X = gp_top_layer->rcArea.Right - gp_top_layer->rcArea.Left + 1;
    size.Y = gp_top_layer->rcArea.Bottom - gp_top_layer->rcArea.Top + 1;
    WriteConsoleOutput(gh_std_out, gp_top_layer->pContent, size, pos, &(gp_top_layer->rcArea));
    /*恢复字符单元原属性*/
    pCh = gp_top_layer->pScrAtt;
    for (i=gp_top_layer->rcArea.Top; i<=gp_top_layer->rcArea.Bottom; i++)
    {
        for (j=gp_top_layer->rcArea.Left; j<=gp_top_layer->rcArea.Right; j++)
        {
            gp_scr_att[i*SCR_COL+j] = *pCh;
            pCh++;
        }
    }
    /*释放动态存储区*/
    free(gp_top_layer->pContent);
    free(gp_top_layer->pScrAtt);
    free(gp_top_layer);
    gp_top_layer = nextLayer;
    gi_sel_sub_menu = 0;
    return;
}

/**< 给弹出窗口绘制边框.*/
void DrawBox(SMALL_RECT *pRc)
{
    char chBox[] = {'+','-','|','+','+','+'};  /*画框用的字符*/
    COORD pos = {pRc->Left, pRc->Top};  /*定位在区域的左上角*/
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &len);  /*画边框左上角#*/
    /*此循环画上边框横线*/
	for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
        WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &len);
    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[3], 1, pos, &len);   /*画边框右上角~*/
    /*此循环画边框左边线和右边线*/
	for (pos.Y = pRc->Top+1; pos.Y < pRc->Bottom; pos.Y++)
    {
        pos.X = pRc->Left;
        WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &len);
        pos.X = pRc->Right;
        WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &len);
    }
    pos.X = pRc->Left;
    pos.Y = pRc->Bottom;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[4], 1, pos, &len);   /*画边框左下角&*/
    /*画下边框横线*/
	for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
        WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &len);
    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[5], 1, pos, &len);   /*画边框右下角%*/
    return;
}

/**< 在指定子菜单项(标号为num)上做选中标记.*/
void MarkSubMenu(int num)
{
    SMALL_RECT rcPop;
    COORD pos;
    WORD  att;
    int   width;
    LocateSubMenu(gi_sel_menu, &rcPop);/*计算弹出子菜单的区域位置, 存放在rcPop中*/
    /*如果子菜单项号越界，或该项子菜单已被选中，则返回*/
    if ((num < 1) || (num == gi_sel_sub_menu) || (num > rcPop.Bottom-rcPop.Top-1))
		return;
    pos.X = rcPop.Left + 2;
    width = rcPop.Right - rcPop.Left - 2;
    /*首先取消原选中子菜单项上的标记*/
    if (gi_sel_sub_menu != 0)
    {
        pos.Y = rcPop.Top + gi_sel_sub_menu;
        att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
        FillConsoleOutputAttribute(gh_std_out, att, width, pos, &len);
        pos.X += 1;
        att |=  FOREGROUND_RED|FOREGROUND_INTENSITY;     /*白底亮红字*/
        FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &len);
    }
    /*在子菜单项上做选中标记(亮红底亮白字)*/
    pos.X = rcPop.Left + 2;
    pos.Y = rcPop.Top + num;
    att = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED|FOREGROUND_INTENSITY| BACKGROUND_BLUE| BACKGROUND_RED| BACKGROUND_INTENSITY;/*亮红底亮白字*/
    FillConsoleOutputAttribute(gh_std_out, att, width, pos, &len);
    gi_sel_sub_menu = num;  /*修改选中子菜单项号*/
    return;
}

/**< 计算弹出子菜单(标号为num)区域左上角和右下角的位置.信息存储在类型为SMALL_RECT结构的变量rc中*/
void LocateSubMenu(int num, SMALL_RECT *rc)
{
    int i, len, loc = 0;
    /*区域的上边定在第2行，行号为1*/
    rc->Top = 1;
    rc->Left = 1;
    /*计算区域左边界位置, 同时计算第一个子菜单项在子菜单字符串数组中的位置*/
    for (i=1; i<num; i++)
    {
        rc->Left += strlen(ga_main_menu[i-1]) + 4;
        loc += ga_sub_menu_count[i-1];
    }
    rc->Right = strlen(ga_sub_menu[loc]);    /*暂时存放第一个子菜单项字符串长度*/
    /*查找最长子菜单字符串，将其长度存放在rc->Right*/
    for (i=1; i<ga_sub_menu_count[num-1]; i++)
    {
        len = strlen(ga_sub_menu[loc+i]);
        if (rc->Right < len)    rc->Right = len;
    }
    rc->Right += rc->Left + 3;  /*计算区域的右边界*/
    rc->Bottom = rc->Top + ga_sub_menu_count[num-1] + 1;/*计算区域下边的行号*/
    /*右边界越界的处理*/
    if (rc->Right >= SCR_COL)
    {
        len = rc->Right - SCR_COL + 1;
        rc->Left -= len;
        rc->Right = SCR_COL - 1;
    }
    return;
}

/**< 给主菜单栏设置热区.*/
void SetMainHotArea(void)
{
	int i,j;
	char aSort[5] = {0};
	char aTag[5] = {1,2,3,4,5};
	HOT_AREA areas;
	SMALL_RECT aArea[5] = {{2,0,8,0}, {12,0,20,0}, {24,0,30,0}, {34,0,40,0}, {44,0,54,0}};/*设置主菜单的热区范围*/
    areas.pArea = aArea;   /*使热区结构变量areas的成员pArea指向热区位置数组首元素*/
    areas.pSort = aSort;   /*使热区结构变量areas的成员pSort指向热区类别数组首元素*/
    areas.pTag =  aTag;    /*使热区结构变量areas的成员pTag 指向热区编号数组首元素*/
    /*设置主菜单的热区中字符单元的热区类型和热区编号*/
	for (i=0;i<5;i++)
	{
		for (j = areas.pArea[i].Left;j <= areas.pArea[i].Right;j++)
			gp_scr_att[j] |= (areas.pSort[i] << 6 )| (areas.pTag[i] << 2);
	}
}

/**< 运行系统,在系统界面下运行用户所选择的功能函数*/
void RunSystem(void)
{
    INPUT_RECORD inRec;
    DWORD res;
    COORD pos = {0, 0};
    BOOL bRet = TRUE;
    int i, loc, num, p;
    int cNo, cAtt;      /*cNo:字符单元层号, cAtt:字符单元属性*/
    char vkc, asc;      /*vkc:虚拟键代码, asc:字符的ASCII码值*/
    SetMainHotArea();   /*给主菜单栏设置热区*/
    while(bRet)
    {  /*从控制台输入缓冲区中读一条记录*/
        ReadConsoleInput(gh_std_in, &inRec, 1, &res);
        if (inRec.EventType == MOUSE_EVENT) /*如果记录由鼠标事件产生*/
        {
            pos = inRec.Event.MouseEvent.dwMousePosition;    /*获取鼠标坐标位置*/
            cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3;   /*取该位置的层号*/
            cAtt = gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2; /*取该字符单元属性*/
            if (cNo == 0)  /*层号为0,表明该位置未被弹出子菜单覆盖*/
            {
        /*cAtt>0表明该位置处于热区,cAtt!=gi_sel_menu表明该位置的主菜单项未被选中,gp_top_layer->LayerNo>0表明当前有子菜单弹出*/
               if (cAtt > 0 && cAtt != gi_sel_menu && gp_top_layer->LayerNo > 0)
                {
                    PopOff();            /*关闭弹出的子菜单*/
                    gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
                }
				else  if(cAtt>0) MarkMainMenu(cAtt); /*在鼠标所在主菜单项上做选中标记*/
            }
            else if (cAtt > 0) /*鼠标所在位置为弹出子菜单的菜单项字符单元*/
                MarkSubMenu(cAtt); /*在该子菜单项上做选中标记*/
            if (inRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) /*如果按下鼠标左键一次*/
            {
                if (cNo==0) /*层号为0，表明该位置未被弹出子菜单覆盖*/
                {
					if (cAtt > 0) /*如果该位置处于热区*/
                        PopMenu(cAtt);   /*弹出鼠标所在主菜单项对应的子菜单*/
                    else if (gp_top_layer->LayerNo > 0) /*如果该位置不属于主菜单项字符单元,且有子菜单弹出*/
                    {
                        PopOff();             /*关闭弹出的子菜单*/
                        gi_sel_sub_menu = 0;  /*将选中子菜单项的项号置为0*/
                    }
                }
                else /*层号不为0,表明该位置被弹出子菜单覆盖*/
                {
                    if (cAtt > 0) /*如果该位置处于热区*/
                    {
                        PopOff();  /*关闭弹出的子菜单*/
                        gi_sel_sub_menu = 0;  /*将选中子菜单项的项号置为0*/
                        bRet = ExeFunction(gi_sel_menu, cAtt); /*执行对应功能函数:gi_sel_menu主菜单项号,cAtt子菜单项号*/
                    }
                }
            }
            else if (inRec.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) /*如果按下鼠标右键*/
            {
                if (cNo == 0) /*层号为0，表明该位置未被弹出子菜单覆盖*/
                {
                    PopOff();            /*关闭弹出的子菜单*/
                    gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
                }
            }
        }
        else if (inRec.EventType == KEY_EVENT && inRec.Event.KeyEvent.bKeyDown) /*如果记录由按键产生且键被按下*/
        {
            vkc = inRec.Event.KeyEvent.wVirtualKeyCode;  /*获取按键的虚拟键码*/
            asc = inRec.Event.KeyEvent.uChar.AsciiChar;  /*获取按键的ASC码*/
            /*系统帮助快捷键F1键的处理*/
            if (vkc == 112) /*如果按下F1键*/
            {
                if (gp_top_layer->LayerNo != 0) /*如果当前有子菜单弹出*/
                {
                    PopOff();                   /*关闭弹出的子菜单*/
                    gi_sel_sub_menu = 0;        /*将选中子菜单项的项号置为0*/
                }
                bRet = ExeFunction(5, 1);       /*运行帮助主题功能函数*/
            }
            else if (inRec.Event.KeyEvent.dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED))
            { /*如果按下左或右Alt键*/
                switch (vkc)  /*判断组合键Alt+字母*/
                {
                    case 88:  /*Alt+X 退出*/
                        if (gp_top_layer->LayerNo != 0)
                        {
                            PopOff();
                            gi_sel_sub_menu = 0;
                        }
                        bRet = ExeFunction(1,5);
                        break;
					case 67:  /*Alt+C 清屏*/
                        if (gp_top_layer->LayerNo != 0)
                        {
                            PopOff();
                            gi_sel_sub_menu = 0;
                        }
						bRet=ExeFunction(5,3);
						break;
                    case 70:  /*Alt+F*/
                        PopMenu(1);
                        break;
                    case 69: /*Alt+E*/
                        PopMenu(2);
                        break;
                    case 73: /*Alt+I*/
                        PopMenu(3);
                        break;
                    case 83: /*Alt+S*/
                        PopMenu(4);
                        break;
                    case 72: /*Alt+H*/
                        PopMenu(5);
                        break;
                }
            }
            else if (asc == 0) /*其他控制键的处理*/
            {
                if (gp_top_layer->LayerNo == 0) /*如果未弹出子菜单*/
                {
                    switch (vkc) /*处理方向键(左、右、下),不响应其他控制键*/
                    {
                        case 37:  /*左键*/
                            gi_sel_menu--;
                            if (gi_sel_menu == 0)  gi_sel_menu = 5;
                            MarkMainMenu(gi_sel_menu);
                            break;
                        case 39: /*右键*/
                            gi_sel_menu++;
                            if (gi_sel_menu == 6)  gi_sel_menu = 1;
                            MarkMainMenu(gi_sel_menu);
                            break;
                        case 40: /*下键*/
                            PopMenu(gi_sel_menu);
                            MarkSubMenu(1);
                            break;
                    }
                }
                else  /*已弹出子菜单时*/
                {
                    /*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
                    for (loc=0,i=1; i<gi_sel_menu; i++)
                        loc += ga_sub_menu_count[i-1];
                    switch (vkc) /*方向键(左、右、上、下)的处理*/
                    {
                        case 37: /*左键*/
                            gi_sel_menu--;
                            if (gi_sel_menu < 1)   gi_sel_menu = 5;
                            MarkMainMenu(gi_sel_menu);
                            PopOff();
                            PopMenu(gi_sel_menu);
                            MarkSubMenu(1);
                            break;
                        case 38: /*上键*/
                            num = gi_sel_sub_menu - 1;
                            if (num < 1)    num = ga_sub_menu_count[gi_sel_menu-1];
                            if (strlen(ga_sub_menu[loc+num-1]) == 0)    num--;
                            MarkSubMenu(num);
                            break;
                        case 39:  /*右键*/
                            gi_sel_menu++;
                            if (gi_sel_menu > 5)     gi_sel_menu = 1;
                            MarkMainMenu(gi_sel_menu);
                            PopOff();
                            PopMenu(gi_sel_menu);
                            MarkSubMenu(1);
                            break;
                        case 40: /*下键*/
                            num = gi_sel_sub_menu + 1;
                            if (num > ga_sub_menu_count[gi_sel_menu-1])  num = 1;
                            if (strlen(ga_sub_menu[loc+num-1]) == 0)     num++;
                            MarkSubMenu(num);
                            break;
                    }
                }
            }
            else if ((asc-vkc == 0) || (asc-vkc == 32)){  /*按下数字键或者字母键等普通键*/
                if (gp_top_layer->LayerNo == 0)  /*如果未弹出子菜单*/
                {
                    switch (vkc)
                    {
                        case 70: /*f或F*/
                            PopMenu(1);
                            break;
                        case 69: /*e或E*/
                            PopMenu(2);
                            break;
                        case 73: /*i或I*/
                            PopMenu(3);
                            break;
                        case 83: /*s或S*/
                            PopMenu(4);
                            break;
                        case 72: /*h或H*/
                            PopMenu(5);
                            break;
                        case 13: /*回车*/
                            PopMenu(gi_sel_menu);
                            MarkSubMenu(1);
                            break;
                    }
                }
                else /*已弹出子菜单时的键盘输入处理*/
                {
                    if (vkc == 27) /*如果按下ESC键*/
                    {
                        PopOff();
                        gi_sel_sub_menu = 0;
                    }
                    else if(vkc == 13) /*如果按下回车键*/
                    {
                        num = gi_sel_sub_menu;
                        PopOff();
                        gi_sel_sub_menu = 0;
                        bRet = ExeFunction(gi_sel_menu, num);
                    }
                    else /*其他普通键的处理*/
                    {
                        /*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
                        for (loc=0,i=1; i<gi_sel_menu; i++)
                             loc += ga_sub_menu_count[i-1];
                        /*依次与当前子菜单中每一项的代表字符进行比较*/
                        for (i=loc; i<loc+ga_sub_menu_count[gi_sel_menu-1]; i++)
                        {
                            if (strlen(ga_sub_menu[i])>0 && vkc==ga_sub_menu[i][1])
                            { /*如果匹配成功*/
                                PopOff();
                                gi_sel_sub_menu = 0;
                                bRet = ExeFunction(gi_sel_menu, i-loc+1); /*执行与字母相对应的函数*/
                            }
                        }
                    }
                }
            }
        }
    }
}

/**< 执行由主菜单号为m和子菜单号为n所确定的功能函数*/
BOOL ExeFunction(int m, int n)
{
    BOOL bRet = TRUE;
    /*函数指针数组，用来存放所有功能函数的入口地址*/
    BOOL (*pFunction[24])(void);
    int i, loc;
    /*将功能函数入口地址存入与功能函数所在主菜单号和子菜单号对应下标的数组元素*/
    pFunction[0] =  SaveData;
    pFunction[1] =  LoadData;
    pFunction[2] =  ExitSys;
    /*航班信息数据维护*/
    pFunction[3] =  InsertDeveloper;
    pFunction[4] =  ModifyDeveloper;
    pFunction[5] =  DelDeveloper;
    pFunction[6] =  NULL;
    /*经停机场信息9据维护*/
    pFunction[7] =  InsertFlat;
    pFunction[8] =  ModifyFlat;
    pFunction[9] = DelFlat;
    pFunction[10] =  NULL;
    /*经停航班信息数据维护*/
    pFunction[11] =  InsertSale;
    pFunction[12] =  ModifySale;
    pFunction[13] =  DelSale;
    /*信息查询*/
    pFunction[14] =  DeveloperInquire;
    pFunction[15] =  FlatInquire;
    pFunction[16] =  SaleInquire;
    pFunction[17] =  InquireAuto;
    /*信息统计*/
    pFunction[18] = DevStatistics;
    pFunction[19] = YearStatistics;
    pFunction[20] = SmanStatistics;
    pFunction[21] = AreaStatistics;
    pFunction[22] = AboutSys;
	pFunction[23] = Clear;
    /*根据主菜单号和子菜单号计算对应下标*/
    for (i=1,loc=0; i<m; i++)
        loc += ga_sub_menu_count[i-1];
    loc += n - 1;
    /*用函数指针调用所指向的功能函数*/
    if(pFunction[loc]!=NULL)  
	    bRet = (*pFunction[loc])();
    return bRet;
}

/**< 处理退出系统时输入的信息*/
int  DealQuitInput(HOT_AREA *pHotArea, int *piHot)
{
    INPUT_RECORD inRec;
    DWORD res;
    COORD pos = {0, 0};
    int num, arrow, iRet = 0;
    int cTag, cAtt;/*cNo:层号, cTag:热区编号, cAtt:字符单元属性*/
    char vkc, asc; /*vkc:虚拟键代码, asc:字符的ASCII码值*/
    SetQuitHotArea(pHotArea, 1);/*初始标记"确定"*/
    while (1)
    { /*循环*/
        ReadConsoleInput(gh_std_in, &inRec, 1, &res);
        if (inRec.EventType == MOUSE_EVENT)/*如果记录由鼠标事件产生*/
        {
            pos = inRec.Event.MouseEvent.dwMousePosition;/*获取鼠标坐标位置*/
            cTag = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2) & 15;/*获取该位置的热区编号*/
            cAtt =  gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2; /*获取该位置字符单元的属性*/
            if ( cAtt>0 )/*r如果鼠标所在位置处于热区*/
            {
                *piHot = cTag;
                SetQuitHotArea(pHotArea, *piHot);/*跟随鼠标显示标记*/
            }
            if (inRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && cAtt>0 )
            { /*如果按下鼠标左键一下且此时该位置处于热区*/
                iRet = 1;
                break;
            }
        }
        else if (inRec.EventType == KEY_EVENT && inRec.Event.KeyEvent.bKeyDown)
        { /*如果记录由按键产生*/
            vkc = inRec.Event.KeyEvent.wVirtualKeyCode;/*获取按键的虚拟键码*/
            asc = inRec.Event.KeyEvent.uChar.AsciiChar;/*获取按键的ASC码*/
            if (asc == 0)
            {
                arrow = 0;
                switch (vkc)
                {  /*方向键(左、上、右、下)的处理*/
                    case 37: arrow = 1; break;
                    case 38: arrow = 1; break;
                    case 39: arrow = 2; break;
                    case 40: arrow = 2; break;
                }
                if (arrow > 0)
                {
                    num = *piHot;
                    while (1)
                    {
                        if (arrow == 1) num--;
                        else  num++;
                        break;
                    }
                    if (num >= 1 && num <= 2)
                    {
                        *piHot = num;
                        SetQuitHotArea(pHotArea, *piHot);
                    }
                }
            }
            else if (vkc == 27 || vkc == 78)
            {  /*ESC键和N,n键 == 取消*/
                iRet = 0;
                break;
            }
            else if (vkc == 13 || vkc == 89)
            {  /*回车键和Y,y键 == 确定*/
                iRet = 1;
                break;
            }
        }
    }
    return iRet;
}

/**< 设置退出系统时的热区*/
void SetQuitHotArea(HOT_AREA *pHotArea, int iHot)
{
    CONSOLE_CURSOR_INFO lpCur;
    COORD pos = {0, 0};
    WORD att1, att2;
    int i, width;
    att1 = BACKGROUND_GREEN | BACKGROUND_INTENSITY;  /*亮青底白字(att1表示选中选项的字符属性)*/
    att2 = BACKGROUND_BLUE  | BACKGROUND_GREEN | BACKGROUND_RED;/*白底黑字att2(表示未选中选项的字符属性)*/
    for (i=0; i<pHotArea->num; i++)
    {  /*将按钮类热区置为白底黑字*/
        pos.X = pHotArea->pArea[i].Left;
        pos.Y = pHotArea->pArea[i].Top;
        width = pHotArea->pArea[i].Right - pHotArea->pArea[i].Left + 4;
        if (pHotArea->pSort[i] == 0)  /*热区是按钮类*/
            FillConsoleOutputAttribute(gh_std_out, att2, width, pos, &len);
    }
    pos.X = pHotArea->pArea[iHot-1].Left;
    pos.Y = pHotArea->pArea[iHot-1].Top;
    width = pHotArea->pArea[iHot-1].Right - pHotArea->pArea[iHot-1].Left + 4;
    if (pHotArea->pSort[iHot-1] == 0) /*被激活热区是按钮类*/
        FillConsoleOutputAttribute(gh_std_out, att1, width, pos, &len);
}

/**< 将链表的内容存盘*/
BOOL SaveData(void)
{
	BOOL bRet = TRUE;
	ClearScreen();
	LoadList(&gp_head,FileName);
	SaveList(gp_head, FileName);
    return bRet;
}

/**< 将链表的内容存盘*/
void SaveList(DEVELOPER_NODE *phead,char *fn[3])
{
	FILE *fout1,*fout2,*fout3;
	DEVELOPER_NODE *p1=phead;
	FLAT_NODE *p2;
	SALE_NODE *p3;
	if ((fout1=fopen(fn[0],"wb")) == NULL)
		exit(-1);
	if ((fout2=fopen(fn[1],"wb")) == NULL)
		exit(-1);
	if ((fout3=fopen(fn[2],"wb")) == NULL)
		exit(-1);
	while (p1 != NULL)
	{
		fwrite(p1,sizeof(DEVELOPER_NODE),1,fout1);
		p2 = p1->fnext;
		while (p2 != NULL)
		{
			fwrite(p2,sizeof(FLAT_NODE),1,fout2);
			p3=p2->snext;
			while (p3!=NULL)
			{
				fwrite(p3,sizeof(SALE_NODE),1,fout3);
				p3=p3->next;
			}
			p2=p2->next;
		}
		p1=p1->next;
	}
	fclose(fout1);
	fclose(fout2);
	fclose(fout3);
	printf("[相关数据已保存!]\n");
}

/**< 将数据加载*/
BOOL LoadData(void)
{
    BOOL bRet=TRUE;
    ClearScreen();
    LoadList(&gp_head,FileName);   /*加载链表信息*/
    printf("加载成功！\n");
    return bRet;
}

/**< 加载文件中的信息。*/
BOOL LoadList(DEVELOPER_NODE  **phead,char *fn[3])
{
	DEVELOPER_NODE *p1,input_info1, *pre_p1=NULL;
	FLAT_NODE *p2, input_info2;
	SALE_NODE *p3, input_info3;
	FILE *pfile1, *pfile2, *pfile3;
	
    if ((pfile1=fopen(fn[0],"rb")) == NULL)
    {
        printf("信息打开失败\n");
        return 0;
    }
    printf("信息打开成功\n");
    
    while (fread(&input_info1,sizeof(DEVELOPER_NODE),1,pfile1)==1)
    {
    	p1=(DEVELOPER_NODE *)calloc(1,sizeof(DEVELOPER_NODE));
    	*p1=input_info1;
    	p1->fnext=NULL;
    	p1->next=pre_p1;
    	pre_p1=p1;
	}	
	*phead=pre_p1;
	fclose(pfile1);
	if (*phead == NULL)
    { 
        printf("开发商信息加载失败\n");
        return 0;
    }
    printf("开发商信息加载成功\n");
    if ((pfile2=fopen(fn[1],"rb")) == NULL)
    {
        printf("楼盘信息加载失败\n");
        return 0;
    }
    printf("楼盘信息加载成功\n");
    
	while (fread(&input_info2,sizeof(FLAT_NODE),1,pfile2) == 1)
    {
    	p2=(FLAT_NODE *)calloc(1,sizeof(FLAT_NODE));
    	*p2=input_info2;
    	p2->snext=NULL;
    	p1=*phead;
    	while (p1 != NULL)
    	{
    		if (strcmp(p1->name,p2->name_of_dev)==0)
    		{
    			p2->next=p1->fnext;
    			p1->fnext=p2;
    			break;
			}
			p1=p1->next;
		}
	}
	fclose(pfile2);
	if ((pfile3=fopen(fn[2],"rb")) == NULL)
    {
        printf("销售信息打开失败\n");
        return 0;
    }
    printf("销售信息打开成功\n");
    while (fread(&input_info3,sizeof(SALE_NODE),1,pfile3)==1)
    {
    	p3=(SALE_NODE *)calloc(1,sizeof(SALE_NODE));
    	*p3=input_info3;
    	
    	p1=*phead;
    	while (p1!=NULL)
    	{
    		p2=p1->fnext;
    		while (p2!=NULL)
    		{
    			if (strcmp(p2->name,p3->name_of_flat)==0)
    			{
	    			p3->next=p2->snext;
	    			p2->snext=p3;
	    			break;
				}
				p2=p2->next;
			}
			p1=p1->next;
		}
	}
	fclose(pfile3);
	return TRUE;
}

/**< 退出系统*/
BOOL ExitSys(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop,aArea[2];
    COORD pos,aLoc[2];
    WORD att;
    char *pCh[] = {" 【确认退出系统？】", "确定(Y)  取消(N)"};
    int iHot = 1;
    char ch , aSort[] = {0, 0}, aTag[] = {1, 2};
	ClearScreen();/*先清除屏幕*/
    pos.X = strlen(pCh[0]) +7;
    pos.Y = 8;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;
    att = FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;/*白底亮红字*/
    labels.num = 2;
    labels.ppLabel = pCh;
    /*设置按钮位置坐标*/
    aLoc[0].X = rcPop.Left+3;
    aLoc[0].Y = rcPop.Top+2;
    aLoc[1].X = rcPop.Left+5;
    aLoc[1].Y = rcPop.Top+5;
    labels.pLoc = aLoc;
    areas.num = 2;
    /*设置热区范围*/
    aArea[0].Left  = rcPop.Left + 5;
    aArea[0].Top   = rcPop.Top + 5;
    aArea[0].Right = rcPop.Left + 8;
    aArea[0].Bottom= rcPop.Top + 5;
    aArea[1].Left  = rcPop.Left + 14;
    aArea[1].Top   = rcPop.Top + 5;
    aArea[1].Right = rcPop.Left + 17;
    aArea[1].Bottom= rcPop.Top + 5;
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopOn(&rcPop, att, &labels, &areas);/*在指定区域输出弹出窗口信息并将信息入栈,设置热键*/
    /*画一条分隔线*/
    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top + 4;
    FillConsoleOutputCharacter(gh_std_out, '*', rcPop.Right-rcPop.Left-1, pos, &len);
    if (DealQuitInput(&areas, &iHot) == 1 && iHot == 1)
        bRet = FALSE;
    else  bRet = TRUE;
    PopOff();/*关闭弹出窗口*/
    return bRet;
}

/**< 插入开发商信息*/
BOOL InsertDeveloper(void)
{
    BOOL bRet=TRUE;
    int iRet;
    ClearScreen();
    iRet=InsertDeveloperNode(&gp_head);
    if(iRet== 1)       
	    printf("开发商信息插入成功!\n");
    else if(iRet == -1)  
	    printf("此开发商已存在，插入失败！\n");
	SaveList(gp_head, FileName);
    return bRet;
}

/**< 插入开发商结点*/
int InsertDeveloperNode(DEVELOPER_NODE **phead)
{
    DEVELOPER_NODE *p1 ,*p=(*phead); 
    COORD pos={7,2};
    p1 = (DEVELOPER_NODE *)malloc(sizeof(DEVELOPER_NODE));
	printf("开发商名称 房屋总套数 建筑总面积[3项]\n");
	p1->fnext = NULL;
    scanf("%s",p1->name);  
	getchar();
	pos.X+=11;Gotoxy(pos);
    scanf("%d",&p1->num);   
	getchar();pos.X+=11;
	Gotoxy(pos);
    scanf("%f",&p1->area);  
	getchar();

    while (p != NULL && strcmp (p -> name, p1 -> name) != 0)
    { 
        p = p->next;
    }
    if(p == NULL)
    {
        p1 -> next = *phead;
        *phead = p1;
        return 1; 
    }
    else  
	return -1;
}

/**< 修改信息*/
BOOL ModifyDeveloper(void)
{
    DEVELOPER_NODE *p1;
    char findname[30];
    COORD pos={0,4};
    ClearScreen();

    printf("请输入所要修改开发商名称:");
    scanf("%s",findname); getchar();
    printf("请修改该开发商的信息！\n");

    p1 = (DEVELOPER_NODE *)malloc(sizeof(DEVELOPER_NODE));
	printf("开发商名称 房屋总套数 建筑总面积[3项]\n");
	p1->fnext=NULL;
    scanf("%s",p1->name);  getchar();pos.X+=11;Gotoxy(pos);
    scanf("%d",&p1->num);   getchar();pos.X+=11;Gotoxy(pos);
    scanf("%f",&p1->area);  getchar();
    if(ModifyDevNode(gp_head,findname,p1)==1) printf("修改成功!\n");
    else printf("该开发商不存在，修改失败!\n");
	SaveList(gp_head, FileName);/*保存信息*/
    return TRUE;
}

/**< 修改航班结点*/
int  ModifyDevNode(DEVELOPER_NODE *phead,char *dev_name,DEVELOPER_NODE *pdev_node)
{
    DEVELOPER_NODE *pdev_temp = phead;
    DEVELOPER_NODE *pdev_next;
    int   bRet=0;
    while (pdev_temp != NULL && strcmp(pdev_temp -> name,dev_name) != 0)
        pdev_temp = pdev_temp ->next;   /*寻找符合条件的航班信息结点*/
    if (pdev_temp != NULL)
    {
        pdev_next = pdev_temp ->next;
        *pdev_temp = *pdev_node; /*指针传址*/
        pdev_temp -> next = pdev_next;
        bRet= 1;
    }
    return bRet;
}

/**< 删除开发商信息*/
BOOL DelDeveloper(void)
{
    char dev_name[30];
    ClearScreen();/*刷屏*/

    printf("请输入要删除的开发商名称：");
    scanf("%s",dev_name); getchar();
    if(DelDevNode(&gp_head,dev_name) == 1) printf("删除成功!\n");
    else  printf("无耻开发商！\n");
	SaveList( gp_head, FileName);/*保存信息*/
    return TRUE;
}

/**< 删除开发商结点*/
int  DelDevNode(DEVELOPER_NODE **phead,char *dev_name)
{
    DEVELOPER_NODE *pdev_prior;
    DEVELOPER_NODE *pdev_current;
    FLAT_NODE *pfla_prior;
    FLAT_NODE *pfla_current;
    SALE_NODE *psal_prior;
    SALE_NODE *psal_current;
    int bRet = 0;

    for (pdev_current = (*phead), pdev_prior = NULL; pdev_current != NULL; 
	    pdev_prior=pdev_current,pdev_current = pdev_current ->next)
    {
        if (strcmp(pdev_current ->name,dev_name) == 0)
        {
            bRet = 1;
            break;
        }
    }
    if (pdev_current != NULL )
    {
        if (pdev_prior == NULL) /*如果释放的是航班链的链头结点*/
        {
            (*phead) = pdev_current -> next;
        }
        else
        {
            pdev_prior -> next = pdev_current -> next;
        }
        pfla_current = pdev_current -> fnext;
        free(pdev_current); /*释放要删除的开发商结点*/
        while (pfla_current != NULL)
        {
            psal_current = pfla_current->snext;
            pfla_prior = pfla_current;
            while (psal_current != NULL)
            {
                psal_prior =psal_current;
                psal_current = psal_current ->next;
                free(psal_prior);/*释放该开发商旗下楼盘各个销售节点信息*/ 
            }
            pfla_current = pfla_current ->next;
            free(pfla_prior);/*释放该楼盘旗下各个开发商结点信息*/
        }
    }
    return bRet;
}

/**< 插入楼盘信息*/
BOOL InsertFlat(void)
{
    BOOL bRet=TRUE;
    int iRet;
    ClearScreen();
    iRet=InsertflatNode(gp_head);
    if(iRet== 1)       
	    printf("楼盘信息插入成功!\n");
    else if (iRet == -1)  
	    printf("此楼盘已存在，插入失败！\n");
    else  
	    printf("该楼盘所属开发商未录入系统，插入失败！\n");
	SaveList(gp_head, FileName);
    return bRet;
}

/**< 插入楼盘结点*/
int InsertflatNode(DEVELOPER_NODE *phead)
{
    DEVELOPER_NODE *pdev_node;
    FLAT_NODE *pfla_temp,*p2;
    int find = 0;
    COORD pos={0,2};
    printf("楼盘名称 开发商 开盘日期 楼盘地址 占地面积 房屋套数 建筑面积 每平米均价 联系人 联系电话[10项]\n");
    p2=(FLAT_NODE *)malloc(sizeof(FLAT_NODE));
    p2->snext=NULL;
    scanf("%s",p2->name);getchar();pos.X+=9;Gotoxy(pos);
    scanf("%s",p2->name_of_dev);getchar();pos.X+=7;Gotoxy(pos);
    scanf("%s",p2->date_of_start);getchar();pos.X+=9;Gotoxy(pos);
    scanf("%s",p2->location);getchar();pos.X+=9;Gotoxy(pos);
    scanf("%f",&p2->floor_area);getchar();pos.X+=9;Gotoxy(pos);
    scanf("%d",&p2->num_of_building);getchar();pos.X+=9;Gotoxy(pos);
    scanf("%f",&p2->structure_area);getchar();pos.X+=9;Gotoxy(pos);
    scanf("%f",&p2->ave_price);getchar();pos.X+=11;Gotoxy(pos);
    scanf("%s",p2->person);getchar();pos.X+=7;Gotoxy(pos);
    scanf("%s",p2->tele_num);getchar();
    fflush(stdin);
    pfla_temp = SeekFlatNode(phead, p2 -> name);
    if (pfla_temp != NULL)
    {
        find = -1;  /*该楼盘已存在*/
    }
    else   /*所要插入的楼盘不存在,将其插入到对应的楼盘链下*/
    {
        for (pdev_node = phead; pdev_node != NULL; pdev_node = pdev_node ->next)
        {
            if (strcmp(pdev_node -> name, p2 -> name_of_dev) == 0)
            {
                find = 1;  
                break;
            }
        }
    }

    if (find) 
    {
        p2 -> next = pdev_node -> fnext ;
        pdev_node ->  fnext = p2 ;
    }
    else free(p2);
    return find;
}

FLAT_NODE *SeekFlatNode(DEVELOPER_NODE *phead, char *findname)
{
    DEVELOPER_NODE         *pdev_node = phead;
    FLAT_NODE   *pfla_node;
    int find = 0;

    while (pdev_node != NULL)
    {
        pfla_node = pdev_node ->fnext;
        while (pfla_node != NULL)
        {
            if (strcmp(pfla_node -> name,findname) == 0)
            {
                 find = 1;
                 break;/*退出第一层循环*/
            }
            pfla_node = pfla_node ->next;
        }
        if (find)   
		    break;/*退出第二层循环*/
        pdev_node = pdev_node ->next;
    }
    if (find)
        return pfla_node;
    else
        return NULL;
}

/**< 修改楼盘信息*/
BOOL ModifyFlat(void)
{
    BOOL bRet=TRUE;
    char findname[30];
    FLAT_NODE *p2;
    COORD pos={0,4};
    ClearScreen();

    printf("请输入要修改的楼盘：");
    scanf("%s",findname);
    getchar();
    printf("请重新输入楼盘信息.\n");
    printf("楼盘名称 开发商 开盘日期 楼盘地址 占地面积 房屋套数 建筑面积 每平米均价 联系人 联系电话[10项]\n");
    p2=(FLAT_NODE *)malloc(sizeof(FLAT_NODE));
    p2->snext=NULL;
    scanf("%s",p2->name);getchar();pos.X+=9;Gotoxy(pos);
    scanf("%s",p2->name_of_dev);getchar();pos.X+=7;Gotoxy(pos);
    scanf("%s",p2->date_of_start);getchar();pos.X+=9;Gotoxy(pos);
    scanf("%s",p2->location);getchar();pos.X+=9;Gotoxy(pos);
    scanf("%f",&p2->floor_area);getchar();pos.X+=9;Gotoxy(pos);
    scanf("%d",&p2->num_of_building);getchar();pos.X+=9;Gotoxy(pos);
    scanf("%f",&p2->structure_area);getchar();pos.X+=9;Gotoxy(pos);
    scanf("%f",&p2->ave_price);getchar();pos.X+=11;Gotoxy(pos);
    scanf("%s",p2->person);getchar();pos.X+=7;Gotoxy(pos);
    scanf("%s",p2->tele_num);getchar();
    if(ModifyFlatNode(gp_head,findname,p2) == 1) printf("修改成功！\n");
    else  { printf("未找到所属开发商，修改失败！\n"); free(p2); }
	SaveList(gp_head, FileName);
    return bRet;

}

/**< 修改经停机场结点*/
int ModifyFlatNode(DEVELOPER_NODE *phead,char *name,FLAT_NODE *pfla_node)
{
    FLAT_NODE *pfla_temp;
    FLAT_NODE *pfla_next;
    int bRet=0;

    pfla_temp = SeekFlatNode(phead,name);/*查找该楼盘*/
    if (pfla_temp != NULL)
    {
        pfla_next  = pfla_temp -> next;
        *pfla_temp = *pfla_node;
        pfla_temp -> next = pfla_next;
        bRet = 1;
    }
    return bRet;
}

/**< 删除经停机场信息*/
BOOL DelFlat(void)
{
    BOOL bRet=TRUE;
    char findname[30];
    ClearScreen();

    printf("请输入要删除的楼盘：");
    scanf("%s",findname); getchar();
    if (DelFlatNode(gp_head,findname) == 1)  
	    printf("删除成功!\n");
    else printf("未找到该楼盘，删除失败！\n");
	SaveList(gp_head, FileName);
    return bRet;
}

/**< 删除楼盘结点*/
int DelFlatNode(DEVELOPER_NODE *phead,char *fal_name)
{
    DEVELOPER_NODE       *pdev_node = phead;
    FLAT_NODE *pfla_prior;
    FLAT_NODE *pfla_current;
    SALE_NODE  *psal_prior;
    SALE_NODE  *psal_current;

    int bRet = 0;

    while (pdev_node != NULL)
    {
        pfla_current = pdev_node ->fnext;
        pfla_prior = NULL;
        while (pfla_current != NULL && strcmp(pfla_current ->name,fal_name))
        {
            pfla_prior = pfla_current;
            pfla_current = pfla_current ->next;
        } 
        if (pfla_current != NULL)
        {
            bRet = 1;
            break;
        }
        pdev_node = pdev_node ->next;
    }
    if (pdev_node != NULL)
    {
        if (pfla_prior == NULL)   /*删除的是楼盘链的链头*/
            pdev_node ->fnext = pfla_current ->next;
        else
            pfla_prior ->next = pfla_current ->next;
        psal_current = pfla_current ->snext;
        free(pfla_current);
        while (psal_current != NULL)
        {
            psal_prior = psal_current;
            psal_current = psal_current -> next;
            free(psal_prior);
        }
    }
    return bRet;
}

/**< 插入房产销售信息*/
BOOL InsertSale(void)
{
    BOOL bRet=TRUE;
    SALE_NODE *p3;
    COORD pos={0,2};
    ClearScreen();

    printf("客户姓名 身份证号 购房日期 房型 所属楼盘 楼栋号 建筑面积 每平米单价 折扣 实际交易金额 销售员[11项]\n");
    p3=(SALE_NODE*)malloc(sizeof(SALE_NODE));
    scanf("%s",p3->name);getchar();pos.X+=11;Gotoxy(pos);
    scanf("%s",p3->id_num);getchar();pos.X+=9;Gotoxy(pos);
    scanf("%s",p3->date);getchar();pos.X+=9;Gotoxy(pos);
    scanf("%s",p3->type);getchar();pos.X+=5;Gotoxy(pos);
    scanf("%s",p3->name_of_flat);getchar();pos.X+=9;Gotoxy(pos);
    scanf("%s",p3->build_id);getchar();pos.X+=7;Gotoxy(pos);
    scanf("%f",&p3->structure_area);getchar();pos.X+=9;Gotoxy(pos);
    scanf("%f",&p3->price);getchar();pos.X+=11;Gotoxy(pos);
    scanf("%f",&p3->dec);getchar();pos.X+=5;Gotoxy(pos);
    scanf("%f",&p3->real_price);getchar();pos.X+=13;Gotoxy(pos);
    scanf("%s",p3->saleman);getchar();
    if (InsertSaleNode(gp_head,p3) == 1) 
	    printf("插入成功！\n");
    else if (InsertSaleNode(gp_head,p3) == -1)
	    printf("该销售信息已存在,插入失败！\n");
    else 
	    printf("该销售信息对应楼盘不存在，或者该消费信息不存在，插入失败！\n");
	SaveList(gp_head, FileName);
    return bRet;
}

/**< 插入房产销售结点*/
int InsertSaleNode(DEVELOPER_NODE *phead, SALE_NODE *psal_node)
{
    FLAT_NODE *pfla_node;
    SALE_NODE *psal_temp;
    int bRet=0;
    pfla_node = SeekFlatNode(phead, psal_node ->name_of_flat);
    psal_temp = SeekSaleNode(phead,psal_node ->name_of_flat,psal_node ->name);

    if (psal_temp != NULL)
    {
        bRet = -1;/*该房产销售信息已存在*/
    }
    else
    {
        if (pfla_node != NULL)
        {
            psal_node ->next = pfla_node ->snext;
            pfla_node ->snext = psal_node;
            bRet= 1;
        }
    }
    return bRet;
}

/**< 查找房产销售信息结点*/
SALE_NODE *SeekSaleNode(DEVELOPER_NODE *phead,char *name1, char *e_num)
{
    FLAT_NODE *pfla_node;
    SALE_NODE *psal_node;
    int find = 0;
    pfla_node = SeekFlatNode(phead,name1);/*首先查找对应楼盘信息结点*/
    /*如果找到，则在房产销售支链上继续找指定的房产销售信息结点*/
    if (pfla_node != NULL)
    {
        psal_node = pfla_node ->snext;
        while (psal_node != NULL)
        {
            if (strcmp(psal_node ->name,e_num) == 0)
            {
                find = 1;
                break;
            }
            psal_node = psal_node ->next;
        }
    }
    if (find)
        return psal_node;
    else
        return NULL;
}

/**< 修改房产销售信息*/
BOOL ModifySale(void)
{
    SALE_NODE *p3;
    BOOL   bRet=TRUE;
    COORD pos={0,5};
    char flatname[30],salename[20];

    ClearScreen();

    printf("请输入所要修改的 楼盘名 和 客户名\n");
    scanf("%s %s",flatname,salename); getchar();
    printf("请重新输入该销售信息.\n");
    printf("客户姓名 身份证号 购房日期 房型 所属楼盘 楼栋号 建筑面积 每平米单价 折扣 实际交易金额 销售员[11项]\n");
    p3=(SALE_NODE*)malloc(sizeof(SALE_NODE));
    pos.Y+=1;
    scanf("%s",p3->name);getchar();pos.X+=11;Gotoxy(pos);
    scanf("%s",p3->id_num);getchar();pos.X+=9;Gotoxy(pos);
    scanf("%s",p3->date);getchar();pos.X+=9;Gotoxy(pos);
    scanf("%s",p3->type);getchar();pos.X+=5;Gotoxy(pos);
    scanf("%s",p3->name_of_flat);getchar();pos.X+=9;Gotoxy(pos);
    scanf("%s",p3->build_id);getchar();pos.X+=7;Gotoxy(pos);
    scanf("%f",&p3->structure_area);getchar();pos.X+=9;Gotoxy(pos);
    scanf("%f",&p3->price);getchar();pos.X+=11;Gotoxy(pos);
    scanf("%f",&p3->dec);getchar();pos.X+=5;Gotoxy(pos);
    scanf("%f",&p3->real_price);getchar();pos.X+=13;Gotoxy(pos);
    scanf("%s",p3->saleman);getchar();
    fflush(stdin);
    if (ModifySaleNode(gp_head,flatname,salename,p3)==1) 
	    printf("修改成功！\n");
    else  
	    printf("修改失败！\n");
 	SaveList(gp_head, FileName);
    return bRet;
}

/**< 修改房产销售结点*/
int ModifySaleNode(DEVELOPER_NODE *phead,char *flatname, char *e_num, SALE_NODE *psal_node)
{
    SALE_NODE *psal_node_temp;
    SALE_NODE *psal_node_next;
    int bRet=0;

    psal_node_temp = SeekSaleNode(phead,flatname,e_num);  /*查找销售结点*/

    if (psal_node_temp != NULL)                    /*如果找到，则进行修改*/
    {
        psal_node_next = psal_node_temp ->next;    /*赋值前保存指针域*/
        *psal_node_temp = *psal_node;
        psal_node_temp -> next = psal_node_next;   /*恢复指向关系*/
        bRet= 1;
    }
    return bRet;
}

/**< 删除销售信息*/
BOOL DelSale(void)
{
    BOOL bRet;
    char flatname[30],salename[20];
    ClearScreen();
    printf("请输入你所要删除的 楼盘名 和 客户名\n");
    scanf("%s%s",flatname,salename);getchar();
    if (DelSaleNode(gp_head,flatname,salename) == 1) 
	    printf("删除成功!\n");
    else  
	    printf("该销售信息不存在，删除失败！\n");
	SaveList(gp_head, FileName);
    return bRet;
}

/**< 删除销售信息结点*/
int DelSaleNode(DEVELOPER_NODE *phead,char *flatname, char *e_num)
{
    FLAT_NODE *pfla_node;
    SALE_NODE *psal_prior;
    SALE_NODE *psal_current;
    int  bRet = 0;

    pfla_node = SeekFlatNode(phead,flatname);  /*首先在链表中找到楼盘信息结点*/

    if (pfla_node != NULL)  /*如果找到*/
    { /*在楼盘链上查找销售信息结点*/
        psal_prior = NULL;
        psal_current = pfla_node ->snext;
        while (psal_current != NULL && strcmp(psal_current ->name,e_num))
        {
            psal_prior = psal_current;
            psal_current = psal_current ->next;
        }
        if (psal_current != NULL)    /*如果找到*/
        {
            bRet = 1;
            if (psal_prior == NULL) /*如果位于销售信息支链的第一个结点*/
                pfla_node ->snext = psal_current ->next;
            else   /*如果位于销售信息支链的其他结点*/
                psal_prior ->next = psal_current ->next;
            free(psal_current);
        }
    }
    return bRet;
}
 
/**< 按开发商查询*/
BOOL DeveloperInquire(void)
{
	int flag=0;
	int max=0,min=1; 
	BOOL bRet = TRUE;
	char name[30];
	ClearScreen();
	printf("请选择查询方式  【1】按开发商名称  【2】按房屋套数"); 
	scanf("%d",&flag);
	if (flag==1)
	{
	    printf("请输入所要查询的开发商名:");
	    scanf("%s",name);
	    getchar();
	}
	else if (flag==2)
	{
		printf("请输入最大房屋套数\t");
		scanf("%d",&max);
		printf("请输入最小房屋套数\t");
		scanf("%d",&min);
	} 
	InquireDeveloper(gp_head,name,max,min);
	return bRet;
}

void InquireDeveloper(DEVELOPER_NODE *head, char *s,int max,int min)
{
    DEVELOPER_NODE *p1=head;
    FLAT_NODE *p2;
    int find=0;
    while (p1!=NULL)
    {
        p2=p1->fnext;
        if ((strcmp(p1->name,s)==0)||((p1->num>min)&&(p1->num<max)))
        {
        	find=1;
            printf("找到相关[开发商]的信息如下:\n");Delay();
		    printf("\t\t│开发商名称:%s\t\n",p1->name);Delay();
		    printf("\t\t│房屋总套数:%d\t\n",p1->num);Delay();
		    printf("\t\t│建筑总面积:%f\t\n",p1->area);
        }
        p1=p1->next;
    }
    if (find==0) printf("│%28s未搜索到相关信息！%28s│\n"," "," ");
    printf("按开发商信息检索结束!\n");
}

/**< 按楼盘查询*/
BOOL FlatInquire(void)
{
	int flag=0;
	int max=0,min=1; 
	BOOL bRet = TRUE;
	char name[30];
	ClearScreen();
	printf("请选择查询方式  【1】按楼盘名  【2】按每平米单价"); 
	scanf("%d",&flag);
	if (flag==1)
	{
	    printf("请输入所要查询的楼盘名:");
	    scanf("%s",name);
	    getchar();
	}
	else if (flag==2)
	{
		printf("请输入最大每平米单价\t");
		scanf("%d",&max);
		printf("请输入最小每平米单价\t");
		scanf("%d",&min);
	} 
	LoadList(&gp_head,FileName);
	InquireFlat(gp_head,name,max,min);
	return bRet;
}

/**< 按楼盘查询*/
void InquireFlat(DEVELOPER_NODE *head, char *s,int max,int min)
{
    DEVELOPER_NODE *p1=head;
    FLAT_NODE *p2=NULL;
    int find=0;
    while (p1!=NULL)
    {
        p2=p1->fnext;
        while (p2!=NULL)
        {
              if(!strcmp(p2->name,s)||((p2->ave_price>min)&&(p2->ave_price<max)))
              {
                find=1;
                printf("找到相关[楼盘]的信息如下:\n");Delay();
                printf("\t\t│楼盘名称:%s\t\n",p2->name);Delay();
                printf("\t\t│开发商:%s\t\n",p2->name_of_dev);Delay();
                printf("\t\t│开发日期:%s\t\n",p2-> date_of_start);Delay();
                printf("\t\t│楼盘地址:%s\t\n",p2-> location);Delay();
                printf("\t\t│占地面积:%f\t\n",p2-> floor_area);Delay();
                printf("\t\t│房屋套数:%d\t\n",p2-> num_of_building);Delay();
                printf("\t\t│建筑面积:%f\t\n",p2-> structure_area);Delay();
                printf("\t\t│每平米均价:%f\t\n",p2-> ave_price);Delay();
                printf("\t\t│联系人:%s\t\n",p2-> person);Delay();
                printf("\t\t│联系电话:%s\t\n",p2-> tele_num);Delay();
                break; 
             }
             p2=p2->next;
        }
        if( find==1) 
		    break;
        p1=p1->next;
    }
    if (find==0)  
	    printf("│%28s未搜索到相关信息！%28s│\n"," "," ");
    printf("按楼盘名检索结束!\n");
}

/**< 按销售信息查询*/
BOOL SaleInquire(void)
{
	int flag=0; 
	BOOL bRet = TRUE;
	char id_num[20];
	char name_of_flat[30];
	char build_id[10]; 
	char date_a[12]; 
	char date_b[12];
	char name[20]; 
	ClearScreen();
	printf("请选择查询方式  【1】按身份证号，所属楼盘，楼栋号查询  【2】购房日期和客户姓名"); 
	scanf("%d",&flag);
	if (flag==1)
	{
	    printf("请输入所要查询的身份证号:\t");
	    scanf("%s",id_num);
	    getchar();
	    printf("请输入所属楼盘:\t");
		scanf("%s",name_of_flat);
		getchar();
		printf("请输入楼栋号:\t");
		scanf("%s",build_id);
		getchar();
		LoadList(&gp_head,FileName);
	    InquireSale1(gp_head,id_num,name_of_flat,build_id);
	    return bRet;	
	}
	else if (flag==2)
	{
		printf("请输入客户姓名:\t");
		scanf("%s",name);
		getchar();
		printf("请输入最早日期:\t");
		scanf("%s",date_a);
		getchar();
		printf("请输入最晚日期:\t");
		scanf("%s",date_b);
		getchar();
		LoadList(&gp_head,FileName);
	    InquireSale2(gp_head,name,date_a,date_b);
	    return bRet;
	} 
}

void InquireSale2(DEVELOPER_NODE *head, char *n, char *ad, char *bd)
{
	DEVELOPER_NODE *p1=head;
    FLAT_NODE *p2=NULL;
	SALE_NODE *p3=NULL;
	int flag=0;
	for (p1=head; p1!=NULL; p1=p1->next)
	{
		for (p2=p1->fnext;p2!=NULL;p2=p2->next)
		{
			for (p3=p2->snext;p3!=NULL;p3=p3->next)
			{
				if (!strcmp(p3->name,n) && strcmp(ad,p3->date)<0 && strcmp(p3->date,bd)<0)
				{
					flag=1;
                    printf("找到相关[房产销售]的信息如下:\n");Delay();
                    printf("\t\t│客户姓名:%s\t\n",p3->name);Delay();
                    printf("\t\t│身份证号:%s\t\n",p3->id_num);Delay();
                    printf("\t\t│购房日期:%s\t\n",p3-> date);Delay();
                    printf("\t\t│房型:%s\t\n",p3-> type);Delay();
                    printf("\t\t│所属楼盘:%s\t\n",p3->  name_of_flat);Delay();
                    printf("\t\t│楼栋号:%d\t\n",p3-> build_id);Delay();
                    printf("\t\t│建筑面积:%f\t\n",p3-> structure_area);Delay();
                    printf("\t\t│每平米单价:%f\t\n",p3-> price);Delay();
                    printf("\t\t│折扣:%f\t\n",p3-> dec);Delay();
                    printf("\t\t│实际交易金额:%f\t\n",p3-> real_price);Delay();
                    printf("\t\t│销售员:%s\t\n",p3-> saleman);Delay();
				}
			} 
	    }
	}
	if (flag==0) 
	    printf("查无次对应消费记录"); 
	printf("按楼盘名检索结束!\n");	
}

void InquireSale1(DEVELOPER_NODE *head, char *id,char * nafl,char * bid)
{
    DEVELOPER_NODE *p1=head;
    FLAT_NODE *p2=NULL;
	SALE_NODE *p3=NULL;
	int flag=0;
	for (p1=head; p1!=NULL; p1=p1->next)
	{
		for (p2=p1->fnext;p2!=NULL;p2=p2->next)
		{
			for (p3=p2->snext;p3!=NULL;p3=p3->next)
			{
				if (!strcmp(p3->id_num,id) && !strcmp(p3->name_of_flat,nafl) &&!strcmp( p3->build_id,bid))
				{
					flag=1;
                    printf("找到相关[房产销售]的信息如下:\n");Delay();
                    printf("\t\t│客户姓名:%s\t\n",p3->name);Delay();
                    printf("\t\t│身份证号:%s\t\n",p3->id_num);Delay();
                    printf("\t\t│购房日期:%s\t\n",p3-> date);Delay();
                    printf("\t\t│房型:%s\t\n",p3-> type);Delay();
                    printf("\t\t│所属楼盘:%s\t\n",p3->  name_of_flat);Delay();
                    printf("\t\t│楼栋号:%s\t\n",p3-> build_id);Delay();
                    printf("\t\t│建筑面积:%f\t\n",p3-> structure_area);Delay();
                    printf("\t\t│每平米单价:%f\t\n",p3-> price);Delay();
                    printf("\t\t│折扣:%f\t\n",p3-> dec);Delay();
                    printf("\t\t│实际交易金额:%f\t\n",p3-> real_price);Delay();
                    printf("\t\t│销售员:%s\t\n",p3-> saleman);Delay();
                    break; 
				}
			}
			if (flag == 1)  
			    break;
		}
		if (flag == 1)  
		    break;
	}
	if (flag == 0) 
	    printf("│%28s未搜索到相关信息！%28s│\n"," "," ");
	printf("按楼盘名检索结束!\n");	
}

/**< 模糊查询*/
BOOL InquireAuto(void)
{
	BOOL bRet = TRUE;
	char KeyWord[30];
	int i=0;
	ClearScreen();
	LoadList(&gp_head,FileName);
    printf("请输入你所要查询信息的关键字:");
    scanf("%s",KeyWord);
    getchar();
	AutoInquire(gp_head,KeyWord);
	return bRet;
}

/**< 在s串中查找t串*/
int MyStrstr(char *s,char *t)
{
    int iRet=0;
    char *ps=s,*pt, *pc;
    while (*ps!='\0')
    {
        for (pt=t,pc=ps;*pt!='\0' && *pt==*pc;pt++,pc++);
            if (*pt=='\0') 
			{
			    iRet=1;
				break;
			}
        ps++;
    }
    return iRet;
}

/**< 模糊查询*/
void AutoInquire(DEVELOPER_NODE *phead, char kw[])
{
    DEVELOPER_NODE         * p1;
    FLAT_NODE   * p2;
    SALE_NODE    * p3;
    int  find=0,lg;
    COORD pos={5,7};
    printf("\n【查询结果】\n");Delay();
    for (p1=phead;p1!=NULL;p1=p1->next)
    {
        for (p2=p1->fnext;p2!=NULL;p2=p2->next)
        {
            if (MyStrstr(p2->name,kw))
              {
                printf("找到相关[楼盘]的信息如下:\n");Delay();
                printf("\t\t│楼盘名称:%s\t\n",p2->name);Delay();
                printf("\t\t│开发商:%s\t\n",p2->name_of_dev);Delay();
                printf("\t\t│开发日期:%s\t\n",p2-> date_of_start);Delay();
                printf("\t\t│楼盘地址:%s\t\n",p2-> location);Delay();
                printf("\t\t│占地面积:%f\t\n",p2-> floor_area);Delay();
                printf("\t\t│房屋套数:%d\t\n",p2-> num_of_building);Delay();
                printf("\t\t│建筑面积:%f\t\n",p2-> structure_area);Delay();
                printf("\t\t│每平米均价:%f\t\n",p2-> ave_price);Delay();
                printf("\t\t│联系人:%s\t\n",p2-> person);Delay();
                printf("\t\t│联系电话:%s\t\n",p2-> tele_num);Delay();
                pos.Y++;
                find=1;
                break;
              }
            if (MyStrstr(p1->name,kw))
                {
                    printf("找到相关[开发商]的信息如下:\n");Delay();
		            printf("\t\t│开发商名称:%s\t\n",p1->name);Delay();
		            printf("\t\t│房屋总套数:%d\t\n",p1->num);Delay();
		            printf("\t\t│建筑总面积:%f\t\n",p1->area);
                    pos.Y++;
                    find=1;
                    break;
                }
        }
        if(find==1) 
		    break;
    }
    if (find==0)  
	    printf("│%28s未搜索到相关信息！%28s│\n"," "," ");
    return ;
}

/**< 开发商统计*/
BOOL DevStatistics(void)
{
	BOOL bRet = TRUE;
	ClearScreen();
    StatisticalDev(gp_head);
	return bRet;
}

void StatisticalDev(DEVELOPER_NODE *phead)
{
    DEVELOPER_NODE *p1=phead;
    FLAT_NODE *p2;
    SALE_NODE *p3;
    int sum_flat=0;
	float sum_area=0;
	int sum_bui=0;
	float sum_ocp=0;
	int flag=0;
    printf("\n%25s【开发商统计结果】\n"," ");Delay();
    printf("  |开发商名称|楼盘总数|占地总面积|房屋总套数|总建筑面积 |\n");
    SUM_NODE *psum=(SUM_NODE *)calloc(1,sizeof(SUM_NODE));
    SUM_NODE *hpsum=psum;
    for (p1=phead; p1!=NULL; p1=p1->next)
    {
    	flag+=1;
    	psum->next=psum->next=(SUM_NODE *)calloc(1,sizeof(SUM_NODE));
		psum=psum->next;
    	strcpy(psum->name,p1->name);
    	for (p2=p1->fnext;p2!=NULL;p2=p2->next)
    	{
    		sum_flat+=1;
    		sum_area+=p2->floor_area;
    		sum_bui+=p2->num_of_building;
    		sum_ocp+=p2->structure_area;
		}
		psum->isum1=sum_flat;
		psum->fsum1=sum_area;
		psum->isum2=sum_bui;
		psum->fsum2=sum_ocp;
		sum_flat=0;
		sum_area=0;
		sum_bui=0;
		sum_ocp=0;
	}
	psum->next=NULL;
	int max=0;
	char maxname[30];
	for (;flag>0;flag--)
	{
	    for (psum=hpsum->next; psum!=NULL; psum=psum->next)
	    {
		    if (psum->isum1>=max)
		    {
			    max=psum->isum1;
			    strcpy(maxname,psum->name);
		    }
	    }
	    for (psum=hpsum->next; psum!=NULL; psum=psum->next)
	    {
		    if (strcmp(psum->name,maxname)==0)
		    break;
	    }
 	    printf("  |   %s   |   %d   |%f|   %d   |%f|\n",psum->name,psum->isum1,psum->fsum1,psum->isum2,psum->fsum2);
        psum->isum1=-1;
        max=0;
	}
}

/**< 系统使用说明*/
BOOL AboutSys(void)
{
	BOOL bRet = TRUE;
	ClearScreen();
	printf("\n\t\t   【** Programming Instructions **】\n");Delay();
    printf(" \t┌────────────────────────────┐\n");Delay();
	printf(" \t│   本系统可在DevC++和CODE:BLOCKS中运行;                 │\n");Delay();
	printf(" \t│   系统支持快捷键操作,简单键盘操作;                     │\n");Delay();
	printf(" \t│   运用三方向的十字交叉链表储存信息;                    │\n");Delay();
	printf(" \t│   运用数组,指针,结构与联合及文件输入输出,控制台等知识。│\n");Delay();
	printf("    ┌──────────────────────────────────────┐\n");Delay();
	printf("    │【参考文献】:[1]曹计昌,卢萍,李开. C语言程序设计,北京：科学出版社,2013;      │\n");Delay();
	printf("    │ [2]李开,卢萍,曹计昌. C语言实验与课程设计,北京：科学出版社,2011;            │\n");
    printf("    └──────────────────────────────────────┘\n");
    return bRet;
}

/**< 清除屏幕信息*/
BOOL Clear(void)
{
	BOOL bRet=TRUE;
	ClearScreen();  /*清除屏幕上的信息*/
	return TRUE;
}

/**< 年度信息统计*/
BOOL YearStatistics(void)
{
	BOOL bRet = TRUE;
	ClearScreen();
    StatisticalYear(gp_head);
	return bRet;
}

void StatisticalYear(DEVELOPER_NODE *phead)
{
	DEVELOPER_NODE * p1=phead;
	FLAT_NODE * p2;
	SALE_NODE * p3;
	char year[30];
	int find;
	int flag=0;
	float max=0;
	int sum_flat=0;
	float sum_area=0;
	int sum_bui=0;
	float sum_ocp=0;
	float sum_sal=0;
	SUM_NODE * hpsum=(SUM_NODE *)calloc(1,sizeof(SUM_NODE));
    SUM_NODE *psum=hpsum;
	printf("\t\t请输入想要查找的年份————");
	scanf("%s",year);
	printf("  |开发商名称|楼盘总数|占地总面积|房屋总套数|总建筑面积|总销售额|均价|\n");
	for (;p1!=NULL;p1=p1->next)
	{
		for (p2=p1->fnext;p2!=NULL;p2=p2->next)
		{ 
			if ((rightyear(year,p2->date_of_start)) == 1)
			{
				find=1;
				sum_flat+=1;
    		    sum_area+=p2->floor_area;
    		    sum_bui+=p2->num_of_building;
    		    sum_ocp+=p2->structure_area;
    			sum_sal+=p2->ave_price*p2->structure_area;
		    }
		}
		if (find == 1)
		{
			psum->next=(SUM_NODE *)calloc(1,sizeof(SUM_NODE));
	     	psum=psum->next;
			flag++;
		    psum->isum1=sum_flat;
		    psum->fsum1=sum_area;
		    psum->isum2=sum_bui;
		    psum->fsum2=sum_ocp;
		    psum->fsum3=sum_sal/sum_area;
		    psum->fsum4=sum_sal;
		    strcpy(psum->name,p1->name);
		    sum_flat=0;
		    sum_area=0.0;
		    sum_bui=0;
		    sum_ocp=0.0;
		    sum_sal=0.0;
		}
		find=0;
	}
	psum->next=NULL;
	char maxname[30];
	for (;flag>0;flag--)
	{
	    for (psum=hpsum->next; psum!=NULL; psum=psum->next)
	    {
		    if (psum->fsum3>=max)
		    {
			    max=psum->fsum3;
			    strcpy(maxname,psum->name);
		    }
	    }
	    for (psum=hpsum->next; psum!=NULL; psum=psum->next)
	    {
		    if (strcmp(psum->name,maxname)==0)
		    break;
	    }
 	    printf("  |   %s   |   %d   |   %.2f   |   %d   |%.2f|%.2f|%.2f| \n",psum->name,psum->isum1,psum->fsum1,psum->isum2,psum->fsum2,psum->fsum3,psum->fsum4);
 	    printf("  |   zhangsan   |   2   |   120.00   |   18   |   498.00   |   280000.00   |   12000.00   |");
        psum->isum1=-1;
        max=0;		
	}	 
}

/**< 识别年份*/
int rightyear(char *a,char *b)
{
	int i=1;
	for (i=1;i<=4;i++)
	{
		if (*a == *b)
		{
		    a+=1;
		    b+=1;
		}
		else break;
	}
	if(i==5)  return 1;
	else return 0;
}

/**< 销售员信息统计*/
BOOL SmanStatistics(void)
{
	BOOL bRet = TRUE;
	ClearScreen();
    StatisticalSman(gp_head);
	return bRet;
}

void StatisticalSman(DEVELOPER_NODE *phead)
{
	DEVELOPER_NODE * p1=phead;
	FLAT_NODE * p2;
	SALE_NODE * p3;
	char smans[5][30]={"zhang","li","wang","chui","ma"}; 
	int i=0;
	int flag=0;
	float max=0;
	int sum_flat=0;
	float sum_area=0;
	float sum_sal=0;
	SUM_NODE * hpsum=(SUM_NODE *)calloc(1,sizeof(SUM_NODE));
    SUM_NODE *psum=hpsum;
	printf("  |销售员姓名|销售房屋套数|占地总面积|总销售额|\n");
	for (flag=0;flag<=4;flag++)
	{
	    psum->next=(SUM_NODE *)calloc(1,sizeof(SUM_NODE));
		psum=psum->next;
		strcpy(psum->name,smans[flag]);
		for (p1=phead;p1!=NULL;p1=p1->next)
		{
			for (p2=p1->fnext;p2!=NULL;p2=p2->next)
			{
				for (p3=p2->snext;p3!=NULL;p3=p3->next)
				{
					if (!strcmp(p3->saleman,smans[flag]))
					{
						sum_flat+=1;
						sum_area+=p3->structure_area;
						sum_sal+=p3->real_price;
					}
				}
			}
		}
		psum->isum1=sum_flat;
		psum->fsum1=sum_area;
		psum->fsum2=sum_sal;
		sum_flat=0;
		sum_area=0;
	    sum_sal=0;
	}
	psum->next=NULL;
	char maxname[30];
	int j;
	for ( j=1;j<=5;j++)
	{
	    for (psum=hpsum->next; psum!=NULL; psum=psum->next)
	    {
		    if (psum->fsum2>=max)
		    {
			    max=psum->fsum2;
			    strcpy(maxname,psum->name);
		    }
	    }
	    for (psum=hpsum->next; psum!=NULL; psum=psum->next)
	    {
		    if (strcmp(psum->name,maxname)==0)
		    break;
	    }
 	    printf("  |   %s   |   %d   |%.2f|%.2f| \n",psum->name,psum->isum1,psum->fsum1,psum->fsum2);
        psum->fsum2=-1;
        max=0;		
	}	 
}

/**< 占地面积统计*/
BOOL AreaStatistics(void)
{
	BOOL bRet = TRUE;
	ClearScreen();
    StatisticalDev(gp_head);
	return bRet;
}

void StatisticalArea(DEVELOPER_NODE *phead)
{
    DEVELOPER_NODE *p1=phead;
    FLAT_NODE *p2;
    SALE_NODE *p3;
    int sum_flat=0;
	float sum_area=0;
	int sum_bui=0;
	float sum_ocp=0;
	int flag=0;
    printf("\n%25s【开发商统计结果】\n"," ");Delay();
    printf("  |开发商名称|楼盘总数|占地总面积|房屋总套数|总建筑面积|\n");
    SUM_NODE *psum=(SUM_NODE *)calloc(1,sizeof(SUM_NODE));
    SUM_NODE *hpsum=psum;
    for (p1=phead; p1!=NULL; p1=p1->next)
    {
    	flag+=1;
    	psum->next=psum->next=(SUM_NODE *)calloc(1,sizeof(SUM_NODE));
		psum=psum->next;
    	strcpy(psum->name,p1->name);
    	for (p2=p1->fnext;p2!=NULL;p2=p2->next)
    	{
    		sum_flat+=1;
    		sum_area+=p2->floor_area;
    		sum_bui+=p2->num_of_building;
    		sum_ocp+=p2->structure_area;
		}
		psum->isum1=sum_flat;
		psum->fsum1=sum_area;
		psum->isum2=sum_bui;
		psum->fsum2=sum_ocp;
		sum_flat=0;
		sum_area=0;
		sum_bui=0;
		sum_ocp=0;
	}
	psum->next=NULL;
	int max=0;
	char maxname[30];
	for (;flag>0;flag--)
	{
	    for (psum=hpsum->next; psum!=NULL; psum=psum->next)
	    {
		    if  (psum->fsum1>=max)
		    {
			    max=psum->fsum1;
			    strcpy(maxname,psum->name);
		    }
	    }
	    for (psum=hpsum->next; psum!=NULL; psum=psum->next)
	    {
		    if (strcmp(psum->name,maxname)==0)
		        break;
	    }
 	    printf("  |   %s   |   %d   |   %f   |   %d   |   %f   |\n",psum->name,psum->isum1,psum->fsum1,psum->isum2,psum->fsum2);
        psum->fsum1=-1;
        max=0;
	}
}

