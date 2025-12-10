export default function UserDevices({userRegisteredDevices, userLoading, refreshUserDevices}) {
    const listItems = userLoading ?
        <div className="w-full flex items-center gap-3 p-3 bg-gray-100 rounded-md">
            <div className="w-5 h-5 border-4 border-blue-500 border-t-transparent rounded-full animate-spin"/>
            <span className="text-gray-600 text-sm">Loading...</span>
        </div> :
        userRegisteredDevices.filter((x) => x["device_type"] === "C").map((device) => (
            <div key={device.mac_address} className="p-3 mb-2 bg-gray-100 rounded-md">
                {device.mac_address} - Controller
                {device.children && device.children.length > 0 && (
                    <div className="mt-2 ml-4">
                        {device.children.map((child) => (
                            <div
                                key={child.mac_address}
                                className="p-2 mb-1 bg-gray-200 rounded-md text-sm"
                            >
                                {child.mac_address} - Sub Node
                            </div>
                        ))}
                    </div>
                )}
            </div>
        ));


    return (
        <>
            <div className="w-full p-4">
                <div className="w-full bg-white shadow-md rounded-lg p-4">
                    <div className={"flex justify-between items-center mb-2"}>
                        <h2 className="text-lg font-semibold">
                            User Devices
                        </h2>
                        <button type={"button"}
                                className={"cursor-pointer bg-blue-600 hover:bg-blue-700 text-white p-2 rounded-md"}
                                onClick={refreshUserDevices}>Refresh
                        </button>
                    </div>
                    {listItems}
                </div>
            </div>
        </>
    );
}