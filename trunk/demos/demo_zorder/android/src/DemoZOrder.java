package com.example.aprilui.demoZOrder;

public class DemoZOrder extends net.sourceforge.april.AprilActivity
{
	static
	{
		System.loadLibrary("demo_zorder");
	}
	
	@Override
	protected void onCreate(android.os.Bundle savedInstanceState)
	{
		this.forceArchivePath(this.getPackageResourcePath()); // forces APK as archive file
		super.onCreate(savedInstanceState);
	}
	
}
