const WebSocket = require('ws');
const { v4: uuidv4 } = require('uuid');
const { Pool } = require('pg');

// Database Configuration (Assumes local dev DB for Phase 5 prototype)
const pool = new Pool({
    user: process.env.DB_USER || 'postgres',
    host: process.env.DB_HOST || 'localhost',
    database: process.env.DB_NAME || 'stepmania_unified',
    password: process.env.DB_PASSWORD || 'password',
    port: process.env.DB_PORT || 5432,
});

const wss = new WebSocket.Server({ port: 8080 });

// Registries
let clients = new Map();
let matchmakingQueue = [];
let activeMatches = new Map();

console.log("Unified StepMania Matchmaking Server running on ws://127.0.0.1:8080");

// ---------------------------------------------------------
// Database Initialization
// ---------------------------------------------------------
async function initDB() {
    try {
        await pool.query(`
            CREATE TABLE IF NOT EXISTS users (
                id UUID PRIMARY KEY,
                username VARCHAR(50) UNIQUE NOT NULL,
                elo INTEGER DEFAULT 1200,
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            );
        `);
        await pool.query(`
            CREATE TABLE IF NOT EXISTS match_history (
                id UUID PRIMARY KEY,
                p1_id UUID REFERENCES users(id),
                p2_id UUID REFERENCES users(id),
                p1_score INTEGER,
                p2_score INTEGER,
                song_name VARCHAR(255),
                played_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            );
        `);
        console.log("[DB] Tables verified successfully.");
    } catch (e) {
        console.error("[DB ERROR] Could not initialize database schemas. Running in fallback mode.", e.message);
    }
}
initDB();

// ---------------------------------------------------------
// Core Logic
// ---------------------------------------------------------
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

    let actual1 = 0.5, actual2 = 0.5;
    if (score1 > score2) { actual1 = 1; actual2 = 0; }
    else if (score2 > score1) { actual1 = 0; actual2 = 1; }

    return {
        newRating1: Math.round(rating1 + K * (actual1 - expected1)),
        newRating2: Math.round(rating2 + K * (actual2 - expected2))
    };
}

async function handleMatchmaking() {
    if (matchmakingQueue.length >= 2) {
        const p1 = matchmakingQueue.shift();
        const p2 = matchmakingQueue.shift();

        const matchId = uuidv4();
        const matchData = {
            id: matchId,
            p1: { id: p1.id, username: p1.username, elo: p1.elo, currentScore: 0, finished: false, dbId: p1.dbId },
            p2: { id: p2.id, username: p2.username, elo: p2.elo, currentScore: 0, finished: false, dbId: p2.dbId },
            song: "Ranked Pool - Random",
            status: "IN_PROGRESS",
            viewers: 0
        };

        activeMatches.set(matchId, matchData);

        const startPayloadP1 = JSON.stringify({ type: "MATCH_START", matchId, opponent: p2.username, opponentElo: p2.elo, song: matchData.song });
        const startPayloadP2 = JSON.stringify({ type: "MATCH_START", matchId, opponent: p1.username, opponentElo: p1.elo, song: matchData.song });

        if (p1.ws.readyState === WebSocket.OPEN) p1.ws.send(startPayloadP1);
        if (p2.ws.readyState === WebSocket.OPEN) p2.ws.send(startPayloadP2);

        console.log(`[MATCHMAKING] Match ${matchId} started: ${p1.username}(${p1.elo}) vs ${p2.username}(${p2.elo})`);
        broadcast({ type: "ACTIVE_MATCHES", matches: Array.from(activeMatches.values()).map(m => ({ id: m.id, p1: m.p1.username, p2: m.p2.username, song: m.song })) });
    }
}

