import {useEffect, useState} from "react";
import CONFIG from "../../config.js";

export default function AllUnregistered({userId, setUserRegisteredDevices, userRegisteredDevices}) {
    const [allUnregisteredDevices, setAllUnregisteredDevices] = useState([]);
    const [unregisteredLoading, setUnregisteredLoading] = useState(true);
    useEffect(() => {
        const fetchAllUnregistered = async () => {
            const response = await fetch(`${CONFIG.API_URL}/api/device/unapproved`);
            const data = await response.json();
            setAllUnregisteredDevices(data);
        }
        fetchAllUnregistered().then(() => setUnregisteredLoading(false));
    }, [])

    async function approveDevice(macAddress) {
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
        const approvedDevice = await approveDeviceRequest.json();
        setUserRegisteredDevices([...userRegisteredDevices, approvedDevice]);
        setAllUnregisteredDevices(allUnregisteredDevices.filter(device => device['mac_address'] !== approvedDevice['mac_address']));
    }


    const listItems = unregisteredLoading ?
        <div className="w-full flex items-center gap-3 p-3 bg-gray-100 rounded-md">
            <div className="w-5 h-5 border-4 border-blue-500 border-t-transparent rounded-full animate-spin"/>
            <span className="text-gray-600 text-sm">Loading...</span>
        </div> :
        allUnregisteredDevices.map((device) => (
            <div key={device['mac_address']} className="p-3 mb-2 bg-gray-100 rounded-md">
                <div className={"flex justify-between items-center"}>
                    <h5>{device['mac_address']}</h5>
                    <button
                        className={"cursor-pointer bg-blue-600 hover:bg-blue-700 text-white p-2 rounded-md"}
                        onClick={async () => {
                            await approveDevice(device['mac_address'])
                        }}>Approve
                    </button>
                </div>
            </div>
        ));


    return (
        <>
            <div className="w-full p-4">
                <div className="w-full bg-white shadow-md rounded-lg p-4">
                    <h2 className="text-lg font-semibold mb-2">All Unregistered Devices</h2>
                    {listItems}
                </div>
            </div>
        </>
    );
}