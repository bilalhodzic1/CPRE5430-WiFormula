package com.bhodzic.wiformulaserver.websockets;

import org.springframework.context.annotation.Configuration;
import org.springframework.web.socket.config.annotation.EnableWebSocket;
import org.springframework.web.socket.config.annotation.WebSocketConfigurer;
import org.springframework.web.socket.config.annotation.WebSocketHandlerRegistry;
@Configuration
@EnableWebSocket
public class WebSocketConfig implements WebSocketConfigurer {
    private final FormulaOneSocketHandler formulaOneSocketHandler;

    public WebSocketConfig(FormulaOneSocketHandler formulaOneSocketHandler) {
        this.formulaOneSocketHandler = formulaOneSocketHandler;
    }
    @Override
    public void registerWebSocketHandlers(WebSocketHandlerRegistry registry) {
        registry.addHandler(formulaOneSocketHandler, "/formula-data-stream")
                .setAllowedOrigins("*");
    }
}
