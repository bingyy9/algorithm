//
// Created by bingy on 2021/3/4.
//

#include "LeeCode.h"
#include "queue"
int LeeCode::AtoI(string str){
    int index = 0;
    int length = str.length();
    long res = 0;  //注意越界的问题
    int MAX_VALUE = 0x7fffffff;
    int MIN_VALUE = 0x80000000;   //2147483647 ~ -2147483648

    while(str[index] == ' '){
        //去除空格
        index++;
    }

    int negative = 1;
    if(str[index] == '-'){
        index++;
        negative = -1;
    } else if(str[index] == '+'){
        index++;
    }

    if(index == length){
        return 0;
    }

    while(index < length){
        char c = str[index++];
        if(c > '9' || c < '0' ){
            break;
        }

        res = res * 10 + (c - '0');

        if(res * negative > MAX_VALUE){
            return MAX_VALUE;
        } else if(res * negative < MIN_VALUE){
            return MIN_VALUE;
        }
    }
    return res * negative;
}

int LeeCode::reverse(int data){
    int negative = 1;
    if(data < 0){
        negative = -1;
        data = -data;
    }

    long res = 0;
    while(data > 0){
        res = res * 10 + data%10;  //这里计算本身就可能越界，所以上面需要long类型
        data /= 10;

        if(negative * res > INT_MAX || negative * res < INT_MIN){
            return 0;
        }
    }

    return res * negative;
}

bool LeeCode::isPowerOfTwo(int n){
    if(n<=0){
        return false;
    }

    //2的最大值是2 pow 31 - 2, 所以int类型2的幂次最大值应该是2的30次方
    int big2 = pow(2, 30);
    return big2 %n == 0;
}
