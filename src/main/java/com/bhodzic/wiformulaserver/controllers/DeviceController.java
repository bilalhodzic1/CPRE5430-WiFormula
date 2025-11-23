package com.bhodzic.wiformulaserver.controllers;

import com.bhodzic.wiformulaserver.entities.RegisteredDevice;
import com.bhodzic.wiformulaserver.entities.RegistrationRequest;
import com.bhodzic.wiformulaserver.entities.WiFormulaUser;
import com.bhodzic.wiformulaserver.repositories.RegisteredDevicesRepo;
import com.bhodzic.wiformulaserver.repositories.RegistrationRequestRepo;
import com.bhodzic.wiformulaserver.repositories.WiFormulaUserRepo;
import com.bhodzic.wiformulaserver.request_objects.ControllerApprovalRequest;
import com.bhodzic.wiformulaserver.request_objects.DeviceApprovalRequest;
import com.bhodzic.wiformulaserver.request_objects.RegistrationRequestObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

@RestController
@RequestMapping("/api/device")
@CrossOrigin(origins = "*")
public class DeviceController {
    @Autowired private RegisteredDevicesRepo deviceRepo;
    @Autowired private WiFormulaUserRepo userRepo;
    @Autowired private RegistrationRequestRepo requestRepo;

    @PostMapping("/register")
    public ResponseEntity<RegisteredDevice> registerDevice(@RequestBody RegistrationRequestObject newDevice){
        Optional<RegisteredDevice> device = deviceRepo.findById(newDevice.mac_address);
        if(device.isPresent()){
            return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
        }
        RegisteredDevice newRegisteredDevice = new RegisteredDevice();
        newRegisteredDevice.mac_address = newDevice.mac_address;
        newRegisteredDevice.device_type = newDevice.device_type;

        if(newDevice.device_type != 'C'){
            Optional<RegisteredDevice> parentDevice = deviceRepo.findById(newDevice.controller_mac_address);
            if(parentDevice.isEmpty()){
                return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
            }
            RegistrationRequest newRequest = new RegistrationRequest();
            newRequest.device = parentDevice.get();
            newRequest.deviceToApprove = newRegisteredDevice;
            newRegisteredDevice.parent = parentDevice.get();
            deviceRepo.save(newRegisteredDevice);
            requestRepo.save(newRequest);
        }else {
            deviceRepo.save(newRegisteredDevice);
        }
        return new ResponseEntity<>(newRegisteredDevice, HttpStatus.CREATED);
    }
    @GetMapping("/")
    public ResponseEntity<Iterable<RegisteredDevice>> getAllDevices(){
        return new ResponseEntity<>(deviceRepo.findAll(), HttpStatus.OK);
    }
    @GetMapping("/unapproved")
    public ResponseEntity<Iterable<RegisteredDevice>> getUnapprovedDevices(){
        Iterable<RegisteredDevice> devices = deviceRepo.findAll();
        List<RegisteredDevice> unapprovedDevices = new ArrayList<>();
        for(RegisteredDevice device : devices){
            if(device.user == null && device.device_type == 'C'){
                unapprovedDevices.add(device);
            }
        }
        return new ResponseEntity<>(unapprovedDevices, HttpStatus.OK);
    }
    @GetMapping("/user-devices/{user_id}")
    public ResponseEntity<List<RegisteredDevice>> getRegisteredDevices(@PathVariable String user_id){
        Optional<WiFormulaUser> user = userRepo.findById(user_id);
        return user.map(wiFormulaUser -> new ResponseEntity<>(wiFormulaUser.registeredDevices, HttpStatus.OK)).orElseGet(() -> new ResponseEntity<>(HttpStatus.BAD_REQUEST));
    }
    @PostMapping("/approve")
    public ResponseEntity<RegisteredDevice> approveDevice(@RequestBody DeviceApprovalRequest request){
        Optional<WiFormulaUser> user = userRepo.findById(request.user_id);
        if(user.isEmpty()){
            return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
        }
        Optional<RegistrationRequest> requestToApprove = requestRepo.findById(request.request_id);
        if(requestToApprove.isEmpty()){
            return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
        }
        RegisteredDevice device = requestToApprove.get().deviceToApprove;
        Optional<RegisteredDevice> deviceToApprove = deviceRepo.findById(device.mac_address);
        if(deviceToApprove.isEmpty()){
            return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
        }
        deviceToApprove.get().user = user.get();
        deviceRepo.save(deviceToApprove.get());
        requestRepo.deleteById(requestToApprove.get().request_id);
        return new ResponseEntity<>(deviceToApprove.get(), HttpStatus.OK);
    }
    @PostMapping("/approve-controller")
    public ResponseEntity<RegisteredDevice> approveControllerDevice(@RequestBody ControllerApprovalRequest request){
        Optional<RegisteredDevice> device = deviceRepo.findById(request.mac_address);
        if(device.isEmpty()){
            return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
        }
        Optional<WiFormulaUser> user = userRepo.findById(request.user_id);
        if(user.isEmpty()){
            return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
        }
        device.get().user = user.get();
        deviceRepo.save(device.get());
        return new ResponseEntity<>(device.get(), HttpStatus.OK);
    }
    @GetMapping("/requests/{user_id}")
    public ResponseEntity<List<RegistrationRequest>> getRegistrationRequests(@PathVariable String user_id){
        Optional<WiFormulaUser> user = userRepo.findById(user_id);
        if(user.isEmpty()){
            return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
        }
        WiFormulaUser wiUser = user.get();
        List<RegistrationRequest> requests = new ArrayList<>();
        for(RegisteredDevice device : wiUser.registeredDevices){
            Iterable<RegistrationRequest> requestedDevices = requestRepo.findRegistrationRequestByDevice(device);
            requestedDevices.forEach(requests::add);
        }
        return new ResponseEntity<>(requests, HttpStatus.OK);
    }
    @GetMapping("/{mac_address}")
    public ResponseEntity<RegisteredDevice> getDevice(@PathVariable String mac_address){
        Optional<RegisteredDevice> device = deviceRepo.findById(mac_address);
        return device.map(registeredDevice -> new ResponseEntity<>(registeredDevice, HttpStatus.OK)).orElseGet(() -> new ResponseEntity<>(HttpStatus.BAD_REQUEST));
    }
}
