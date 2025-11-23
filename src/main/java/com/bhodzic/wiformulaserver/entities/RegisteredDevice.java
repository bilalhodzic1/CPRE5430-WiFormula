package com.bhodzic.wiformulaserver.entities;

import com.fasterxml.jackson.annotation.JsonBackReference;
import com.fasterxml.jackson.annotation.JsonManagedReference;
import jakarta.persistence.*;

import java.util.List;

@Entity
@Table(name = "registered_device")
public class RegisteredDevice {
    @Id
    public String mac_address;
    public String device_name;
    public char device_type;
    @ManyToOne
    @JsonBackReference
    public WiFormulaUser user;

    @ManyToOne
    @JoinColumn(name = "parent_mac_address") // FK column
    @JsonBackReference(value = "device-parent")
    public RegisteredDevice parent;

    @OneToMany(mappedBy = "parent")
    @JsonManagedReference(value = "device-parent")
    public List<RegisteredDevice> children;

}
