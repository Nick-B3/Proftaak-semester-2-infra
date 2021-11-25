package main

import (
	"fmt"
	"github.com/gorilla/websocket"
	"time"
)

func main() {
	//protocol, err := ioutil.ReadFile("./protocol.json")
	//if err != nil {fmt.Println("problem opening json", err)}
	//v, err := json.Marshal(protocol)
	//if err != nil {fmt.Print("problem marshalling json", err)}

	c, _, err := websocket.DefaultDialer.Dial("wss://socket.fhict.be/ws", nil)
	if err != nil {
		fmt.Println("c dial error", err)
	}
	for {
		// send message
		err = c.WriteMessage(websocket.TextMessage, []byte("{test}"))
		//err = c.WriteJSON(v)
		if err != nil {
			fmt.Println("send error", err)
		}
		// receive message
		_, message, err := c.ReadMessage()
		if err != nil {
			fmt.Println("receive error")
		} else {
			fmt.Println(message)
		}
		time.Sleep(5 * time.Second)
	}
}
