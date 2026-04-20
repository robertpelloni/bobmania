<<<<<<< Updated upstream
<<<<<<< HEAD:docs/SERVER_SETUP.md
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
<<<<<<< Updated upstream
=======
=======
>>>>>>> origin/unified-ui-features-13937230807013224518
# Unified StepMania Server Setup Guide

The Unified StepMania platform requires a central backend server to handle matchmaking, chat relay, spectator streams, and economy syncs.

A fully functional "Mock Server" written in Node.js is provided to simulate these behaviors locally.

## Requirements
*   Node.js (v14+ recommended)
*   npm

## Installation

Navigate to the `server/` directory in the repository and install the required WebSocket and UUID packages:

```bash
cd server
npm install ws uuid
```

## Running the Server

Start the server using Node:

```bash
node mock_server.js
```

The server will bind to `ws://127.0.0.1:8080`.

## Supported Protocols

The Mock Server currently parses JSON payloads via WebSockets. It replaces the legacy `EzSockets` binary protocol.

### Client -> Server Messages
*   `{"type": "LOGIN", "username": "PlayerName"}`: Authenticates the session.
*   `{"type": "JOIN_QUEUE"}`: Adds the player to the ELO matchmaking queue.
*   `{"type": "REPORT_SCORE", "matchId": "uuid", "score": 98.4}`: Submits the final percentage for a tournament match.
*   `{"type": "CHAT", "text": "Hello World"}`: Broadcasts a message to the global lobby.

### Server -> Client Broadcasts
*   `{"type": "MATCH_START", "matchId": "uuid", "opponent": "Player2", "song": "Chart Name"}`: Emitted when two players in the queue are paired.
*   `{"type": "ACTIVE_MATCHES", "matches": [...]}`: An array of currently playing matches. The `SpectatorManager` (`ScreenSpectate`) listens to this to populate its TV list.

## Future Production Considerations
When moving this mock server to production:
1.  **Database:** Replace the in-memory `clients` map with a Redis cache or PostgreSQL database to persist ELO and Bobcoin balances.
2.  **Authentication:** Implement JWT tokens instead of trusting the `"username"` payload.
3.  **WSS:** Ensure the WebSocket runs over TLS (`wss://`) to prevent packet sniffing.
>>>>>>> origin/unified-ui-features-13937230807013224518:Docs/SERVER_SETUP.md
=======
>>>>>>> Stashed changes
