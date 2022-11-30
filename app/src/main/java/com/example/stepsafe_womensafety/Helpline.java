package com.example.stepsafe_womensafety;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;
import android.widget.Toast;

public class Helpline extends AppCompatActivity {
    TextView police;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_helpline);
        police = findViewById(R.id.police);
        police.setOnClickListener(view ->{
            Toast.makeText(Helpline.this, "User registered successfully", Toast.LENGTH_SHORT).show();
        });
    }
}