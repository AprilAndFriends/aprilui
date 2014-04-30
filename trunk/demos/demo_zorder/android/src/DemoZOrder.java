package com.aprilui.demoZOrder;

public class DemoZOrder extends com.april.Activity
{
	static
	{
		System.loadLibrary("demo_zorder");
	}
	
	@Override
	protected void onCreate(android.os.Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		this.forceArchivePath(com.april.NativeInterface.ApkPath); // forces APK as archive file
	}
	
}
