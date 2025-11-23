package com.bhodzic.wiformulaserver.controllers;

import com.bhodzic.wiformulaserver.entities.RegisteredDevice;
import com.bhodzic.wiformulaserver.entities.WiFormulaUser;
import com.bhodzic.wiformulaserver.repositories.RegisteredDevicesRepo;
import com.bhodzic.wiformulaserver.repositories.WiFormulaUserRepo;
import com.bhodzic.wiformulaserver.request_objects.DeviceApprovalRequest;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Optional;

@RestController
@RequestMapping("/api/device")
public class DeviceController {
    @Autowired private RegisteredDevicesRepo deviceRepo;
    @Autowired private WiFormulaUserRepo userRepo;

    @PostMapping("/register")
    public ResponseEntity<RegisteredDevice> registerDevice(RegisteredDevice newDevice){
        Optional<RegisteredDevice> device = deviceRepo.findById(newDevice.mac_address);
        if(device.isPresent()){
            return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
        }
        deviceRepo.save(newDevice);
        return new ResponseEntity<>(newDevice, HttpStatus.CREATED);
    }
    @GetMapping("/")
    public ResponseEntity<Iterable<RegisteredDevice>> getAllDevices(){
        return new ResponseEntity<>(deviceRepo.findAll(), HttpStatus.OK);
    }
    @GetMapping("/{user_id}")
    public ResponseEntity<List<RegisteredDevice>> getRegisteredDevices(@PathVariable String user_id){
        Optional<WiFormulaUser> user = userRepo.findById(user_id);
        return user.map(wiFormulaUser -> new ResponseEntity<>(wiFormulaUser.registeredDevices, HttpStatus.OK)).orElseGet(() -> new ResponseEntity<>(HttpStatus.BAD_REQUEST));
    }
    @PostMapping("/approve")
    public ResponseEntity<RegisteredDevice> approveDevice(@RequestBody DeviceApprovalRequest device){
        Optional<WiFormulaUser> user = userRepo.findById(device.user_id);
        if(user.isEmpty()){
            return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
        }
        Optional<RegisteredDevice> deviceToApprove = deviceRepo.findById(device.mac_address);
        if(deviceToApprove.isEmpty()){
            return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
        }
        deviceToApprove.get().user = user.get();
        deviceRepo.save(deviceToApprove.get());
        return new ResponseEntity<>(deviceToApprove.get(), HttpStatus.OK);
    }
}
