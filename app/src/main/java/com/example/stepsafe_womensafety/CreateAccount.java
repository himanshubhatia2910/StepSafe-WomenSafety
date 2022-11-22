package com.example.stepsafe_womensafety;
import androidx.appcompat.app.AppCompatActivity;
import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.text.method.HideReturnsTransformationMethod;
import android.text.method.PasswordTransformationMethod;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;


public class CreateAccount extends AppCompatActivity {
    private static final String FILE_NAME = "example1.txt";
    private static final String FILE_NAME2 = "example2.txt";

    EditText email;
    EditText password;
    TextView signup;
    TextView result;
    ImageView showHidePass;

    @SuppressLint("MissingInflatedId")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_create_account);

        email = findViewById(R.id.email);
        password = findViewById(R.id.password);
        signup = findViewById(R.id.signUp);
        showHidePass = findViewById(R.id.show_pass_btn);
        showHidePass.setOnClickListener(new View.OnClickListener() {
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

        signup.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                    String text1 = email.getText().toString();
                    String text2 = password.getText().toString();


                    FileOutputStream fos = null;
                    FileOutputStream fos2;

                    try {
                        fos = openFileOutput(FILE_NAME, MODE_PRIVATE);
                        fos2 = openFileOutput(FILE_NAME2, MODE_PRIVATE);
                        fos.write(text1.getBytes());
                        fos2.write(text2.getBytes());

                        email.getText().clear();
                        password.getText().clear();
                        //Toast.makeText(this, "Saved to " + getFilesDir() + "/" + FILE_NAME, Toast.LENGTH_LONG).show();

                    } catch (FileNotFoundException e) {
                        e.printStackTrace();
                    } catch (IOException e) {
                        e.printStackTrace();
                    } finally {
                        if (fos != null) {
                            try {
                                fos.close();
                            } catch (IOException e) {
                                e.printStackTrace();
                            }
                        }
                    }




                    //File data  retrival code working correctly
//                    FileInputStream fis = null;
//                    FileInputStream fis2 = null;
//
//                    try {
//                        fis = openFileInput(FILE_NAME);
//                        fis2 = openFileInput(FILE_NAME2);
//                        InputStreamReader isr = new InputStreamReader(fis);
//                        InputStreamReader isr2 = new InputStreamReader(fis2);
//                        BufferedReader br = new BufferedReader(isr);
//                        BufferedReader br2 = new BufferedReader(isr2);
//                        StringBuilder sb = new StringBuilder();
//                        StringBuilder sb2 = new StringBuilder();
//                        String text;
//                        String text3;
//
//                        while ((text = br.readLine()) != null) {
//                            sb.append(text).append("\n");
//                        }
//                        //result.setText(sb.toString());
//                        while ((text3 = br2.readLine()) != null) {
//                            sb2.append(text3).append("\n");
//                        }
//                        //result.setText(sb2.toString());
//
//                    } catch (FileNotFoundException e) {
//                        e.printStackTrace();
//                    } catch (IOException e) {
//                        e.printStackTrace();
//                    } finally {
//                        if (fis != null) {
//                            try {
//                                fis.close();
//                            } catch (IOException e) {
//                                e.printStackTrace();
//                            }
//                        }
//                        if (fis2 != null) {
//                            try {
//                                fis2.close();
//                            } catch (IOException e) {
//                                e.printStackTrace();
//                            }
//                        }
//                    }

                Intent intent=new Intent(CreateAccount.this,Information.class);
                startActivity(intent);
                finish();


            }
        });
    }
}