package com.example.stepsafe_womensafety;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Bundle;
import android.widget.TextView;
import android.widget.Toast;

public class Helpline extends AppCompatActivity {
    TextView police;
    static int PERMISSION_CODE= 100;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_helpline);
        police = findViewById(R.id.police);
        if (ContextCompat.checkSelfPermission(Helpline.this,Manifest.permission.CALL_PHONE) != PackageManager.PERMISSION_GRANTED){

            ActivityCompat.requestPermissions(Helpline.this,new String[]{Manifest.permission.CALL_PHONE},PERMISSION_CODE);
        }
        police.setOnClickListener(view ->{
            Intent callIntent = new Intent(Intent.ACTION_CALL);
            callIntent.setData(Uri.parse("tel:7887575773"));
            startActivity(callIntent);
        });
    }
}