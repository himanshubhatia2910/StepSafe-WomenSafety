package com.example.stepsafe_womensafety;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;

import java.util.HashMap;

public class Information extends AppCompatActivity {
    EditText name,contact,blood,age,dob,weight,height,emergency;
    TextView save;
    FirebaseAuth mAuth;
    DatabaseReference reference;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_information);
        name = findViewById(R.id.name);
        save = findViewById(R.id.save);
        contact = findViewById(R.id.contact);
        blood = findViewById(R.id.blood);
        age = findViewById(R.id.age);
        dob = findViewById(R.id.dob);
        weight = findViewById(R.id.weight);
        height = findViewById(R.id.height);
        emergency = findViewById(R.id.emergency);
        save.setOnClickListener(view ->{
            String username = name.getText().toString();
            String phone = contact.getText().toString();
            String blood_group = blood.getText().toString();
            String user_age = age.getText().toString();
            String user_dob = dob.getText().toString();
            String user_weight = weight.getText().toString();
            String user_height = height.getText().toString();
            String emergency_contact = emergency.getText().toString();;
            String email = getIntent().getStringExtra("email");
            String password = getIntent().getStringExtra("password");
//            FirebaseUser firebaseUser = mAuth.getCurrentUser();
//            assert firebaseUser != null;
//            String userid = firebaseUser.getUid();
//            reference = FirebaseDatabase.getInstance().getReference("Users").child(userid);
            mAuth = FirebaseAuth.getInstance();
            FirebaseUser firebaseUser = mAuth.getCurrentUser();
            assert firebaseUser != null;
            String userid = firebaseUser.getUid();
            reference = FirebaseDatabase.getInstance().getReference("Users").child(userid);
            HashMap<String, String> hashMap = new HashMap<>();
            hashMap.put("id", userid);
            hashMap.put("email",email);
            hashMap.put("password",password);
            hashMap.put("name", username);
            hashMap.put("phone_number", phone);
            hashMap.put("blood_group", blood_group);
            hashMap.put("age", user_age);
            hashMap.put("dob", user_dob);
            hashMap.put("weight", user_weight);
            hashMap.put("height", user_height);
            hashMap.put("emergency_contact", emergency_contact);
            reference.setValue(hashMap).addOnCompleteListener(new OnCompleteListener<Void>() {
                @Override
                public void onComplete(@NonNull Task<Void> task) {
                    if (task.isSuccessful()){
                        Intent intent = new Intent(Information.this, Home.class);
                        Toast.makeText(Information.this, "Information Stored Successfully", Toast.LENGTH_SHORT).show();
                        intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK | Intent.FLAG_ACTIVITY_NEW_TASK);
                        startActivity(intent);
                        finish();
                    }
                }
            });
        });
    }
}