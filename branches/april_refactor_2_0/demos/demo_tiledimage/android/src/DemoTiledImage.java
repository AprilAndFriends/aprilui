package com.example.aprilui.demoTiledImage;

public class DemoTiledImage extends net.sourceforge.april.AprilActivity
{
	static
	{
		System.loadLibrary("demo_tiledimage");
	}
	
	@Override
	protected void onCreate(android.os.Bundle savedInstanceState)
	{
		this.forceArchivePath(this.getPackageResourcePath()); // forces APK as archive file
		super.onCreate(savedInstanceState);
	}
	
}
