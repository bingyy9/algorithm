package com.example.testjni;

import android.util.Log;

import java.util.HashMap;
import java.util.Map;

public class LeeCode {
    private static final String TAG = "LeeCode";

    public static void twoSum_(){
        int[] nums = {2, 7, 11, 15};
        int target = 9;
        int[] result = twoSum(nums, 9);
        Log.e(TAG, 9 + " = " + nums[result[0]] + " + " + nums[result[1]]);
//        result = twoSum(nums, 13);
//        Log.e(TAG, 13 + " = " + nums[result[0]] + " + " + nums[result[1]]);
//        result = twoSum(nums, 17);
//        Log.e(TAG, 17 + " = " + nums[result[0]] + " + " + nums[result[1]]);
//        result = twoSum(nums, 88);
//        Log.e(TAG, 88 + " = " + nums[result[0]] + " + " + nums[result[1]]);
    }

    public static int[] twoSum(int[] nums, int target){
        if(nums == null){
            return null;
        }

        Integer[] tmp = new Integer[2048];
        for(int i=0; i<nums.length; i++){
            int diff = target - nums[i];
            Integer index = tmp[diff & 2047];
            if(index != null){
                return new int[]{i, index};
            }
            tmp[nums[i] & 2047] = i;
        }

        return null;
    }
}
