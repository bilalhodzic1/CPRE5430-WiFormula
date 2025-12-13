import {useCallback, useEffect, useState} from "react";
import CONFIG from "../../config.js";
import RegistrationRequests from "../registration-requests/RegistrationRequests";
import UserDevices from "../user-devices/UserDevices";
import AllUnregistered from "../all-unregistered/AllUnregistered";
import {SafeAreaView} from "react-native-safe-area-context";
import {ScrollView} from "react-native";

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
        <SafeAreaView>
            <ScrollView>
                <AllUnregistered userId={userId} setUserRegisteredDevices={setUserRegisteredDevices}
                                 userRegisteredDevices={userRegisteredDevices}/>
                <UserDevices userLoading={userLoading} userRegisteredDevices={userRegisteredDevices}
                             refreshUserDevices={refreshUserDevices}/>
                <RegistrationRequests userId={userId} setUserRegisteredDevices={setUserRegisteredDevices}
                                      userRegisteredDevices={userRegisteredDevices}/>
            </ScrollView>
        </SafeAreaView>
    )
}