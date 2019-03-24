#include "developer.h"

/**< Main����.*/
int main(void)
{

    TextInterface();  /*�ı��˵������ʼ��*/ 
    LoadList(&gp_head,FileName);  /*������Ϣ*/
	RunSystem();    /*ϵͳ����ģ���ѡ������*/
    SaveList(gp_head,FileName);   /*������Ϣ*/
    CloseSys(gp_head);         /*�ر�ϵͳ*/
	return 0;
}

/**< ���������posλ�ô�*/
inline void Gotoxy(COORD pos)
{
    gh_std_out = GetStdHandle(STD_OUTPUT_HANDLE);  /*��ȡ��׼����豸���*/
	SetConsoleCursorPosition(gh_std_out,pos);      /*���ù��λ����pos��*/
}

inline void Delay(void)
{
  int goal;
  goal = 50 + clock();
  while ( goal > clock() );
}

/**< �ر�ϵͳ,�ͷŶ�̬�洢��.*/
void CloseSys(DEVELOPER_NODE *hd)
{
    DEVELOPER_NODE *p1 = hd,  *pp1;
    FLAT_NODE *p2, *pp2;
    SALE_NODE *p3, *pp3;
    /*�ͷ�ʮ�ֽ�������Ķ�̬�洢��*/
    while (p1 != NULL)
    {
        pp1 = p1->next;
        p2 = p1->fnext;
        while (p2 != NULL)     /*�ͷ�¥��֧���Ķ�̬�洢��*/
        {
            pp2 = p2->next;
            p3 = p2->snext;
            while (p3 != NULL) /*�ͷ�������Ϣ֧���Ķ�̬�洢��*/
            {
                pp3 = p3->next;
                free(p3);
                p3 = pp3;
            }
            free(p2);
            p2 = pp2;
        }
        free(p1);            /*�ͷſ������������Ķ�̬�洢��*/
        p1 = pp1;
    }
	ClearScreen();     /*�����Ļ��Ϣ*/
    /*�ͷŴ�Ų˵�����״̬������Ϣ��̬�洢��*/
    free(gp_buff_menubar_info);
    /*�رձ�׼���������豸���*/
    CloseHandle(gh_std_out);
    CloseHandle(gh_std_in);
    return;
}

/**< �ı��˵�����.*/
void TextInterface(void)
{
    COORD size = {SCR_COL, SCR_ROW};  /*���ڻ�������С.��:80, ��:25 */

    gh_std_out = GetStdHandle(STD_OUTPUT_HANDLE);  /* ��ȡ��׼����豸���*/
    gh_std_in  = GetStdHandle(STD_INPUT_HANDLE);   /* ��ȡ��׼�����豸���*/

    SetConsoleTitle("������������Ϣ����ϵͳ��"); /*���ô��ڱ���*/
    SetConsoleScreenBufferSize(gh_std_out, size); /*���ô��ڻ�������С80*25*/
    ClearScreen(); /* ����*/

    /*��������������Ϣ��ջ������ʼ�������Ļ���ڵ�����һ�㵯������*/
    gp_scr_att = (char *)calloc(SCR_COL * SCR_ROW, sizeof(char));/*��Ļ�ַ�����*/
    gp_top_layer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));/*��̬����gp_top_layer���*/
    gp_top_layer->LayerNo = 0;      /*�������ڵĲ��Ϊ0*/
    gp_top_layer->rcArea.Left = 0;  /*�������ڵ�����Ϊ������Ļ����*/
    gp_top_layer->rcArea.Top = 0;   /*������������λ�ó�ʼ��*/
    gp_top_layer->rcArea.Right = SCR_COL - 1;
    gp_top_layer->rcArea.Bottom = SCR_ROW - 1;
    gp_top_layer->pContent = NULL;  /*�������������ַ���Ԫԭ��Ϣ�洢��������ʼֵΪNULL*/
    gp_top_layer->pScrAtt = gp_scr_att; /*gp_scr_attָ�򵯳����������ַ���Ԫԭ����ֵ�洢��������Ϣ*/
    gp_top_layer->next = NULL; /*ָ�����һ�㴰����ϢΪNULL*/

    ShowMenu();   /*��ʾ���˵���*/
    return ;
}

/**< ����Ļ����ʾ���˵���.*/
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
    GetConsoleScreenBufferInfo( gh_std_out, &bInfo );  /*��ȡ��Ļ��������Ϣ������bInfo��*/
    size.X = bInfo.dwSize.X;
    size.Y = 1;
    SetConsoleCursorPosition(gh_std_out, pos); /* ������õ���(0,0) */
    for (i=0; i < 5; i++) /*�ڴ��ڵ�һ�д�������˵���*/
        printf("��%s��", ga_main_menu[i]);
    /*���붯̬�洢����Ϊ��Ų˵�����Ļ���ַ���Ϣ�Ļ�����*/
    gp_buff_menubar_info = (CHAR_INFO *)malloc(size.X * size.Y * sizeof(CHAR_INFO));
    /*���û�������Χ*/
	rcMenu.Left=0;
	rcMenu.Top=0;
	rcMenu.Right=size.X-1;
	rcMenu.Bottom=0;
    /*�����ڵ�һ�е����ݶ��뵽��Ų˵�����Ļ���ַ���Ϣ�Ļ�������*/
    ReadConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
    /*����һ����Ӣ����ĸ��Ϊ��ɫ�������ַ���Ԫ��Ϊ�׵׺���*/
    for (i=0; i<size.X; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN| BACKGROUND_RED;/*�׵׺���*/
        ch = (char)((gp_buff_menubar_info+i)->Char.AsciiChar);
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
            (gp_buff_menubar_info+i)->Attributes |= FOREGROUND_RED|FOREGROUND_INTENSITY;  /*Ӣ����ĸΪ����ɫ*/
    }
    /*�޸ĺ�Ĳ˵����ַ���Ϣ��д�����ڵĵ�һ��*/
    WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
    MarkMainMenu(gi_sel_menu);  /*��ѡ�����˵���������ǣ�gi_sel_menu��ֵΪ1*/
	printf("\n");
    return;
}

