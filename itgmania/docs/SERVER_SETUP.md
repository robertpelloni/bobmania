# Server Setup Guide

## Requirements
*   Node.js (v14 or higher)
*   NPM

## Installation
1.  Navigate to the `server/` directory:
    ```bash
    cd server
    ```
2.  Install dependencies:
    ```bash
    npm install
    ```

## Running the Server
Start the backend server on port 9000:
```bash
node index.js
```

## Protocol
The server uses WebSockets (`ws`).
*   **Port:** 9000
*   **Format:** JSON

### Messages
*   `LOGIN`: `{ "type": "LOGIN", "username": "CoolPlayer" }`
*   `CHAT`: `{ "type": "CHAT", "msg": "Hello World" }`
*   `QUEUE_MATCH`: `{ "type": "QUEUE_MATCH" }`
