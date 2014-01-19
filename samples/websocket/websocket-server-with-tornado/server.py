import tornado.ioloop
import tornado.web
import tornado.websocket
import tornado.template

class MainHandler(tornado.web.RequestHandler):
    def get(self):
	loader = tornado.template.Loader(".")
        self.write(loader.load("index.html").generate())

class WSHandler(tornado.websocket.WebSocketHandler):
    def open(self):
    	print 'Connection opened.'
    	self.write_message("Welcome to my websocket server.")
      
    def on_message(self, message):
        if message == "help":
            self.write_message("Need help, huh?")
    	print 'Message received: \'%s\'' % message

    def on_close(self):
      print 'Connection closed.'

application = tornado.web.Application([
    (r'/ws', WSHandler),
    (r"/", MainHandler),
    (r"/(.*)", tornado.web.StaticFileHandler, {"path": "./resources"}),
])

if __name__ == "__main__":
    application.listen(8080)
    tornado.ioloop.IOLoop.instance().start()
