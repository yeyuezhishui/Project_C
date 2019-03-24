// The functions contained in this file are pretty dummy
// and are included only as a placeholder. Nevertheless,
// they *will* get included in the static library if you
// don't remove them :)
//
// Obviously, you 'll have to write yourself the super-duper
// functions to include in the resulting library...
// Also, it's not necessary to write every function in this file.
// Feel free to add more files in this project. They will be
// included in the resulting library.

// A function adding two integers and returning the result
#include<stdio.h>
#include<stdarg.h>
#include<stdlib.h>
#include<string.h>



char* mygets(char *s)
{
    int len = 0;
    char c;
    while ((c = getchar()) != '\n')
    {
        s[len] = c;
        len ++;
    }
    s[len] = '\n';
    s[len + 1] = '\0';
    return s;
}


int myputs(char *s)
{
    int i = 0;
    while (s[i] != '\0')
    {
        putchar(s[i]);
        i++;
    }
    putchar('\n');
    return i;
}



int myprintf(char *fmt, ...)
{
    va_list ap;
    char *pch;          //pch用于解析格式串
    char cvt[320];
    int m, n;

    char *sval;
    int ival;
    char cval;
    double dval;

    int cnt = 0;        //记录输出字符的个数
    va_start(ap, fmt);
    for (pch = fmt; *pch; pch++)        //解析格式字符串，遇到串尾结束循环
    {
        if (*pch != '%')
        {
            putchar(*pch);
            cnt++;
            continue;           //直接进入下一轮循环
        }
        switch(*++pch)          //使pch指向后续字符
        {
            case 'c':           //char 类型
                 cval = va_arg(ap, int);    //从可变参数区读入int类型参数
                 putchar(cval);
                 cnt++;
                 break;
            case 'd':
                ival = va_arg(ap, int);
                itoa(ival, cvt, 10);        //将该int型数据转换成十进制字符串
                for (sval = cvt; *sval; sval++, cnt++) putchar(*sval);
                break;
            case 's':
                for (sval = va_arg(ap, char *); *sval; sval++, cnt++)
                    putchar(*sval);
                break;
            case 'l':
                if (*(pch + 1) != 'f')
                {
                    putchar(*--pch);
                    cnt++;
                    break;
                }
                pch++;          //后续字符为f,即构成%lf
            case 'f':
                dval = va_arg(ap, double);      //从可变参数区取double数据
                sval = fcvt(dval, 6, &m, &n);
                if (n) { putchar('-'); cnt++; }
                if (m <= 0)
                {
                    int i;
                    cnt += strlen(sval) + 2 - m;
                    putchar('0');
                    putchar('.');
                    for (i = 0; i <-m; i++) putchar('0');
                }
                else        //m>0, 小数点在串首往右m位
                {
                    int i;
                    cnt += strlen(sval) + 1;
                    for (i = 0; i < m; i++) putchar(*sval++);
                    putchar('.');
                }
                while (*sval) putchar(*sval++);
                break;
            default:            //未构成指定的五种转换说明
                putchar(*--pch);
                cnt++;
                break;
        }
    }
    va_end(ap);
    return cnt;
}





