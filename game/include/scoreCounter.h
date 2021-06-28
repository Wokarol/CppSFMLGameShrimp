#pragma once

namespace wok
{
    class ScoreCounter
    {
    public:
        ScoreCounter(int lastHighscore);
        ScoreCounter();

        void addPoints(int);
        auto getPoints() -> int;

        auto getHighscore() -> int;
        auto wasHighscoreBeat() -> int;

    private:
        int currentScore;
        int lastHighscore;
    };
}