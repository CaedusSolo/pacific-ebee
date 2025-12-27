#include "game_client_manager.h"
#include "messages.h"


GameClientManager::GameClientManager(Connection& serverConn, Renderer& renderer) {
    this->serverConn = serverConn;
    this->renderer = renderer;
}

GameClientManager::~GameClientManager() {
    // Destructor implementation
}

void GameClientManager::gameLoop() {
    this->sendReadySignal();
    this->waitForGameReady();

    string playerName = this->askPlayerName();
    this->sendPlayerName(playerName);

    this->listenForNewBattlefield();

    bool isOurTurn = this->listenForFirstTurnDecision();

    GameStatus gameStatus = GameStatus::ONGOING;
    while (gameStatus == GameStatus::ONGOING) {
        this->renderer.render(battlefield);

        if (isOurTurn) {
            Vector2D coords = this->askShotCoords();
            HitResult result = this->sendShot(coords);
            this->processHitResult(result, false);

            isOurTurn = false;
        }
        else {
            HitResult opponentShotResult = this->listenForOpponentShot();
            this->processHitResult(opponentShotResult, false);

            isOurTurn = true;
        }
    }

    gameStatus = this->listenForGameStatus();
    switch (gameStatus) {
        case GameStatus::WON:
            this->endGame(true);
            break;
        case GameStatus::LOST:
            this->endGame(false);
            break;
        case GameStatus::ONGOING:
            break;
    }
}

void GameClientManager::endGame(bool isWinner) {
    int score = this->listenForFinalScore();
    if (isWinner) {
        this->renderer.showWin();
    } else {
        this->renderer.showLose();
    }
}

void GameClientManager::processHitResult(const HitResult& result, bool isOpponent) {
    switch (result.type) {
        case HitResult::Type::MISS:
            this->renderer.showMiss(result.position, isOpponent);
            break;
        case HitResult::Type::HIT:
            this->renderer.showHit(result.position, isOpponent);
            break;
        case HitResult::Type::SINK:
            this->renderer.showSunk(result.position, isOpponent, result.shipSunk);
            break;
    }
}
