import { StatusBar } from 'expo-status-bar';
import { StyleSheet, Text, View } from 'react-native';
import LoginSignup from "./login-component/LoginSignup";
import {useEffect, useState} from "react";

export default function App() {
    const [isLoggedIn, setIsLoggedIn] = useState(false)
    const [userId, setUserId] = useState("")
    if (!isLoggedIn) {
        return (
            <LoginSignup setIsLoggedIn={setIsLoggedIn} setUserId={setUserId}/>
        )
    } else {
        return (
            <View style={styles.container}>
                <Text>Open up App.js to start working on your app!</Text>
                <StatusBar style="auto" />
            </View>
        )
    }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#fff',
    alignItems: 'center',
    justifyContent: 'center',
  },
});
