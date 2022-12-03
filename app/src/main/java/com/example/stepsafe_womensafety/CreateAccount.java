package com.example.stepsafe_womensafety;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import android.content.Intent;
import android.os.Bundle;
import android.text.TextUtils;
import android.text.method.HideReturnsTransformationMethod;
import android.text.method.PasswordTransformationMethod;
import android.util.Log;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;

import java.util.HashMap;


public class CreateAccount extends AppCompatActivity {

    TextView etRegEmail;
    TextView etRegPassword;
    TextView tvLoginHere;
    TextView btnRegister;
    FirebaseAuth mAuth;
    ImageView ShowHidePass;

    DatabaseReference reference;
    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);


        setContentView(R.layout.activity_create_account);
        ShowHidePass = findViewById(R.id.show_pass_btn);
        etRegEmail = findViewById(R.id.etRegEmail);
        etRegPassword = findViewById(R.id.etRegPass);
        tvLoginHere = findViewById(R.id.SignIn);//not necesary
        btnRegister = findViewById(R.id.btnRegister);
        ShowHidePass.setOnClickListener(view -> {

            if(view.getId()==R.id.show_pass_btn){
                if(etRegPassword.getTransformationMethod().equals(PasswordTransformationMethod.getInstance())){
                    ((ImageView)(view)).setImageResource(R.drawable.eyeshow);
                    //Show Password
                    etRegPassword.setTransformationMethod(HideReturnsTransformationMethod.getInstance());
                }
                else{
                    ((ImageView)(view)).setImageResource(R.drawable.eyehide);
                    //Hide Password
                    etRegPassword.setTransformationMethod(PasswordTransformationMethod.getInstance());
                }
            }
        });

        mAuth = FirebaseAuth.getInstance();

        btnRegister.setOnClickListener(view ->{
            createUser();
        });

        tvLoginHere.setOnClickListener(view ->{
            startActivity(new Intent(CreateAccount.this, Login.class));
        });
    }

    private void createUser(){
        String email = etRegEmail.getText().toString();
        String password = etRegPassword.getText().toString();

        if (TextUtils.isEmpty(email)){
            etRegEmail.setError("Email cannot be empty");
            etRegEmail.requestFocus();
        }else if (TextUtils.isEmpty(password)){
            etRegPassword.setError("Password cannot be empty");
            etRegPassword.requestFocus();
        }else{
            mAuth.createUserWithEmailAndPassword(email,password).addOnCompleteListener(new OnCompleteListener<AuthResult>() {
//                @Override
//                public void onComplete(@NonNull Task<AuthResult> task) {
//                    if (task.isSuccessful()) {
//                        FirebaseUser firebaseUser = mAuth.getCurrentUser();
//                        assert firebaseUser != null;
//                        String userid = firebaseUser.getUid();
//                        reference = FirebaseDatabase.getInstance().getReference("Users").child(userid);
//                        HashMap<String, String> hashMap = new HashMap<>();
//                        hashMap.put("id", userid);
//                        hashMap.put("email",email);
//                        hashMap.put("password",password);
//
//                        reference.setValue(hashMap).addOnCompleteListener(new OnCompleteListener<Void>() {
//                            @Override
//                            public void onComplete(@NonNull Task<Void> task) {
//                                if (task.isSuccessful()){
//                                    Intent intent = new Intent(CreateAccount.this, Login.class);
//                                    Toast.makeText(CreateAccount.this, "User registered successfully", Toast.LENGTH_SHORT).show();
//                                    intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK | Intent.FLAG_ACTIVITY_NEW_TASK);
//                                    startActivity(intent);
//                                    finish();
//                                }
//                            }
//                        });
//                    }
                    public void onComplete(@NonNull Task<AuthResult> task) {
                    if (task.isSuccessful()){
                        Toast.makeText(CreateAccount.this, "User registered successfully", Toast.LENGTH_SHORT).show();
                        Intent i= new Intent(CreateAccount.this, Information.class);
                        i.putExtra("email",email);
                        //i.putExtra("password",password);

                       startActivity(i);
                    }else{
                        Toast.makeText(CreateAccount.this, "Registration Error: " + task.getException().getMessage(), Toast.LENGTH_SHORT).show();
                    }
                }

            });
        }
    }

}