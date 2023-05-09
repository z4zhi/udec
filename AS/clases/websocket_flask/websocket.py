import asyncio
import websockets
 
wss = [] 

async def handler(websocket, path):
    
    try:     
        print("Client connected")

        if websocket not in wss:
            wss.append(websocket)
        
        print(f"Clientes: {len(wss)}")
        data = await websocket.recv()
        reply = f"Data recieved as: {data}!"
        print(reply)
        
        for ws in wss:
            print("Enviado dentro for")
            await  ws.send(reply)
    except:
        print  ("error!!")
   
 
start_server = websockets.serve(handler, "localhost", 8091)
asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()

