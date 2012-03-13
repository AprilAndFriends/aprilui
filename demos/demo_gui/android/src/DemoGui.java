package com.example.aprilui.demoGui;

public class DemoGui extends net.sourceforge.april.AprilActivity
{
	static
	{
		System.loadLibrary("demo_gui");
	}
	
	@Override
	protected void onCreate(android.os.Bundle savedInstanceState)
	{
		this.forceArchivePath(this.getPackageResourcePath()); // forces APK as archive file
		super.onCreate(savedInstanceState);
	}
	
}
