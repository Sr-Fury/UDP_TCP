# BlackBox

> A robust, standalone C++ Client-Server application built with Qt. This project demonstrates network programming using both TCP and UDP protocols, and features military-grade encryption using OpenSSL.

## ✨ Key Features

* **Multi-Protocol Support:** Includes implementations for both reliable TCP communication and fast UDP broadcasting.
* **OpenSSL Encryption:** The TCP version includes a fully integrated OpenSSL handshake, securing messages with TLS (Transport Layer Security) and `.crt` / `.key` certificate verification.
* **Standalone Deployment:** No Qt installation required! The repository includes pre-packaged release folders complete with all necessary `.dll` files, OpenSSL libraries, and TLS plugins. You can run this on any modern Windows PC instantly.
* **XOR Cipher Layer:** Includes an additional lightweight XOR encryption layer for custom message scrambling.
* **Clean UI:** Simple, responsive Graphical User Interface (GUI) for both the Server dashboard and Client chat windows.

## 📁 Repository Structure

The project is divided into different versions so you can explore the code easily:

* `/tcp_version/` - Standard TCP Client-Server without encryption.
* `/tcp_withOpenSSL/` - Secure TCP version requiring certificate handshakes.
* `/udp/` - UDP implementation for connectionless messaging.
* `/final_version/` - **(Ready to Run)** Contains the fully deployed, standalone USB folders. Just double-click and use!

## 🚀 How to Run (No Setup Required!)

If you just want to test the app without opening code, use the standalone folders:

1. Download or clone this repository.
2. Navigate to the `final_version` folder.
3. **Start the Server:** Open the `Server` folder and double-click `ChatServer.exe`.
   * *Note:* For the OpenSSL version, ensure `server.crt` and `server.key` remain in the same folder as the `.exe`.
4. **Start the Client:** Open the `Client` folder and double-click `ChatClient.exe`.
5. Enter the Server's IP (default is usually `127.0.0.1` for local testing) and Port (e.g., `1234`), then click **Connect**!

## 💻 How to Build from Source (For Developers)

If you want to edit the C++ code, you will need the following tools:

### Prerequisites
* **Qt Creator** (Qt 6.x recommended)
* **MSVC 2022 64-bit** compiler (or MinGW)
* **OpenSSL 3.x** installed (for the secure version)

### Build Instructions
1. Open Qt Creator.
2. Click **File -> Open File or Project** and select the `.pro` or `CMakeLists.txt` file for the version you want to build (Server or Client).
3. Configure the project using your 64-bit kit.
4. If building the OpenSSL version, ensure your `qputenv("QT_TLS_BACKEND", "openssl");` is active and you have generated a self-signed `server.crt` and `server.key` file in your build directory.
5. Click the **Green Play button** to build and run.

## 🛠️ Tech Stack

* **Language:** C++
* **Framework:** Qt 6 (Widgets, Network, Core)
* **Cryptography:** OpenSSL 3.x
* **Network Protocols:** TCP/IP, UDP

## 🔒 A Note on OpenSSL Deployment (DLLs)
Packaging OpenSSL with Qt on Windows requires specific environment variables and DLL structures. The standalone folders in this project have been carefully structured to bypass Microsoft Secure Channel (`Schannel`) and strictly enforce OpenSSL using custom `tls/qopensslbackend.dll` configurations. 

## 🤝 Contributing
Contributions, issues, and feature requests are welcome! Feel free to check the issues page if you want to add new features like file sharing or multi-client group chats.

---
**Created by Sr_Fury**