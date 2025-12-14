import {useCallback, useEffect, useState} from "react";
import CONFIG from "../../config.js";
import {ActivityIndicator, Pressable, StyleSheet, Text, View} from "react-native";
/**
 * Login signup component for handling user login and sign up
 *
 * @param {Object} props - Component props
 * @param {Function} props.userId - Logged in user ID
 * @param {Function} props.refreshUserDevices - function to refresh user devices
 */
export default function RegistrationRequests({userId, refreshUserDevices}) {
    // initialize state for all user registration requests
    const [allUserRequests, setAllUserRequests] = useState([]);
    const [allRequestsLoading, setAllRequestsLoading] = useState(true);
    //Stylesheet for registration requests component
    const styles = StyleSheet.create({
        loadingContainer: {
            display: "flex",
            justifyContent: "center",
            alignItems: "center",
        },
        deviceCard: {
            display: "flex",
            flexDirection: "row",
            backgroundColor: "#f3f4f6",
            justifyContent: "space-between",
            alignItems: "center",
            paddingVertical: 20,
            paddingHorizontal: 10,
            borderRadius: 10,
            width: "95%"
        },
        approveButton: {
            backgroundColor: "#0047AB",
            padding: 15,
            borderRadius: 10,
            color: "white",
        },
        headerText: {
            fontSize: 18,
            fontWeight: "600",
        },
        layoutCard: {
            display: "flex",
            flexDirection: "column",
            borderRadius: 10,
            borderWidth: 1,
            borderColor: "#000000",
            padding: 10,
            margin: 5,
            marginBottom: 10
        }
    });
    //Callback to refresh user registration requests on user id change
    const refreshUserRequests = useCallback(() => {
        const fetchAllRequests = async () => {
            setAllRequestsLoading(true);
            const response = await fetch(`${CONFIG.API_URL}/api/device/requests/${userId}`);
            const data = await response.json();
            console.log(data)
            setAllUserRequests(data);
        }
        fetchAllRequests().then(() => setAllRequestsLoading(false));
    }, [userId])
    //Run refreshUserRequests on callback change
    useEffect(() => {
        refreshUserRequests()
    }, [refreshUserRequests])

    /**
     * Login signup component for handling user login and sign up
     *
     * @param {string} request_id - guid of request to approve
     */
    async function approveDevice(request_id) {
        //Create request object and make fetch request to approve device
        const approveRequestObject = {
            "request_id": request_id,
            "user_id": userId
        }
        const approveDeviceRequest =
            await fetch(
                `${CONFIG.API_URL}/api/device/approve`, {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json'
                    },
                    body: JSON.stringify(approveRequestObject)
                }
            );
        //Refresh user devices and remove request from list
        await approveDeviceRequest.json();
        refreshUserDevices();
        setAllUserRequests(allUserRequests.filter(device => device['request_id'] !== request_id));
    }

    //List of all user registration requests and spinner if loading
    const listItems = allRequestsLoading ?
        <View style={styles.loadingContainer}>
            <ActivityIndicator size="large" color="#007AFF"/>
        </View>
        :
        allUserRequests.map((device) => (
            <View key={device['request_id']} style={styles.deviceCard}>
                <Text>{device['deviceToApprove']['mac_address']}</Text>
                <Pressable style={styles.approveButton} onPress={async () => {
                    await approveDevice(device['request_id'])
                }}>
                    <Text style={{color: "white"}}>Approve</Text>
                </Pressable>
            </View>
        ));
    return (
        <View style={styles.layoutCard}>
            <View style={{
                width: "95%",
                marginBottom: 20,
                display: "flex",
                justifyContent: "space-between",
                alignItems: "center",
                flexDirection: "row"
            }}>
                <Text style={styles.headerText}>Approval Requests</Text>
                <Pressable onPress={refreshUserRequests}>
                    <Text style={{color: "#007AFF", marginLeft: "auto"}}>Refresh</Text>
                </Pressable>
            </View>
            <View style={{display: "flex", alignItems: "center"}}>
                {listItems}
            </View>
        </View>
    )
}