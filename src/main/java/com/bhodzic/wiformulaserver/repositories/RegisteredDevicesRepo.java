package com.bhodzic.wiformulaserver.repositories;

import com.bhodzic.wiformulaserver.entities.RegisteredDevice;
import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface RegisteredDevicesRepo extends CrudRepository<RegisteredDevice, String> {
}
