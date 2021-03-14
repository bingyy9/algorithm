package com.example.testjni;

import android.util.Log;

import java.util.ArrayDeque;
import java.util.Queue;

public class Graph {
    public static final String TAG = "Graph";

    int vertexSize;
    int[] vertexes; //顶点集合
    int[][] matrix; //边集合

    public Graph(int vertexSize, int[] vertexes, int[][] matrix) {
        this.vertexSize = vertexSize;
        this.vertexes = vertexes;
        this.matrix = matrix;
    }

    public void breadthFirstSearch() {
        boolean[] visited = new boolean[vertexSize];
        Queue<Integer> queue = new ArrayDeque<>();
        queue.add(vertexes[0]);
        while(!queue.isEmpty()){
            int vertex = queue.poll();
            if(!visited[vertex]){
                Log.e(TAG, "vertex = " + vertex);
                visited[vertex] = true;

                for(int i=0; i<vertexSize; i++){
                    if(matrix[i][vertex] == 1 && !visited[i]){
                        queue.add(i);
                    }
                }
            }
        }
    }

    public void prim() {
        //定义一个数组，当前修改的村庄, lowcost = 0 代表已经修了
        int[] lowcost = new int[vertexSize];
        int[] adjvex = new int [vertexSize];

        //第一列的数据先放到lowcost
        for(int i=0; i<vertexSize; i++){
            //初始化lowcost[]数组，当前存储图中各个顶点到源点的权值
            lowcost[i] = matrix[0][i];
            //初始化adjvex[]数组，此时下一个接入树中的顶点的前驱必然是0号顶点
            adjvex[i] = 0;
        }
        int sum = 0;
        //此处为重点，这个for循环执行N-1次，即我们只需添加N-1条边
        for(int i=1; i<vertexSize; i++){
            int min = Integer.MAX_VALUE;
            int min_weight_vertex = 0;
            //找最小的且连通的边
            for(int j = 1; j<vertexSize; j++){
                if(lowcost[j] < min && lowcost[j] != 0){
                    min = lowcost[j];
                    min_weight_vertex = j;
                }
            }

            Log.e(TAG, "找到村庄： " + vertexes[min_weight_vertex] + " 修路距离： " + min);
            sum += min;

            for(int k=0; k<vertexSize; k++){
                //考虑淘汰
                if(matrix[min_weight_vertex][k] < lowcost[k] && lowcost[k] != 0){
                    lowcost[k] = matrix[min_weight_vertex][k];
                    adjvex[k] = min_weight_vertex;
                }
            }

            for(int number: lowcost){
                Log.e(TAG, "number: " + number);
            }

            Log.e(TAG, "================== 最短路径是： " + sum);
        }
    }

    void dijstra(){
        //最短路径，有没有找到最短路径
        //有没有找到最短路径
        boolean[] isPath = new boolean[vertexSize];
        int[] shortPath = new int[vertexSize];

        //第一列的数据先放到lowcost
        for(int i=0; i<vertexSize; i++){
            shortPath[i] = matrix[0][i];
        }

        shortPath[0] = 0;
        isPath[0] = true;

        int minId = 0;
        for(int i=1; i<vertexSize; i++){
            int min = Integer.MAX_VALUE;
            for(int j = 1; j<vertexSize; j++){
                //当前有没有被找到
                if(!isPath[j] && shortPath[j]<min){
                    minId = j;
                    min = shortPath[j];
                }
            }
            isPath[minId] = true;
            shortPath[minId] = min;
        }
    }
}
