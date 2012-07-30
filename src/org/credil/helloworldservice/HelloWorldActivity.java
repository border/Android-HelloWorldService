package org.credil.helloworldservice;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.InputStreamReader;

import android.app.Activity;
import android.os.Bundle;
import android.os.SystemService;
import android.util.Log;
import android.widget.TextView;

/**
 * Sample client using purely public Android interfaces
 */
public class HelloWorldActivity extends Activity {
    private static final String LOG_TAG = HelloWorldActivity.class.getSimpleName();

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
        setContentView(R.layout.hello);
        
        android.os.SystemService.start("helloworldservice");
        
        
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
        
        mHelloBox = (TextView) findViewById(R.id.HelloView01);
        mHelloBox.setText("start\n");
        
//        Log.d(LOG_TAG, "Activity call JNI: " + printJNI());
//        mHelloBox.append("Activity call JNI: " + printJNI() + "\n");
        
        Log.d(LOG_TAG, "Activity call JNI: ");
        String from = "/data/data/cn.com.zpad";
        String to = "/sdcard/kvbian";
        int status = copyfile(from, to);
        mHelloBox.append("Activity call JNI copyfile: " + status + "\n");
        
        //We can currently not perform a bindService because the native code
        //did not register to the activity manager
//        bindService(new Intent("org.credil.helloworldservice.IHelloWorld"),serviceConnection, BIND_AUTO_CREATE);
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