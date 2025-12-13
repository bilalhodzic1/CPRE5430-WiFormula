import {useCallback, useEffect, useState} from "react";
import CONFIG from "../../config.js";
import {ActivityIndicator, Pressable, StyleSheet, Text, View} from "react-native";

export default function RegistrationRequests({userId, setUserRegisteredDevices, userRegisteredDevices}) {
    const [allUserRequests, setAllUserRequests] = useState([]);
    const [allRequestsLoading, setAllRequestsLoading] = useState(true);
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
    useEffect(() => {
        refreshUserRequests()
    }, [refreshUserRequests])

    async function approveDevice(request_id) {
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
        const approvedDevice = await approveDeviceRequest.json();
        setUserRegisteredDevices([...userRegisteredDevices, approvedDevice]);
        setAllUserRequests(allUserRequests.filter(device => device['request_id'] !== request_id));
    }

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