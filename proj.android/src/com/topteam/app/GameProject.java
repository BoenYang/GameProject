/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package com.topteam.app;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.widget.FrameLayout;
import android.widget.RelativeLayout;
import android.widget.RelativeLayout.LayoutParams;

import com.bodong.dianjinweb.DianJinPlatform;
import com.bodong.dianjinweb.banner.DianJinMiniBanner;

public class GameProject extends Cocos2dxActivity {

	static {
		System.loadLibrary("cocos2dcpp");
	}

	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		DianJinPlatform.initialize(this, 59372,
				"f17cb928b67fd28c420792592e968efd");
		DianJinPlatform.hideFloatView(this);
		setUpAds();
	}

	public Cocos2dxGLSurfaceView onCreateView() {
		Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
		glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
		return glSurfaceView;
	}

	// dian jing ad
	public static DianJinMiniBanner bannerLayout;

	public void setUpAds() {
		RelativeLayout container = new RelativeLayout(this);
		addContentView(container, new RelativeLayout.LayoutParams(
				LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
		bannerLayout = new DianJinMiniBanner(this);
		bannerLayout.setVisibility(FrameLayout.VISIBLE);
		bannerLayout.setBackgroundColor(Color.BLACK);
		bannerLayout.setBannerInterval(5000);
		bannerLayout.startBanner();

		LayoutParams layoutParams = new LayoutParams(LayoutParams.WRAP_CONTENT,
				LayoutParams.WRAP_CONTENT);
		layoutParams.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
		container.addView(bannerLayout, layoutParams);
		initDialog();
	}

	public static void dismissAd() {
		bannerLayout.stopBanner();
	}
	

	@Override
	protected void onDestroy() {
		DianJinPlatform.destory(this);
		super.onDestroy();
	}

	// exit dialog
	private void initDialog() {
		mDialog = new AlertDialog.Builder(this).create();

		mDialog.setTitle("提示");

		mDialog.setMessage("是否退出游戏?");

		mDialog.setButton(AlertDialog.BUTTON_POSITIVE, "是",
				new DialogInterface.OnClickListener() {

					@Override
					public void onClick(DialogInterface dialog, int which) {
						GameProject.this.finish();
						System.exit(0);
					}

				});

		mDialog.setButton(AlertDialog.BUTTON_NEGATIVE, "否",
				new DialogInterface.OnClickListener() {

					@Override
					public void onClick(DialogInterface dialog, int which) {
						mDialog.dismiss();
					}
				});
	}

	private static AlertDialog mDialog = null;

	private static Handler mHandler = new Handler(new Handler.Callback() {

		@Override
		public boolean handleMessage(Message msg) {
			mDialog.show();
			return true;
		}

	});

	public static void show() {
		mHandler.sendEmptyMessage(0);
	};

}