/**< ��ָ�����˵��num������ѡ�б�־.*/
void MarkMainMenu(int num)
{
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD size;
    COORD pos = {0, 0};
    int PosA = 2, PosB;
    SMALL_RECT rcMenu ;
    char ch;
    int i;
    if (num == 0)  /*numΪ0ʱ������ȥ�����˵���ѡ�б��*/
    {
        PosA = 0;
        PosB = 0;
    }
    else  /*���򣬶�λѡ�����˵������ֹλ��: PosAΪ��ʼλ��, PosBΪ��ֹλ��*/
    {
        for (i=1; i<num; i++)
             PosA += strlen(ga_main_menu[i-1]) + 4;
        PosB = PosA + strlen(ga_main_menu[num-1]);
    }
    GetConsoleScreenBufferInfo( gh_std_out, &bInfo ); /*��ȡ��Ļ��������Ϣ������bInfo��*/
    size.X = bInfo.dwSize.X;
    size.Y = 1;
    /*ȥ��ѡ�в˵���ǰ��Ĳ˵���ѡ�б��*/
    for (i=0; i<PosA; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN| BACKGROUND_RED;/*�׵׺���*/
        ch = (gp_buff_menubar_info+i)->Char.AsciiChar;
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
            (gp_buff_menubar_info+i)->Attributes |= FOREGROUND_RED|FOREGROUND_INTENSITY;/*��ĸΪ����ɫ*/
    }
    /*��ѡ�в˵���������ǣ�����ɫ�׺���*/
    for (i=PosA; i<PosB; i++)
        (gp_buff_menubar_info+i)->Attributes =BACKGROUND_GREEN|BACKGROUND_BLUE|BACKGROUND_INTENSITY;
    /*ȥ��ѡ�в˵������Ĳ˵���ѡ�б��*/
    for (i=PosB; i<bInfo.dwSize.X; i++)
    {
        (gp_buff_menubar_info+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;/*�׵׺���*/
        ch = (char)((gp_buff_menubar_info+i)->Char.AsciiChar);
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
            (gp_buff_menubar_info+i)->Attributes |= FOREGROUND_RED|FOREGROUND_INTENSITY;/*��ĸΪ����ɫ*/
    }
    /*������Ļ��������Χ��С*/
	rcMenu.Left=0;
	rcMenu.Top=0;
	rcMenu.Right=size.X-1;
	rcMenu.Bottom=0;
    /*�����ñ�ǵĲ˵�����Ϣд�����ڵ�һ��*/
    WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
    return;
}

/**< �����Ļ�ϵ���Ϣ.*/
void ClearScreen(void)
{
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD home = {0, 1};
    unsigned long size;
    GetConsoleScreenBufferInfo( gh_std_out, &bInfo );/*ȡ��Ļ��������Ϣ*/
    size = bInfo.dwSize.X * bInfo.dwSize.Y;     /*������Ļ�������ַ���Ԫ��*/
    /*����Ļ���������е�Ԫ���ַ���������Ϊ��ǰ��Ļ�������ַ�����*/
    FillConsoleOutputAttribute(gh_std_out, bInfo.wAttributes, size, home, &len);
    /*����Ļ���������е�Ԫ���Ϊ�ո��ַ�*/
    FillConsoleOutputCharacter(gh_std_out, ' ', size, home, &len);
	SetConsoleCursorPosition(gh_std_out,home);  /*�����λ�ûط���{0,1}��*/
}

/**< ����ָ�����Ϊnum�����˵����Ӧ���Ӳ˵�.*/
void PopMenu(int num)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT rcPop,*aArea;
    COORD pos,*aLoc;
    WORD att;
    int i, j, loc = 0;
	char *aSort, *aTag;/*aSort:������� aTag:�������*/
	char *pch;
    if (num != gi_sel_menu)    /*���ָ�����˵�������ѡ�в˵�*/
    {
        if (gp_top_layer->LayerNo != 0) /*�����ǰ�����Ӳ˵�����*/
        {
            PopOff();/*�رոõ�������*/
            gi_sel_sub_menu = 0;/*����ѡ�е��Ӳ˵������Ϊ0*/
        }
    }
    else if (gp_top_layer->LayerNo != 0) /*���ѵ������Ӳ˵����򷵻�*/
        return;
    gi_sel_menu = num;               /*��ѡ�����˵�����Ϊָ�������˵���*/
    MarkMainMenu(gi_sel_menu);        /*��ѡ�е����˵����������*/
    LocateSubMenu(gi_sel_menu, &rcPop); /*���㵯���Ӳ˵�������λ��, �����rcPop��*/
    /*������Ӳ˵��еĵ�һ�����Ӳ˵��ַ��������е�λ��(�±�)*/
    for (i=1; i<gi_sel_menu; i++)
        loc += ga_sub_menu_count[i-1];
    /*�������Ӳ˵������������ǩ���ṹ����*/
    labels.ppLabel = ga_sub_menu + loc;   /*��ǩ����һ����ǩ�ַ����ĵ�ַ*/
    labels.num = ga_sub_menu_count[gi_sel_menu-1]; /*��ǩ���б�ǩ�ַ����ĸ���*/
    /*����һ���������飬���ÿ����ǩ�ַ������λ�õ�����ȼ���:COORD aLoc[labels.num];*/
	aLoc=(COORD *)malloc(labels.num*sizeof(COORD));
    for (i=0; i<labels.num; i++) /*ȷ����ǩ�ַ��������λ�ã����������������*/
    {
        aLoc[i].X = rcPop.Left + 2;
        aLoc[i].Y = rcPop.Top + i + 1;
    }
    labels.pLoc = aLoc; /*ʹ��ǩ���ṹ����labels�ĳ�ԱpLocָ�������������Ԫ��*/
    /*�����Ӳ˵�������Ϣ*/
    areas.num = labels.num;       /*�����ĸ��������ڱ�ǩ�ĸ��������Ӳ˵�������*/
	aArea=(SMALL_RECT *)malloc(areas.num*sizeof(SMALL_RECT));/*������������������λ�õȼ���:SMALL_RECT aArea[areas.num];*/
	aSort=(char *)malloc(areas.num*sizeof(char));    /*��������������������Ӧ���ȼ���:char aSort[areas.num];*/
	aTag=(char *)malloc(areas.num*sizeof(char));    /*����������ÿ�������ı��:char aTag[areas.num];*/
    /*������λ*/
	for (i=0; i<areas.num; i++)
    {/*��������Χ*/
        aArea[i].Left = rcPop.Left + 2;
        aArea[i].Top = rcPop.Top + i + 1;
        aArea[i].Right = rcPop.Right - 2;
        aArea[i].Bottom = aArea[i].Top;
        aSort[i] = 0;     /*�������Ϊ0(��ť��)*/
        aTag[i] = i + 1; /*������˳����1,2,3,4...*/
    }
    areas.pArea = aArea;  /*ʹ�����ṹ����areas�ĳ�ԱpAreaָ������λ��������Ԫ��*/
    areas.pSort = aSort;  /*ʹ�����ṹ����areas�ĳ�ԱpSortָ���������������Ԫ��*/
    areas.pTag = aTag;    /*ʹ�����ṹ����areas�ĳ�ԱpTagָ���������������Ԫ��*/
    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�Ӳ˵��ַ�����:�׵׺���*/
	PopOn(&rcPop, att, &labels, &areas);
    DrawBox(&rcPop);       /*���������ڻ��߿�*/
    pos.X = rcPop.Left + 2;
    for (pos.Y=rcPop.Top+1; pos.Y<rcPop.Bottom; pos.Y++)
    { /*��ѭ�������ڿմ��Ӳ���λ�û����γɷָ�����ȡ���˲˵������������*/
        pch = ga_sub_menu[loc+pos.Y-rcPop.Top-1];
        if (strlen(pch)==0) /*����Ϊ0������Ϊ�մ�*/
        {   /*���Ȼ�����*/
            FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right-rcPop.Left-3,pos, &len);
            for (j=rcPop.Left+2; j<rcPop.Right-1; j++)
            {   /*ȡ���������ַ���Ԫ����������*/
                gp_scr_att[pos.Y*SCR_COL+j] &= 3; /*��λ��Ľ�������˵���λ*/
            }
        }
    }
    /*���Ӳ˵���Ĺ��ܼ���Ϊ�׵׺���*/
    pos.X = rcPop.Left + 3;
    att =  FOREGROUND_GREEN |FOREGROUND_INTENSITY;
    for (pos.Y=rcPop.Top+1; pos.Y<rcPop.Bottom; pos.Y++)
    {
        if (strlen(ga_sub_menu[loc+pos.Y-rcPop.Top-1])==0)
        {
            continue;  /*�����մ�*/
        }
        FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &len);
    }
    return;
}

/**< ��ָ�������������������Ϣ, ͬʱ�����Ӳ˵�����, ����������λ����Ϣ�洢��ջ.*/
void PopOn(SMALL_RECT *pRc, WORD att, LABEL_BUNDLE *pLabel, HOT_AREA *pHotArea)
{
    LAYER_NODE *nextLayer;  /*����������Ϣ�ṹ����ָ��*/
    COORD size;
    COORD pos = {0, 0};
    char  *pCh;
    int i, j, row;
    /*������������λ���ַ���Ԫ��Ϣ��ջ*/
    size.X = pRc->Right - pRc->Left + 1;    /*�������ڵĿ��*/
    size.Y = pRc->Bottom - pRc->Top + 1;    /*�������ڵĸ߶�*/
    /*�����ŵ������������Ϣ�Ķ�̬�洢��*/
    nextLayer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
    nextLayer->next = gp_top_layer;
    nextLayer->LayerNo = gp_top_layer->LayerNo + 1;
    nextLayer->rcArea = *pRc;
    nextLayer->pContent = (CHAR_INFO *)malloc(size.X * size.Y * sizeof(CHAR_INFO));
    nextLayer->pScrAtt = (char *)malloc(size.X * size.Y * sizeof(char));
    pCh = nextLayer->pScrAtt;
    /*���������ڸ���������ַ���Ϣ���棬�����ڹرյ�������ʱ�ָ�ԭ��*/
    ReadConsoleOutput(gh_std_out, nextLayer->pContent, size, pos, pRc);
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {   /*�˶���ѭ�����������ַ���Ԫ��ԭ������ֵ���붯̬�洢���������Ժ�ָ�*/
        for (j=pRc->Left; j<=pRc->Right; j++)
        {
            *pCh = gp_scr_att[i*SCR_COL+j];  /*��pChָ������ݷ���gp_scr_attָ����ַ���Ԫ����ֵ�Ļ�����*/
            pCh++;
        }
    }
    gp_top_layer = nextLayer;  /*��ɵ������������Ϣ��ջ����,�Խ�gp_top_layer����Ϊ����������Ϣ����ͷ*/
    /*���õ������������ַ���������*/
    pos.X = pRc->Left;
    pos.Y = pRc->Top;
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {
        FillConsoleOutputAttribute(gh_std_out, att, size.X, pos, &len);
        pos.Y++;
    }
    /*����ǩ���еı�ǩ�ַ������趨��λ�����*/
    for (i=0; i<pLabel->num; i++)
    {
        pCh = pLabel->ppLabel[i];
        if (strlen(pCh) != 0)
            WriteConsoleOutputCharacter(gh_std_out, pCh, strlen(pCh),pLabel->pLoc[i], &len);
    }
    /*���õ������������ַ���Ԫ��������*/
    for (i=pRc->Top; i<=pRc->Bottom; i++)
    {   /*�˶���ѭ�������ַ���Ԫ�Ĳ��*/
        for (j=pRc->Left; j<=pRc->Right; j++)
            gp_scr_att[i*SCR_COL+j] = gp_top_layer->LayerNo;
    }
    /*�˶���ѭ�����������������ַ���Ԫ���������ͺ��������*/
    for (i=0; i<pHotArea->num; i++)
    {
        row = pHotArea->pArea[i].Top;
        for (j=pHotArea->pArea[i].Left; j<=pHotArea->pArea[i].Right; j++)
            gp_scr_att[row*SCR_COL+j] |= (pHotArea->pSort[i] << 6)| (pHotArea->pTag[i] << 2);
    }
    return;
}

