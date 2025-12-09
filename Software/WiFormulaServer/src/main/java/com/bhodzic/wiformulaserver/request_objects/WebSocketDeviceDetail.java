package com.bhodzic.wiformulaserver.request_objects;

import org.springframework.web.socket.WebSocketSession;

public class WebSocketDeviceDetail {
    public String deviceType;
    public String macAddress;
    public WebSocketSession session;
}
