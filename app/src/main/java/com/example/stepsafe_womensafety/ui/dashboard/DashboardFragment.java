package com.example.stepsafe_womensafety.ui.dashboard;



import static android.Manifest.permission.CALL_PHONE;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;

import android.os.Build;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import android.widget.Toast;


import androidx.annotation.NonNull;

import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import com.example.stepsafe_womensafety.R;
import com.example.stepsafe_womensafety.databinding.FragmentDashboardBinding;

public class DashboardFragment extends Fragment {

    private FragmentDashboardBinding binding;
    TextView police,women,domestic,ambulance,fire;
    static int PERMISSION_CODE= 100;
    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        DashboardViewModel dashboardViewModel =
                new ViewModelProvider(this).get(DashboardViewModel.class);

        binding = FragmentDashboardBinding.inflate(inflater, container, false);
        View root = binding.getRoot();
        police= root.findViewById(R.id.police);
        women= root.findViewById(R.id.women);
        domestic = root.findViewById(R.id.domestic);
        ambulance = root.findViewById(R.id.ambulance);
        fire = root.findViewById(R.id.fire);
        police.setOnClickListener(view ->{
            if (ContextCompat.checkSelfPermission(getContext(), Manifest.permission.CALL_PHONE) == PackageManager.PERMISSION_GRANTED) {
                Intent callIntent = new Intent(Intent.ACTION_CALL);
                callIntent.setData(Uri.parse("tel:" + "100"));
                getActivity().startActivity(callIntent);
            } else {
                ActivityCompat.requestPermissions(getActivity(), new String[]{Manifest.permission.CALL_PHONE},
                        PERMISSION_CODE);
            }

        });
        women.setOnClickListener(view ->{
            if (ContextCompat.checkSelfPermission(getContext(), Manifest.permission.CALL_PHONE) == PackageManager.PERMISSION_GRANTED) {
                Intent callIntent = new Intent(Intent.ACTION_CALL);
                callIntent.setData(Uri.parse("tel:" + "1091"));
                getActivity().startActivity(callIntent);
            } else {
                ActivityCompat.requestPermissions(getActivity(), new String[]{Manifest.permission.CALL_PHONE},
                        PERMISSION_CODE);
            }

        });
        domestic.setOnClickListener(view ->{
            if (ContextCompat.checkSelfPermission(getContext(), Manifest.permission.CALL_PHONE) == PackageManager.PERMISSION_GRANTED) {
                Intent callIntent = new Intent(Intent.ACTION_CALL);
                callIntent.setData(Uri.parse("tel:" + "181"));
                getActivity().startActivity(callIntent);
            } else {
                ActivityCompat.requestPermissions(getActivity(), new String[]{Manifest.permission.CALL_PHONE},
                        PERMISSION_CODE);
            }

        });
        ambulance.setOnClickListener(view ->{
            if (ContextCompat.checkSelfPermission(getContext(), Manifest.permission.CALL_PHONE) == PackageManager.PERMISSION_GRANTED) {
                Intent callIntent = new Intent(Intent.ACTION_CALL);
                callIntent.setData(Uri.parse("tel:" + "102"));
                getActivity().startActivity(callIntent);
            } else {
                ActivityCompat.requestPermissions(getActivity(), new String[]{Manifest.permission.CALL_PHONE},
                        PERMISSION_CODE);
            }

        });
        fire.setOnClickListener(view ->{

            if (ContextCompat.checkSelfPermission(getContext(), Manifest.permission.CALL_PHONE) == PackageManager.PERMISSION_GRANTED) {
                Intent callIntent = new Intent(Intent.ACTION_CALL);
                callIntent.setData(Uri.parse("tel:" + "101"));
                getActivity().startActivity(callIntent);
            } else {
                ActivityCompat.requestPermissions(getActivity(), new String[]{Manifest.permission.CALL_PHONE},
                        PERMISSION_CODE);
            }

        });
        //final TextView textView = binding.textDashboard;
        //dashboardViewModel.getText().observe(getViewLifecycleOwner(), textView::setText);
        return root;
    }
    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }
}