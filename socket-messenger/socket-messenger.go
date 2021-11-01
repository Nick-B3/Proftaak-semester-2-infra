package main

import (
	"fmt"
	"net/http"
	"net/url"
	"strconv"
	"time"
)

type socket struct {
	Title   string
	Message string
}

func main() {
	token := "AuUHIhJ7CJVsiJZ"
	baseURL := "https://socket.fhict.be/"
	websocketURL := baseURL + "message?token=" + token
	Socket1 := socket{Title: "aan uit", Message: ""}
	var aanuit bool

	for {
		aanuit = !aanuit
		aanuitstring := strconv.FormatBool(aanuit)
		http.PostForm(websocketURL,
			url.Values{"message": {aanuitstring}, "title": {Socket1.Title}})
		fmt.Println(aanuitstring)
		time.Sleep(5 * time.Second)
	}
}
