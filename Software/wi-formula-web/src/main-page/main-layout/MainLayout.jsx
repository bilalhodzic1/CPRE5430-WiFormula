import AllUnregistered from "../all-unregisterd/AllUnregistered.jsx";
import UserDevices from "../user-devices/UserDevices.jsx";
import {useCallback, useEffect, useState} from "react";
import RegistrationRequests from "../registration-requests/RegistrationRequests.jsx";
import CONFIG from "../../config.js";

export default function MainLayout({userId}) {
    const [userRegisteredDevices, setUserRegisteredDevices] = useState([])
    const [userLoading, setUserLoading] = useState(true);
    const refreshUserDevices = useCallback(() => {
        const fetchUserRegistered = async () => {
            setUserLoading(true);
            const response = await fetch(`${CONFIG.API_URL}/api/device/user-devices/${userId}`);
            const data = await response.json();
            setUserRegisteredDevices(data);
        }
        fetchUserRegistered().then(() => setUserLoading(false));
    }, [userId]);
    useEffect(() => {
        refreshUserDevices()
    }, [refreshUserDevices])
    return (
        <>
            <AllUnregistered userId={userId} setUserRegisteredDevices={setUserRegisteredDevices} userRegisteredDevices={userRegisteredDevices}/>
            <UserDevices userLoading={userLoading} userRegisteredDevices={userRegisteredDevices} refreshUserDevices={refreshUserDevices}/>
            <RegistrationRequests userId={userId} setUserRegisteredDevices={setUserRegisteredDevices} userRegisteredDevices={userRegisteredDevices}/>
        </>
    )
}