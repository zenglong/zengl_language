package com.zengl.runscript;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.app.Activity;
import android.app.ProgressDialog;
import android.content.SharedPreferences;
import android.content.res.AssetManager;
import android.view.Gravity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

public class RunScript extends Activity {
	TextView  mylog;
	EditText  inputScript;
	Button runBtn;
	String logContent;
	String inputName;
	ProgressDialog progress = null;
	RunTask task = null;
	int debuginfo = 0;
	String inner_script_dir = "";
	String data_scripts_dir= "";
	SharedPreferences userInfo;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		data_scripts_dir = getScriptPath();
		String script_exist_flag_file = data_scripts_dir + "/script_exist_flag";
		if(!fileIsExists(script_exist_flag_file))
		{
			copyAssetFolder(getAssets(), "scripts", data_scripts_dir);
		}
		LinearLayout head_layout = new LinearLayout(this);  
		head_layout.setGravity(Gravity.TOP);
		head_layout.setOrientation(LinearLayout.HORIZONTAL);
		inputScript = new EditText(this);
		userInfo = getSharedPreferences("user_info", 0);
		String input = userInfo.getString("inputScript", "");
		if(input == "")
			inputScript.setText("test.zl");
		else
			inputScript.setText(input);
		LinearLayout.LayoutParams inputParam = new LinearLayout.LayoutParams(     
                0,     
                LinearLayout.LayoutParams.WRAP_CONTENT,
                3
        ); 
		inputScript.setLayoutParams(inputParam);
		head_layout.addView(inputScript, 0);
		runBtn = new Button(this);
		runBtn.setText("运行");
		LinearLayout.LayoutParams runParam = new LinearLayout.LayoutParams(     
                0,     
                LinearLayout.LayoutParams.WRAP_CONTENT,
                1
        );
		runBtn.setLayoutParams(runParam);
		head_layout.addView(runBtn, 1);
		mylog = new TextView(this);
		mylog.setText(logContent = "");
		ScrollView myLogScroller = new ScrollView(this);
		myLogScroller.addView(mylog);
		LinearLayout wrap_layout = new LinearLayout(this);
		LinearLayout.LayoutParams wrapParam = new LinearLayout.LayoutParams(     
                LinearLayout.LayoutParams.MATCH_PARENT,     
                LinearLayout.LayoutParams.MATCH_PARENT
        );
		wrap_layout.setLayoutParams(wrapParam);
		wrap_layout.setOrientation(LinearLayout.VERTICAL);
		wrap_layout.addView(head_layout);
		wrap_layout.addView(myLogScroller);
		setContentView(wrap_layout);
		progress = new ProgressDialog(this);
        progress.setMessage("Please wait ...");
		runBtn.setOnClickListener(new View.OnClickListener(){  
            public void onClick(View v) 
            { 
            	if(task != null) {
                    Toast.makeText(RunScript.this, "Task is running ...", Toast.LENGTH_LONG).show();
                    return ;
                }
                //progress.show();
                task = new RunTask();
                task.execute((Void) null);
                userInfo = getSharedPreferences("user_info", 0);
                userInfo.edit().putString("inputScript", inputScript.getText().toString())
                																.commit();
            } 
        });
	}

	public String getScriptPath(){
		File sdDir = null;
		boolean sdCardExist = Environment.getExternalStorageState()
							.equals(android.os.Environment.MEDIA_MOUNTED); //判断sd卡是否存在
		if (sdCardExist)
		{
			sdDir = Environment.getExternalStorageDirectory();//获取根目录
			return sdDir.toString() + "/zengl_language/scripts";
		}
		else
			return inner_script_dir = "/data/data/"+getApplicationContext().getPackageName()+
										"/scripts";
	}
	
	class RunTask extends AsyncTask<Void, String, Boolean> {

		@Override  
        protected void onPreExecute() {  
			mylog.setText(logContent = "");
        	runBtn.setEnabled(false);  
        } 
		
        @Override
        public void onPostExecute(Boolean result) {
        	runBtn.setEnabled(true);
			task = null;
			//progress.dismiss();
        }

		@Override
		protected Boolean doInBackground(Void... params) {
			// TODO Auto-generated method stub
			inputName = inputScript.getText().toString();
			RunZenglFromJNI(data_scripts_dir,inputName,debuginfo);
			return null;
		}
		
		@Override
		protected void onProgressUpdate(String... values) {
			mylog.append(values[0]); 
		}
		
		public void myCallBack(String arg)
		{
			publishProgress(arg);
		}
    }
	
	public void PrintCallBack(String arg)
	{
		//logContent += arg;
		task.myCallBack(arg);  
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		menu.add(0, 1, 1, "退出");  
        menu.add(0, 2, 2, "关于");  
        menu.add(0, 3, 3, "生成调试信息");  
        return super.onCreateOptionsMenu(menu);  
	}

	@Override  
    public boolean onOptionsItemSelected(MenuItem item) {  
        // TODO Auto-generated method stub  
        if(item.getItemId()==1)  
        {  
            finish();  
        }
        else if(item.getItemId()==2)  
        {  
            Toast toast=Toast.makeText(this, "Powered by zengl\n www.zengl.com", Toast.LENGTH_SHORT);  
            toast.show();  
        } 
        else if(item.getItemId()==3)
        {
        	if(debuginfo == 0)
        	{
	        	item.setTitle("不生成调试信息");
	        	debuginfo = 1;
        	}
        	else
        	{
        		item.setTitle("生成调试信息");
	        	debuginfo = 0;
        	}
        }
        return super.onOptionsItemSelected(item);  
    }  
	
    public boolean fileIsExists(String filename){
        File f=new File(filename);
        if(!f.exists()){
                return false;
        }
        return true;
    }
	
    private static boolean copyAssetFolder(AssetManager assetManager,
            String fromAssetPath, String toPath) {
        try {
            String[] files = assetManager.list(fromAssetPath);
            new File(toPath).mkdirs();
            boolean res = true;
            for (String file : files)
                if (file.contains("."))
                    res &= copyAsset(assetManager, 
                            fromAssetPath + "/" + file,
                            toPath + "/" + file);
                else 
                    res &= copyAssetFolder(assetManager, 
                            fromAssetPath + "/" + file,
                            toPath + "/" + file);
            return res;
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }
    
    private static boolean copyAsset(AssetManager assetManager,
            String fromAssetPath, String toPath) {
        InputStream in = null;
        OutputStream out = null;
        try {
          in = assetManager.open(fromAssetPath);
          new File(toPath).createNewFile();
          out = new FileOutputStream(toPath);
          copyFile(in, out);
          in.close();
          in = null;
          out.flush();
          out.close();
          out = null;
          return true;
        } catch(Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    private static void copyFile(InputStream in, OutputStream out) throws IOException {
        byte[] buffer = new byte[1024];
        int read;
        while((read = in.read(buffer)) != -1){
          out.write(buffer, 0, read);
        }
    }

    
	/* A native method that is implemented by the
     * 'hello-jni' native library, which is packaged
     * with this application.
     */
    public native String  RunZenglFromJNI(String Path,String s,int debuginfo);

    /* This is another native method declaration that is *not*
     * implemented by 'hello-jni'. This is simply to show that
     * you can declare as many native methods in your Java code
     * as you want, their implementation is searched in the
     * currently loaded native libraries only the first time
     * you call them.
     *
     * Trying to call this function will result in a
     * java.lang.UnsatisfiedLinkError exception !
     */
    public native String  unimplementedStringFromJNI();

    /* this is used to load the 'hello-jni' library on application
     * startup. The library has already been unpacked into
     * /data/data/com.example.hellojni/lib/libhello-jni.so at
     * installation time by the package manager.
     */
    static {
        System.loadLibrary("runscript");
    }
}