/**< �رյ�������,�ָ���������ԭ��ۺ��ַ���Ԫԭ����.*/
void PopOff(void)
{
    LAYER_NODE *nextLayer;
    COORD size;
    COORD pos = {0, 0};
    char *pCh;
    int i, j;
    /*ջ�״�ŵ���������Ļ��Ϣ�����ùر�*/
    if ((gp_top_layer->next==NULL) || (gp_top_layer->pContent==NULL))
        return;
    nextLayer = gp_top_layer->next;
    /*�ָ�������������ԭ���*/
    size.X = gp_top_layer->rcArea.Right - gp_top_layer->rcArea.Left + 1;
    size.Y = gp_top_layer->rcArea.Bottom - gp_top_layer->rcArea.Top + 1;
    WriteConsoleOutput(gh_std_out, gp_top_layer->pContent, size, pos, &(gp_top_layer->rcArea));
    /*�ָ��ַ���Ԫԭ����*/
    pCh = gp_top_layer->pScrAtt;
    for (i=gp_top_layer->rcArea.Top; i<=gp_top_layer->rcArea.Bottom; i++)
    {
        for (j=gp_top_layer->rcArea.Left; j<=gp_top_layer->rcArea.Right; j++)
        {
            gp_scr_att[i*SCR_COL+j] = *pCh;
            pCh++;
        }
    }
    /*�ͷŶ�̬�洢��*/
    free(gp_top_layer->pContent);
    free(gp_top_layer->pScrAtt);
    free(gp_top_layer);
    gp_top_layer = nextLayer;
    gi_sel_sub_menu = 0;
    return;
}

/**< ���������ڻ��Ʊ߿�.*/
void DrawBox(SMALL_RECT *pRc)
{
    char chBox[] = {'+','-','|','+','+','+'};  /*�����õ��ַ�*/
    COORD pos = {pRc->Left, pRc->Top};  /*��λ����������Ͻ�*/
    WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &len);  /*���߿����Ͻ�#*/
    /*��ѭ�����ϱ߿����*/
	for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
        WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &len);
    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[3], 1, pos, &len);   /*���߿����Ͻ�~*/
    /*��ѭ�����߿�����ߺ��ұ���*/
	for (pos.Y = pRc->Top+1; pos.Y < pRc->Bottom; pos.Y++)
    {
        pos.X = pRc->Left;
        WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &len);
        pos.X = pRc->Right;
        WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &len);
    }
    pos.X = pRc->Left;
    pos.Y = pRc->Bottom;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[4], 1, pos, &len);   /*���߿����½�&*/
    /*���±߿����*/
	for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
        WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &len);
    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(gh_std_out, &chBox[5], 1, pos, &len);   /*���߿����½�%*/
    return;
}

/**< ��ָ���Ӳ˵���(���Ϊnum)����ѡ�б��.*/
void MarkSubMenu(int num)
{
    SMALL_RECT rcPop;
    COORD pos;
    WORD  att;
    int   width;
    LocateSubMenu(gi_sel_menu, &rcPop);/*���㵯���Ӳ˵�������λ��, �����rcPop��*/
    /*����Ӳ˵����Խ�磬������Ӳ˵��ѱ�ѡ�У��򷵻�*/
    if ((num < 1) || (num == gi_sel_sub_menu) || (num > rcPop.Bottom-rcPop.Top-1))
		return;
    pos.X = rcPop.Left + 2;
    width = rcPop.Right - rcPop.Left - 2;
    /*����ȡ��ԭѡ���Ӳ˵����ϵı��*/
    if (gi_sel_sub_menu != 0)
    {
        pos.Y = rcPop.Top + gi_sel_sub_menu;
        att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
        FillConsoleOutputAttribute(gh_std_out, att, width, pos, &len);
        pos.X += 1;
        att |=  FOREGROUND_RED|FOREGROUND_INTENSITY;     /*�׵�������*/
        FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &len);
    }
    /*���Ӳ˵�������ѡ�б��(�����������)*/
    pos.X = rcPop.Left + 2;
    pos.Y = rcPop.Top + num;
    att = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED|FOREGROUND_INTENSITY| BACKGROUND_BLUE| BACKGROUND_RED| BACKGROUND_INTENSITY;/*�����������*/
    FillConsoleOutputAttribute(gh_std_out, att, width, pos, &len);
    gi_sel_sub_menu = num;  /*�޸�ѡ���Ӳ˵����*/
    return;
}

/**< ���㵯���Ӳ˵�(���Ϊnum)�������ϽǺ����½ǵ�λ��.��Ϣ�洢������ΪSMALL_RECT�ṹ�ı���rc��*/
void LocateSubMenu(int num, SMALL_RECT *rc)
{
    int i, len, loc = 0;
    /*������ϱ߶��ڵ�2�У��к�Ϊ1*/
    rc->Top = 1;
    rc->Left = 1;
    /*����������߽�λ��, ͬʱ�����һ���Ӳ˵������Ӳ˵��ַ��������е�λ��*/
    for (i=1; i<num; i++)
    {
        rc->Left += strlen(ga_main_menu[i-1]) + 4;
        loc += ga_sub_menu_count[i-1];
    }
    rc->Right = strlen(ga_sub_menu[loc]);    /*��ʱ��ŵ�һ���Ӳ˵����ַ�������*/
    /*������Ӳ˵��ַ��������䳤�ȴ����rc->Right*/
    for (i=1; i<ga_sub_menu_count[num-1]; i++)
    {
        len = strlen(ga_sub_menu[loc+i]);
        if (rc->Right < len)    rc->Right = len;
    }
    rc->Right += rc->Left + 3;  /*����������ұ߽�*/
    rc->Bottom = rc->Top + ga_sub_menu_count[num-1] + 1;/*���������±ߵ��к�*/
    /*�ұ߽�Խ��Ĵ���*/
    if (rc->Right >= SCR_COL)
    {
        len = rc->Right - SCR_COL + 1;
        rc->Left -= len;
        rc->Right = SCR_COL - 1;
    }
    return;
}

/**< �����˵�����������.*/
void SetMainHotArea(void)
{
	int i,j;
	char aSort[5] = {0};
	char aTag[5] = {1,2,3,4,5};
	HOT_AREA areas;
	SMALL_RECT aArea[5] = {{2,0,8,0}, {12,0,20,0}, {24,0,30,0}, {34,0,40,0}, {44,0,54,0}};/*�������˵���������Χ*/
    areas.pArea = aArea;   /*ʹ�����ṹ����areas�ĳ�ԱpAreaָ������λ��������Ԫ��*/
    areas.pSort = aSort;   /*ʹ�����ṹ����areas�ĳ�ԱpSortָ���������������Ԫ��*/
    areas.pTag =  aTag;    /*ʹ�����ṹ����areas�ĳ�ԱpTag ָ���������������Ԫ��*/
    /*�������˵����������ַ���Ԫ���������ͺ��������*/
	for (i=0;i<5;i++)
	{
		for (j = areas.pArea[i].Left;j <= areas.pArea[i].Right;j++)
			gp_scr_att[j] |= (areas.pSort[i] << 6 )| (areas.pTag[i] << 2);
	}
}

