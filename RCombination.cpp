#include "RCombination.h"
#include <string>
#include <QStringList>
#include <QDebug>
/**
 * @brief RCombination::RCombination
 * @param combination_lineEdit
 * @param r_lineEdit
 */
RCombination::RCombination(QString combination_lineEdit, QString r_lineEdit){
    combinationStr = combination_lineEdit;
    rValue = r_lineEdit.toInt();

    qDebug() << "r = "<<rValue;

    //以 ','分割字符串   不计空窜（连续两个','）
    //1*ad,68*as,8*a2
    //4*a1,3*a2,4*a3,5*a4
    QStringList list = combinationStr.split(',', QString::SkipEmptyParts);
    setNum = 0;
    foreach(QString s, list){
        int key = s.indexOf('*');
        set[setNum].k_i = s.left(key).toInt();
        set[setNum].a_i = s.right(s.length() - key - 1);

        qDebug() << "set[" << setNum << "]: "<< set[setNum].k_i << " " << set[setNum].a_i;

        setNum += 1;
    }

}

/**
 * @brief 计算F(n, r)
 * @param n
 * @param r
 * @return
 */
int RCombination::F(int n, int r){
    if(r < 0)
        return 0;
    int max_num = n + r -1;
    int k = r;
    int molecule = 1;   //分子
    int denominator = 1;    //分母
    while(k > 0){
        molecule *= max_num;
        max_num -= 1;
        denominator *= k;
        k -= 1;
    }

    return molecule / denominator;
}

/**
 * @brief 容斥原理计算 r-组合 计数
 * @return
 */
long RCombination::calculate(){
    answer = RCombination::F(setNum, rValue);   // 首先计算 |S|

    return answer;
}


/**
 * @brief 计算count个Ai交集的组合个数
 * @param setNum  集合中ai种类数
 * @param start  选择Ai的起始位置 下标从0开始
 * @param r     剩余的r值
 * @param count  计数的Ai个数
 * @return
 */
int RCombination::calACombination(const Node (&set)[], const int &setNum, int start, int r, int count){
    if(r < 0 || count <= 0)
        return 0;
    if(setNum - start < count)  //set[start...end] 个数 < count
        return 0;

    int sum = 0;
    int end = setNum - count;    //保证能选count个Ai的最大下标
    count -= 1;
    for(int index = start; index <= end; index+=1){
        if(count > 0){
            sum += calACombination(set, setNum, index+1, r-set[index].k_i, count);
        }
        else {
            sum += F(setNum, r-set[index].k_i);
        }
    }

    return sum;
}

