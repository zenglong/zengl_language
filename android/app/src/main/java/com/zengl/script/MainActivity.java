package com.zengl.script;

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

public class MainActivity extends Activity {
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
        // 获取zengl测试脚本的存放路径
        data_scripts_dir = getScriptPath();
        String script_exist_flag_file = data_scripts_dir + "/script_exist_flag3";
        // 如果手机存储空间存放zengl测试脚本的目录中没有script_exist_flag3文件，
        // 则将app中打包进来的assets资源中的scripts目录下存放的zengl测试脚本等文件，拷贝到data_scripts_dir对应的手机存储位置
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
        // 第一次执行脚本时，默认在输入框中的需要运行的脚本文件名是test.zl
        if(input == "")
            inputScript.setText("test.zl");
        else // 如果之前执行过某脚本，则输入框中显示的会是之前执行过的脚本文件名
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
        // mylog文本框用于记录zengl脚本在执行过程中的输出信息，例如脚本中通过print语句输出的信息等
        mylog = new TextView(this);
        mylog.setText(logContent = "脚本目录位置:" + data_scripts_dir);
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
        // 当用户点击了运行按钮后，会触发下面的onClick方法，在该方法中，会启动一个异步任务，并在异步任务中根据用户在输入框中输入的脚本文件名，来运行zengl脚本
        runBtn.setOnClickListener(new View.OnClickListener(){
            public void onClick(View v)
            {
                if(task != null) {
                    Toast.makeText(MainActivity.this, "Task is running ...", Toast.LENGTH_LONG).show();
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

    /**
     * 获取zengl测试脚本的路径信息，需要运行的zengl测试脚本都会先拷贝到该路径下
     * @return 返回存放测试脚本的路径
     */
    public String getScriptPath(){
        String cachePath;
        if (Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())
                || !Environment.isExternalStorageRemovable()) {
            cachePath = getApplicationContext().getExternalFilesDir("myfiles").getAbsolutePath() +
                    "/zengl_language/scripts";
//            cachePath = context.getExternalCacheDir().getPath();//也可以这么写，只是返回的路径不一样，具体打log看
        } else {
            cachePath = getApplicationContext().getFilesDir().getAbsolutePath() + "/scripts";
//            cachePath = context.getCacheDir().getPath();//也可以这么写，只是返回的路径不一样，具体打log看
        }
        return cachePath;


        /* File sdDir = null;
        boolean sdCardExist = Environment.getExternalStorageState()
                .equals(android.os.Environment.MEDIA_MOUNTED); //判断sd卡是否存在
        if (sdCardExist)
        {
            sdDir = Environment.getExternalStorageDirectory();//获取根目录
            return sdDir.toString() + "/zengl_language/scripts";
        }
        else
            return inner_script_dir = "/data/data/"+getApplicationContext().getPackageName()+
                    "/scripts";*/
    }

    /**
     * 为了防止阻塞UI线程，会在异步任务中执行zengl脚本
     * TODO AsyncTask是一个过时的类，目前暂时不影响编译和运行，以后有时间再研究其他的替代方案！！！
     */
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
        // 当选择第一个“退出”菜单时，会退出App
        if(item.getItemId()==1)
        {
            finish();
        }
        else if(item.getItemId()==2) // 当选择第二个“关于”菜单时，通过Toast显示Powered by zengl及官网地址信息
        {
            Toast toast=Toast.makeText(this, "Powered by zengl\n www.zengl.com", Toast.LENGTH_SHORT);
            toast.show();
        }
        else if(item.getItemId()==3) // 第三个菜单可以设置是否生成调试信息，当选择了生成调试信息时，会将zengl脚本的编译信息写入zengl_debuglogs.txt日志文件
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

    /**
     * 判断指定的文件是否存在
     * @param filename 需要进行判断的文件对应的文件路径
     * @return 返回true表示文件存在，否则表示文件不存在
     */
    public boolean fileIsExists(String filename){
        File f=new File(filename);
        if(!f.exists()){
            return false;
        }
        return true;
    }

    /**
     * 将assets中的资源目录拷贝到手机存储中的目标位置
     * @param assetManager 资源管理器，可以获取到某个资源目录中的文件列表，还可以打开指定的资源文件
     * @param fromAssetPath 需要拷贝的源资源目录
     * @param toPath 需要拷贝到手机存储中的目标位置
     * @return 返回true表示拷贝成功，否则表示拷贝失败
     */
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

    /**
     * 将单个资源文件拷贝到手机存储的目标位置
     * @param assetManager 资源管理器，用于打开指定的资源文件
     * @param fromAssetPath 需要拷贝的资源文件的资源路径
     * @param toPath  需要拷贝到手机存储中的目标位置
     * @return 返回true表示拷贝成功，否则表示拷贝失败
     */
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

    /**
     * 通过输入输出流执行具体的文件拷贝操作
     * @param in 来源文件输入流
     * @param out 目标文件输出流
     * @throws IOException 如果发生IO错误，则抛出IOException异常
     */
    private static void copyFile(InputStream in, OutputStream out) throws IOException {
        byte[] buffer = new byte[1024];
        int read;
        while((read = in.read(buffer)) != -1){
            out.write(buffer, 0, read);
        }
    }

    static {
        System.loadLibrary("runscript"); // 加载runscript库，里面包含了执行zengl脚本相关的底层C函数
    }

    // RunZenglFromJNI是runscript库中定义的C函数(实际的C函数名是Java_com_zengl_script_MainActivity_RunZenglFromJNI)，是执行zengl脚本的入口函数
    // 第一个参数Path表示zengl测试脚本在手机存储中的目录路径
    // 第二个参数s表示需要执行的脚本文件名
    // 第三个参数debuginfo表示是否需要将zengl脚本相关的编译信息写入zengl_debuglogs.txt日志文件
    public native String  RunZenglFromJNI(String Path,String s,int debuginfo);
}