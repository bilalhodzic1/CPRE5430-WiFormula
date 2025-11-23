package com.bhodzic.wiformulaserver.entities;

import com.fasterxml.jackson.annotation.JsonBackReference;
import jakarta.persistence.Entity;
import jakarta.persistence.Id;
import jakarta.persistence.ManyToOne;

@Entity
public class RegisteredDevices {
    @Id
    public String mac_address;
    public String device_name;
    public char device_type;
    @ManyToOne
    @JsonBackReference
    public WiFormulaUser user;
}