/**< ����ϵͳ,��ϵͳ�����������û���ѡ��Ĺ��ܺ���*/
void RunSystem(void)
{
    INPUT_RECORD inRec;
    DWORD res;
    COORD pos = {0, 0};
    BOOL bRet = TRUE;
    int i, loc, num, p;
    int cNo, cAtt;      /*cNo:�ַ���Ԫ���, cAtt:�ַ���Ԫ����*/
    char vkc, asc;      /*vkc:���������, asc:�ַ���ASCII��ֵ*/
    SetMainHotArea();   /*�����˵�����������*/
    while(bRet)
    {  /*�ӿ���̨���뻺�����ж�һ����¼*/
        ReadConsoleInput(gh_std_in, &inRec, 1, &res);
        if (inRec.EventType == MOUSE_EVENT) /*�����¼������¼�����*/
        {
            pos = inRec.Event.MouseEvent.dwMousePosition;    /*��ȡ�������λ��*/
            cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3;   /*ȡ��λ�õĲ��*/
            cAtt = gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2; /*ȡ���ַ���Ԫ����*/
            if (cNo == 0)  /*���Ϊ0,������λ��δ�������Ӳ˵�����*/
            {
        /*cAtt>0������λ�ô�������,cAtt!=gi_sel_menu������λ�õ����˵���δ��ѡ��,gp_top_layer->LayerNo>0������ǰ���Ӳ˵�����*/
               if (cAtt > 0 && cAtt != gi_sel_menu && gp_top_layer->LayerNo > 0)
                {
                    PopOff();            /*�رյ������Ӳ˵�*/
                    gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
                }
				else  if(cAtt>0) MarkMainMenu(cAtt); /*������������˵�������ѡ�б��*/
            }
            else if (cAtt > 0) /*�������λ��Ϊ�����Ӳ˵��Ĳ˵����ַ���Ԫ*/
                MarkSubMenu(cAtt); /*�ڸ��Ӳ˵�������ѡ�б��*/
            if (inRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) /*�������������һ��*/
            {
                if (cNo==0) /*���Ϊ0��������λ��δ�������Ӳ˵�����*/
                {
					if (cAtt > 0) /*�����λ�ô�������*/
                        PopMenu(cAtt);   /*��������������˵����Ӧ���Ӳ˵�*/
                    else if (gp_top_layer->LayerNo > 0) /*�����λ�ò��������˵����ַ���Ԫ,�����Ӳ˵�����*/
                    {
                        PopOff();             /*�رյ������Ӳ˵�*/
                        gi_sel_sub_menu = 0;  /*��ѡ���Ӳ˵���������Ϊ0*/
                    }
                }
                else /*��Ų�Ϊ0,������λ�ñ������Ӳ˵�����*/
                {
                    if (cAtt > 0) /*�����λ�ô�������*/
                    {
                        PopOff();  /*�رյ������Ӳ˵�*/
                        gi_sel_sub_menu = 0;  /*��ѡ���Ӳ˵���������Ϊ0*/
                        bRet = ExeFunction(gi_sel_menu, cAtt); /*ִ�ж�Ӧ���ܺ���:gi_sel_menu���˵����,cAtt�Ӳ˵����*/
                    }
                }
            }
            else if (inRec.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) /*�����������Ҽ�*/
            {
                if (cNo == 0) /*���Ϊ0��������λ��δ�������Ӳ˵�����*/
                {
                    PopOff();            /*�رյ������Ӳ˵�*/
                    gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
                }
            }
        }
        else if (inRec.EventType == KEY_EVENT && inRec.Event.KeyEvent.bKeyDown) /*�����¼�ɰ��������Ҽ�������*/
        {
            vkc = inRec.Event.KeyEvent.wVirtualKeyCode;  /*��ȡ�������������*/
            asc = inRec.Event.KeyEvent.uChar.AsciiChar;  /*��ȡ������ASC��*/
            /*ϵͳ������ݼ�F1���Ĵ���*/
            if (vkc == 112) /*�������F1��*/
            {
                if (gp_top_layer->LayerNo != 0) /*�����ǰ���Ӳ˵�����*/
                {
                    PopOff();                   /*�رյ������Ӳ˵�*/
                    gi_sel_sub_menu = 0;        /*��ѡ���Ӳ˵���������Ϊ0*/
                }
                bRet = ExeFunction(5, 1);       /*���а������⹦�ܺ���*/
            }
            else if (inRec.Event.KeyEvent.dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED))
            { /*������������Alt��*/
                switch (vkc)  /*�ж���ϼ�Alt+��ĸ*/
                {
                    case 88:  /*Alt+X �˳�*/
                        if (gp_top_layer->LayerNo != 0)
                        {
                            PopOff();
                            gi_sel_sub_menu = 0;
                        }
                        bRet = ExeFunction(1,5);
                        break;
					case 67:  /*Alt+C ����*/
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
            else if (asc == 0) /*�������Ƽ��Ĵ���*/
            {
                if (gp_top_layer->LayerNo == 0) /*���δ�����Ӳ˵�*/
                {
                    switch (vkc) /*�������(���ҡ���),����Ӧ�������Ƽ�*/
                    {
                        case 37:  /*���*/
                            gi_sel_menu--;
                            if (gi_sel_menu == 0)  gi_sel_menu = 5;
                            MarkMainMenu(gi_sel_menu);
                            break;
                        case 39: /*�Ҽ�*/
                            gi_sel_menu++;
                            if (gi_sel_menu == 6)  gi_sel_menu = 1;
                            MarkMainMenu(gi_sel_menu);
                            break;
                        case 40: /*�¼�*/
                            PopMenu(gi_sel_menu);
                            MarkSubMenu(1);
                            break;
                    }
                }
                else  /*�ѵ����Ӳ˵�ʱ*/
                {
                    /*������Ӳ˵��еĵ�һ�����Ӳ˵��ַ��������е�λ��(�±�)*/
                    for (loc=0,i=1; i<gi_sel_menu; i++)
                        loc += ga_sub_menu_count[i-1];
                    switch (vkc) /*�����(���ҡ��ϡ���)�Ĵ���*/
                    {
                        case 37: /*���*/
                            gi_sel_menu--;
                            if (gi_sel_menu < 1)   gi_sel_menu = 5;
                            MarkMainMenu(gi_sel_menu);
                            PopOff();
                            PopMenu(gi_sel_menu);
                            MarkSubMenu(1);
                            break;
                        case 38: /*�ϼ�*/
                            num = gi_sel_sub_menu - 1;
                            if (num < 1)    num = ga_sub_menu_count[gi_sel_menu-1];
                            if (strlen(ga_sub_menu[loc+num-1]) == 0)    num--;
                            MarkSubMenu(num);
                            break;
                        case 39:  /*�Ҽ�*/
                            gi_sel_menu++;
                            if (gi_sel_menu > 5)     gi_sel_menu = 1;
                            MarkMainMenu(gi_sel_menu);
                            PopOff();
                            PopMenu(gi_sel_menu);
                            MarkSubMenu(1);
                            break;
                        case 40: /*�¼�*/
                            num = gi_sel_sub_menu + 1;
                            if (num > ga_sub_menu_count[gi_sel_menu-1])  num = 1;
                            if (strlen(ga_sub_menu[loc+num-1]) == 0)     num++;
                            MarkSubMenu(num);
                            break;
                    }
                }
            }
            else if ((asc-vkc == 0) || (asc-vkc == 32)){  /*�������ּ�������ĸ������ͨ��*/
                if (gp_top_layer->LayerNo == 0)  /*���δ�����Ӳ˵�*/
                {
                    switch (vkc)
                    {
                        case 70: /*f��F*/
                            PopMenu(1);
                            break;
                        case 69: /*e��E*/
                            PopMenu(2);
                            break;
                        case 73: /*i��I*/
                            PopMenu(3);
                            break;
                        case 83: /*s��S*/
                            PopMenu(4);
                            break;
                        case 72: /*h��H*/
                            PopMenu(5);
                            break;
                        case 13: /*�س�*/
                            PopMenu(gi_sel_menu);
                            MarkSubMenu(1);
                            break;
                    }
                }
                else /*�ѵ����Ӳ˵�ʱ�ļ������봦��*/
                {
                    if (vkc == 27) /*�������ESC��*/
                    {
                        PopOff();
                        gi_sel_sub_menu = 0;
                    }
                    else if(vkc == 13) /*������»س���*/
                    {
                        num = gi_sel_sub_menu;
                        PopOff();
                        gi_sel_sub_menu = 0;
                        bRet = ExeFunction(gi_sel_menu, num);
                    }
                    else /*������ͨ���Ĵ���*/
                    {
                        /*������Ӳ˵��еĵ�һ�����Ӳ˵��ַ��������е�λ��(�±�)*/
                        for (loc=0,i=1; i<gi_sel_menu; i++)
                             loc += ga_sub_menu_count[i-1];
                        /*�����뵱ǰ�Ӳ˵���ÿһ��Ĵ����ַ����бȽ�*/
                        for (i=loc; i<loc+ga_sub_menu_count[gi_sel_menu-1]; i++)
                        {
                            if (strlen(ga_sub_menu[i])>0 && vkc==ga_sub_menu[i][1])
                            { /*���ƥ��ɹ�*/
                                PopOff();
                                gi_sel_sub_menu = 0;
                                bRet = ExeFunction(gi_sel_menu, i-loc+1); /*ִ������ĸ���Ӧ�ĺ���*/
                            }
                        }
                    }
                }
            }
        }
    }
}

/**< ִ�������˵���Ϊm���Ӳ˵���Ϊn��ȷ���Ĺ��ܺ���*/
BOOL ExeFunction(int m, int n)
{
    BOOL bRet = TRUE;
    /*����ָ�����飬����������й��ܺ�������ڵ�ַ*/
    BOOL (*pFunction[24])(void);
    int i, loc;
    /*�����ܺ�����ڵ�ַ�����빦�ܺ����������˵��ź��Ӳ˵��Ŷ�Ӧ�±������Ԫ��*/
    pFunction[0] =  SaveData;
    pFunction[1] =  LoadData;
    pFunction[2] =  ExitSys;
    /*������Ϣ����ά��*/
    pFunction[3] =  InsertDeveloper;
    pFunction[4] =  ModifyDeveloper;
    pFunction[5] =  DelDeveloper;
    pFunction[6] =  NULL;
    /*��ͣ������Ϣ9��ά��*/
    pFunction[7] =  InsertFlat;
    pFunction[8] =  ModifyFlat;
    pFunction[9] = DelFlat;
    pFunction[10] =  NULL;
    /*��ͣ������Ϣ����ά��*/
    pFunction[11] =  InsertSale;
    pFunction[12] =  ModifySale;
    pFunction[13] =  DelSale;
    /*��Ϣ��ѯ*/
    pFunction[14] =  DeveloperInquire;
    pFunction[15] =  FlatInquire;
    pFunction[16] =  SaleInquire;
    pFunction[17] =  InquireAuto;
    /*��Ϣͳ��*/
    pFunction[18] = DevStatistics;
    pFunction[19] = YearStatistics;
    pFunction[20] = SmanStatistics;
    pFunction[21] = AreaStatistics;
    pFunction[22] = AboutSys;
	pFunction[23] = Clear;
    /*�������˵��ź��Ӳ˵��ż����Ӧ�±�*/
    for (i=1,loc=0; i<m; i++)
        loc += ga_sub_menu_count[i-1];
    loc += n - 1;
    /*�ú���ָ�������ָ��Ĺ��ܺ���*/
    if(pFunction[loc]!=NULL)  
	    bRet = (*pFunction[loc])();
    return bRet;
}

/**< �����˳�ϵͳʱ�������Ϣ*/
int  DealQuitInput(HOT_AREA *pHotArea, int *piHot)
{
    INPUT_RECORD inRec;
    DWORD res;
    COORD pos = {0, 0};
    int num, arrow, iRet = 0;
    int cTag, cAtt;/*cNo:���, cTag:�������, cAtt:�ַ���Ԫ����*/
    char vkc, asc; /*vkc:���������, asc:�ַ���ASCII��ֵ*/
    SetQuitHotArea(pHotArea, 1);/*��ʼ���"ȷ��"*/
    while (1)
    { /*ѭ��*/
        ReadConsoleInput(gh_std_in, &inRec, 1, &res);
        if (inRec.EventType == MOUSE_EVENT)/*�����¼������¼�����*/
        {
            pos = inRec.Event.MouseEvent.dwMousePosition;/*��ȡ�������λ��*/
            cTag = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2) & 15;/*��ȡ��λ�õ��������*/
            cAtt =  gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2; /*��ȡ��λ���ַ���Ԫ������*/
            if ( cAtt>0 )/*r����������λ�ô�������*/
            {
                *piHot = cTag;
                SetQuitHotArea(pHotArea, *piHot);/*���������ʾ���*/
            }
            if (inRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && cAtt>0 )
            { /*�������������һ���Ҵ�ʱ��λ�ô�������*/
                iRet = 1;
                break;
            }
        }
        else if (inRec.EventType == KEY_EVENT && inRec.Event.KeyEvent.bKeyDown)
        { /*�����¼�ɰ�������*/
            vkc = inRec.Event.KeyEvent.wVirtualKeyCode;/*��ȡ�������������*/
            asc = inRec.Event.KeyEvent.uChar.AsciiChar;/*��ȡ������ASC��*/
            if (asc == 0)
            {
                arrow = 0;
                switch (vkc)
                {  /*�����(���ϡ��ҡ���)�Ĵ���*/
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
            {  /*ESC����N,n�� == ȡ��*/
                iRet = 0;
                break;
            }
            else if (vkc == 13 || vkc == 89)
            {  /*�س�����Y,y�� == ȷ��*/
                iRet = 1;
                break;
            }
        }
    }
    return iRet;
}

/**< �����˳�ϵͳʱ������*/
void SetQuitHotArea(HOT_AREA *pHotArea, int iHot)
{
    CONSOLE_CURSOR_INFO lpCur;
    COORD pos = {0, 0};
    WORD att1, att2;
    int i, width;
    att1 = BACKGROUND_GREEN | BACKGROUND_INTENSITY;  /*����װ���(att1��ʾѡ��ѡ����ַ�����)*/
    att2 = BACKGROUND_BLUE  | BACKGROUND_GREEN | BACKGROUND_RED;/*�׵׺���att2(��ʾδѡ��ѡ����ַ�����)*/
    for (i=0; i<pHotArea->num; i++)
    {  /*����ť��������Ϊ�׵׺���*/
        pos.X = pHotArea->pArea[i].Left;
        pos.Y = pHotArea->pArea[i].Top;
        width = pHotArea->pArea[i].Right - pHotArea->pArea[i].Left + 4;
        if (pHotArea->pSort[i] == 0)  /*�����ǰ�ť��*/
            FillConsoleOutputAttribute(gh_std_out, att2, width, pos, &len);
    }
    pos.X = pHotArea->pArea[iHot-1].Left;
    pos.Y = pHotArea->pArea[iHot-1].Top;
    width = pHotArea->pArea[iHot-1].Right - pHotArea->pArea[iHot-1].Left + 4;
    if (pHotArea->pSort[iHot-1] == 0) /*�����������ǰ�ť��*/
        FillConsoleOutputAttribute(gh_std_out, att1, width, pos, &len);
}

/**< ����������ݴ���*/
BOOL SaveData(void)
{
	BOOL bRet = TRUE;
	ClearScreen();
	LoadList(&gp_head,FileName);
	SaveList(gp_head, FileName);
    return bRet;
}

/**< ����������ݴ���*/
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
	printf("[��������ѱ���!]\n");
}

/**< �����ݼ���*/
BOOL LoadData(void)
{
    BOOL bRet=TRUE;
    ClearScreen();
    LoadList(&gp_head,FileName);   /*����������Ϣ*/
    printf("���سɹ���\n");
    return bRet;
}

/**< �����ļ��е���Ϣ��*/
BOOL LoadList(DEVELOPER_NODE  **phead,char *fn[3])
{
	DEVELOPER_NODE *p1,input_info1, *pre_p1=NULL;
	FLAT_NODE *p2, input_info2;
	SALE_NODE *p3, input_info3;
	FILE *pfile1, *pfile2, *pfile3;
	
    if ((pfile1=fopen(fn[0],"rb")) == NULL)
    {
        printf("��Ϣ��ʧ��\n");
        return 0;
    }
    printf("��Ϣ�򿪳ɹ�\n");
    
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
        printf("��������Ϣ����ʧ��\n");
        return 0;
    }
    printf("��������Ϣ���سɹ�\n");
    if ((pfile2=fopen(fn[1],"rb")) == NULL)
    {
        printf("¥����Ϣ����ʧ��\n");
        return 0;
    }
    printf("¥����Ϣ���سɹ�\n");
    
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
        printf("������Ϣ��ʧ��\n");
        return 0;
    }
    printf("������Ϣ�򿪳ɹ�\n");
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

