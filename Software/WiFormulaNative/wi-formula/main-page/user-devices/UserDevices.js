import {ActivityIndicator, Pressable, StyleSheet, Text, View} from "react-native";

export default function UserDevices({userRegisteredDevices, userLoading, refreshUserDevices}) {
    const styles = StyleSheet.create({
        loadingContainer: {
            display: "flex",
            justifyContent: "center",
            alignItems: "center",
        },
        deviceCard: {
            display: "flex",
            flexDirection: "column",
            backgroundColor: "#f3f4f6",
            justifyContent: "space-between",
            paddingVertical: 20,
            paddingHorizontal: 10,
            borderRadius: 10,
            borderWidth : 1,
            borderColor : "#c5c5c5",
            marginBottom : 4,
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
        },
        childCard : {
            padding : 6,
            backgroundColor : "#e5e7eb",
            borderRadius : 5,
            borderWidth : 1,
            borderColor : "#c5c5c5",
            marginTop : 5

        }
    });
    const listItems = userLoading ?
        <View style={styles.loadingContainer}>
            <ActivityIndicator size="large" color="#007AFF"/>
        </View>
        :
        userRegisteredDevices.map((device) => (
            <View key={device['request_id']} style={styles.deviceCard}>
                <Text> {device.mac_address} - Controller</Text>
                {device.children && device.children.length > 0 && (
                    <View style={{marginTop: 2, marginLeft: 10}}>
                        {device.children.map((child) => (
                            <View
                                key={child.mac_address}
                                style={styles.childCard}
                            >
                                <Text>{child.mac_address} - Sub Node</Text>
                            </View>
                        ))}
                    </View>
                )}
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
                <Text style={styles.headerText}>User Devices</Text>
                <Pressable onPress={refreshUserDevices}>
                    <Text style={{color: "#007AFF", marginLeft: "auto"}}>Refresh</Text>
                </Pressable>
            </View>
            <View style={{display: "flex", alignItems: "center"}}>
                {listItems}
            </View>
        </View>
    )
}