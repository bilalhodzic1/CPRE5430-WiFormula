package com.bhodzic.wiformulaserver.repositories;

import com.bhodzic.wiformulaserver.entities.RegisteredDevice;
import com.bhodzic.wiformulaserver.entities.RegistrationRequest;
import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

import java.util.Optional;

@Repository
public interface RegistrationRequestRepo extends CrudRepository<RegistrationRequest, String> {
    Iterable<RegistrationRequest> findRegistrationRequestByDevice(RegisteredDevice device);
}
