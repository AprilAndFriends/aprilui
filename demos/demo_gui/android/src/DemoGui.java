package com.example.aprilui.android.demoGui;

public class DemoGui extends net.sourceforge.april.android.Activity
{
	static
	{
		System.loadLibrary("demo_gui");
	}
	
	@Override
	protected void onCreate(android.os.Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		this.forceArchivePath(net.sourceforge.april.android.NativeInterface.ApkPath); // forces APK as archive file
	}
	
}
