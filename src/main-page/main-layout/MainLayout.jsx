import AllUnregistered from "../all-unregisterd/AllUnregistered.jsx";
import UserDevices from "../user-devices/UserDevices.jsx";
import {useEffect, useState} from "react";
import RegistrationRequests from "../registration-requests/RegistrationRequests.jsx";
import CONFIG from "../../config.js";

export default function MainLayout({userId}) {
    const [userRegisteredDevices, setUserRegisteredDevices] = useState([])
    const [userLoading, setUserLoading] = useState(true);
    useEffect(() => {
        const fetchUserUnregistered = async () => {
            const response = await fetch(`${CONFIG.API_URL}/api/device/user-devices/${userId}`);
            const data = await response.json();
            setUserRegisteredDevices(data);
        }
        fetchUserUnregistered().then(() => setUserLoading(false));
    }, [userId])
    return (
        <>
            <AllUnregistered userId={userId} setUserRegisteredDevices={setUserRegisteredDevices} userRegisteredDevices={userRegisteredDevices}/>
            <UserDevices userLoading={userLoading} userRegisteredDevices={userRegisteredDevices}/>
            <RegistrationRequests userId={userId} setUserRegisteredDevices={setUserRegisteredDevices} userRegisteredDevices={userRegisteredDevices}/>
        </>
    )
}