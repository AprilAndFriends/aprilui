package com.aprilui.demoTiledImage;

public class DemoTiledImage extends com.april.Activity
{
	static
	{
		System.loadLibrary("demo_tiledimage");
	}
	
	@Override
	protected void onCreate(android.os.Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		this.forceArchivePath(com.april.NativeInterface.ApkPath); // forces APK as archive file
	}
	
}
