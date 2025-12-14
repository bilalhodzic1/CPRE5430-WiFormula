import {useCallback, useEffect, useState} from "react";
import CONFIG from "../../config.js";
import RegistrationRequests from "../registration-requests/RegistrationRequests";
import UserDevices from "../user-devices/UserDevices";
import AllUnregistered from "../all-unregistered/AllUnregistered";
import {SafeAreaView} from "react-native-safe-area-context";
import {ScrollView} from "react-native";
/**
 * Main layout component holds main page components and needed state
 *
 * @param {Object} props - Component props
 * @param {Function} props.userId - Set user on successful login
 */
export default function MainLayout({userId}) {
    //Set needed shared state
    const [userRegisteredDevices, setUserRegisteredDevices] = useState([])
    const [userLoading, setUserLoading] = useState(true);
    //Callback to refresh user devices on component mount and on user registration/approval change
    const refreshUserDevices = useCallback(() => {
        const fetchUserRegistered = async () => {
            setUserLoading(true);
            const response = await fetch(`${CONFIG.API_URL}/api/device/user-devices/${userId}`);
            const data = await response.json();
            setUserRegisteredDevices(data);
        }
        fetchUserRegistered().then(() => setUserLoading(false));
    }, [userId]);
    //Run refreshUserDevices on callback change
    useEffect(() => {
        refreshUserDevices()
    }, [refreshUserDevices])

    return (
        <SafeAreaView>
            <ScrollView style={{height: "100%"}} >
                <AllUnregistered userId={userId} setUserRegisteredDevices={setUserRegisteredDevices}
                                 userRegisteredDevices={userRegisteredDevices}/>
                <UserDevices userLoading={userLoading} userRegisteredDevices={userRegisteredDevices}
                             refreshUserDevices={refreshUserDevices}/>
                <RegistrationRequests userId={userId} refreshUserDevices={refreshUserDevices}/>
            </ScrollView>
        </SafeAreaView>
    )
}