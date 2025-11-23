export default function UserDevices({userRegisteredDevices, userLoading}) {
    const listItems = userLoading ?
        <div className="w-full flex items-center gap-3 p-3 bg-gray-100 rounded-md">
            <div className="w-5 h-5 border-4 border-blue-500 border-t-transparent rounded-full animate-spin"/>
            <span className="text-gray-600 text-sm">Loading...</span>
        </div> :
        userRegisteredDevices.map((device) => (
            <div className="p-3 mb-2 bg-gray-100 rounded-md">
                {device['mac_address']}
            </div>
        ));


    return (
        <>
            <div className="w-full p-4">
                <div className="w-full bg-white shadow-md rounded-lg p-4">
                    <h2 className="text-lg font-semibold mb-2">User Devices</h2>
                    {listItems}
                </div>
            </div>
        </>
    );
}