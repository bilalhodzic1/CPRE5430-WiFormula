package com.bhodzic.wiformulaserver.entities;

import jakarta.persistence.*;

@Entity
public class RegistrationRequest {
    @Id
    @GeneratedValue(strategy = GenerationType.UUID)
    public String request_id;
    @ManyToOne
    public RegisteredDevice device;
    @ManyToOne
    public RegisteredDevice deviceToApprove;
}
