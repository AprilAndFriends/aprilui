package com.aprilui.demoTileimage;

public class Main extends com.april.Activity
{
	static
	{
		System.loadLibrary("demo_tileimage");
	}
	
	@Override
	protected void onCreate(android.os.Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		this.forceArchivePath(com.april.NativeInterface.apkPath); // forces APK as archive file
	}
	
}