int myscanf(char *fmt, ...)
{
    va_list ap;         //定义va_list类型的对象ap
    char *pch;          //pch用于解析格式串
    char *sval;
    int *ival;           //ival指向整形可变参数
    char *cval;
    double *dval;
    float *fval;
    char yue, ccc;      //读取输入字符
    int cnt = 0;        //记录数据项数个数
    va_start(ap, fmt);
    for (pch = fmt; *pch; pch++)
    {
        if (*pch != '%')
        {
            continue;           //直接进入下一轮循环
        }
        switch(*++pch)          //使pch指后续字符
        {
            case 'c':           //char 类型
                cval = va_arg(ap, char *);    //从可变参数区读入int *类型参数
                while ((yue = getchar()) == ' '  || yue == '\n');
                *cval = yue;
                yue = getchar();
                if (yue != ' ' && yue != '\n') ILLEGALDATA();
                cnt++;
                break;
            case 'd':
                ival = va_arg(ap, int *);
                *ival = 0;
                while ((yue = getchar()) == ' '  || yue  == '\n');
                ccc = yue;
                if (ccc == '+' || ccc == '-') yue = getchar();      //判断是否有符号
                while (yue >= '0' && yue <= '9')        //读入字符
                {
                    *ival = (*ival) * 10 + (yue - 48) ;
                    yue = getchar();
                }
                if (yue == ' ' || yue == '\n')
                {
                    if (ccc == '-') (*ival) = -(*ival);
                }
                else
                {
                    char *p = "ILLEGAL DATA";
                    while (*p)
                    {
                        putchar(*p);
                        p++;
                    }
                    exit(-1);
                }
                cnt++;
                break;
            case 's':
                sval = va_arg(ap, char *);
                while ((yue = getchar()) == ' '||  yue  == '\n');
                while (yue != ' ' && yue != '\n')
                {
                    *sval = yue;
                    sval ++;
                    yue = getchar();
                }
                *sval = '\0';

                cnt++;
                break;

            case 'l':
                pch++;
                dval = va_arg(ap, double *);
                while ((yue = getchar()) == ' '|| yue  == '\n');
                if ( (yue > '9' || yue < '0') && (yue != '.') && (yue != '+') && (yue != '-'))
                {
                    ILLEGALDATA();
                }
                *dval = 0;
                ccc = yue;
                if (yue == '+' || yue == '-') yue = getchar();
                while (yue >= '0' && yue <= '9')
                {
                    *dval = (*dval) * 10 + (yue - 48);
                    yue = getchar();
                }
                if (yue == ' ' || yue == '\n') yue = yue;
                else if (yue == '.')
                {
                    int n = 1, i;
                    yue = getchar();
                    double wyh = 0;
                    while (yue >= '0' && yue <= '9')
                    {
                        wyh = yue - 48;
                        for (i = 0; i < n; i++) wyh = wyh / 10;
                        (*dval) = (*dval) + wyh;
                        yue = getchar();
                        n++;
                    }
                    if (yue != ' ' && yue != '\n')
                    {
                        ILLEGALDATA();
                    }
                }
                else
                {
                    ILLEGALDATA();
                }
                if (ccc == '-') *dval = -(*dval);

                cnt++;
                break;

            case 'f':
                fval = va_arg(ap, float *);
                while ((yue = getchar()) == ' '|| yue  == '\n');
                if ( (yue > '9' || yue < '0') && (yue != '.') && (yue != '+') && (yue != '-'))
                {
                    ILLEGALDATA();
                }
                *fval = 0;
                ccc = yue;
                if (yue == '+' || yue == '-') yue = getchar();
                while (yue >= '0' && yue <= '9')
                {
                    *fval = (*fval) * 10 + (yue - 48);
                    yue = getchar();
                }
                if (yue == ' ' || yue == '\n') yue = yue;
                else if (yue == '.')
                {
                    int n = 1, i;
                    yue = getchar();
                    double wyh = 0;
                    while (yue >= '0' && yue <= '9')
                    {
                        wyh = yue - 48;
                        for (i = 0; i < n; i++) wyh = wyh / 10;
                        (*fval) = (*fval) + wyh;
                        yue = getchar();
                        n++;
                    }
                    if (yue != ' ' && yue != '\n')
                    {
                        ILLEGALDATA();
                    }
                }
                else
                {
                    ILLEGALDATA();
                }
                if (ccc == '-') *fval = -(*fval);

                cnt++;
                break;
            default:ILLEGALDATA();
        }
    }
    va_end(ap);
    return cnt;
}




void ILLEGALDATA(void)
{
    char *p = "数据错误";
    while (*p)
    {
        putchar(*p);
        p++;
    }
    exit(-1);

}
