import {useEffect, useState} from "react";
import CONFIG from "../../config.js";

export default function RegistrationRequests({userId, setUserRegisteredDevices, userRegisteredDevices}) {
    const [allUserRequests, setAllUserRequests] = useState([]);
    const [allRequestsLoading, setAllRequestsLoading] = useState(true);
    useEffect(() => {
        const fetchAllRequests = async () => {
            const response = await fetch(`${CONFIG.API_URL}/api/device/requests/${userId}`);
            const data = await response.json();
            console.log(data)
            setAllUserRequests(data);
        }
        fetchAllRequests().then(() => setAllRequestsLoading(false));
    }, [userId])

    async function approveDevice(request_id) {
        const approveRequestObject = {
            "request_id": request_id,
            "user_id": userId
        }
        const approveDeviceRequest =
            await fetch(
                `${CONFIG.API_URL}/api/device/approve`, {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json'
                    },
                    body: JSON.stringify(approveRequestObject)
                }
            );
        const approvedDevice = await approveDeviceRequest.json();
        setUserRegisteredDevices([...userRegisteredDevices, approvedDevice]);
        setAllUserRequests(allUserRequests.filter(device => device['request_id'] !== request_id));
    }


    const listItems = allRequestsLoading ?
        <div className="w-full flex items-center gap-3 p-3 bg-gray-100 rounded-md">
            <div className="w-5 h-5 border-4 border-blue-500 border-t-transparent rounded-full animate-spin"/>
            <span className="text-gray-600 text-sm">Loading...</span>
        </div> :
        allUserRequests.map((device) => (
            <div key={device['request_id']} className="p-3 mb-2 bg-gray-100 rounded-md">
                <div className={"flex justify-between items-center"}>
                    <h5>{device['deviceToApprove']['mac_address']}</h5>
                    <button
                        className={"cursor-pointer bg-blue-600 hover:bg-blue-700 text-white p-2 rounded-md"}
                        onClick={async () => {
                            await approveDevice(device['request_id'])
                        }}>Approve
                    </button>
                </div>
            </div>
        ));


    return (
        <>
            <div className="w-full p-4">
                <div className="w-full bg-white shadow-md rounded-lg p-4">
                    <h2 className="text-lg font-semibold mb-2">Approval Requests</h2>
                    {listItems}
                </div>
            </div>
        </>
    );
}