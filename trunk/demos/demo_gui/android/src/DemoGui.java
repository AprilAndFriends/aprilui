package com.example.aprilui.android.demoGui;

public class DemoGui extends com.googlecode.april.android.Activity
{
	static
	{
		System.loadLibrary("demo_gui");
	}
	
	@Override
	protected void onCreate(android.os.Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		this.forceArchivePath(com.googlecode.april.android.NativeInterface.ApkPath); // forces APK as archive file
	}
	
}
