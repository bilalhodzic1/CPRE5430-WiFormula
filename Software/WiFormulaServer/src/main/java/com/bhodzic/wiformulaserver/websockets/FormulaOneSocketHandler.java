package com.bhodzic.wiformulaserver.websockets;

import com.bhodzic.wiformulaserver.request_objects.WebSocketDeviceDetail;
import org.springframework.stereotype.Component;
import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;
import org.springframework.web.socket.handler.TextWebSocketHandler;
import org.springframework.web.socket.CloseStatus;

import java.util.HashMap;
import java.util.Map;

@Component
public class FormulaOneSocketHandler extends TextWebSocketHandler {
    private Map<String ,WebSocketDeviceDetail> sessionsMap = new HashMap<>();

    @Override
    public void afterConnectionEstablished(WebSocketSession session) {
        String uri = session.getUri().getQuery();
        boolean actualDevice = false;
        if (uri != null && uri.contains("device=")) {
            String[] deviceParams = uri.split("&");
            WebSocketDeviceDetail deviceDetail = new WebSocketDeviceDetail();
            for (String param : deviceParams) {
                String[] keyValue = param.split("=");
                if(keyValue[0].equals("device")){
                    deviceDetail.deviceType = keyValue[1];
                }
                if(keyValue[0].equals("mac")){
                    deviceDetail.macAddress = keyValue[1];
                }
            }
            deviceDetail.session = session;
            sessionsMap.put(session.getId(), deviceDetail);
        }
    }
    @Override
    protected void handleTextMessage(WebSocketSession session, TextMessage message) throws Exception {
        for (Map.Entry<String, WebSocketDeviceDetail> entry : sessionsMap.entrySet()) {
            if (entry.getValue().deviceType.equals("ESP32")) {
                entry.getValue().session.sendMessage(message);
            }
        }
    }
    @Override
    public void afterConnectionClosed(WebSocketSession session, CloseStatus status) throws Exception {
        sessionsMap.remove(session.getId());
    }
}
