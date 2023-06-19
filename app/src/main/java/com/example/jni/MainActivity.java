package com.example.jni;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import com.example.jni.databinding.ActivityMainBinding;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'jni' library on application startup.
    static {
        System.loadLibrary("jni");
    }

    private ActivityMainBinding binding;
    private String logFilePath;

    public String ensureLogFile() {
        if (this.logFilePath == null) {
            File[] dirs = getExternalFilesDirs(Environment.DIRECTORY_DOCUMENTS);
            String dir = null;
            if (dirs.length > 0) {
                dir = dirs[0].getAbsolutePath();
            } else {
                return null;
            }

            File file = new File(dir, "leak_report.txt");
            String filePath = file.getAbsolutePath();
            Log.d("log file path: ", filePath);
            if (!file.exists()) {
                try {
                    file.createNewFile();
                    Log.d("", "创建日志文件成功");
                } catch (IOException e) {
                    Log.d("", "创建日志文件失败");
                    throw new RuntimeException(e);
                }
            }
            this.logFilePath = filePath;
        }
        return this.logFilePath;
    }

    public void showLogContent() {
        String filePath = this.ensureLogFile();
        try {
            File file = new File(filePath);
            InputStream inputStream = new FileInputStream(file);
            byte[] bytes = new byte[(int) file.length()];
            inputStream.read(bytes);
            String content = new String(bytes);
            inputStream.close();
            Log.d("", "File content: \n" + content);
        } catch (FileNotFoundException e) {
            throw new RuntimeException(e);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        String file = this.ensureLogFile();
        initLeakTracer(file);
        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());

        findViewById(R.id.button).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                MainActivity.this.showLogContent();
            }
        });
    }

    /**
     * A native method that is implemented by the 'jni' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native void initLeakTracer(String path);
}