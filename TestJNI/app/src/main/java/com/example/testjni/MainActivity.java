package com.example.testjni;

import androidx.appcompat.app.AppCompatActivity;
import io.reactivex.rxjava3.android.schedulers.AndroidSchedulers;
import io.reactivex.rxjava3.core.Observable;
import io.reactivex.rxjava3.functions.Consumer;
import io.reactivex.rxjava3.functions.Function;
import io.reactivex.rxjava3.schedulers.Schedulers;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Environment;
import android.os.Parcel;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import com.tbruyelle.rxpermissions3.RxPermissions;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;

public class MainActivity extends AppCompatActivity {

    public static final String TAG = "MainActivity";

    private File mInFile = new File(Environment.getExternalStorageState(), "test.mp4");
    private File mOutFile = new File(Environment.getExternalStorageState(), "out.mp4");

    private ImageView mFaceImg;
    private Bitmap mFaceBitmap;
    private FaceDetection mFaceDetection;
    private File mCascadeFile;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        PackageInfo packageInfo = null;
        try {
            packageInfo = this.getPackageManager().getPackageInfo(this.getPackageName(), PackageManager.GET_SIGNATURES);
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        Signature[] signatures = packageInfo.signatures;
        Log.e(TAG, "signature: " + signatures[0].toCharsString());

        mFaceImg = findViewById(R.id.img_face);
        mFaceBitmap = BitmapFactory.decodeResource(getResources(), R.drawable.face);
        mFaceImg.setImageBitmap(mFaceBitmap);
        mFaceDetection = new FaceDetection();

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(SignatureUtils.signatureParams("username=123&password=123"));

        VideoCompress.callStaticMethod();
        Point p = VideoCompress.createPoint();
        Log.e(TAG, "p.x " + p.getX());
        compressVideo(null);

        Parcel parcel = Parcel.obtain();
        parcel.writeInt(12);
        parcel.writeInt(24);
        parcel.setDataPosition(0);
        int num1 = parcel.readInt();
        int num2 = parcel.readInt();
        Log.e(TAG, "parcel read " + num1 + " " + num2);


        SignatureUtils signatureUtils = new SignatureUtils();
        signatureUtils.writeInt(2);
        signatureUtils.setDataPosition(0);
        int i = signatureUtils.readInt();
        Log.e(TAG, "read int " + i);

        sort();
        globalRef();
        setLocalCache();
        exception();
        systemArrayCopy();
    }

    public void compressVideo(View view) {
        RxPermissions rxPermissions = new RxPermissions(this);
        rxPermissions.request(Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE)
                .subscribe(new Consumer<Boolean>() {
                    @Override
                    public void accept(Boolean aBoolean) throws Throwable {
                        if(aBoolean){
                            compressVideo();
                        }
                    }
                });


    }

    private void compressVideo(){
        String[] compressCommand = {"ffmpeg", "-i", mInFile.getAbsolutePath(), "-b:v", "1024k", mOutFile.getAbsolutePath()};
        Observable.just(compressCommand)
                .map(new Function<String[], File>() {
                    @Override
                    public File apply(String[] compressCommand) throws Throwable {
                        //ffmpeg -i test.mp4 -b:v 1024k out.mp4
                        //-b:v 码率是什么？码率越高视频越清晰，视频越大
                        VideoCompress videoCompress = new VideoCompress();
                        videoCompress.compressVideo(compressCommand, (int current, int total)->{
                            Log.e(TAG, "compress progress current: " + current + " total: " + total);
                        });
                        return mOutFile;
                    }
                })
                .subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe(new Consumer<File>() {
                    @Override
                    public void accept(File file) throws Throwable {
                        Log.i(TAG, "compress completed");
                    }
                });

    }

    private void sort(){
        int arr[] = {1, 2, 8, 4, 1};
        Sample.sort(arr);
        for (int i=0; i<arr.length; i++){
            System.out.println(arr[i]);
        }
    }

    private void globalRef(){
        Log.e(TAG, "globalRef");
        Sample.createGlobalRef(" global Ref");
        Log.e(TAG, "get global ref:" + Sample.getGlobalRef());
        Sample.delGlobalRef();
//        Log.e(TAG, "get global ref:" + Sample.getGlobalRef());
    }

    private void setLocalCache(){
        Sample.staticLocalCache("hello");
        Log.e(TAG, "Sample name " + Sample.name);
        Sample.staticLocalCache("world");
        Log.e(TAG, "Sample name " + Sample.name);
    }

    private void exception(){
        try {
            Sample.exception();
        }catch (Exception e){
            Log.e(TAG, " cache exception: " + e);
        }
    }

    private void systemArrayCopy(){
        Person[] persons = new Person[10];
        Person[] persons2 = new Person[10];

        for (int i = 0; i<persons.length; i++){
            persons[i] = new Person("Bing", i);
        }

        Sample.arraycopy(persons, 0, persons2, 0, 10);
        for (int i = 0; i<persons2.length; i++){
//            Log.e(TAG, "persons copy value: " + persons2[i].getAge());
        }
    }

    public void faceDetection(View view) {
        copyCascadeFile();
        mFaceDetection.loadCascade(mCascadeFile.getAbsolutePath());
        mFaceDetection.faceDectionSaveInfo(mFaceBitmap);

    }

    private void copyCascadeFile(){
        try{
            InputStream is = getResources().openRawResource(R.raw.lbpcascade_frontalface);
            File cascadeDir = getDir("cascade", Context.MODE_PRIVATE);
            mCascadeFile = new File(cascadeDir, "lbpcascade_frontalface.xml");
            if(mCascadeFile.exists()){
                return;
            }
            FileOutputStream os = new FileOutputStream(mCascadeFile);

            byte[] buffer = new byte[4096];
            int bytesRead;
            while((bytesRead = is.read(buffer)) != -1){
                os.write(buffer, 0, bytesRead);
            }
            is.close();
            os.close();
        } catch (Exception e){
            e.printStackTrace();
        }
    }
}