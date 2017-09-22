server = MOAIHttpServer.new ()
MOAIFileSystem.setWorkingDirectory(INVOKE_DIR)



server:start ({
  listening_ports = '8080',
  document_root = INVOKE_DIR
})
print("Listening on http://localhost:8080")
print("Press any key to exit")
io.read()