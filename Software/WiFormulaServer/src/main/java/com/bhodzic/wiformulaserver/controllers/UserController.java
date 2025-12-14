package com.bhodzic.wiformulaserver.controllers;

import com.bhodzic.wiformulaserver.entities.WiFormulaUser;
import com.bhodzic.wiformulaserver.repositories.WiFormulaUserRepo;
import com.bhodzic.wiformulaserver.request_objects.LoginRequest;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.Optional;
/**
 * REST controller for user related operations.
 */
@RestController
@RequestMapping("/api/user")
@CrossOrigin(origins = "*")
public class UserController {
    @Autowired
    public WiFormulaUserRepo userRepo;
    /**
     * Register User account in DB
     *
     * @param newUserRequest New user obejct with username and password
     * @return ResponseEntity with newly created user object
     */
    @PostMapping("/register")
    public ResponseEntity<WiFormulaUser> registerUser(@RequestBody LoginRequest newUserRequest) {
        //Check username not already taken
        Optional<WiFormulaUser> existingUser = userRepo.findByUsername(newUserRequest.username);
        if(existingUser.isPresent()){
            return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
        }
        //Create new user and save
        WiFormulaUser newUser = new WiFormulaUser();
        newUser.username = newUserRequest.username;
        newUser.password = newUserRequest.password;
        userRepo.save(newUser);
        //Return created user
        return new ResponseEntity<>(newUser, HttpStatus.CREATED);
    }
    /**
     * Login user and return user object
     *
     * @param user user details for login
     * @return ResponseEntity with user logged in as
     */
    @PostMapping("/login")
    public ResponseEntity<WiFormulaUser> loginUser(@RequestBody LoginRequest user) {
        //Check username and password match
        Optional<WiFormulaUser> existingUser = userRepo.findByUsername(user.username);
        if(existingUser.isPresent() && existingUser.get().password.equals(user.password)){
            //Return okay
            return new ResponseEntity<>(existingUser.get(), HttpStatus.OK);
        }
        //If not return unauthorized
        return new ResponseEntity<>(HttpStatus.UNAUTHORIZED);
    }
}
