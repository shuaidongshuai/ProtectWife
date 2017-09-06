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
package com.dong.ProtectWife;

import java.util.Map;

import org.cocos2dx.lib.Cocos2dxActivity;


import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import com.alipay.sdk.app.EnvUtils;
import com.alipay.sdk.app.PayTask;
import com.dong.ProtectWife.ProtectWife;
import com.dong.ProtectWife.PayResult;
import com.dong.util.OrderInfoUtil2_0;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.util.Log;
import android.widget.Toast;

public class ProtectWife extends Cocos2dxActivity {
	private static ProtectWife context;
	private int fireType;//子弹类型
	private int fireNum;//数量
	private int price;//价格
	
	public static Object getInstance(){
		return context;
	}
	
    protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);	
		context = this;
	}

    public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// ProtectWife should create stencil buffer
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	
    	return glSurfaceView;
    }

    static {
        System.loadLibrary("cocos2dcpp");
    }     
    
    /** 支付宝支付业务：入参app_id */
	public static final String APPID = "2016080200149100";
	
	/** 支付宝账户登录授权业务：入参pid值 */
	public static final String PID = "2088102169602856";
	/** 支付宝账户登录授权业务：入参target_id值 */
	public static final String TARGET_ID = "";

	public static final String RSA2_PRIVATE = "MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQCxYulASqewTNR9gqa8KW1jmZaQ1CnE7+KMlcAGLnX1bcs3WfVxnzXvfGRH6CCxH/+hlHL7DGa9ltNlnrZqJ50WYRJWvlBZqwW6+1A/Q7smho8g4PEk+8uVk7ATDu5qrlAy0ZrolD+lPDrcYDZ3oaOSkYNJUtuSyyP3k2nRe3/8vrTuPGRbl05OpMPzaK8N5+rpdP0q6feMI5f7d1JveI10imCqTmguzakp8GO9h/05XLrI9P9cG9uLoVlff/irqXO4EOC5Ib63shTOphhR8WgAxkHSgGYJciEMNcI9AOyZ27nGxwWKJ9xtsbwYu8/IuIRd9WoXNTGOs/nVKEgk3ApRAgMBAAECggEBAJF9BsJseMzotv+aQRbjA41FU3DGEtwrxCx6RlzLjoQV4S8s4Vqnsp0R8U3o3adpD0l/cC7oJPwq0IfwfLzkcLsCMwBiZ5Sj6LdLmK3K55zZkFrvI9mpi3jLAQcgn/tvrOeBcwR/UxQ7KivF3eaQ9JAqOPvSsbTcWEue5JOkL30jr2m2xaABnwTqz9vFAhfNF6UTbzapbApwHD5a6SZ+MA1thXJsCdMaCzJLUkKCL4AvDkuzTUjuShmgKYUVuh4lbOfisCcxuKxG3+eydteedUqpwsqsh/mS0uTOk0qAR7dYW4bshjYBNpoXENhsFy00IWkxkPZDvNwWyiwSsIg+KmECgYEA3sg6HJLcKl8Fwj87W0dJK+pNBdAOeNTeqOfQCsLvZ3jj72OjplSYquPfmsEKOWPFiV/qDi9eZfoXvLgiDQp2Ddqnq+fjPajARKqeVuZAYcsUFOx2eL1uN+x1NN3QmubHSZazYriRqiR3o+kV17pDQQovfJvEpFLpzDE0RlbYtssCgYEAy9XkLw/7ud6704hC+o8I5dlr6vZOWzm+v6/BwzC/wP+WC5+C4iqBWwdbyLrcPuOUnADnxS+oJXsV2K0CZ+V60pRfms31Y52sOItFwK/UKwKH7wBoZ5Jf06zLctk1fYRxqFBZEctqxw3qOJMHsyAyhE1R5m8v12q2c0Z37bYrA9MCgYAfqHJhlxkESGaIGPY1XWbvf6ccWVmFIDLX68ErllHw8UPqw80AmOrs9d7TnfT2/M+YWvHmvCJ8+VIgNkZYdhEanOTQz+4B2BYia+B+Vs/1Vadv5b5cIAcmmUp6jb7UFFFjiq/JKpNAi1gZCclZ05U1C959paEG/OVOZ/uWt0aqiwKBgBryc/Ak1MqFv57/bJtjPXHlhHbTiRhWUflVavA7CZ6iK1vuAoeTxvYZyvCGg4ds3sCg/gL7gecylnjGpoLFu68LHniAsegxeAshnl8S36Gv+/iEvUiz+ScquNq+DGxDjiMKPDw2AC/Ajy5bgbEUmJkU/7bPDCwWV3h14HT0mEsxAoGAPYXBLfWQZ8bhfyep/3OUySAyGJOZbjtDUtrolPG2jNvQHPnA1UxzKIlCCJ6JWIuPQ+FSGKRKKrCEXAFdgDyjsX9faJpk4VYrm7FY1D57KU3k2Eihmo87RM3jgubPzSKDiI1ZxYeH+dSdS/gAehti6YJfGSZxuu+C+Ya20J+n7VI=";
	public static final String RSA_PRIVATE = "";
	
	private static final int SDK_PAY_FLAG = 1;
	private static final int SDK_AUTH_FLAG = 2;

	@SuppressLint("HandlerLeak")
	private Handler mHandler = new Handler() {
		@SuppressWarnings("unused")
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case SDK_PAY_FLAG: {
				@SuppressWarnings("unchecked")
				PayResult payResult = new PayResult((Map<String, String>) msg.obj);
				/**
				 对于支付结果，请商户依赖服务端的异步通知结果。同步通知结果，仅作为支付结束的通知。
				 */
				String resultInfo = payResult.getResult();// 同步返回需要验证的信息
				String resultStatus = payResult.getResultStatus();
				// 判断resultStatus 为9000则代表支付成功
				if (TextUtils.equals(resultStatus, "9000")) {
					// 该笔订单是否真实支付成功，需要依赖服务端的异步通知。
					Toast.makeText(ProtectWife.this, "支付成功", Toast.LENGTH_SHORT).show();
					successPayCB(fireType,fireNum);//把数量
				} else {
					// 该笔订单真实的支付结果，需要依赖服务端的异步通知。
					Toast.makeText(ProtectWife.this, "支付失败", Toast.LENGTH_SHORT).show();
				}
				break;
			}
			default:
				break;
			}
		};
	};
	/**
	 * 支付成功的回调函数
	 * @param fireNum
	 * @param price
	 */
	public static native void successPayCB(int fireType,int fireNum);
	
	/**
	 * 支付宝支付业务
	 * 
	 * @param v
	 */
	public void payV2(int fireType,int fireNum,int price) {
		/**
		 * 这里只是为了方便直接向商户展示支付宝的整个支付流程；所以Demo中加签过程直接放在客户端完成；
		 * 真实App里，privateKey等数据严禁放在客户端，加签过程务必要放在服务端完成；
		 * 防止商户私密数据泄露，造成不必要的资金损失，及面临各种安全风险； 
		 * 
		 * orderInfo 的获取必须来自服务端；
		 */
		this.fireType = fireType;
		this.fireNum = fireNum;
		this.price = price;
		
		boolean rsa2 = (RSA2_PRIVATE.length() > 0);
		Map<String, String> params = OrderInfoUtil2_0.buildOrderParamMap(APPID, rsa2,price);
		String orderParam = OrderInfoUtil2_0.buildOrderParam(params);

		String privateKey = rsa2 ? RSA2_PRIVATE : RSA_PRIVATE;
		String sign = OrderInfoUtil2_0.getSign(params, privateKey, rsa2);
		final String orderInfo = orderParam + "&" + sign;
		
		/* 沙箱 */
		EnvUtils.setEnv(EnvUtils.EnvEnum.SANDBOX);
		
		Runnable payRunnable = new Runnable() {

			@Override
			public void run() {
				PayTask alipay = new PayTask(ProtectWife.this);
				Map<String, String> result = alipay.payV2(orderInfo, true);
				Log.i("msp", result.toString());
				
				Message msg = new Message();
				msg.what = SDK_PAY_FLAG;
				msg.obj = result;
				mHandler.sendMessage(msg);
			}
		};

		Thread payThread = new Thread(payRunnable);
		payThread.start();
	}

	/**
	 * get the sdk version. 获取SDK版本号
	 * 
	 */
	public void getSDKVersion() {
		PayTask payTask = new PayTask(this);
		String version = payTask.getVersion();
		Toast.makeText(this, version, Toast.LENGTH_SHORT).show();
	}  
}
