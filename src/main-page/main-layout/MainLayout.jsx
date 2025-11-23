import {useEffect, useState} from "react";

export default function MainLayout({userId}) {
    const [allUnregisteredDevices, setAllUnregisteredDevices] = useState([]);
    const [unregisteredLoading, setUnregisteredLoading] = useState(true);
    const [userRegisteredDevices, setUserRegisteredDevices] = useState([])
    const [userLoading, setUserLoading] = useState(true);
    useEffect(() => {
        const fetchAllUnregistered = async () => {
            const response = await fetch('http://localhost:8080/api/device/');
            const data = await response.json();
            setAllUnregisteredDevices(data);
        }
        fetchAllUnregistered().then(() => setUnregisteredLoading(false));
        const fetchUserUnregistered = async () => {
            const response = await fetch(`http://localhost:8080/api/device/${userId}`);
            const data = await response.json();
            setUserRegisteredDevices(data);
        }
        fetchUserUnregistered().then(() => setUserLoading(false));
    }, []);
    return (
        <>
            <div className="w-full p-4">
                <div className="w-full bg-white shadow-md rounded-lg p-4">
                    <h2 className="text-lg font-semibold mb-2">All Unregistered Devices</h2>

                    <div className="p-3 bg-gray-100 rounded-md">
                        Example Item Content
                    </div>
                </div>
            </div>
            <div className="w-full p-4">
                <div className="w-full bg-white shadow-md rounded-lg p-4">
                    <h2 className="text-lg font-semibold mb-2">Users Devices</h2>

                    <div className="p-3 bg-gray-100 rounded-md">
                        Example Item Content
                    </div>
                </div>
            </div>
            <div className="w-full p-4">
                <div className="w-full bg-white shadow-md rounded-lg p-4">
                    <h2 className="text-lg font-semibold mb-2">Registration Requests</h2>

                    <div className="p-3 bg-gray-100 rounded-md">
                        Example Item Content
                    </div>
                </div>
            </div>
        </>
    )
}