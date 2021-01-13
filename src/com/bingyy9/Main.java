package com.bingyy9;

import com.bingyy9.algorithm.sort.Bubble;
import com.bingyy9.algorithm.sort.Merge;
import jdk.nashorn.tools.Shell;

import java.util.Arrays;

public class Main {

    public static void main(String[] args) {
        int a[] = {8,1,4,6,5,6,2,3,4,5};
//        Bubble.shellSort(a);
        Merge.sort(a);
        System.out.println(Arrays.toString(a));
    }
}
