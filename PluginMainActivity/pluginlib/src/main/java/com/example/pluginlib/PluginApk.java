package com.example.pluginlib;

import android.content.pm.PackageInfo;
import android.content.res.AssetManager;
import android.content.res.Resources;

import dalvik.system.DexClassLoader;

public class PluginApk {
    //plugin apk instance object
    public DexClassLoader mClassLoader;
    public Resources mResources;
    public PackageInfo mPackageInfo;
    public AssetManager mAssetManager;

    public PluginApk(DexClassLoader mClassLoader, Resources mResources, PackageInfo mPackageInfo, AssetManager mAssetManager) {
        this.mClassLoader = mClassLoader;
        this.mResources = mResources;
        this.mPackageInfo = mPackageInfo;
        this.mAssetManager = mAssetManager;
    }


}
