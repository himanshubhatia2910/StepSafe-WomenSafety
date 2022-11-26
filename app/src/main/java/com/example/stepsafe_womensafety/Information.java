package com.example.stepsafe_womensafety;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.app.ProgressDialog;
import android.content.Intent;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.example.stepsafe_womensafety.databinding.ActivityMainBinding;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.storage.FirebaseStorage;
import com.google.firebase.storage.StorageReference;
import com.google.firebase.storage.UploadTask;

import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Locale;
import java.util.Objects;
import java.util.UUID;

public class Information extends AppCompatActivity {
    EditText name,contact,blood,age,dob,weight,height,emergency;
    TextView save;
    FirebaseAuth mAuth;
    DatabaseReference reference;
    ImageView IVPreviewImage;
    int SELECT_PICTURE = 200;
    StorageReference storageReference;
    ProgressDialog progressDialog;
    Uri selectedImageUri;
    FirebaseStorage storage;
    ActivityMainBinding binding;
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
        IVPreviewImage = findViewById(R.id.photo);
        storage = FirebaseStorage.getInstance();
        storageReference = storage.getReference();
        IVPreviewImage.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                imageChooser();
            }




        });
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
    private void imageChooser() {
        Intent i = new Intent();
        i.setType("image/*");
        i.setAction(Intent.ACTION_GET_CONTENT);

        // pass the constant to compare it
        // with the returned requestCode
        startActivityForResult(Intent.createChooser(i, "Select Picture"), SELECT_PICTURE);
    }
    // this function is triggered when user
    // selects the image from the imageChooser
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode == RESULT_OK) {

            // compare the resultCode with the
            // SELECT_PICTURE constant
            if (requestCode == SELECT_PICTURE) {
                // Get the url of the image from data
                selectedImageUri = data.getData();
                try {
                    Bitmap bitmap = MediaStore.Images.Media.getBitmap(getContentResolver(), selectedImageUri);
                    IVPreviewImage.setImageBitmap(bitmap);
                    uploadImage();
                }
                catch (IOException e)
                {
                    e.printStackTrace();
                }
//                if (null != selectedImageUri) {
//                    // update the preview image in the layout
//                    IVPreviewImage.setImageURI(selectedImageUri);

            }
        }
    }

    private void uploadImage() {
        progressDialog = new ProgressDialog(this);
        progressDialog.setTitle("Uploading File....");
        progressDialog.show();

        SimpleDateFormat formatter = new SimpleDateFormat("yyyy_MM_dd_HH_mm_ss", Locale.CANADA);
        Date now = new Date();
        String fileName = formatter.format(now);
        StorageReference ref = storageReference.child("images/");


        ref.putFile(selectedImageUri)
//                .addOnSuccessListener(taskSnapshot -> {
//
//
//                    //binding.firebaseimage.setImageURI(null);
//                   IVPreviewImage.setImageURI(selectedImageUri);//testng
//
//                    Toast.makeText(Information.this, "Successfully Uploaded", Toast.LENGTH_SHORT).show();
//                    if (progressDialog.isShowing())
//                        progressDialog.dismiss();
//
//                })
                .addOnSuccessListener(new OnSuccessListener<UploadTask.TaskSnapshot>() {
                    @Override
                    public void onSuccess(UploadTask.TaskSnapshot taskSnapshot) {
                        //IVPreviewImage.setImageURI(selectedImageUri);
                        progressDialog.dismiss();
                        Toast.makeText(Information.this, "Uploaded", Toast.LENGTH_SHORT).show();
                    }
                })
                .addOnFailureListener(e -> {


                    if (progressDialog.isShowing())
                        progressDialog.dismiss();
                    Toast.makeText(Information.this,"Failed to Upload",Toast.LENGTH_SHORT).show();


                });

    }
}