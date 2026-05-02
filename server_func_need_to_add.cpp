// ใน Server.cpp (ตัวอย่างการเชื่อมต่อ)
bool Server::handleEvents() {
    // ... loop poll() ของเพื่อน ...
    
    char buffer[1024];
    int bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        std::string raw_data(buffer);

        // --- จุดเชื่อมต่อของคุณ ---
        // 1. แปลงข้อมูลดิบเป็น IRCMessage (ใช้ Parser ของคุณ)
        // สมมติว่าคุณมีฟังก์ชันที่รับ string แล้วคืนค่า IRCMessage
        IRCMessage msg = this->parser.parse(raw_data); 

        // 2. ส่งไปให้ Command ประมวลผล
        // ต้องส่ง pointer หรือ reference ของ Client ไปด้วยเพื่อให้ Command รู้ว่าใครส่งมา
        Client* sender = _clients[client_fd];
        this->command_handler.execute_command(msg, *sender);
    }
}