// ---------------------------------------------------------
// WebSocket Connection Handler
// ---------------------------------------------------------
wss.on('connection', (ws) => {
    const id = uuidv4();
    const clientData = { id, ws, username: `Guest_${id.substring(0,4)}`, elo: 1200, dbId: null };
    clients.set(id, clientData);

    console.log(`[CONNECT] ${clientData.username} connected.`);

    ws.on('message', async (message) => {
        try {
            const data = JSON.parse(message);

            if (data.type === "LOGIN") {
                const username = data.username;
                try {
                    // Upsert user into DB
                    let res = await pool.query('SELECT * FROM users WHERE username = $1', [username]);
                    if (res.rows.length === 0) {
                        const dbId = uuidv4();
                        await pool.query('INSERT INTO users (id, username, elo) VALUES ($1, $2, 1200)', [dbId, username]);
                        clientData.dbId = dbId;
                        clientData.username = username;
                        clientData.elo = 1200;
                        console.log(`[DB] Created new user ${username}`);
                    } else {
                        clientData.dbId = res.rows[0].id;
                        clientData.username = res.rows[0].username;
                        clientData.elo = res.rows[0].elo;
                        console.log(`[DB] Loaded existing user ${username} (Elo: ${clientData.elo})`);
                    }
                    ws.send(JSON.stringify({ type: "WELCOME", id, username: clientData.username, elo: clientData.elo }));
                } catch(e) {
                    console.error("[LOGIN ERROR] DB Error", e.message);
                }
            }

            else if (data.type === "GET_LEADERBOARD") {
                try {
                    const res = await pool.query('SELECT username, elo FROM users ORDER BY elo DESC LIMIT 100');
                    ws.send(JSON.stringify({ type: "LEADERBOARD", data: res.rows }));
                } catch(e) {}
            }

            else if (data.type === "JOIN_QUEUE") {
                if (!clientData.dbId) return; // Must be logged in to play ranked
                if (!matchmakingQueue.find(p => p.id === id)) {
                    matchmakingQueue.push(clientData);
                    console.log(`[QUEUE] ${clientData.username} joined. Size: ${matchmakingQueue.length}`);
                    handleMatchmaking();
                }
            }

            else if (data.type === "REPORT_SCORE_TICK") {
                const match = activeMatches.get(data.matchId);
                if (match && match.status === "IN_PROGRESS") {
                    const isP1 = (match.p1.id === id);
                    if (isP1) match.p1.currentScore = data.score;
                    else match.p2.currentScore = data.score;

                    const relayTarget = isP1 ? match.p2.id : match.p1.id;
                    const targetClient = clients.get(relayTarget);
                    if (targetClient && targetClient.ws.readyState === WebSocket.OPEN) {
                        targetClient.ws.send(JSON.stringify({ type: "LIVE_SCORE_UPDATE", opponentScore: data.score }));
                    }
                }
            }

            else if (data.type === "REPORT_MATCH_COMPLETE") {
                const match = activeMatches.get(data.matchId);
                if (match) {
                    const isP1 = (match.p1.id === id);
                    if (isP1) { match.p1.finished = true; match.p1.currentScore = data.score; }
                    else { match.p2.finished = true; match.p2.currentScore = data.score; }

                    if (match.p1.finished && match.p2.finished) {
                        // Calculate Elo
                        const eloResult = calculateElo(match.p1.elo, match.p2.elo, match.p1.currentScore, match.p2.currentScore);

                        try {
                            // Update Users Table
                            await pool.query('UPDATE users SET elo = $1 WHERE id = $2', [eloResult.newRating1, match.p1.dbId]);
                            await pool.query('UPDATE users SET elo = $1 WHERE id = $2', [eloResult.newRating2, match.p2.dbId]);

                            // Log Match History
                            await pool.query(
                                'INSERT INTO match_history (id, p1_id, p2_id, p1_score, p2_score, song_name) VALUES ($1, $2, $3, $4, $5, $6)',
                                [uuidv4(), match.p1.dbId, match.p2.dbId, match.p1.currentScore, match.p2.currentScore, match.song]
                            );
                            console.log(`[DB] Match ${data.matchId} committed to PostgreSQL.`);
                        } catch(e) {
                            console.error("[DB ERROR] Failed to save match result", e.message);
                        }

                        // Notify players
                        const resultPayload = JSON.stringify({
                            type: "MATCH_RESULT",
                            p1Score: match.p1.currentScore, p2Score: match.p2.currentScore,
                            p1NewElo: eloResult.newRating1, p2NewElo: eloResult.newRating2
                        });

                        const c1 = clients.get(match.p1.id);
                        const c2 = clients.get(match.p2.id);
                        if (c1 && c1.ws.readyState === WebSocket.OPEN) { c1.ws.send(resultPayload); c1.elo = eloResult.newRating1; }
                        if (c2 && c2.ws.readyState === WebSocket.OPEN) { c2.ws.send(resultPayload); c2.elo = eloResult.newRating2; }

                        match.status = "COMPLETE";
                        activeMatches.delete(data.matchId);
                    }
                }
            }
        } catch (e) {}
    });

    ws.on('close', () => {
        clients.delete(id);
        matchmakingQueue = matchmakingQueue.filter(p => p.id !== id);
    });
});
