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
/**
 * REST controller for Device related operations.
 */
@RestController
@RequestMapping("/api/device")
@CrossOrigin(origins = "*")
public class DeviceController {
    @Autowired private RegisteredDevicesRepo deviceRepo;
    @Autowired private WiFormulaUserRepo userRepo;
    @Autowired private RegistrationRequestRepo requestRepo;

    /**
     * Registers a device for approval in the DB
     *
     * @param newDevice registration request object with new device details
     * @return ResponseEntity with the newly created device
     */
    @PostMapping("/register")
    public ResponseEntity<RegisteredDevice> registerDevice(@RequestBody RegistrationRequestObject newDevice){
        //Check if device already exists if so return bad request
        Optional<RegisteredDevice> device = deviceRepo.findById(newDevice.mac_address);
        if(device.isPresent()){
            return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
        }
        //Make new object from request body
        RegisteredDevice newRegisteredDevice = new RegisteredDevice();
        newRegisteredDevice.mac_address = newDevice.mac_address;
        newRegisteredDevice.device_type = newDevice.device_type;

        //If its not a controller make Request object and save it
        if(newDevice.device_type != 'C'){
            //Ensure parent device exists and is authorized for sub nodes
            Optional<RegisteredDevice> parentDevice = deviceRepo.findById(newDevice.controller_mac_address);
            if(parentDevice.isEmpty()){
                return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
            }else if(parentDevice.get().user == null){
                return new ResponseEntity<>(HttpStatus.UNAUTHORIZED);
            }
            //Make and save request object
            RegistrationRequest newRequest = new RegistrationRequest();
            newRequest.device = parentDevice.get();
            newRequest.deviceToApprove = newRegisteredDevice;
            newRegisteredDevice.parent = parentDevice.get();
            deviceRepo.save(newRegisteredDevice);
            requestRepo.save(newRequest);
        }else {
            //Save controller object as unclaimed device
            deviceRepo.save(newRegisteredDevice);
        }
        //Return created response with device
        return new ResponseEntity<>(newRegisteredDevice, HttpStatus.CREATED);
    }
    /**
     * Returns all devices in the DB
     *
     * @return ResponseEntity with all devices in the DB
     */
    @GetMapping("/")
    public ResponseEntity<Iterable<RegisteredDevice>> getAllDevices(){
        return new ResponseEntity<>(deviceRepo.findAll(), HttpStatus.OK);
    }
    /**
     * Returns all unapproved devices
     *
     * @return ResponseEntity with the all unapproved devices
     */
    @GetMapping("/unapproved")
    public ResponseEntity<Iterable<RegisteredDevice>> getUnapprovedDevices(){
        //Get all devices
        Iterable<RegisteredDevice> devices = deviceRepo.findAll();
        //Loop through devices that are controllers and no user
        List<RegisteredDevice> unapprovedDevices = new ArrayList<>();
        for(RegisteredDevice device : devices){
            if(device.user == null && device.device_type == 'C'){
                unapprovedDevices.add(device);
            }
        }
        //Return list
        return new ResponseEntity<>(unapprovedDevices, HttpStatus.OK);
    }
    /**
     * Gets users approved devices
     *
     * @param user_id user id of the user to get devices for
     * @return ResponseEntity with user approved devices
     */
    @GetMapping("/user-devices/{user_id}")
    public ResponseEntity<List<RegisteredDevice>> getRegisteredDevices(@PathVariable String user_id){
        //Ensure user exists, get devices from user
        Optional<WiFormulaUser> user = userRepo.findById(user_id);
        return user.map(wiFormulaUser -> new ResponseEntity<>(wiFormulaUser.registeredDevices, HttpStatus.OK)).orElseGet(() -> new ResponseEntity<>(HttpStatus.BAD_REQUEST));
    }
    /**
     * Approves a device from a request
     *
     * @param request approval request object with user id and request id
     * @return ResponseEntity with the newly created device
     */
    @PostMapping("/approve")
    public ResponseEntity<RegisteredDevice> approveDevice(@RequestBody DeviceApprovalRequest request){
        //Check for user existence and request existence
        Optional<WiFormulaUser> user = userRepo.findById(request.user_id);
        if(user.isEmpty()){
            return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
        }
        Optional<RegistrationRequest> requestToApprove = requestRepo.findById(request.request_id);
        if(requestToApprove.isEmpty()){
            return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
        }
        //Check if device already approved
        RegisteredDevice device = requestToApprove.get().deviceToApprove;
        Optional<RegisteredDevice> deviceToApprove = deviceRepo.findById(device.mac_address);
        if(deviceToApprove.isEmpty()){
            return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
        }
        //Save approval and return
        deviceToApprove.get().user = user.get();
        deviceRepo.save(deviceToApprove.get());
        requestRepo.deleteById(requestToApprove.get().request_id);
        return new ResponseEntity<>(deviceToApprove.get(), HttpStatus.OK);
    }
    /**
     * Approves a controller device
     *
     * @param request controller approval request object with user id and mac address
     * @return ResponseEntity with controler device object
     */
    @PostMapping("/approve-controller")
    public ResponseEntity<RegisteredDevice> approveControllerDevice(@RequestBody ControllerApprovalRequest request){
        //Check device and user exist
        Optional<RegisteredDevice> device = deviceRepo.findById(request.mac_address);
        if(device.isEmpty()){
            return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
        }
        Optional<WiFormulaUser> user = userRepo.findById(request.user_id);
        if(user.isEmpty()){
            return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
        }
        //Assign user to device and save
        device.get().user = user.get();
        deviceRepo.save(device.get());
        //Return device
        return new ResponseEntity<>(device.get(), HttpStatus.OK);
    }
    /**
     * Gets registration requests for a user
     *
     * @param user_id user id of user to get requests for
     * @return ResponseEntity with registration requests
     */
    @GetMapping("/requests/{user_id}")
    public ResponseEntity<List<RegistrationRequest>> getRegistrationRequests(@PathVariable String user_id){
        //Check users exists
        Optional<WiFormulaUser> user = userRepo.findById(user_id);
        if(user.isEmpty()){
            return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
        }
        //Get all requests for their registered devices
        WiFormulaUser wiUser = user.get();
        List<RegistrationRequest> requests = new ArrayList<>();
        for(RegisteredDevice device : wiUser.registeredDevices){
            Iterable<RegistrationRequest> requestedDevices = requestRepo.findRegistrationRequestByDevice(device);
            requestedDevices.forEach(requests::add);
        }
        //Return the requests
        return new ResponseEntity<>(requests, HttpStatus.OK);
    }
    /**
     * Check authentication status of a given device
     *
     * @param mac_address mac address of device to check
     * @return ResponseEntity with device auth object
     */
    @GetMapping("/auth/{mac_address}")
    public ResponseEntity<RegisteredDevice> getDevice(@PathVariable String mac_address){
        //Check device exists. If not return not found status
        Optional<RegisteredDevice> device = deviceRepo.findById(mac_address);
        if(device.isEmpty()){
            return new ResponseEntity<>(HttpStatus.NOT_FOUND);
        }else {
            //If exists check if user assigned, if not then return unauthorized
            RegisteredDevice registeredDevice = device.get();
            if(registeredDevice.user == null){
                return new ResponseEntity<>(HttpStatus.UNAUTHORIZED);
            }else{
                return new ResponseEntity<>(registeredDevice, HttpStatus.OK);
            }
        }
    }
}
