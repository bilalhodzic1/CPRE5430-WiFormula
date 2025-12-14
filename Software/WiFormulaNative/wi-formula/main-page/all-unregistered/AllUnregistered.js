import {useEffect, useState} from "react";
import CONFIG from "../../config";
import {ActivityIndicator, Pressable, StyleSheet, View, Text, ScrollView} from "react-native";
import {SafeAreaView} from "react-native-safe-area-context";
/**
 * Login signup component for handling user login and sign up
 *
 * @param {Object} props - Component props
 * @param {Function} props.userId - logged in user id
 * @param {Function} props.setUserRegisteredDevices - set user registered devices list
 * @param {Array} props.userRegisteredDevices - user registered devices list
 */
export default function AllUnregistered({userId, setUserRegisteredDevices, userRegisteredDevices}) {
    //State for all unregistered devices
    const [allUnregisteredDevices, setAllUnregisteredDevices] = useState([]);
    const [unregisteredLoading, setUnregisteredLoading] = useState(true);

    //Stylesheet
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
            margin : 5,
            marginBottom : 10
        }
    });

    /**
     * Fetch all unregistered devices from server and set state.
     */
    function refreshUnregisteredDevices() {
        //Make fetch request and set state on response
        const fetchAllUnregistered = async () => {
            setUnregisteredLoading(true);
            const response = await fetch(`${CONFIG.API_URL}/api/device/unapproved`);
            const data = await response.json();
            setAllUnregisteredDevices(data);
        }
        fetchAllUnregistered().then(() => setUnregisteredLoading(false));
    }

    //Run refreshUnregisteredDevices on component mount
    useEffect(() => {
        refreshUnregisteredDevices()
    }, [])

    /**
     * Fucntion approves a device and adds it to user registered devices list.
     *
     * @param {string} macAddress - mac address of device to approve
     */
    async function approveDevice(macAddress) {
        //Set request object and make fetch request to approve device
        const approveControllerObject = {
            "mac_address": macAddress,
            "user_id": userId
        }
        const approveDeviceRequest =
            await fetch(
                `${CONFIG.API_URL}/api/device/approve-controller`, {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json'
                    },
                    body: JSON.stringify(approveControllerObject)
                }
            );
        //Set user deives to include new device and remove from unregistered devices
        const approvedDevice = await approveDeviceRequest.json();
        setUserRegisteredDevices([...userRegisteredDevices, approvedDevice]);
        setAllUnregisteredDevices(allUnregisteredDevices.filter(device => device['mac_address'] !== approvedDevice['mac_address']));
    }

    //List of all unregistered devices and spinner if loading
    const listItems = unregisteredLoading ?
        <View style={styles.loadingContainer}>
            <ActivityIndicator size="large" color="#007AFF"/>
        </View>
        :
        allUnregisteredDevices.map((device) => (
            <View key={device['mac_address']} style={styles.deviceCard}>
                <Text>{device['mac_address']}</Text>
                <Pressable style={styles.approveButton} onPress={async () => {
                    await approveDevice(device['mac_address'])
                }}>
                    <Text style={{color: "white"}}>Approve</Text>
                </Pressable>
            </View>
        ));
    return (
        <View style={styles.layoutCard}>
            <View style={{width: "95%", marginBottom: 20, display: "flex", justifyContent : "space-between", alignItems : "center", flexDirection: "row"}}>
                <Text style={styles.headerText}>All Unregistered Devices</Text>
                <Pressable onPress={refreshUnregisteredDevices}>
                    <Text style={{color: "#007AFF", marginLeft: "auto"}}>Refresh</Text>
                </Pressable>
            </View>
            <View style={{display: "flex", alignItems: "center"}}>
                {listItems}
            </View>
        </View>
    )
}