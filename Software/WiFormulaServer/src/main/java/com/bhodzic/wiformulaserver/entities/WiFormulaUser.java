package com.bhodzic.wiformulaserver.entities;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonManagedReference;
import jakarta.persistence.*;

import java.util.List;

@Entity
@Table(name = "wi_formula_user")
public class WiFormulaUser {
    @Id
    @GeneratedValue(strategy = GenerationType.UUID)
    public String user_id;
    public String username;
    @JsonIgnore
    public String password;

    @OneToMany(mappedBy = "user")
    @JsonManagedReference
    public List<RegisteredDevice> registeredDevices;
}
