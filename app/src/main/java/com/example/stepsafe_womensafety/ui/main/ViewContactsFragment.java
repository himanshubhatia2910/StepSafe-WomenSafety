package com.example.stepsafe_womensafety.ui.main;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import com.example.stepsafe_womensafety.databinding.FragmentMainBinding;

public class ViewContactsFragment extends Fragment {

    private MainViewModel mViewModel;
    private FragmentMainBinding binding;


    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container,
                              Bundle savedInstanceState) {
        MainViewModel viewContacts =
                new ViewModelProvider(this).get(MainViewModel.class);
        binding = FragmentMainBinding.inflate(inflater, container, false);
        View root = binding.getRoot();
        final TextView textView = binding.textHome;
        viewContacts.getText().observe(getViewLifecycleOwner(), textView::setText);
        return root;
    }
    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }

}