package com.example.testjni;

import java.util.Objects;

public class HashMap_<K, V> {
    public MapEntry[] table;
    transient int size;
    int threshold = 8;
    final float loadFactor = 0.75f;

    public class MapEntry<K, V>{
        int hash;
        K key;
        V value;
        MapEntry<K, V> next;

        public MapEntry(K key, V value, MapEntry<K, V> next, int hash) {
            this.key = key;
            this.value = value;
            this.next = next;
            this.hash = hash;
        }
    }

    public V put(K key, V value){
        if(table == null){
            table = new MapEntry[8];
        }
        if(key == null){
            return null;
        }
        
        int hash = hash(key);
        int index = getIndex(hash, table.length);
        for(MapEntry<K, V> e = table[index]; e!= null; e=e.next){
            Object k; //一丁点优化
            if(e.hash == hash && (((k=e.key) == key) || key.equals(k)))   {
                V oldValue = e.value;
                e.value = value;
                return oldValue;
            }
        }
        //添加一个新的MapEntry
        addEntry(hash, key, value, index);
        return null;
    }

    private void addEntry(int hash, K key, V value, int index) {
        //要不要扩容？
        if(size >= threshold && table[index] != null){
            resize(size << 1);
            //重新计算hash
            index = getIndex(hash, table.length);
            //重新计算index
        }
        createEntry(hash, key, value, index);
        size++;
    }

    private void createEntry(int hash, K key, V value, int index) {
        MapEntry<K, V> newEntry = new MapEntry<>(key, value, table[index], hash);
        table[index] = newEntry; //头插
        size++;
    }

    private void resize(int newCapacity) {
        MapEntry<K, V>[] newTable = new MapEntry[newCapacity];
        transform(newTable);
        table = newTable;
        threshold = (int)(newCapacity * loadFactor);
    }

    private void transform(MapEntry<K,V>[] newTable) {
        int newCapacity = newTable.length;
        for(MapEntry<K, V> e: table){
            while(null != e){
                //从原来数组中获取数据Entry，保证新的数组能连接上
                MapEntry<K, V> next = e.next;
                int index = getIndex(e.hash, newCapacity);

                //保证扩容后新的头插
                e.next = newTable[index]; //扩容新数组头插
                newTable[index] = e;

                e = next;
            }
        }
    }

    private int getIndex(int hash, int length) {
        return hash & (table.length - 1);
    }

    private int hash(K key) {
        int h = 0;
        return (key == null)? 0: (key.hashCode() ^ (h >>> 16));
    }

    public V get(K key){
        if(key == null){
            return null;  //refer sorce code
        }

        MapEntry<K, V> entry = getEntry(key);
        return entry == null? null : entry.value;
    }

    private MapEntry<K,V> getEntry(K key) {
        int index = getIndex(hash(key), table.length);
        for(MapEntry<K, V> e = table[index]; e!= null; e=e.next){
            Object k;
            if(e.hash == hash(key) && (((k=e.key) == key) || key.equals(k)))   {
                return e;
            }
        }
        return null;
    }


}