/**< �˳�ϵͳ*/
BOOL ExitSys(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT rcPop,aArea[2];
    COORD pos,aLoc[2];
    WORD att;
    char *pCh[] = {" ��ȷ���˳�ϵͳ����", "ȷ��(Y)  ȡ��(N)"};
    int iHot = 1;
    char ch , aSort[] = {0, 0}, aTag[] = {1, 2};
	ClearScreen();/*�������Ļ*/
    pos.X = strlen(pCh[0]) +7;
    pos.Y = 8;
    rcPop.Left = (SCR_COL - pos.X) / 2;
    rcPop.Right = rcPop.Left + pos.X - 1;
    rcPop.Top = (SCR_ROW - pos.Y) / 2;
    rcPop.Bottom = rcPop.Top + pos.Y - 1;
    att = FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;/*�׵�������*/
    labels.num = 2;
    labels.ppLabel = pCh;
    /*���ð�ťλ������*/
    aLoc[0].X = rcPop.Left+3;
    aLoc[0].Y = rcPop.Top+2;
    aLoc[1].X = rcPop.Left+5;
    aLoc[1].Y = rcPop.Top+5;
    labels.pLoc = aLoc;
    areas.num = 2;
    /*����������Χ*/
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
    PopOn(&rcPop, att, &labels, &areas);/*��ָ�������������������Ϣ������Ϣ��ջ,�����ȼ�*/
    /*��һ���ָ���*/
    pos.X = rcPop.Left + 1;
    pos.Y = rcPop.Top + 4;
    FillConsoleOutputCharacter(gh_std_out, '*', rcPop.Right-rcPop.Left-1, pos, &len);
    if (DealQuitInput(&areas, &iHot) == 1 && iHot == 1)
        bRet = FALSE;
    else  bRet = TRUE;
    PopOff();/*�رյ�������*/
    return bRet;
}

