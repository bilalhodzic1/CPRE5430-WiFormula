package com.bhodzic.wiformulaserver.controllers;

import com.bhodzic.wiformulaserver.entities.WiFormulaUser;
import com.bhodzic.wiformulaserver.repositories.WiFormulaUserRepo;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.Optional;

@RestController
@RequestMapping("/api/user")
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
}
