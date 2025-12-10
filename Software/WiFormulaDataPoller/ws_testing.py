import asyncio
import websockets
import random
import json
import time

    # Generate a random integer between 1 and 10 (inclusive)
async def connect_to_websocket(uri):
    async with websockets.connect(uri) as ws:
        print(f"Connected to {uri}")
        while True:
            random_int = random.randint(1, 100)
            test_json = {
                "random_int" : random_int
            }
            await ws.send(json.dumps(test_json))
            time.sleep(5)
        # # Receive messages
        # while True:
        #     try:
        #         message = await ws.recv()
        #         print(f"Received: {message}")
        #     except websockets.exceptions.ConnectionClosedOK:
        #         print("Connection closed by server.")
        #         break
        #     except Exception as e:
        #         print(f"Error receiving message: {e}")
        #         break

if __name__ == "__main__":
    websocket_uri = "ws://207.211.177.254:8080/formula-data-stream?device=PYTHON&mac=NOMAC"
    asyncio.run(connect_to_websocket(websocket_uri))