/**< ���뿪������Ϣ*/
BOOL InsertDeveloper(void)
{
    BOOL bRet=TRUE;
    int iRet;
    ClearScreen();
    iRet=InsertDeveloperNode(&gp_head);
    if(iRet== 1)       
	    printf("��������Ϣ����ɹ�!\n");
    else if(iRet == -1)  
	    printf("�˿������Ѵ��ڣ�����ʧ�ܣ�\n");
	SaveList(gp_head, FileName);
    return bRet;
}

/**< ���뿪���̽��*/
int InsertDeveloperNode(DEVELOPER_NODE **phead)
{
    DEVELOPER_NODE *p1 ,*p=(*phead); 
    COORD pos={7,2};
    p1 = (DEVELOPER_NODE *)malloc(sizeof(DEVELOPER_NODE));
	printf("���������� ���������� ���������[3��]\n");
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

/**< �޸���Ϣ*/
BOOL ModifyDeveloper(void)
{
    DEVELOPER_NODE *p1;
    char findname[30];
    COORD pos={0,4};
    ClearScreen();

    printf("��������Ҫ�޸Ŀ���������:");
    scanf("%s",findname); getchar();
    printf("���޸ĸÿ����̵���Ϣ��\n");

    p1 = (DEVELOPER_NODE *)malloc(sizeof(DEVELOPER_NODE));
	printf("���������� ���������� ���������[3��]\n");
	p1->fnext=NULL;
    scanf("%s",p1->name);  getchar();pos.X+=11;Gotoxy(pos);
    scanf("%d",&p1->num);   getchar();pos.X+=11;Gotoxy(pos);
    scanf("%f",&p1->area);  getchar();
    if(ModifyDevNode(gp_head,findname,p1)==1) printf("�޸ĳɹ�!\n");
    else printf("�ÿ����̲����ڣ��޸�ʧ��!\n");
	SaveList(gp_head, FileName);/*������Ϣ*/
    return TRUE;
}

/**< �޸ĺ�����*/
int  ModifyDevNode(DEVELOPER_NODE *phead,char *dev_name,DEVELOPER_NODE *pdev_node)
{
    DEVELOPER_NODE *pdev_temp = phead;
    DEVELOPER_NODE *pdev_next;
    int   bRet=0;
    while (pdev_temp != NULL && strcmp(pdev_temp -> name,dev_name) != 0)
        pdev_temp = pdev_temp ->next;   /*Ѱ�ҷ��������ĺ�����Ϣ���*/
    if (pdev_temp != NULL)
    {
        pdev_next = pdev_temp ->next;
        *pdev_temp = *pdev_node; /*ָ�봫ַ*/
        pdev_temp -> next = pdev_next;
        bRet= 1;
    }
    return bRet;
}

/**< ɾ����������Ϣ*/
BOOL DelDeveloper(void)
{
    char dev_name[30];
    ClearScreen();/*ˢ��*/

    printf("������Ҫɾ���Ŀ��������ƣ�");
    scanf("%s",dev_name); getchar();
    if(DelDevNode(&gp_head,dev_name) == 1) printf("ɾ���ɹ�!\n");
    else  printf("�޳ܿ����̣�\n");
	SaveList( gp_head, FileName);/*������Ϣ*/
    return TRUE;
}

/**< ɾ�������̽��*/
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
        if (pdev_prior == NULL) /*����ͷŵ��Ǻ���������ͷ���*/
        {
            (*phead) = pdev_current -> next;
        }
        else
        {
            pdev_prior -> next = pdev_current -> next;
        }
        pfla_current = pdev_current -> fnext;
        free(pdev_current); /*�ͷ�Ҫɾ���Ŀ����̽��*/
        while (pfla_current != NULL)
        {
            psal_current = pfla_current->snext;
            pfla_prior = pfla_current;
            while (psal_current != NULL)
            {
                psal_prior =psal_current;
                psal_current = psal_current ->next;
                free(psal_prior);/*�ͷŸÿ���������¥�̸������۽ڵ���Ϣ*/ 
            }
            pfla_current = pfla_current ->next;
            free(pfla_prior);/*�ͷŸ�¥�����¸��������̽����Ϣ*/
        }
    }
    return bRet;
}

/**< ����¥����Ϣ*/
BOOL InsertFlat(void)
{
    BOOL bRet=TRUE;
    int iRet;
    ClearScreen();
    iRet=InsertflatNode(gp_head);
    if(iRet== 1)       
	    printf("¥����Ϣ����ɹ�!\n");
    else if (iRet == -1)  
	    printf("��¥���Ѵ��ڣ�����ʧ�ܣ�\n");
    else  
	    printf("��¥������������δ¼��ϵͳ������ʧ�ܣ�\n");
	SaveList(gp_head, FileName);
    return bRet;
}

/**< ����¥�̽��*/
int InsertflatNode(DEVELOPER_NODE *phead)
{
    DEVELOPER_NODE *pdev_node;
    FLAT_NODE *pfla_temp,*p2;
    int find = 0;
    COORD pos={0,2};
    printf("¥������ ������ �������� ¥�̵�ַ ռ����� �������� ������� ÿƽ�׾��� ��ϵ�� ��ϵ�绰[10��]\n");
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
        find = -1;  /*��¥���Ѵ���*/
    }
    else   /*��Ҫ�����¥�̲�����,������뵽��Ӧ��¥������*/
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
                 break;/*�˳���һ��ѭ��*/
            }
            pfla_node = pfla_node ->next;
        }
        if (find)   
		    break;/*�˳��ڶ���ѭ��*/
        pdev_node = pdev_node ->next;
    }
    if (find)
        return pfla_node;
    else
        return NULL;
}

/**< �޸�¥����Ϣ*/
BOOL ModifyFlat(void)
{
    BOOL bRet=TRUE;
    char findname[30];
    FLAT_NODE *p2;
    COORD pos={0,4};
    ClearScreen();

    printf("������Ҫ�޸ĵ�¥�̣�");
    scanf("%s",findname);
    getchar();
    printf("����������¥����Ϣ.\n");
    printf("¥������ ������ �������� ¥�̵�ַ ռ����� �������� ������� ÿƽ�׾��� ��ϵ�� ��ϵ�绰[10��]\n");
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
    if(ModifyFlatNode(gp_head,findname,p2) == 1) printf("�޸ĳɹ���\n");
    else  { printf("δ�ҵ����������̣��޸�ʧ�ܣ�\n"); free(p2); }
	SaveList(gp_head, FileName);
    return bRet;

}

/**< �޸ľ�ͣ�������*/
int ModifyFlatNode(DEVELOPER_NODE *phead,char *name,FLAT_NODE *pfla_node)
{
    FLAT_NODE *pfla_temp;
    FLAT_NODE *pfla_next;
    int bRet=0;

    pfla_temp = SeekFlatNode(phead,name);/*���Ҹ�¥��*/
    if (pfla_temp != NULL)
    {
        pfla_next  = pfla_temp -> next;
        *pfla_temp = *pfla_node;
        pfla_temp -> next = pfla_next;
        bRet = 1;
    }
    return bRet;
}

