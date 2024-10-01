#include <iostream>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
using namespace std;
class trans_array
{
protected:
    int *transed,count_n;//transed為轉換後的陣列，count_n為n(總數)
public:
    trans_array(fstream &a)//父類別建構元(初始化資料成員)
    {
        int h[100];
        count_n=0;
        while(a>>h[count_n])
            count_n++;
        transed=new int[count_n];
        for(int k=0;k<count_n;k++)
            transed[k]=h[k];
    }
    ~trans_array()//父類別解構元(動態記憶體)
    {
        delete [] transed;//將動態陣列transed[]銷毀
    }
    virtual void set_file(){};//將資料寫入檔案
    virtual void draw_bar(){};//長條圖作圖
};
class statistics:protected trans_array
{
protected:
    void sorting()//將數字由小到大進行排列
    {
        int temp,MAX,n=count_n,max_ad;
        for(int i=0;i<count_n;i++)
        {
            MAX=0;
            for(int k=0;k<n;k++)//這個迴圈找最大值然後放到最後一格
            {
                if(transed[k]>MAX)
                {
                    MAX=transed[k];
                    max_ad=k;
                }
            }
            temp=transed[n-1];
            transed[n-1]=MAX;
            transed[max_ad]=temp;
            n--;
        }
    }
    double variance()//計算變異數
    {
        double sum;
        double m=this->mean();
        for(int i=0;i<count_n;i++)
            sum+=pow(transed[i]-m,2);
        return sum/count_n;
    }
    float mean()//計算平均數
    {
        int sum;
        for(int i=0;i<count_n;i++)
            sum+=transed[i];
        return (double)sum/count_n;
    }
    float median()//計算中位數
    {
        if((double)(count_n)/2-count_n/2==0)
            return ((float)(transed[count_n/2-1]+transed[count_n/2]))/2;
        else
            return transed[count_n/2];
    }
    double iqr()//四分位距(包含q1、q3)
    {
        double q3,q1;
        if((double)(count_n)/4==count_n/4)
        {
            q1=((float)(transed[count_n/4-1]+transed[count_n/4]))/2;
            q3=((float)(transed[count_n*3/4-1]+transed[count_n*3/4]))/2;
        }
        else
        {
            q1=transed[count_n/4];
            q3=transed[count_n*3/4];
        }
        return q3-q1;
    }
public:
    statistics(fstream &a):trans_array(a){}//指定繼承父類別特定建構元
    ~statistics()//子類別解構元(動態記憶體)
    {
        delete [] transed;//將動態陣列transed[]銷毀
    }
    virtual void set_file(ofstream&);
    virtual void draw_bar(ofstream&);
};
void statistics::set_file(ofstream &b)
{
    if(b.is_open())
    {
        cout<<"檔案開啟success!!!...\n";
        this->sorting();
        b<<"\n\n資料排序後：\n";
        for(int i=0;i<count_n;i++)
            b<<transed[i]<<" ";
        cout<<"資料排序success!!!...\n";
        b<<"\n\n[資料參數]";
        b<<"\n資料數量："<<count_n;
        b<<"\n總和："<<this->mean()*count_n;
        b<<"\n最大值："<<transed[count_n-1];
        b<<"\n最小值："<<transed[0];
        b<<"\n平均數："<<this->mean();
        b<<"\n變異數："<<this->variance();
        b<<"\n標準差："<<sqrt(this->variance());
        b<<"\n中位數："<<this->median();
        b<<"\n全距："<<transed[count_n-1]-transed[0];
        b<<"\n四分位距："<<this->iqr();
        cout<<"寫入資料參數success!!!...\n";
    }
    else
        cout<<"檔案開啟失敗...\n";
}
void statistics::draw_bar(ofstream &b)
{
    int bar_n,bar_s;
    cout<<"[長條圖]\n(Tips*最大值為："<<transed[count_n-1]<<")\n";
    while(1)
    {
        cout<<"請輸入長條數目(需為最大值的因數)：";cin>>bar_n;
        if((double)(transed[count_n-1])/bar_n==transed[count_n-1]/bar_n)
            break;
        else
        {
            cout<<bar_n<<"不是"<<transed[count_n-1]<<"的因數，請重新輸入!!\n";
            continue;
        }
    }
    bar_s=transed[count_n-1]/bar_n;
    int i,j,bar[bar_n]={0};
    for(i=0;i<count_n;i++)
        for(j=1;j<=bar_n;j++)
            if(transed[i]<=j*bar_s&&transed[i]>=j*bar_s-bar_s+1)
                bar[j-1]++;
    int unit_digit=0,n=bar_n*bar_s,tunit;
    while(n!=0)
    {
        n/=10;
        unit_digit++;
    }
    b<<"\n\n[長條圖] 組距："<<bar_s<<"\n";
    for(int i=1;i<=bar_n;i++)
    {
        n=i*bar_s;
        tunit=0;
        while(n!=0)
        {
            n/=10;
            tunit++;
        }
        if(unit_digit-tunit>0)
            for(int k=0;k<unit_digit-tunit;k++)
                b<<" ";
        b<<i*bar_s<<":";
        for(int j=0;j<bar[i-1];j++)
            b<<"*";
        b<<"\n";
    }
    cout<<"長條圖建立success!!!...\n";
}
int main()
{
    srand(time(NULL));
    ofstream ofst("D:\\統計.txt",ios::out);
    for(int i=0;i<100;i++) //這邊用亂數產生一筆資料做測試
        ofst<<rand()%100+1<<" ";
    ofst.close();
    fstream inf("D:\\統計.txt",ios::in);
    statistics ex(inf);
    inf.close();
    ofstream ofs("D:\\統計.txt",ios::app);
    ex.set_file(ofs);
    ex.draw_bar(ofs);
    ofs.close();
    cout<<"程式執行完畢!!!...\n";
}