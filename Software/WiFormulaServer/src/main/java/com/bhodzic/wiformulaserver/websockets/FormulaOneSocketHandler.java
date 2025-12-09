package com.bhodzic.wiformulaserver.websockets;

import com.bhodzic.wiformulaserver.request_objects.WebSocketDeviceDetail;
import org.springframework.stereotype.Component;
import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;
import org.springframework.web.socket.handler.TextWebSocketHandler;

import java.util.HashMap;
import java.util.Map;
import java.util.Random;
@Component
public class FormulaOneSocketHandler extends TextWebSocketHandler {
    private Map<WebSocketDeviceDetail ,WebSocketSession> esp32Session = new HashMap<>();

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
            esp32Session.put(deviceDetail, session);
            actualDevice = true;
        }
        if (actualDevice) {
            new Thread(() -> {
                Random r = new Random();
                while (session.isOpen()) {
                    try {
                        int num = r.nextInt(100) + 1; // 1–100
                        session.sendMessage(new TextMessage(String.valueOf(num)));
                        System.out.println("Sent: " + num);
                        Thread.sleep(10000); // 1 second delay
                    } catch (Exception e) {
                        break;
                    }
                }
            }).start();
        }
    }
}
