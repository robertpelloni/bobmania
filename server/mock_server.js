const WebSocket = require('ws');
const { v4: uuidv4 } = require('uuid');

const wss = new WebSocket.Server({ port: 8080 });

// Registries
let clients = new Map();
let matchmakingQueue = [];
let activeMatches = new Map();
let globalChatHistory = [];

console.log("Unified StepMania Mock Server running on ws://127.0.0.1:8080");

function broadcast(data) {
    const payload = JSON.stringify(data);
    wss.clients.forEach((client) => {
        if (client.readyState === WebSocket.OPEN) {
            client.send(payload);
        }
    });
}

function handleMatchmaking() {
    if (matchmakingQueue.length >= 2) {
        const p1 = matchmakingQueue.shift();
        const p2 = matchmakingQueue.shift();

        const matchId = uuidv4();
        const matchData = {
            id: matchId,
            p1: p1.username,
            p2: p2.username,
            song: "Random Mock Song",
            status: "IN_PROGRESS",
            viewers: 0
        };

        activeMatches.set(matchId, matchData);

        const startPayload = JSON.stringify({
            type: "MATCH_START",
            matchId: matchId,
            opponent: p2.username,
            song: matchData.song
        });

        // Notify P1
        if (p1.ws.readyState === WebSocket.OPEN) p1.ws.send(startPayload);
        // Notify P2
        if (p2.ws.readyState === WebSocket.OPEN) p2.ws.send(JSON.stringify({
            type: "MATCH_START",
            matchId: matchId,
            opponent: p1.username,
            song: matchData.song
        }));

        console.log(`[MATCHMAKING] Match ${matchId} started between ${p1.username} and ${p2.username}`);

        // Broadcast new active match for spectators
        broadcast({ type: "ACTIVE_MATCHES", matches: Array.from(activeMatches.values()) });
    }
}

wss.on('connection', (ws) => {
    const id = uuidv4();
    const clientData = { id, ws, username: `Guest_${id.substring(0,4)}`, elo: 1200 };
    clients.set(id, clientData);

    console.log(`[CONNECT] Client ${clientData.username} connected.`);

    // Send initial state
    ws.send(JSON.stringify({ type: "WELCOME", id, username: clientData.username, elo: clientData.elo }));
    ws.send(JSON.stringify({ type: "ACTIVE_MATCHES", matches: Array.from(activeMatches.values()) }));

    ws.on('message', (message) => {
        try {
            const data = JSON.parse(message);
            console.log(`[RECV ${clientData.username}]`, data);

            if (data.type === "LOGIN") {
                clientData.username = data.username || clientData.username;
                console.log(`[LOGIN] ${id} authenticated as ${clientData.username}`);
            }

            else if (data.type === "CHAT") {
                const chatMsg = { type: "CHAT", sender: clientData.username, text: data.text, timestamp: Date.now() };
                globalChatHistory.push(chatMsg);
                if (globalChatHistory.length > 50) globalChatHistory.shift();
                broadcast(chatMsg);
            }

            else if (data.type === "JOIN_QUEUE") {
                if (!matchmakingQueue.find(p => p.id === id)) {
                    matchmakingQueue.push(clientData);
                    console.log(`[QUEUE] ${clientData.username} joined matchmaking. Queue size: ${matchmakingQueue.length}`);
                    handleMatchmaking();
                }
            }

            else if (data.type === "REPORT_SCORE") {
                const match = activeMatches.get(data.matchId);
                if (match) {
                    console.log(`[SCORE] ${clientData.username} reported ${data.score} for match ${data.matchId}`);
                    // If both reported, resolve match (simplified mock)
                    match.status = "COMPLETE";
                    activeMatches.delete(data.matchId);
                    broadcast({ type: "ACTIVE_MATCHES", matches: Array.from(activeMatches.values()) });
                }
            }

        } catch (e) {
            console.error("[ERROR] Failed to parse message:", message.toString());
        }
    });

    ws.on('close', () => {
        console.log(`[DISCONNECT] Client ${clientData.username} disconnected.`);
        clients.delete(id);
        matchmakingQueue = matchmakingQueue.filter(p => p.id !== id);
    });
});
