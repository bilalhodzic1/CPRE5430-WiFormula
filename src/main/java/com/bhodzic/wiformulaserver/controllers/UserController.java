package com.bhodzic.wiformulaserver.controllers;

import com.bhodzic.wiformulaserver.entities.WiFormulaUser;
import com.bhodzic.wiformulaserver.repositories.WiFormulaUserRepo;
import com.bhodzic.wiformulaserver.request_objects.LoginRequest;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.Optional;

@RestController
@RequestMapping("/api/user")
@CrossOrigin(origins = "*")
public class UserController {
    @Autowired
    public WiFormulaUserRepo userRepo;

    @PostMapping("/register")
    public ResponseEntity<WiFormulaUser> registerUser(@RequestBody WiFormulaUser newUser) {
        Optional<WiFormulaUser> existingUser = userRepo.findByUsername(newUser.username);
        if(existingUser.isPresent()){
            return new ResponseEntity<>(HttpStatus.BAD_REQUEST);
        }
        userRepo.save(newUser);
        return new ResponseEntity<>(newUser, HttpStatus.CREATED);
    }

    @PostMapping("/login")
    public ResponseEntity<WiFormulaUser> loginUser(@RequestBody LoginRequest user) {
        Optional<WiFormulaUser> existingUser = userRepo.findByUsername(user.username);
        if(existingUser.isPresent() && existingUser.get().password.equals(user.password)){
            return new ResponseEntity<>(existingUser.get(), HttpStatus.OK);
        }
        return new ResponseEntity<>(HttpStatus.UNAUTHORIZED);
    }
}
