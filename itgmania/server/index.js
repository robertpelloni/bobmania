const WebSocket = require('ws');
const { v4: uuidv4 } = require('uuid');

const PORT = 9000;
const wss = new WebSocket.Server({ port: PORT });

console.log(`StepMania Network Server running on port ${PORT}`);

// State
const clients = new Map(); // ws -> { id, username, state }
const matchQueue = [];

wss.on('connection', (ws) => {
    const id = uuidv4();
    const metadata = { id, username: `Player_${id.substr(0,4)}`, state: 'LOBBY' };
    clients.set(ws, metadata);

    console.log(`[${id}] Connected.`);

    // Welcome Message
    send(ws, { type: 'SYSTEM', msg: 'Welcome to StepMania Network.' });

    ws.on('message', (message) => {
        try {
            const data = JSON.parse(message);
            handleMessage(ws, data);
        } catch (e) {
            console.error('Invalid JSON:', message);
        }
    });

    ws.on('close', () => {
        console.log(`[${id}] Disconnected.`);
        removeFromQueue(ws);
        clients.delete(ws);
        broadcast({ type: 'LOBBY', msg: `${metadata.username} left.` });
    });
});

function handleMessage(ws, data) {
    const meta = clients.get(ws);

    switch (data.type) {
        case 'LOGIN':
            meta.username = data.username || meta.username;
            console.log(`[${meta.id}] Logged in as ${meta.username}`);
            broadcast({ type: 'LOBBY', msg: `${meta.username} joined.` });
            break;

        case 'CHAT':
            console.log(`[Chat] ${meta.username}: ${data.msg}`);
            broadcast({ type: 'CHAT', username: meta.username, msg: data.msg });
            break;

        case 'QUEUE_MATCH':
            console.log(`[Match] ${meta.username} joined queue.`);
            addToQueue(ws);
            break;

        case 'SPECTATE':
            // Logic to subscribe to another player's stream
            break;
    }
}

function send(ws, payload) {
    if (ws.readyState === WebSocket.OPEN) {
        ws.send(JSON.stringify(payload));
    }
}

function broadcast(payload) {
    wss.clients.forEach(client => {
        if (client.readyState === WebSocket.OPEN) {
            client.send(JSON.stringify(payload));
        }
    });
}

function addToQueue(ws) {
    if (!matchQueue.includes(ws)) {
        matchQueue.push(ws);
        send(ws, { type: 'SYSTEM', msg: 'Joined Matchmaking Queue.' });
        checkQueue();
    }
}

function removeFromQueue(ws) {
    const index = matchQueue.indexOf(ws);
    if (index > -1) matchQueue.splice(index, 1);
}

function checkQueue() {
    if (matchQueue.length >= 2) {
        const p1 = matchQueue.shift();
        const p2 = matchQueue.shift();

        const matchID = uuidv4();
        console.log(`Starting Match ${matchID} between ${clients.get(p1).username} and ${clients.get(p2).username}`);

        const payload = { type: 'MATCH_START', matchID, opponent: '' };

        payload.opponent = clients.get(p2).username;
        send(p1, payload);

        payload.opponent = clients.get(p1).username;
        send(p2, payload);
    }
}
