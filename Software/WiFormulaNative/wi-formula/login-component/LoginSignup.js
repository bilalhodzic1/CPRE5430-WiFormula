import {useState} from "react";
import CONFIG from "../config";
import {StyleSheet, View, Text, TextInput, Button, Pressable, TouchableWithoutFeedback, Keyboard} from "react-native";
export default function LoginSignup({setIsLoggedIn, setUserId}){
    const [userName, setUserName] = useState("");
    const [isUsernameFocused, setIsUsernameFocused] = useState(false);
    const [validUsername, setValidUsername] = useState(true);
    const [password, setPassword] = useState("");
    const [isPasswordFocused, setPasswordIsFocused] = useState(false);
    const [validPassword, setValidPassword] = useState(true);
    const [formSubmitting, setFormSubmitting] = useState(false);
    const [inValidLogin, setInvalidLogin] = useState(false);

    const styles = StyleSheet.create({
        mainContainer: {
            display: "flex",
            alignItems : "center",
            justifyContent: "center",
            backgroundColor: "#f3f4f6",
            minHeight: "100%",
        },
        headerText : {
            fontSize: 24,
            fontWeight: "600",
            marginBottom: 20
        },
        input : {
            width: "80%",
            borderRadius: 4,
            height: 40,
            borderColor: "#000000",
            borderWidth: 1,
            marginBottom: 10,
            padding: 10
        },
        inputFocused : {
            borderColor: "#3b82f6",
            shadowColor: "#3b82f6",
            shadowOffset: { width: 0, height: 0 },
            shadowOpacity: 0.15,
            shadowRadius: 6,
            elevation: 2
        },
        button: {
            width: "80%",
            backgroundColor: "#2563eb", // blue-600
            paddingVertical: 8,
            borderRadius: 6,
            alignItems: "center",
        },
        text: {
            color: "white",
            fontSize: 14,
            fontWeight: "500",
        },
        pressed: {
            backgroundColor: "#1d4ed8", // blue-700
        },
        focused: {
            borderWidth: 2,
            borderColor: "#3b82f6", // blue-500
        },
        disabled: {
            backgroundColor: "#6b7280", // gray-500
        },
    })

    async function handleLogin() {
        setFormSubmitting(true);
        const loginRequest = {
            "username": userName,
            "password": password
        };
        console.log(loginRequest)
        const response = await fetch(`${CONFIG.API_URL}/api/user/login`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(loginRequest)
        });
        setFormSubmitting(false)
        if (response.status === 401) {
            setInvalidLogin(true)
        }
        {
            const data = await response.json();
            setUserId(data['user_id'])
            setIsLoggedIn(true)
        }
    }

    async function handleSignUp() {
        console.log("Sign up clicked")
        if (userName === "") {
            setValidUsername(false)
            return
        } else {
            setValidUsername(true)
        }
        if (password === "") {
            setValidPassword(false)
            return
        } else {
            setValidPassword(true)
        }
        setFormSubmitting(true);
        const signUpRequest = {
            "username": userName,
            "password": password
        };
        console.log(signUpRequest)
        const response = await fetch(`${CONFIG.API_URL}/api/user/register`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(signUpRequest)
        });
        setFormSubmitting(false);
        if (response.status === 201) {
            const data = await response.json();
            setUserId(data['user_id'])
            setIsLoggedIn(true)
        }
    }
    return (
        <TouchableWithoutFeedback onPress={() => {
            setPasswordIsFocused(false);
            setIsUsernameFocused(false);
            Keyboard.dismiss();
        }}>
        <View style={styles.mainContainer}>
            <Text style={styles.headerText}>Login</Text>
            <View style={{width: "80%", display: "flex", justifyContent: "flex-start", marginBottom: 6}}>
                <Text style={{color: "#6e6e6e"}}>Username</Text>
            </View>
            <TextInput
                style={[styles.input, isUsernameFocused && styles.inputFocused]}
                onChangeText={setUserName}
                value={userName}
                placeholder="Username"
                onFocus={() => setIsUsernameFocused(true)}
                onBlur={() => setIsUsernameFocused(false)}
            />
            <View style={{width: "80%", display: "flex", justifyContent: "flex-start", marginBottom: 6}}>
                <Text style={{color: "#6e6e6e"}}>Password</Text>
            </View>

            <TextInput
                style={[styles.input, {marginBottom: 20}, isPasswordFocused && styles.inputFocused]}
                onChangeText={setPassword}
                value={password}
                placeholder="*********"
                onFocus={() => setPasswordIsFocused(true)}
                onBlur={() => setPasswordIsFocused(false)}
                secureTextEntry={true}
            />
            <Pressable
                onPress={handleLogin}
                disabled={formSubmitting}
                style={({ pressed, focused }) => [
                    styles.button,
                    formSubmitting && styles.disabled,
                    pressed && !formSubmitting && styles.pressed,
                    focused && styles.focused,
                    {marginBottom: 10}
                ]}
            >
                <Text style={styles.text}>
                    Login
                </Text>
            </Pressable>
            <Pressable
                onPress={handleSignUp}
                disabled={formSubmitting}
                style={({ pressed, focused }) => [
                    styles.button,
                    formSubmitting && styles.disabled,
                    pressed && !formSubmitting && styles.pressed,
                    focused && styles.focused,
                ]}
            >
                <Text style={styles.text}>
                    Sign Up
                </Text>
            </Pressable>
        </View>
        </TouchableWithoutFeedback>
    )
}