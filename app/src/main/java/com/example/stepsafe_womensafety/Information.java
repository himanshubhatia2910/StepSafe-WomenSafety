package com.example.stepsafe_womensafety;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.app.ProgressDialog;
import android.content.ContentResolver;
import android.content.Intent;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import com.google.android.gms.tasks.Continuation;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.Task;
import android.provider.MediaStore;
import android.text.TextUtils;
import android.view.View;
import android.webkit.MimeTypeMap;
import android.widget.EditText;
import com.google.firebase.storage.StorageTask;
import com.google.firebase.storage.UploadTask;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.example.stepsafe_womensafety.databinding.ActivityMainBinding;
import com.google.android.gms.tasks.Continuation;
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
import com.google.firebase.storage.StorageTask;
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
    private final int  SELECT_PICTURE = 22;
    StorageReference storageReference;
    ProgressDialog progressDialog;
    private Uri selectedImageUri;
    FirebaseStorage storage;
    ActivityMainBinding binding;
    private StorageTask uploadTask;
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
            String emergency_contact = emergency.getText().toString();
            String email = getIntent().getStringExtra("email");
            //String password = getIntent().getStringExtra("password");
//            FirebaseUser firebaseUser = mAuth.getCurrentUser();
//            assert firebaseUser != null;
//            String userid = firebaseUser.getUid();
//            reference = FirebaseDatabase.getInstance().getReference("Users").child(userid);
            if(TextUtils.isEmpty(username) || TextUtils.isEmpty(phone) ||TextUtils.isEmpty(blood_group) ||TextUtils.isEmpty(user_age) ||
                    TextUtils.isEmpty(user_dob) ||TextUtils.isEmpty(user_weight) ||TextUtils.isEmpty(user_height) ||
                    TextUtils.isEmpty(emergency_contact)){
                Toast.makeText(Information.this, "Please Enter all the Fields", Toast.LENGTH_SHORT).show();
            }
            else{
            mAuth = FirebaseAuth.getInstance();
            FirebaseUser firebaseUser = mAuth.getCurrentUser();
            assert firebaseUser != null;
            String userid = firebaseUser.getUid();
            reference = FirebaseDatabase.getInstance().getReference("Users").child(userid);
            HashMap<String, String> hashMap = new HashMap<>();
            hashMap.put("id", userid);
            hashMap.put("email",email);
            //hashMap.put("password",password);
                hashMap.put("imageUrl","default");
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
            });}
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
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);


            // compare the resultCode with the
            // SELECT_PICTURE constant
        if (requestCode == SELECT_PICTURE
                && resultCode == RESULT_OK
                && data != null
                && data.getData() != null){
                // Get the url of the image from data
                selectedImageUri = data.getData();
                try {
                    // Setting image on image view using Bitmap
                    Bitmap bitmap = MediaStore
                            .Images
                            .Media
                            .getBitmap(
                                    getContentResolver(),
                                    selectedImageUri);
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

    private String getFileExtension(Uri uri){
        ContentResolver contentResolver = this.getContentResolver();
        MimeTypeMap mimeTypeMap = MimeTypeMap.getSingleton();
        return mimeTypeMap.getExtensionFromMimeType(contentResolver.getType(uri));
    }
    private void uploadImage() {
        if (selectedImageUri != null) {
//            progressDialog = new ProgressDialog(this);
//            progressDialog.setTitle("Uploading File....");
//            progressDialog.show();
            final  StorageReference fileReference = storageReference.child(System.currentTimeMillis()
                    +"."+getFileExtension(selectedImageUri));
//            SimpleDateFormat formatter = new SimpleDateFormat("yyyy_MM_dd_HH_mm_ss", Locale.CANADA);
//            Date now = new Date();
//            String fileName = formatter.format(now);
            StorageReference ref = storageReference.child("images/" + UUID.randomUUID() + toString());

            uploadTask = fileReference.putFile(selectedImageUri);
           // ref.putFile(selectedImageUri)
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
            uploadTask.continueWithTask(new Continuation<UploadTask.TaskSnapshot, Task<Uri>>() {
                        @Override
                        public Task<Uri> then(@NonNull Task<UploadTask.TaskSnapshot> task) throws Exception {
                            if (!task.isSuccessful()){
                                throw  task.getException();
                            }

                            return  fileReference.getDownloadUrl();
                        }
                    })
                    .addOnCompleteListener(new OnCompleteListener<Uri>() {
                        @Override
                        public void onComplete(@NonNull Task<Uri> task) {
                            if (task.isSuccessful()){
                                Uri downloadUri = task.getResult();
                                String mUri = downloadUri.toString();

                                mAuth = FirebaseAuth.getInstance();
                                FirebaseUser firebaseUser = mAuth.getCurrentUser();
                                assert firebaseUser != null;
                                String userid = firebaseUser.getUid();
                                reference = FirebaseDatabase.getInstance().getReference("Users").child(userid);
                                HashMap<String, Object> map = new HashMap<>();
                                map.put("imageURL", ""+mUri);
                                reference.updateChildren(map);
                            } else {
                                Toast.makeText(Information.this, "Failed!", Toast.LENGTH_SHORT).show();
                            }
                            Toast.makeText(Information.this, "Uploaded Successfully!", Toast.LENGTH_SHORT).show();
                        }
                    }).addOnFailureListener(e -> {


                        if (progressDialog.isShowing())
                            progressDialog.dismiss();
                        Toast.makeText(Information.this, "Failed to Upload", Toast.LENGTH_SHORT).show();


                    });
        }}
    }
