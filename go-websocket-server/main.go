package main

import (
	static "github.com/gin-contrib/static"
	"github.com/gin-gonic/gin"
	"gopkg.in/olahol/melody.v1"
	"net/http"
)

func main() {
	r := gin.Default()
	m := melody.New()

	r.Use(static.Serve("/", static.LocalFile("./public", true)))

	r.GET("/ws", func(c *gin.Context) {
		err := m.HandleRequest(c.Writer, c.Request)
		if err != nil {
			return
		}
	})

	authorized := r.Group("/", gin.BasicAuth(gin.Accounts{
		"user1": "love",
		"user2": "god",
		"user3": "sex",
	}))

	authorized.GET("/secret", func(c *gin.Context) {
		c.JSON(http.StatusOK, gin.H{
			"secret": "The secret ingredient to the BBQ sauce is stiring it in an old whiskey barrel.",
		})
	})

	m.HandleMessage(func(s *melody.Session, msg []byte) {
		err := m.Broadcast(msg)
		if err != nil {
			return
		}
	})

	err := r.Run(":5000")
	if err != nil {
		return
	}
}
