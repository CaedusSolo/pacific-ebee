#include <stdio.h>
#include <stdlib.h>

#include "game_client_manager.h"
#include "battlefield.h"
#include "connection.h"
#include "messages.h"


void handle_game_start(GameClientData* game_client_data) {
    char* bf_buffer = (char*)malloc(1000000);
    listen_for_message(game_client_data->fd, bf_buffer);
    battlefield_deserialize(&game_client_data->battlefield, bf_buffer);
    printf("Battlefield acquired!\n");
}

void handle_turn(GameClientData* game_client_data) {
    int fd = game_client_data->fd;

    send_message(fd, READY_FOR_TURN, 15);
    int x, y;
    printf("Enter coords: ");
    scanf("%d %d", &x, &y);
    Vector2D pos = vector2d_create(x, y);

    char pos_buffer[8];
    vector2d_serialize(&pos, pos_buffer);
    send_message(fd, pos_buffer, 8);
}

void handle_game_update(GameClientData* game_client_data) {
    int fd = game_client_data->fd;

    char update_msg[8];
    size_t update_msg_len;
    listen_for_message(fd, update_msg);
    Vector2D ans = vector2d_deserialize(update_msg);
    printf("Game update: (%d, %d)\n", ans.x, ans.y);
}

void gameLoop(GameClientData* game_client_manager) {
    // this->sendReadySignal();
    // this->waitForGameReady();
    //
    // string playerName = this->askPlayerName();
    // this->sendPlayerName(playerName);
    //
    // this->listenForNewBattlefield();
    //
    // bool isOurTurn = this->listenForFirstTurnDecision();
    //
    // GameStatus gameStatus = GameStatus::ONGOING;
    // while (gameStatus == GameStatus::ONGOING) {
    //     this->renderer.grid(PLAYER_NUM, battlefield);
    //
    //     if (isOurTurn) {
    //         Vector2D coords = this->askShotCoords();
    //         HitResult result = this->sendShot(coords);
    //         this->processHitResult(result, false);
    //
    //         isOurTurn = false;
    //     }
    //     else {
    //         HitResult opponentShotResult = this->listenForOpponentShot();
    //         this->processHitResult(opponentShotResult, true);
    //
    //         isOurTurn = true;
    //     }
    // }
    //
    // gameStatus = this->listenForGameStatus();
    // switch (gameStatus) {
    //     case GameStatus::WON:
    //         this->endGame(true);
    //         break;
    //     case GameStatus::LOST:
    //         this->endGame(false);
    //         break;
    //     case GameStatus::ONGOING:
    //         break;
    // }
}

// void GameClientManager::endGame(bool isWinner) {
//     // int score = this->listenForFinalScore();
//     // if (isWinner) {
//     //     this->renderer.showWin();
//     // } else {
//     //     this->renderer.showLose();
//     // }
// }
//
// void GameClientManager::processHitResult(const HitResult& result, bool isOpponent) {
//     // switch (result.type) {
//     //     case HitResult::Type::MISS:
//     //         this->renderer.showMiss(result.position, isOpponent);
//     //         break;
//     //     case HitResult::Type::HIT:
//     //         this->renderer.showHit(result.position, isOpponent);
//     //         break;
//     //     case HitResult::Type::SINK:
//     //         this->renderer.showSunk(result.position, isOpponent, result.shipSunk);
//     //         break;
//     // }
// }
