package com.example.stepsafe_womensafety.ui;

public class Users {
    String email,name,blood_group,dob,password,imageUrl;
    int phone_number,age,height,weight,emergency_contact;

    public Users() {
    }

    public Users(String email,String password, String name, String blood_group, String dob, int phone_number, int age, int height, int weight, int emergency_contact) {
        this.email = email;
        this.password=password;
        this.name = name;
        this.blood_group = blood_group;
        this.dob = dob;
        this.phone_number = phone_number;
        this.age = age;
        this.height = height;
        this.weight = weight;
        this.emergency_contact = emergency_contact;
    }

    public String getEmail() {
        return email;
    }
    public String getPassword() {
        return password;
    }

    public String getName() {
        return name;
    }

    public String getBlood_group() {
        return blood_group;
    }

    public String getDob() {
        return dob;
    }
    public String getImageUrl() {
        return imageUrl;
    }

    public int getPhone_number() {
        return phone_number;
    }

    public int getAge() {
        return age;
    }

    public int getHeight() {
        return height;
    }

    public int getWeight() {
        return weight;
    }

    public int getEmergency_contact() {
        return emergency_contact;
    }

}
