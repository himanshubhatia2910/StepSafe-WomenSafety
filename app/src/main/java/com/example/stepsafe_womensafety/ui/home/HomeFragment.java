package com.example.stepsafe_womensafety.ui.home;

import static android.content.Context.SENSOR_SERVICE;

import static androidx.core.content.ContextCompat.*;

import android.Manifest;
import android.app.Activity;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.IntentSender;
import android.content.pm.PackageManager;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.location.LocationManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Looper;
import android.telephony.SmsManager;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Switch;
import android.widget.TextView;
import com.bumptech.glide.Glide;
import android.widget.Toast;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import com.example.stepsafe_womensafety.MainActivity;
import com.example.stepsafe_womensafety.R;
import com.example.stepsafe_womensafety.databinding.FragmentHomeBinding;
import com.example.stepsafe_womensafety.ui.Users;
import com.google.android.gms.common.api.ApiException;
import com.google.android.gms.common.api.ResolvableApiException;
import com.google.android.gms.location.LocationCallback;
import com.google.android.gms.location.LocationRequest;
import com.google.android.gms.location.LocationResult;
import com.google.android.gms.location.LocationServices;
import com.google.android.gms.location.LocationSettingsRequest;
import com.google.android.gms.location.LocationSettingsResponse;
import com.google.android.gms.location.LocationSettingsStatusCodes;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class HomeFragment extends Fragment {
    ImageView photo;
    public static final String SENSOR_SERVICE = "sensor";
    TextView coordinates,username;
    private LocationRequest locationRequest;
    private FragmentHomeBinding binding;
    Button shareLocation;
    boolean flag = false;
    Context cont = getContext();
    Switch toggleSwitch;
    static int PERMISSION_CODE= 100;
    DatabaseReference reference;
    FirebaseUser fuser;
    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        HomeViewModel homeViewModel =
                new ViewModelProvider(this).get(HomeViewModel.class);
        binding = FragmentHomeBinding.inflate(inflater, container, false);

        View root = binding.getRoot();
        coordinates = root.findViewById(R.id.coordinates);
        shareLocation = root.findViewById(R.id.share);
        toggleSwitch = root.findViewById(R.id.idSwitch);
        username = root.findViewById(R.id.name);
        photo =root.findViewById(R.id.photo);
        //////////////////////////////////////////////////////////////////////////////////////////

        fuser = FirebaseAuth.getInstance().getCurrentUser();

        reference = FirebaseDatabase.getInstance().getReference("Users").child(fuser.getUid());
        reference.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                String user = (String) dataSnapshot.child("name").getValue();
                username.setText(user);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {
            }
        });
        reference.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                if(isAdded()){
                    String user = (String) dataSnapshot.child("imageURL").getValue();
                        Glide.with(getContext()).load(user).into(photo);
                }
            }
            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        });

        /////////////////////////////////////////////////////////////
        locationRequest = LocationRequest.create();
        locationRequest.setPriority(LocationRequest.PRIORITY_HIGH_ACCURACY);
        locationRequest.setInterval(5000);
        locationRequest.setFastestInterval(2000);


            toggleSwitch.setOnClickListener(view ->{
                if(toggleSwitch.isChecked()){
                    getCurrentLocation();
                    shareLocation.setEnabled(true);
                }else {
                    Toast.makeText(getContext(),"Turn on the toggle switch",Toast.LENGTH_SHORT).show();
                    coordinates.setText(" ");
                    shareLocation.setEnabled(false);
                }
                if(toggleSwitch.isChecked()){
                    shakeAlert();
                }
            });


        return root;

    }

    private void shakeAlert() {
        if (ContextCompat.checkSelfPermission(getActivity(),Manifest.permission.CALL_PHONE) != PackageManager.PERMISSION_GRANTED){

            ActivityCompat.requestPermissions(getActivity(),new String[]{Manifest.permission.CALL_PHONE},PERMISSION_CODE);
        }


        Context cont;
        cont=getActivity();
        SensorManager sensorManager = (SensorManager) cont.getSystemService(Context.SENSOR_SERVICE);
        Sensor sensorShake = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        SensorEventListener sensorEventListener = new SensorEventListener() {
            @Override
            public void onSensorChanged(SensorEvent sensorEvent) {
                if (sensorEvent!=null){
                    float x_accl = sensorEvent.values[0];
                    float y_accl = sensorEvent.values[1];
                    float z_accl = sensorEvent.values[2];

                    float floatSum = Math.abs(x_accl) + Math.abs(y_accl) + Math.abs(z_accl);
                    flag=true;


                    if (floatSum > 30){
                        //textView.setText("Yes, Shaking");
                        String phoneno = "7887575773";
                        String msg = "Test Shake Message";
                        flag=true;
                        //message not working but call working
                        try {
                            SmsManager sms = SmsManager.getDefault();
                            PendingIntent sentPI;
                            String SENT = "SMS_SENT";

                            sentPI = PendingIntent.getBroadcast(getContext(), 0,new Intent(SENT), 0);

                            sms.sendTextMessage(phoneno, null, msg, sentPI, null);




//                            SmsManager smsManager=SmsManager.getDefault();
//                            smsManager.sendTextMessage(phoneno,null,msg,null,null);
                            Toast.makeText(getContext(),"Message Sent",Toast.LENGTH_LONG).show();
                        }catch (Exception e)
                        {
                            Toast.makeText(getContext(),e.getLocalizedMessage(),Toast.LENGTH_LONG).show();
                            Log.d("tag",e.getLocalizedMessage());
                        }
//

                    }
                    else {
                        //textView.setText("No, NOT Shaking");
                    }
                }
            }

            @Override
            public void onAccuracyChanged(Sensor sensor, int i) {
            }
        };
        sensorManager.registerListener(sensorEventListener, sensorShake, SensorManager.SENSOR_DELAY_NORMAL);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);

        if (requestCode == 1){
            if (grantResults[0] == PackageManager.PERMISSION_GRANTED){

                if (isGPSEnabled()) {

                    getCurrentLocation();

                }else {

                    turnOnGPS();
                }
            }
        }


    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (requestCode == 2) {
            if (resultCode == Activity.RESULT_OK) {

                getCurrentLocation();
            }
        }
    }

    private void getCurrentLocation() {


        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (ActivityCompat.checkSelfPermission(getContext(), Manifest.permission.ACCESS_FINE_LOCATION) == PackageManager.PERMISSION_GRANTED) {

                if (isGPSEnabled()) {

                    LocationServices.getFusedLocationProviderClient(getContext())
                            .requestLocationUpdates(locationRequest, new LocationCallback() {
                                @Override
                                public void onLocationResult(@NonNull LocationResult locationResult) {
                                    super.onLocationResult(locationResult);

                                    LocationServices.getFusedLocationProviderClient(getContext())
                                            .removeLocationUpdates(this);

                                    if (locationResult.getLocations().size() >0){

                                        int index = locationResult.getLocations().size() - 1;
                                        double latitude = locationResult.getLocations().get(index).getLatitude();
                                        double longitude = locationResult.getLocations().get(index).getLongitude();

                                        coordinates.setText("Latitude: "+ latitude + "\n" + "Longitude: "+ longitude);
                                        shareLocation.setOnClickListener(view ->{
                                            String uri = "https://www.google.com/maps/?q=" + latitude+ "," +longitude ;
                                            Intent sharingIntent = new Intent(android.content.Intent.ACTION_SEND);
                                            sharingIntent.setType("text/plain");
                                            sharingIntent.putExtra(android.content.Intent.EXTRA_TEXT,  uri);
                                            startActivity(Intent.createChooser(sharingIntent, "Share in..."));
                                        });

                                    }
                                }
                            }, Looper.getMainLooper());

                } else {
                    turnOnGPS();
                }

            } else {
                requestPermissions(new String[]{Manifest.permission.ACCESS_FINE_LOCATION}, 1);
            }
        }
    }

    private void turnOnGPS() {

        LocationSettingsRequest.Builder builder = new LocationSettingsRequest.Builder()
                .addLocationRequest(locationRequest);
        builder.setAlwaysShow(true);

        Task<LocationSettingsResponse> result = LocationServices.getSettingsClient(getContext())
                .checkLocationSettings(builder.build());

        result.addOnCompleteListener(task -> {

            try {
                LocationSettingsResponse response = task.getResult(ApiException.class);
                Toast.makeText(getContext(), "GPS is already tured on", Toast.LENGTH_SHORT).show();

            } catch (ApiException e) {

                switch (e.getStatusCode()) {
                    case LocationSettingsStatusCodes.RESOLUTION_REQUIRED:

                        try {
                            ResolvableApiException resolvableApiException = (ResolvableApiException) e;
                            resolvableApiException.startResolutionForResult(getActivity(), 2);
                        } catch (IntentSender.SendIntentException ex) {
                            ex.printStackTrace();
                        }
                        break;

                    case LocationSettingsStatusCodes.SETTINGS_CHANGE_UNAVAILABLE:
                        //Device does not have location
                        break;
                }
            }
        });

    }

    private boolean isGPSEnabled() {
        LocationManager locationManager = null;
        boolean isEnabled;

        if (locationManager == null) {

            assert cont != null;///failing here mostly
            locationManager = (LocationManager) cont.getSystemService(Context.LOCATION_SERVICE);
        }

        isEnabled = locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER);
        return isEnabled;

    }
    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }
}