package com.example.stepsafe_womensafety;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.text.method.HideReturnsTransformationMethod;
import android.text.method.PasswordTransformationMethod;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

public class Login extends AppCompatActivity {
    ImageView ShowHidePass;
    TextView  password,SignUp;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);
        ShowHidePass=findViewById(R.id.show_pass_btn);
        password=findViewById(R.id.password);
        SignUp=findViewById(R.id.signUp);
        ShowHidePass.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                if(view.getId()==R.id.show_pass_btn){
                    if(password.getTransformationMethod().equals(PasswordTransformationMethod.getInstance())){
                        ((ImageView)(view)).setImageResource(R.drawable.eyeshow);
                        //Show Password
                        password.setTransformationMethod(HideReturnsTransformationMethod.getInstance());
                    }
                    else{
                        ((ImageView)(view)).setImageResource(R.drawable.eyehide);
                        //Hide Password
                        password.setTransformationMethod(PasswordTransformationMethod.getInstance());
                    }
                }
            }
        });
        SignUp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent=new Intent(getApplicationContext(),CreateAccount.class);
                startActivity(intent);
                
            }
        });
    }
}