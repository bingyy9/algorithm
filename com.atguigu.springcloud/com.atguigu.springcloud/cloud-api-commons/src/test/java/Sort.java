import java.util.ArrayList;
import java.util.List;

public class Sort {
    public static void main(String[] args) {
//       List<Integer> a = new ArrayList<Integer>();
//       a.add(6);
//       a.add(5);
//       a.add(3);
//       a.add(1);
//       a.add(8);
//       a.add(7);
//       a.add(2);
//       a.add(4);
       int a[] = {6,5,3,1,8,7,2,4};

       if(a == null || a.length == 0){
           return;
       }

       int j;
       for(int i = 1; i<a.length; i++){
           for(j=i; j>0; j--){
               if(a[j-1] < a[j]){
                   int tmp = a[j];
                   a[j] = a[j-1];
                   a[j-1] = tmp;
               }
           }
       }

        for (Integer i: a){
            System.out.println(i);
        }
    }
}
