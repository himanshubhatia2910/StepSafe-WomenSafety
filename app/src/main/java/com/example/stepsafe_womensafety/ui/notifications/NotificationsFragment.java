package com.example.stepsafe_womensafety.ui.notifications;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import com.example.stepsafe_womensafety.Home;
import com.example.stepsafe_womensafety.Information;
import com.example.stepsafe_womensafety.R;
import com.example.stepsafe_womensafety.databinding.FragmentNotificationsBinding;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;

import java.util.HashMap;

public class NotificationsFragment extends Fragment {

    private FragmentNotificationsBinding binding;
    TextView name,contact;
    Button save;
    FirebaseAuth mAuth;
    DatabaseReference reference;
    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        NotificationsViewModel notificationsViewModel =
                new ViewModelProvider(this).get(NotificationsViewModel.class);

        binding = FragmentNotificationsBinding.inflate(inflater, container, false);
        View root = binding.getRoot();
        name = root.findViewById(R.id.name);
        contact = root.findViewById(R.id.contact);
        save = root.findViewById(R.id.save);
        save.setOnClickListener(view ->{
            String user = name.getText().toString();
            String contact_no = contact.getText().toString();
            if(TextUtils.isEmpty(user) || TextUtils.isEmpty(contact_no)){
                Toast.makeText(getContext(), "Please Enter all the Fields", Toast.LENGTH_SHORT).show();
            }
            mAuth = FirebaseAuth.getInstance();
            FirebaseUser firebaseUser = mAuth.getCurrentUser();
            assert firebaseUser != null;
            String userid = firebaseUser.getUid();
            reference = FirebaseDatabase.getInstance().getReference("Users").child(userid).push();
            HashMap<String, String> hashMap = new HashMap<>();
            hashMap.put("id", userid);
            hashMap.put("emergency_contact_new",contact_no);
            hashMap.put("contact_name",user);
            reference.setValue(hashMap).addOnCompleteListener(new OnCompleteListener<Void>() {
                @Override
                public void onComplete(@NonNull Task<Void> task) {
                    if (task.isSuccessful()){
                        Toast.makeText(getContext(), "Information Stored Successfully", Toast.LENGTH_SHORT).show();
                    }
                }
            });
        });
        //final TextView textView = binding.textNotifications;
        //notificationsViewModel.getText().observe(getViewLifecycleOwner(), textView::setText);
        return root;
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }
}