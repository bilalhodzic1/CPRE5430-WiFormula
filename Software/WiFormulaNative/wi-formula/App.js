import {StyleSheet, Text, View} from 'react-native';
import LoginSignup from "./login-component/LoginSignup";
import {useState} from "react";
import MainLayout from "./main-page/main-layout/MainLayout";
import {SafeAreaProvider} from "react-native-safe-area-context";

export default function App() {
    const [isLoggedIn, setIsLoggedIn] = useState(false)
    const [userId, setUserId] = useState("")
    if (!isLoggedIn) {
        return (
            <LoginSignup setIsLoggedIn={setIsLoggedIn} setUserId={setUserId}/>
        )
    } else {
        return (
            <SafeAreaProvider>
                <MainLayout userId={userId}/>
            </SafeAreaProvider>
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