/**< ɾ����ͣ������Ϣ*/
BOOL DelFlat(void)
{
    BOOL bRet=TRUE;
    char findname[30];
    ClearScreen();

    printf("������Ҫɾ����¥�̣�");
    scanf("%s",findname); getchar();
    if (DelFlatNode(gp_head,findname) == 1)  
	    printf("ɾ���ɹ�!\n");
    else printf("δ�ҵ���¥�̣�ɾ��ʧ�ܣ�\n");
	SaveList(gp_head, FileName);
    return bRet;
}

/**< ɾ��¥�̽��*/
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
        if (pfla_prior == NULL)   /*ɾ������¥��������ͷ*/
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

/**< ���뷿��������Ϣ*/
BOOL InsertSale(void)
{
    BOOL bRet=TRUE;
    SALE_NODE *p3;
    COORD pos={0,2};
    ClearScreen();

    printf("�ͻ����� ���֤�� �������� ���� ����¥�� ¥���� ������� ÿƽ�׵��� �ۿ� ʵ�ʽ��׽�� ����Ա[11��]\n");
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
	    printf("����ɹ���\n");
    else if (InsertSaleNode(gp_head,p3) == -1)
	    printf("��������Ϣ�Ѵ���,����ʧ�ܣ�\n");
    else 
	    printf("��������Ϣ��Ӧ¥�̲����ڣ����߸�������Ϣ�����ڣ�����ʧ�ܣ�\n");
	SaveList(gp_head, FileName);
    return bRet;
}

/**< ���뷿�����۽��*/
int InsertSaleNode(DEVELOPER_NODE *phead, SALE_NODE *psal_node)
{
    FLAT_NODE *pfla_node;
    SALE_NODE *psal_temp;
    int bRet=0;
    pfla_node = SeekFlatNode(phead, psal_node ->name_of_flat);
    psal_temp = SeekSaleNode(phead,psal_node ->name_of_flat,psal_node ->name);

    if (psal_temp != NULL)
    {
        bRet = -1;/*�÷���������Ϣ�Ѵ���*/
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

/**< ���ҷ���������Ϣ���*/
SALE_NODE *SeekSaleNode(DEVELOPER_NODE *phead,char *name1, char *e_num)
{
    FLAT_NODE *pfla_node;
    SALE_NODE *psal_node;
    int find = 0;
    pfla_node = SeekFlatNode(phead,name1);/*���Ȳ��Ҷ�Ӧ¥����Ϣ���*/
    /*����ҵ������ڷ�������֧���ϼ�����ָ���ķ���������Ϣ���*/
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

/**< �޸ķ���������Ϣ*/
BOOL ModifySale(void)
{
    SALE_NODE *p3;
    BOOL   bRet=TRUE;
    COORD pos={0,5};
    char flatname[30],salename[20];

    ClearScreen();

    printf("��������Ҫ�޸ĵ� ¥���� �� �ͻ���\n");
    scanf("%s %s",flatname,salename); getchar();
    printf("�����������������Ϣ.\n");
    printf("�ͻ����� ���֤�� �������� ���� ����¥�� ¥���� ������� ÿƽ�׵��� �ۿ� ʵ�ʽ��׽�� ����Ա[11��]\n");
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
	    printf("�޸ĳɹ���\n");
    else  
	    printf("�޸�ʧ�ܣ�\n");
 	SaveList(gp_head, FileName);
    return bRet;
}

/**< �޸ķ������۽��*/
int ModifySaleNode(DEVELOPER_NODE *phead,char *flatname, char *e_num, SALE_NODE *psal_node)
{
    SALE_NODE *psal_node_temp;
    SALE_NODE *psal_node_next;
    int bRet=0;

    psal_node_temp = SeekSaleNode(phead,flatname,e_num);  /*�������۽��*/

    if (psal_node_temp != NULL)                    /*����ҵ���������޸�*/
    {
        psal_node_next = psal_node_temp ->next;    /*��ֵǰ����ָ����*/
        *psal_node_temp = *psal_node;
        psal_node_temp -> next = psal_node_next;   /*�ָ�ָ���ϵ*/
        bRet= 1;
    }
    return bRet;
}

/**< ɾ��������Ϣ*/
BOOL DelSale(void)
{
    BOOL bRet;
    char flatname[30],salename[20];
    ClearScreen();
    printf("����������Ҫɾ���� ¥���� �� �ͻ���\n");
    scanf("%s%s",flatname,salename);getchar();
    if (DelSaleNode(gp_head,flatname,salename) == 1) 
	    printf("ɾ���ɹ�!\n");
    else  
	    printf("��������Ϣ�����ڣ�ɾ��ʧ�ܣ�\n");
	SaveList(gp_head, FileName);
    return bRet;
}

/**< ɾ��������Ϣ���*/
int DelSaleNode(DEVELOPER_NODE *phead,char *flatname, char *e_num)
{
    FLAT_NODE *pfla_node;
    SALE_NODE *psal_prior;
    SALE_NODE *psal_current;
    int  bRet = 0;

    pfla_node = SeekFlatNode(phead,flatname);  /*�������������ҵ�¥����Ϣ���*/

    if (pfla_node != NULL)  /*����ҵ�*/
    { /*��¥�����ϲ���������Ϣ���*/
        psal_prior = NULL;
        psal_current = pfla_node ->snext;
        while (psal_current != NULL && strcmp(psal_current ->name,e_num))
        {
            psal_prior = psal_current;
            psal_current = psal_current ->next;
        }
        if (psal_current != NULL)    /*����ҵ�*/
        {
            bRet = 1;
            if (psal_prior == NULL) /*���λ��������Ϣ֧���ĵ�һ�����*/
                pfla_node ->snext = psal_current ->next;
            else   /*���λ��������Ϣ֧�����������*/
                psal_prior ->next = psal_current ->next;
            free(psal_current);
        }
    }
    return bRet;
}
 
/**< �������̲�ѯ*/
BOOL DeveloperInquire(void)
{
	int flag=0;
	int max=0,min=1; 
	BOOL bRet = TRUE;
	char name[30];
	ClearScreen();
	printf("��ѡ���ѯ��ʽ  ��1��������������  ��2������������"); 
	scanf("%d",&flag);
	if (flag==1)
	{
	    printf("��������Ҫ��ѯ�Ŀ�������:");
	    scanf("%s",name);
	    getchar();
	}
	else if (flag==2)
	{
		printf("���������������\t");
		scanf("%d",&max);
		printf("��������С��������\t");
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
            printf("�ҵ����[������]����Ϣ����:\n");Delay();
		    printf("\t\t������������:%s\t\n",p1->name);Delay();
		    printf("\t\t������������:%d\t\n",p1->num);Delay();
		    printf("\t\t�����������:%f\t\n",p1->area);
        }
        p1=p1->next;
    }
    if (find==0) printf("��%28sδ�����������Ϣ��%28s��\n"," "," ");
    printf("����������Ϣ��������!\n");
}

/**< ��¥�̲�ѯ*/
BOOL FlatInquire(void)
{
	int flag=0;
	int max=0,min=1; 
	BOOL bRet = TRUE;
	char name[30];
	ClearScreen();
	printf("��ѡ���ѯ��ʽ  ��1����¥����  ��2����ÿƽ�׵���"); 
	scanf("%d",&flag);
	if (flag==1)
	{
	    printf("��������Ҫ��ѯ��¥����:");
	    scanf("%s",name);
	    getchar();
	}
	else if (flag==2)
	{
		printf("���������ÿƽ�׵���\t");
		scanf("%d",&max);
		printf("��������Сÿƽ�׵���\t");
		scanf("%d",&min);
	} 
	LoadList(&gp_head,FileName);
	InquireFlat(gp_head,name,max,min);
	return bRet;
}

/**< ��¥�̲�ѯ*/
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
                printf("�ҵ����[¥��]����Ϣ����:\n");Delay();
                printf("\t\t��¥������:%s\t\n",p2->name);Delay();
                printf("\t\t��������:%s\t\n",p2->name_of_dev);Delay();
                printf("\t\t����������:%s\t\n",p2-> date_of_start);Delay();
                printf("\t\t��¥�̵�ַ:%s\t\n",p2-> location);Delay();
                printf("\t\t��ռ�����:%f\t\n",p2-> floor_area);Delay();
                printf("\t\t����������:%d\t\n",p2-> num_of_building);Delay();
                printf("\t\t���������:%f\t\n",p2-> structure_area);Delay();
                printf("\t\t��ÿƽ�׾���:%f\t\n",p2-> ave_price);Delay();
                printf("\t\t����ϵ��:%s\t\n",p2-> person);Delay();
                printf("\t\t����ϵ�绰:%s\t\n",p2-> tele_num);Delay();
                break; 
             }
             p2=p2->next;
        }
        if( find==1) 
		    break;
        p1=p1->next;
    }
    if (find==0)  
	    printf("��%28sδ�����������Ϣ��%28s��\n"," "," ");
    printf("��¥������������!\n");
}

