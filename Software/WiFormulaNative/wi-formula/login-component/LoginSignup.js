import {useState} from "react";
import CONFIG from "../config";
import {StyleSheet, View, Text, TextInput, Button, Pressable, TouchableWithoutFeedback, Keyboard} from "react-native";
/**
 * Login signup component for handling user login and sign up
 *
 * @param {Object} props - Component props
 * @param {Function} props.setIsLoggedIn - Set isLoggedIn to true on successful login
 * @param {Function} props.setUserId - Set user on successful login
 */
export default function LoginSignup({setIsLoggedIn, setUserId}) {
    //Manage form state and validation state
    const [userName, setUserName] = useState("");
    const [isUsernameFocused, setIsUsernameFocused] = useState(false);
    const [validUsername, setValidUsername] = useState(true);
    const [password, setPassword] = useState("");
    const [isPasswordFocused, setPasswordIsFocused] = useState(false);
    const [validPassword, setValidPassword] = useState(true);
    const [formSubmitting, setFormSubmitting] = useState(false);
    const [inValidLogin, setInvalidLogin] = useState(false);

    //Stylesheet for form
    const styles = StyleSheet.create({
        mainContainer: {
            display: "flex",
            alignItems: "center",
            justifyContent: "center",
            backgroundColor: "#f3f4f6",
            minHeight: "100%",
        },
        headerText: {
            fontSize: 24,
            fontWeight: "600",
            marginBottom: 20
        },
        input: {
            width: "80%",
            borderRadius: 4,
            height: 40,
            borderColor: "#000000",
            borderWidth: 1,
            marginBottom: 10,
            padding: 10
        },
        inputFocused: {
            borderColor: "#3b82f6",
            shadowColor: "#3b82f6",
            shadowOffset: {width: 0, height: 0},
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

    /**
     * Handle login function. called on form submission
     */
    async function handleLogin() {
        //Set flag to submitting to prevent spamming
        setFormSubmitting(true);
        //Generate request object
        const loginRequest = {
            "username": userName,
            "password": password
        };
        console.log(loginRequest)
        //Make request
        const response = await fetch(`${CONFIG.API_URL}/api/user/login`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(loginRequest)
        });
        console.log(response)
        //On success, set isLoggedIn to true and set userId to user id returned from server
        setFormSubmitting(false)
        if (response.status === 401) {
            //Set invalid message on failure
            setInvalidLogin(true)
        }
        {
            const data = await response.json();
            setUserId(data['user_id'])
            setIsLoggedIn(true)
        }
    }
    /**
     * function to handle sign up. called on form submission.
     */
    async function handleSignUp() {
        //Check input validity
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
        //Set flag to prevent spamming
        setFormSubmitting(true);
        //Generate request object and make request
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
        //IF created successfully, set isLoggedIn to true and set userId to user id returned from server
        setFormSubmitting(false);
        if (response.status === 201) {
            const data = await response.json();
            setUserId(data['user_id'])
            setIsLoggedIn(true)
        }
    }

    //Object that shows error message if invalid login or username or password
    const errorView = inValidLogin || !validUsername || !validPassword ? (
        <View style={{backgroundColor: "#FF8A8A", padding: 7, borderColor : "red", borderRadius : 4, borderWidth : 1, marginTop : 10,  width: "80%"}}>
            {inValidLogin && <Text style={{color: "white"}}>Invalid username or password</Text>}
            {!validUsername && <Text style={{color: "white"}}>Username is required</Text>}
            {!validPassword && <Text style={{color: "white"}}>Password is required</Text>}
        </View>
    ) : null;
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
                    style={({pressed, focused}) => [
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
                    style={({pressed, focused}) => [
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
                {errorView}
            </View>
        </TouchableWithoutFeedback>
    )
}