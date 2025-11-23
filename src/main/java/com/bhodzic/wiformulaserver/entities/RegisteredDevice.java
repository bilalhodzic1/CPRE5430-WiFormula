package com.bhodzic.wiformulaserver.entities;

import com.fasterxml.jackson.annotation.JsonBackReference;
import jakarta.persistence.Entity;
import jakarta.persistence.Id;
import jakarta.persistence.ManyToOne;
import jakarta.persistence.Table;

@Entity
@Table(name = "registered_device")
public class RegisteredDevice {
    @Id
    private String mac_address;
    private String device_name;
    private char device_type;
    @ManyToOne
    @JsonBackReference
    public WiFormulaUser user;
}
