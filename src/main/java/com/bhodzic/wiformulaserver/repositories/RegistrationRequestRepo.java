package com.bhodzic.wiformulaserver.repositories;

import com.bhodzic.wiformulaserver.entities.RegistrationRequest;
import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface RegistrationRequestRepo extends CrudRepository<RegistrationRequest, String> {
}
