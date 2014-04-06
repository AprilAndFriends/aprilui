package com.example.aprilui.android.demoZOrder;

public class DemoZOrder extends com.googlecode.april.android.Activity
{
	static
	{
		System.loadLibrary("demo_zorder");
	}
	
	@Override
	protected void onCreate(android.os.Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		this.forceArchivePath(com.googlecode.april.android.NativeInterface.ApkPath); // forces APK as archive file
	}
	
}
