package main

import (
	"encoding/json"
	fmt "fmt"
	"github.com/gorilla/websocket"
	"time"
)

//struct for protocol
type protocol struct {
	Duration  string
	Intensity struct {
		LightIntensity int
		Sound          string
	}
	Location string
	Type     struct {
		Light struct {
			Hex   string
			Onoff string
			RGB   struct {
				Red   int
				Green int
				Blue  int
			}
		}
		Movement string
		Sound    struct {
			Frequency string
		}
	}
}

func main() {
	//setup data
	payload := []byte(`{
	}`)

	//pointer to protocol struct
	protocol1 := &protocol{}

	//unmarshal using payload and protocol1
	err := json.Unmarshal(payload, protocol1)
	if err != nil {
		fmt.Print("problem unmarshalling json:", err)
	}

	c, _, err := websocket.DefaultDialer.Dial("wss://socket.fhict.be/ws", nil)
	if err != nil {
		fmt.Println("c dial error", err)
	}

	for {
		//create values for websocket
		for i := 0; i < 3; i++ {
			//marshal again
			if i == 0 {
				//red
				protocol1.Type.Light.RGB.Red = 255
				protocol1.Type.Light.RGB.Green = 0
				protocol1.Type.Light.RGB.Blue = 0
			} else if i == 1 {
				//green
				protocol1.Type.Light.RGB.Red = 0
				protocol1.Type.Light.RGB.Green = 255
				protocol1.Type.Light.RGB.Blue = 0

			} else {
				//blue
				protocol1.Type.Light.RGB.Red = 0
				protocol1.Type.Light.RGB.Green = 0
				protocol1.Type.Light.RGB.Blue = 255
			}
			for i := 0; i < 2; i++ {
				if i == 1 {
					protocol1.Intensity.LightIntensity = 0
				} else {
					protocol1.Intensity.LightIntensity = 255
				}
				v, err := json.Marshal(protocol1)
				if err != nil {
					fmt.Println("json marshal error", err)
				}
				err = c.WriteMessage(websocket.TextMessage, []byte(string(v)))

				if err != nil {
					fmt.Println("send error", err)
					time.Sleep(30 * time.Second)
					main()
				} else {
					fmt.Println(string(v))
				}
				time.Sleep((500 * time.Millisecond))
			}

			//for i := 0; i < 255; i++ {
			//	protocol1.Intensity.LightIntensity = i
			//	v, err := json.Marshal(protocol1)
			//	if err != nil {
			//		fmt.Println("send error", err)
			//	}
			//	err = c.WriteMessage(websocket.TextMessage, []byte(string(v)))
			//	//err = c.WriteJSON(v)
			//	if err != nil {
			//		fmt.Println("send error", err)
			//	}
			//	fmt.Println(string(v))
			//	time.Sleep(25 * time.Millisecond)
			//}
			//
			//for i := 255; i >= 0; i-- {
			//	protocol1.Intensity.LightIntensity = i
			//	v, err := json.Marshal(protocol1)
			//	if err != nil {
			//		fmt.Println("send error", err)
			//	}
			//	err = c.WriteMessage(websocket.TextMessage, []byte(string(v)))
			//	//err = c.WriteJSON(v)
			//	if err != nil {
			//		fmt.Println("send error", err)
			//		time.Sleep(30 * time.Second)
			//		main()
			//	} else {
			//		fmt.Println(string(v))
			//	}
			//	time.Sleep(25 * time.Millisecond)
			//}

		}
		// send message
		//err = c.WriteMessage(websocket.TextMessage, []byte())
		//err = c.WriteJSON(v)
		//if err != nil {
		//	fmt.Println("send error", err)
		//}
		//fmt.Println("%s", string(v))

		// receive message
		//_, message, err := c.ReadMessage()
		//if err != nil {
		//	fmt.Println("receive error")
		//} else {
		//	fmt.Println(message)
		//}
	}
}
