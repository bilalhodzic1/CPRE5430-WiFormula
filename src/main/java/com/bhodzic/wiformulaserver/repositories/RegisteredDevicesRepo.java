package com.bhodzic.wiformulaserver.repositories;

import com.bhodzic.wiformulaserver.entities.RegisteredDevices;
import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface RegisteredDevicesRepo extends CrudRepository<RegisteredDevices, String> {
}
