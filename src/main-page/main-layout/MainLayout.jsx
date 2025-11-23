import AllUnregistered from "../all-unregisterd/AllUnregistered.jsx";
import UserDevices from "../user-devices/UserDevices.jsx";
import {useEffect, useState} from "react";

export default function MainLayout({userId}) {
    const [userRegisteredDevices, setUserRegisteredDevices] = useState([])
    const [userLoading, setUserLoading] = useState(true);
    useEffect(() => {
        const fetchUserUnregistered = async () => {
            const response = await fetch(`http://localhost:8080/api/device/${userId}`);
            const data = await response.json();
            setUserRegisteredDevices(data);
        }
        fetchUserUnregistered().then(() => setUserLoading(false));
    }, [userId])
    return (
        <>
            <AllUnregistered userId={userId} setUserRegisteredDevices={setUserRegisteredDevices} userRegisteredDevices={userRegisteredDevices}/>
            <UserDevices userLoading={userLoading} userRegisteredDevices={userRegisteredDevices}/>
        </>
    )
}