import './App.css'
import {useState} from "react";
import LoginSignup from "./login-component/LoginSignup.jsx";

function App() {
    const [isLoggedIn, setIsLoggedIn] = useState(false)
    if (!isLoggedIn) {
        return (
            <LoginSignup setIsLoggedIn={setIsLoggedIn}/>
        )
    } else {
        return (
            <>
                <p className={"text-amber-800"}>Test</p>
            </>
        )
    }
}

export default App
