import {useState} from "react";


export default function LoginSignup({setIsLoggedIn}) {
    const [userName, setUserName] = useState("");
    const [validUsername, setValidUsername] = useState(true);
    const [password, setPassword] = useState("");
    const [validPassword, setValidPassword] = useState(true);
    const [formSubmitting, setFormSubmitting] = useState(false);
    const [inValidLogin, setInvalidLogin] = useState(false);

    function handleLogin() {
        setFormSubmitting(true);
        const loginRequest = {
            "username": userName,
            "password": password
        };
        const response = fetch('http://localhost:8080/api/user/login', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(loginRequest)
        });
        response.then((res) => {
            setFormSubmitting(false)
            if (res.status === 401) {
                setInvalidLogin(true)
            }
            {
                setIsLoggedIn(true)
            }
        });
    }

    function handleSignUp() {
        console.log("Sign up clicked")
        if (userName === "") {
            setValidUsername(false)
            return
        }else{
            setValidUsername(true)
        }
        if (password === "") {
            setValidPassword(false)
            return
        }else{
            setValidPassword(true)
        }
        setFormSubmitting(true);
        const signUpRequest = {
            "username": userName,
            "password": password
        };
        const response = fetch('http://localhost:8080/api/user/register', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(signUpRequest)
        });
        response.then((res) => {
            setFormSubmitting(false);
            if (res.status === 201) {
                setIsLoggedIn(true)
            }
        });
    }

    return (<div className="min-h-screen flex items-center justify-center bg-gray-100">
        <div className="w-full max-w-sm bg-white p-6 rounded-lg shadow">
            <h2 className="text-2xl font-semibold text-center mb-6">Login</h2>

            <form className="space-y-4">
                <div>
                    <label className="block text-sm font-medium mb-1">Username</label>
                    <input
                        type="text"
                        className={`w-full border border-gray-300 rounded-md px-3 py-2 text-sm focus:outline-none focus:ring-2 focus:ring-blue-500 ${validUsername ? '' : 'border-red-500'}`}
                        placeholder="Username"
                        onChange={(e) => setUserName(e.target.value)}
                    />
                </div>

                <div>
                    <label className="block text-sm font-medium mb-1">Password</label>
                    <input
                        type="password"
                        className={`w-full border border-gray-300 rounded-md px-3 py-2 text-sm focus:outline-none focus:ring-2 focus:ring-blue-500 ${validPassword ? '' : 'border-red-500'}`}
                        placeholder="••••••••"
                        onChange={(e) => setPassword(e.target.value)}
                    />
                </div>

                <button
                    type="button"
                    onClick={handleLogin}
                    className={`${formSubmitting ? "cursor-not-allowed bg-gray-500" : "cursor-pointer bg-blue-600 hover:bg-blue-700"} w-full text-white py-2 rounded-md text-sm focus:outline-none
                focus:ring-2 focus:ring-blue-500`}
                >
                    Login
                </button>
                <button
                    type="button"
                    onClick={handleSignUp}
                    className={`${formSubmitting ? "cursor-not-allowed bg-gray-500" : "cursor-pointer bg-blue-600 hover:bg-blue-700"}  w-full text-white py-2 rounded-md text-sm cus:outline-none
                focus:ring-2 focus:ring-blue-500`}
                >
                    Sign Up
                </button>
            </form>
            {inValidLogin && <p className="text-red-500 text-sm mt-2">Invalid username or password</p>}
        </div>
    </div>)
        ;
}