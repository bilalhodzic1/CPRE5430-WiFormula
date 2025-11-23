package com.bhodzic.wiformulaserver.repositories;

import com.bhodzic.wiformulaserver.entities.WiFormulaUser;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

import java.util.Optional;

@Repository
public interface WiFormulaUserRepo extends CrudRepository<WiFormulaUser, String> {
    Optional<WiFormulaUser> findByUsername(String username);
}
