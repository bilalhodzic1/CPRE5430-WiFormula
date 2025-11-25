package com.bhodzic.wiformulaserver.websockets;

import org.springframework.stereotype.Component;
import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;
import org.springframework.web.socket.handler.TextWebSocketHandler;

import java.util.Random;
@Component
public class FormulaOneSocketHandler extends TextWebSocketHandler {
    private WebSocketSession esp32Session;

    @Override
    public void afterConnectionEstablished(WebSocketSession session) {
        this.esp32Session = session;
        System.out.println("ESP32 connected!");

        // Start simple loop
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