/**< ��������Ϣ��ѯ*/
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
	printf("��ѡ���ѯ��ʽ  ��1�������֤�ţ�����¥�̣�¥���Ų�ѯ  ��2���������ںͿͻ�����"); 
	scanf("%d",&flag);
	if (flag==1)
	{
	    printf("��������Ҫ��ѯ�����֤��:\t");
	    scanf("%s",id_num);
	    getchar();
	    printf("����������¥��:\t");
		scanf("%s",name_of_flat);
		getchar();
		printf("������¥����:\t");
		scanf("%s",build_id);
		getchar();
		LoadList(&gp_head,FileName);
	    InquireSale1(gp_head,id_num,name_of_flat,build_id);
	    return bRet;	
	}
	else if (flag==2)
	{
		printf("������ͻ�����:\t");
		scanf("%s",name);
		getchar();
		printf("��������������:\t");
		scanf("%s",date_a);
		getchar();
		printf("��������������:\t");
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
                    printf("�ҵ����[��������]����Ϣ����:\n");Delay();
                    printf("\t\t���ͻ�����:%s\t\n",p3->name);Delay();
                    printf("\t\t�����֤��:%s\t\n",p3->id_num);Delay();
                    printf("\t\t����������:%s\t\n",p3-> date);Delay();
                    printf("\t\t������:%s\t\n",p3-> type);Delay();
                    printf("\t\t������¥��:%s\t\n",p3->  name_of_flat);Delay();
                    printf("\t\t��¥����:%d\t\n",p3-> build_id);Delay();
                    printf("\t\t���������:%f\t\n",p3-> structure_area);Delay();
                    printf("\t\t��ÿƽ�׵���:%f\t\n",p3-> price);Delay();
                    printf("\t\t���ۿ�:%f\t\n",p3-> dec);Delay();
                    printf("\t\t��ʵ�ʽ��׽��:%f\t\n",p3-> real_price);Delay();
                    printf("\t\t������Ա:%s\t\n",p3-> saleman);Delay();
				}
			} 
	    }
	}
	if (flag==0) 
	    printf("���޴ζ�Ӧ���Ѽ�¼"); 
	printf("��¥������������!\n");	
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
                    printf("�ҵ����[��������]����Ϣ����:\n");Delay();
                    printf("\t\t���ͻ�����:%s\t\n",p3->name);Delay();
                    printf("\t\t�����֤��:%s\t\n",p3->id_num);Delay();
                    printf("\t\t����������:%s\t\n",p3-> date);Delay();
                    printf("\t\t������:%s\t\n",p3-> type);Delay();
                    printf("\t\t������¥��:%s\t\n",p3->  name_of_flat);Delay();
                    printf("\t\t��¥����:%s\t\n",p3-> build_id);Delay();
                    printf("\t\t���������:%f\t\n",p3-> structure_area);Delay();
                    printf("\t\t��ÿƽ�׵���:%f\t\n",p3-> price);Delay();
                    printf("\t\t���ۿ�:%f\t\n",p3-> dec);Delay();
                    printf("\t\t��ʵ�ʽ��׽��:%f\t\n",p3-> real_price);Delay();
                    printf("\t\t������Ա:%s\t\n",p3-> saleman);Delay();
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
	    printf("��%28sδ�����������Ϣ��%28s��\n"," "," ");
	printf("��¥������������!\n");	
}

/**< ģ����ѯ*/
BOOL InquireAuto(void)
{
	BOOL bRet = TRUE;
	char KeyWord[30];
	int i=0;
	ClearScreen();
	LoadList(&gp_head,FileName);
    printf("����������Ҫ��ѯ��Ϣ�Ĺؼ���:");
    scanf("%s",KeyWord);
    getchar();
	AutoInquire(gp_head,KeyWord);
	return bRet;
}

/**< ��s���в���t��*/
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

/**< ģ����ѯ*/
void AutoInquire(DEVELOPER_NODE *phead, char kw[])
{
    DEVELOPER_NODE         * p1;
    FLAT_NODE   * p2;
    SALE_NODE    * p3;
    int  find=0,lg;
    COORD pos={5,7};
    printf("\n����ѯ�����\n");Delay();
    for (p1=phead;p1!=NULL;p1=p1->next)
    {
        for (p2=p1->fnext;p2!=NULL;p2=p2->next)
        {
            if (MyStrstr(p2->name,kw))
              {
                printf("�ҵ����[¥��]����Ϣ����:\n");Delay();
                printf("\t\t��¥������:%s\t\n",p2->name);Delay();
                printf("\t\t��������:%s\t\n",p2->name_of_dev);Delay();
                printf("\t\t����������:%s\t\n",p2-> date_of_start);Delay();
                printf("\t\t��¥�̵�ַ:%s\t\n",p2-> location);Delay();
                printf("\t\t��ռ�����:%f\t\n",p2-> floor_area);Delay();
                printf("\t\t����������:%d\t\n",p2-> num_of_building);Delay();
                printf("\t\t���������:%f\t\n",p2-> structure_area);Delay();
                printf("\t\t��ÿƽ�׾���:%f\t\n",p2-> ave_price);Delay();
                printf("\t\t����ϵ��:%s\t\n",p2-> person);Delay();
                printf("\t\t����ϵ�绰:%s\t\n",p2-> tele_num);Delay();
                pos.Y++;
                find=1;
                break;
              }
            if (MyStrstr(p1->name,kw))
                {
                    printf("�ҵ����[������]����Ϣ����:\n");Delay();
		            printf("\t\t������������:%s\t\n",p1->name);Delay();
		            printf("\t\t������������:%d\t\n",p1->num);Delay();
		            printf("\t\t�����������:%f\t\n",p1->area);
                    pos.Y++;
                    find=1;
                    break;
                }
        }
        if(find==1) 
		    break;
    }
    if (find==0)  
	    printf("��%28sδ�����������Ϣ��%28s��\n"," "," ");
    return ;
}

/**< ������ͳ��*/
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
    printf("\n%25s��������ͳ�ƽ����\n"," ");Delay();
    printf("  |����������|¥������|ռ�������|����������|�ܽ������ |\n");
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

/**< ϵͳʹ��˵��*/
BOOL AboutSys(void)
{
	BOOL bRet = TRUE;
	ClearScreen();
	printf("\n\t\t   ��** Programming Instructions **��\n");Delay();
    printf(" \t������������������������������������������������������������\n");Delay();
	printf(" \t��   ��ϵͳ����DevC++��CODE:BLOCKS������;                 ��\n");Delay();
	printf(" \t��   ϵͳ֧�ֿ�ݼ�����,�򵥼��̲���;                     ��\n");Delay();
	printf(" \t��   �����������ʮ�ֽ�����������Ϣ;                    ��\n");Delay();
	printf(" \t��   ��������,ָ��,�ṹ�����ϼ��ļ��������,����̨��֪ʶ����\n");Delay();
	printf("    ��������������������������������������������������������������������������������\n");Delay();
	printf("    �����ο����ס�:[1]�ܼƲ�,¬Ƽ,�. C���Գ������,��������ѧ������,2013;      ��\n");Delay();
	printf("    �� [2]�,¬Ƽ,�ܼƲ�. C����ʵ����γ����,��������ѧ������,2011;            ��\n");
    printf("    ��������������������������������������������������������������������������������\n");
    return bRet;
}

/**< �����Ļ��Ϣ*/
BOOL Clear(void)
{
	BOOL bRet=TRUE;
	ClearScreen();  /*�����Ļ�ϵ���Ϣ*/
	return TRUE;
}

/**< �����Ϣͳ��*/
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
	printf("\t\t��������Ҫ���ҵ���ݡ�������");
	scanf("%s",year);
	printf("  |����������|¥������|ռ�������|����������|�ܽ������|�����۶�|����|\n");
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

/**< ʶ�����*/
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

/**< ����Ա��Ϣͳ��*/
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
	printf("  |����Ա����|���۷�������|ռ�������|�����۶�|\n");
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

/**< ռ�����ͳ��*/
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
    printf("\n%25s��������ͳ�ƽ����\n"," ");Delay();
    printf("  |����������|¥������|ռ�������|����������|�ܽ������|\n");
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

