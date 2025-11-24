import './App.css'
import {useState} from "react";
import LoginSignup from "./login-component/LoginSignup.jsx";
import MainLayout from "./main-page/main-layout/MainLayout.jsx";

function App() {
    const [isLoggedIn, setIsLoggedIn] = useState(false)
    const [userId, setUserId] = useState("")
    if (!isLoggedIn) {
        return (
            <LoginSignup setIsLoggedIn={setIsLoggedIn} setUserId={setUserId}/>
        )
    } else {
        return (
            <MainLayout userId={userId}/>
        )
    }
}

export default App
