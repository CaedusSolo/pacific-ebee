# Pacific E-Bee

Multiplayer Battleship over TCP.

## Compiling and Running

These commands will produce 2 executables inside the `src/bin/` directory: `server` and `client`.
```
cd src/
make
```

Run the server and the client in separate terminal windows. The server will wait for a client to connect.

## Game Flow
```mermaid
sequenceDiagram
    participant S as Server
    participant C1 as Client 1
    participant C2 as Client 2
    
    Note over S: Server starts
    S->>S: socket(), bind(), listen()
    
    C1->>S: connect()
    S->>S: accept()
    Note over C1,S: Connection established
    
    C2->>S: connect()
    S->>S: accept()
    Note over C2,S: Connection established
    
    C1->>S: Send Ready signal
    C2->>S: Send Ready signal

    S->>C1: Send Game Ready signal
    S->>C2: Send Game Ready signal

    C1->>S: Send player's name
    C2->>S: Send player's name
    
    S->>C1: Send battlefield
    S->>C2: Send battlefield
    
    S->>S: Choose first player
    S->>C1: Your turn
    S->>C2: Opponent's turn
    
    Note over S,C2: Game Loop
    
    loop Each Turn
        C1->>S: Attack coordinates
        S->>S: Process attack, update state
        S->>C1: Attack result (hit/miss/sunk)
        S->>C2: Attack result (hit/miss/sunk)
        
        alt Game Over
            S->>S: Calculate scores
            S->>C1: Game Over + win/loss + score
            S->>C2: Game Over + win/loss + score
            C1->>C1: Display game over
            C2->>C2: Display game over
            C1->>S: close()
            C2->>S: close()
            S->>S: close()
        else Game Continues
            S->>C2: Your turn
            S->>C1: Opponent's turn
            C2->>S: Attack coordinates
            S->>S: Process attack, update state
            S->>C2: Attack result (hit/miss/sunk)
            S->>C1: Attack result (hit/miss/sunk)
        end
    end
```
