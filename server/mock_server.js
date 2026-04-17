const WebSocket = require('ws');
const { v4: uuidv4 } = require('uuid');

const wss = new WebSocket.Server({ port: 8080 });

// Registries
let clients = new Map();
let matchmakingQueue = [];
let activeMatches = new Map();
let globalChatHistory = [];

console.log("Unified StepMania Matchmaking Server running on ws://127.0.0.1:8080");

function broadcast(data) {
    const payload = JSON.stringify(data);
    wss.clients.forEach((client) => {
        if (client.readyState === WebSocket.OPEN) {
            client.send(payload);
        }
    });
}

function calculateElo(rating1, rating2, score1, score2) {
    const K = 32;
    const expected1 = 1 / (1 + Math.pow(10, (rating2 - rating1) / 400));
    const expected2 = 1 / (1 + Math.pow(10, (rating1 - rating2) / 400));

    // Normalize scores to a 0-1 win ratio
    const totalScore = score1 + score2;
    let actual1 = 0.5, actual2 = 0.5;
    if (totalScore > 0) {
        actual1 = score1 / totalScore;
        actual2 = score2 / totalScore;
    }

    // Simplistic win/loss for standard Elo (1 for win, 0 for loss, 0.5 draw)
    if (score1 > score2) { actual1 = 1; actual2 = 0; }
    else if (score2 > score1) { actual1 = 0; actual2 = 1; }

    return {
        newRating1: Math.round(rating1 + K * (actual1 - expected1)),
        newRating2: Math.round(rating2 + K * (actual2 - expected2))
    };
}

function handleMatchmaking() {
    // Basic greedy match: just take the first two
    if (matchmakingQueue.length >= 2) {
        // Sort by Elo difference in a real app, here we just pop
        const p1 = matchmakingQueue.shift();
        const p2 = matchmakingQueue.shift();

        const matchId = uuidv4();
        const matchData = {
            id: matchId,
            p1: { id: p1.id, username: p1.username, elo: p1.elo, currentScore: 0, finished: false },
            p2: { id: p2.id, username: p2.username, elo: p2.elo, currentScore: 0, finished: false },
            song: "Random Mock Song (Draft Phase Bypassed)",
            status: "IN_PROGRESS",
            viewers: 0
        };

        activeMatches.set(matchId, matchData);

        const startPayloadP1 = JSON.stringify({ type: "MATCH_START", matchId: matchId, opponent: p2.username, opponentElo: p2.elo, song: matchData.song });
        const startPayloadP2 = JSON.stringify({ type: "MATCH_START", matchId: matchId, opponent: p1.username, opponentElo: p1.elo, song: matchData.song });

        if (p1.ws.readyState === WebSocket.OPEN) p1.ws.send(startPayloadP1);
        if (p2.ws.readyState === WebSocket.OPEN) p2.ws.send(startPayloadP2);

        console.log(`[MATCHMAKING] Match ${matchId} started: ${p1.username}(${p1.elo}) vs ${p2.username}(${p2.elo})`);

        // Broadcast to spectators
        broadcast({ type: "ACTIVE_MATCHES", matches: Array.from(activeMatches.values()).map(m => ({ id: m.id, p1: m.p1.username, p2: m.p2.username, song: m.song })) });
    }
}

wss.on('connection', (ws) => {
    const id = uuidv4();
    // Default Elo is 1200
    const clientData = { id, ws, username: `Guest_${id.substring(0,4)}`, elo: 1200 };
    clients.set(id, clientData);

    console.log(`[CONNECT] ${clientData.username} connected.`);
    ws.send(JSON.stringify({ type: "WELCOME", id, username: clientData.username, elo: clientData.elo }));

    ws.on('message', (message) => {
        try {
            const data = JSON.parse(message);

            if (data.type === "LOGIN") {
                clientData.username = data.username || clientData.username;
            }
            else if (data.type === "CHAT") {
                const chatMsg = { type: "CHAT", sender: clientData.username, text: data.text };
                broadcast(chatMsg);
            }
            else if (data.type === "JOIN_QUEUE") {
                if (!matchmakingQueue.find(p => p.id === id)) {
                    matchmakingQueue.push(clientData);
                    console.log(`[QUEUE] ${clientData.username} joined. Size: ${matchmakingQueue.length}`);
                    handleMatchmaking();
                }
            }
            else if (data.type === "REPORT_SCORE_TICK") {
                // Real-time score streaming during the song
                const match = activeMatches.get(data.matchId);
                if (match && match.status === "IN_PROGRESS") {
                    const isP1 = (match.p1.id === id);
                    if (isP1) match.p1.currentScore = data.score;
                    else match.p2.currentScore = data.score;

                    // Relay the opponent's score immediately
                    const relayTarget = isP1 ? match.p2.id : match.p1.id;
                    const targetClient = clients.get(relayTarget);
                    if (targetClient && targetClient.ws.readyState === WebSocket.OPEN) {
                        targetClient.ws.send(JSON.stringify({
                            type: "LIVE_SCORE_UPDATE",
                            opponentScore: data.score
                        }));
                    }
                }
            }
            else if (data.type === "REPORT_MATCH_COMPLETE") {
                const match = activeMatches.get(data.matchId);
                if (match) {
                    const isP1 = (match.p1.id === id);
                    if (isP1) { match.p1.finished = true; match.p1.currentScore = data.score; }
                    else { match.p2.finished = true; match.p2.currentScore = data.score; }

                    console.log(`[SCORE] ${clientData.username} finished with ${data.score}`);

                    if (match.p1.finished && match.p2.finished) {
                        // Calculate Elo
                        const eloResult = calculateElo(match.p1.elo, match.p2.elo, match.p1.currentScore, match.p2.currentScore);

                        console.log(`[MATCH COMPLETE] ${match.p1.username}(${match.p1.currentScore}) vs ${match.p2.username}(${match.p2.currentScore})`);
                        console.log(`[ELO UPDATE] ${match.p1.username}: ${match.p1.elo}->${eloResult.newRating1} | ${match.p2.username}: ${match.p2.elo}->${eloResult.newRating2}`);

                        // Update local registries
                        const c1 = clients.get(match.p1.id);
                        if(c1) c1.elo = eloResult.newRating1;
                        const c2 = clients.get(match.p2.id);
                        if(c2) c2.elo = eloResult.newRating2;

                        // Notify players
                        const resultPayload = JSON.stringify({
                            type: "MATCH_RESULT",
                            p1Score: match.p1.currentScore, p2Score: match.p2.currentScore,
                            p1NewElo: eloResult.newRating1, p2NewElo: eloResult.newRating2
                        });

                        if (c1 && c1.ws.readyState === WebSocket.OPEN) c1.ws.send(resultPayload);
                        if (c2 && c2.ws.readyState === WebSocket.OPEN) c2.ws.send(resultPayload);

                        match.status = "COMPLETE";
                        activeMatches.delete(data.matchId);
                    }
                }
            }
        } catch (e) {
            // Ignore bad JSON
        }
    });

    ws.on('close', () => {
        clients.delete(id);
        matchmakingQueue = matchmakingQueue.filter(p => p.id !== id);
    });
});
