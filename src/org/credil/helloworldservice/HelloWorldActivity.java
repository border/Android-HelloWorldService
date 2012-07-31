package org.credil.helloworldservice;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.InputStreamReader;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.ComponentName;
import android.content.Context;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;
import android.widget.TextView;

/**
 * Sample client using purely public Android interfaces
 */
public class HelloWorldActivity extends Activity {
    private static final String LOG_TAG = HelloWorldActivity.class.getSimpleName();
    private IBinder mIBinder;
    private ActivityManager mActivityManager = null;
            
    static {
        System.loadLibrary("zpadbackupservice");
    }

    private native String printJNI();
    private native int copyfile(String from, String to);
    
    TextView mHelloBox;
//    IHelloWorld mIHelloWorld;

//    public ServiceConnection serviceConnection = new ServiceConnection() {
//        public void onServiceConnected(ComponentName name, IBinder service) {
//            Log.d(LOG_TAG, "Service connected");
//            mIHelloWorld = IHelloWorld.Stub.asInterface(service);
//            mHelloBox.append("connected\n");
//            try {
//                int filesize = mIHelloWorld.hellothere("Calling from Java");
//                Log.d(LOG_TAG, "filesize: " + filesize);
//                mHelloBox.append("filesize: " + filesize + " \n");
//            } catch (RemoteException e) {
//                Log.w(LOG_TAG," Failed to invoke hellothere:" + e.getMessage(),e);
//            }
//        }
//
//        public void onServiceDisconnected(ComponentName name) {
//            mHelloBox.append("diconnected\n");
//            Log.e(LOG_TAG, "disconnected");
//            //This is where the current story ends. once disconnected we are not able to automatically reconnect
//            mIHelloWorld = null;
//        }
//    };

    /**
     * Called when the activity is first created.
     */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
//        setContentView(R.layout.hello);
        
//        android.os.SystemService.start("helloworldservice");
        
        Object object = new Object();
        try {
            Method getService = Class.forName("android.os.ServiceManager")
                    .getMethod("getService", String.class);
            mIBinder = (IBinder) getService.invoke(object,
                    new Object[] { new String(
                            "cn.com.zpad.backup.IZpad") });
        } catch (Exception e) {
            e.printStackTrace();
        }
        
        if (mIBinder == null) {
            Log.e(LOG_TAG, "mIBinder is null");
              Log.e(LOG_TAG, "Hello service  try to runing /system/bin/zpadbackupservice");
              new Thread(new Runnable() {
                  public void run() {
                      RootComand("/system/bin/zpadbackupservice");
                  }
              }).start();
              
              try {
                  Thread.sleep(2 * 1000);
              } catch (InterruptedException e) {
                  e.printStackTrace();
              }
        } else {
            Log.e(LOG_TAG, "mIBinder is not null");
        }
//        mHelloBox = (TextView) findViewById(R.id.HelloView01);
        mHelloBox.setText("start\n");
        
        getRunningServiceInfo();
        
//        Log.d(LOG_TAG, "Activity call JNI: " + printJNI());
//        mHelloBox.append("Activity call JNI: " + printJNI() + "\n");
        
        Log.d(LOG_TAG, "Activity call JNI: ");
        String from = "/data/data/cn.com.zpad_consult/databases";
        String to = "/sdcard/kvbian";
        int status = copyfile(from, to);
        mHelloBox.append("Activity call JNI copyfile: " + status + "\n");
        
        //We can currently not perform a bindService because the native code
        //did not register to the activity manager
//        bindService(new Intent("org.credil.helloworldservice.IHelloWorld"),serviceConnection, BIND_AUTO_CREATE);
    }
    
    // 获得系统正在运行的进程信息
    private void getRunningServiceInfo() {

        // 设置一个默认Service的数量大小
        int defaultNum = 200;
        // 获得ActivityManager服务的对象
        mActivityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        // 通过调用ActivityManager的getRunningAppServicees()方法获得系统里所有正在运行的进程
        List<ActivityManager.RunningServiceInfo> runServiceList = mActivityManager
                .getRunningServices(defaultNum);

        System.out.println(runServiceList.size());

        for (ActivityManager.RunningServiceInfo runServiceInfo : runServiceList) {

            // 获得Service所在的进程的信息
            int pid = runServiceInfo.pid; // service所在的进程ID号
            int uid = runServiceInfo.uid; // 用户ID 类似于Linux的权限不同，ID也就不同 比如 root等
            // 进程名，默认是包名或者由属性android：process指定
            String processName = runServiceInfo.process; 

            // 该Service启动时的时间值
            long activeSince = runServiceInfo.activeSince;

            // 如果该Service是通过Bind方法方式连接，则clientCount代表了service连接客户端的数目
            int clientCount = runServiceInfo.clientCount;

            // 获得该Service的组件信息 可能是pkgname/servicename
            ComponentName serviceCMP = runServiceInfo.service;
            String serviceName = serviceCMP.getShortClassName(); // service 的类名
            String pkgName = serviceCMP.getPackageName(); // 包名

            // 打印Log
            Log.i(LOG_TAG, "所在进程id :" + pid + " 所在进程名：" + processName + " 所在进程uid:"
                    + uid + "\n" + " service启动的时间值：" + activeSince
                    + " 客户端绑定数目:" + clientCount + "\n" + "该service的组件信息:"
                    + serviceName + " and " + pkgName);
        }
    }

    public void onDestroy(){
        super.onDestroy();
//        unbindService(serviceConnection);
    }
    
    public static boolean RootComand(String cmd) {

        Log.i(LOG_TAG, "root command ---------" + cmd);
        Process process = null;
        DataOutputStream os = null;

        try {

            process = Runtime.getRuntime().exec(cmd);

            int status = process.waitFor();

            BufferedReader r = new BufferedReader(new InputStreamReader(
                    process.getInputStream()));

            String line = null;
            while ((line = r.readLine()) != null) {
                Log.i(LOG_TAG, line);
            }
            if (status == 0) {
                Log.i(LOG_TAG, "success****************");
                return true;
            } else {
                Log.i(LOG_TAG, "fail****************" + status);
                return false;
            }
        } catch (Exception e) {
            Log.i(LOG_TAG, "exception****************" + e.getMessage());
            e.printStackTrace();
            return false;
        } finally {
            try {
                if (os != null) {
                    os.close();
                }
                process.destroy();
            } catch (Exception e) {
            }
        }
    }
}