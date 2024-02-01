#include "const.h"
#include "map.h"
#include "player.h"
#include "engine.h"
#include "minimap.h"
#include "base/game.h"

class FPSGame: public Game {
public:
    FPSGame(): Game("Raycasting 3D Game", SCREEN_WIDTH, SCREEN_HEIGHT) {}

    bool onGameCreate() override {
        return true;
    }

    bool onGameUpdate(double elapsedTime) override {
        engine.render(renderer.get());
        minimap.render(renderer.get());
        return true;
    }

    void onKeyDown(const SDL_Keysym &keysym) override {
        player.onKeyDown(keysym);
        minimap.onKeyDown(keysym);
    }
private:
    Player player = Player(PLAYER_X, PLAYER_Y, PLAYER_YAW);
    Engine engine = Engine(player, width, height);
    Minimap minimap = Minimap(player, width, height);
};

int main() {

    FPSGame game;
    game.start();

    return 0;
}
